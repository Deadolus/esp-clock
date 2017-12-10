#include "EspAudioPlayer.h"
#include "freertos/FreeRTOS.h"
#include "driver/i2s.h"
#include "driver/adc.h"
#include "esp_partition.h"
#include "esp_log.h"
#include "audio_example_file.h"
#include <thread>
#include <mutex>
static const char* TAG = "AudioPlayer";
//i2s number
static const i2s_port_t EXAMPLE_I2S_NUM{static_cast<i2s_port_t>(0)};
//i2s sample rate
#define EXAMPLE_I2S_SAMPLE_RATE   (16000.0)
//i2s data bits
static const i2s_bits_per_sample_t EXAMPLE_I2S_SAMPLE_BITS{static_cast<i2s_bits_per_sample_t>(16)};
//enable display buffer for debug
#define EXAMPLE_I2S_BUF_DEBUG     (0)
//I2S read buffer length
#define EXAMPLE_I2S_READ_LEN      (16 * 1024.0)
//I2S data format
//#define EXAMPLE_I2S_FORMAT        (I2S_CHANNEL_FMT_ONLY_RIGHT)
#define EXAMPLE_I2S_FORMAT        (I2S_CHANNEL_FMT_RIGHT_LEFT)
//I2S channel number
static const i2s_channel_t EXAMPLE_I2S_CHANNEL_NUM{static_cast<i2s_channel_t>(2)};

namespace {
static std::mutex AUDIO_PLAYER_MUTEX{};
    /**
     * @brief I2S ADC/DAC mode init.
     */
    void example_i2s_init()
    {
        i2s_port_t i2s_num = EXAMPLE_I2S_NUM;
        i2s_config_t i2s_config{};
        i2s_config.mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN | I2S_MODE_ADC_BUILT_IN);
        i2s_config.sample_rate =  EXAMPLE_I2S_SAMPLE_RATE;
        i2s_config.bits_per_sample = EXAMPLE_I2S_SAMPLE_BITS;
        i2s_config.communication_format = I2S_COMM_FORMAT_I2S_MSB;
        i2s_config.channel_format = EXAMPLE_I2S_FORMAT;
        i2s_config.intr_alloc_flags = 0;
        i2s_config.dma_buf_count = 2;
        i2s_config.dma_buf_len = 1024;
        //};
        //install and start i2s driver
        i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
        //init DAC pad
        i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
        //init ADC pad
        //i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_0);
    }
    /**
     * @brief Reset i2s clock and mode
     */
    void example_reset_play_mode()
    {
        i2s_set_clk(EXAMPLE_I2S_NUM, EXAMPLE_I2S_SAMPLE_RATE, EXAMPLE_I2S_SAMPLE_BITS, EXAMPLE_I2S_CHANNEL_NUM);
    }

    /**
     * @brief Set i2s clock for example audio file
     */
    void example_set_file_play_mode()
    {
        i2s_set_clk(EXAMPLE_I2S_NUM, 16000, EXAMPLE_I2S_SAMPLE_BITS, static_cast<i2s_channel_t>(1));
    }
    /**
     * @brief Scale data to 16bit/32bit for I2S DMA output.
     *        DAC can only output 8bit data value.
     *        I2S DMA will still send 16 bit or 32bit data, the highest 8bit contains DAC data.
     */
    int example_i2s_dac_data_scale(uint8_t* d_buff, uint8_t* s_buff, uint32_t len)
    {
        uint32_t j = 0;
//#if (EXAMPLE_I2S_SAMPLE_BITS == 16)
        for (int i = 0; i < len; i++) {
            d_buff[j++] = 0;
            d_buff[j++] = s_buff[i];
        }
        return (len * 2);
//#else
//        for (int i = 0; i < len; i++) {
//            d_buff[j++] = 0;
//            d_buff[j++] = 0;
//            d_buff[j++] = 0;
//            d_buff[j++] = s_buff[i];
//        }
//        return (len * 4);
//#endif
    }

    /**
     * @brief Scale data to 8bit for data from ADC.
     *        Data from ADC are 12bit width by default.
     *        DAC can only output 8 bit data.
     *        Scale each 12bit ADC data to 8bit DAC data.
     */
    void example_i2s_adc_data_scale(uint8_t * d_buff, uint8_t* s_buff, uint32_t len)
    {
        uint32_t j = 0;
        uint32_t dac_value = 0;
//#if (EXAMPLE_I2S_SAMPLE_BITS == 16)
        for (int i = 0; i < len; i += 2) {
            dac_value = ((((uint16_t) (s_buff[i + 1] & 0xf) << 8) | ((s_buff[i + 0]))));
            d_buff[j++] = 0;
            d_buff[j++] = dac_value * 256 / 4096;
        }
//#else
//        for (int i = 0; i < len; i += 4) {
//            dac_value = ((((uint16_t)(s_buff[i + 3] & 0xf) << 8) | ((s_buff[i + 2]))));
//            d_buff[j++] = 0;
//            d_buff[j++] = 0;
//            d_buff[j++] = 0;
//            d_buff[j++] = dac_value * 256 / 4096;
//        }
//#endif
    }

/**
 * @brief debug buffer data
 */
void example_disp_buf(uint8_t* buf, int length)
{
    printf("======\n");
    for (int i = 0; i < length; i++) {
        printf("%02x ", buf[i]);
        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }
    printf("======\n");
}

    void esp_play_audio() {
    ESP_LOGI(TAG, "Playing audio");
    int offset = 0;
    int tot_size = sizeof(audio_table);
    int i2s_read_len = EXAMPLE_I2S_READ_LEN;
    uint8_t* i2s_write_buff = (uint8_t*) calloc(i2s_read_len, sizeof(char));
    example_set_file_play_mode();
ESP_LOGI(TAG, "Audio, tot size: %d", tot_size);
    while (offset < tot_size) {
        int play_len = ((tot_size - offset) > (4 * 1024)) ? (4 * 1024) : (tot_size - offset);

        int i2s_wr_len = example_i2s_dac_data_scale(i2s_write_buff, (uint8_t*)(audio_table + offset), play_len);
        i2s_write_bytes(EXAMPLE_I2S_NUM, (const char*) i2s_write_buff, i2s_wr_len, portMAX_DELAY);
        offset += play_len;
        //example_disp_buf((uint8_t*) i2s_write_buff, 32);
    }
    ESP_LOGI(TAG, "Finished playing audio");
    //vTaskDelay(100 / portTICK_PERIOD_MS);
    example_reset_play_mode();
    //free(flash_read_buff);
    free(i2s_write_buff);
    AUDIO_PLAYER_MUTEX.unlock();
    }
}



EspAudioPlayer::EspAudioPlayer() {
    static bool player_initialized{false};
    if(!player_initialized) {
    ESP_LOGI(TAG, "Initializing Audio player");
    example_i2s_init();
    example_set_file_play_mode();
    player_initialized = true;
    }
}

void EspAudioPlayer::startAudio() {
    //esp_play_audio();
    bool result = AUDIO_PLAYER_MUTEX.try_lock();
    if(result) {
    std::thread play_audio(esp_play_audio);
    play_audio.detach();
    //AUDIO_PLAYER_MUTEX.unlock();
    //esp_play_audio();
    }

}

void EspAudioPlayer::stopAudio() {

}
