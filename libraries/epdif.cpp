/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "epdif.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include "esp_system.h"
#include "freertos/task.h"
#include <mutex>
//#include <spi.h>
#define INPUT 0
#define OUTPUT 1
#define HIGH 1
#define LOW 0

spi_device_handle_t EpdIf::spi;

EpdIf::EpdIf() {
};

EpdIf::~EpdIf() {
};

void EpdIf::DigitalWrite(int pin, int value) {
    //digitalWrite(pin, value);
    gpio_set_level(static_cast<gpio_num_t>(pin), value);
}

int EpdIf::DigitalRead(int pin) {
    return gpio_get_level(static_cast<gpio_num_t>(pin));
    //return digitalRead(pin);
}

void EpdIf::DelayMs(unsigned int delaytime) {
    vTaskDelay(delaytime / portTICK_PERIOD_MS);
    //delay(delaytime);
}

void EpdIf::SpiTransfer(unsigned char data) {
	static std::mutex mutex{};
	std::lock_guard<std::mutex> lock(mutex);
    spi_transaction_t t{};
    esp_err_t ret{ESP_OK};
//    if (len==0) return;             //no need to send anything
    t.length=1*8;                 //Len is in bytes, transaction length is in bits.
    //t.tx_buffer=&data;               //Data
    //t.flags = SPI_USE_TXDATA;
    t.flags = SPI_TRANS_USE_TXDATA;// | SPI_TRANS_USE_RXDATA;
    t.tx_data[0]=data;               //Data
    t.tx_data[1]=data;               //Data
    t.tx_data[2]=data;               //Data
    t.tx_data[3]=data;               //Data
    t.rx_buffer = nullptr;
    //t.user = nullptr;
    //t.user=(void*)1;                //D/C needs to be set to 1
//    gpio_set_level(static_cast<gpio_num_t>(CS_PIN), LOW);
    //printf("Transmitting data, %u\n", data);
    //vTaskDelay(1/ portTICK_PERIOD_MS);
    ret=spi_device_transmit(spi, &t);  //Transmit!
    //assert( ret == ESP_OK );
    if( ret != ESP_OK )
        printf("Transmitting error\n");
}

//This function is called (in irq context!) just before a transmission starts. It will
//set the D/C line to the value indicated in the user field.
void lcd_spi_pre_transfer_callback(spi_transaction_t *t) 
{
    //int dc=(int)t->user;
    //gpio_set_level(static_cast<gpio_num_t>(CONFIG_DC_GPIO), dc);
}

int EpdIf::IfInit(void) {
    gpio_pad_select_gpio(static_cast<gpio_num_t>(CONFIG_CS_GPIO));
    gpio_set_direction(static_cast<gpio_num_t>(CONFIG_CS_GPIO), GPIO_MODE_OUTPUT);  
    gpio_pullup_en(static_cast<gpio_num_t>(CONFIG_CS_GPIO));  
    gpio_pad_select_gpio(static_cast<gpio_num_t>(CONFIG_RST_GPIO));
    gpio_set_direction(static_cast<gpio_num_t>(CONFIG_RST_GPIO), GPIO_MODE_OUTPUT); 
    gpio_pullup_en(static_cast<gpio_num_t>(CONFIG_RST_GPIO));  
    gpio_pad_select_gpio(static_cast<gpio_num_t>(CONFIG_DC_GPIO));
    gpio_set_direction(static_cast<gpio_num_t>(CONFIG_DC_GPIO), GPIO_MODE_OUTPUT); 
    gpio_pullup_en(static_cast<gpio_num_t>(CONFIG_DC_GPIO));  
    gpio_pad_select_gpio(static_cast<gpio_num_t>(CONFIG_BUSY_GPIO));
    gpio_pulldown_dis(static_cast<gpio_num_t>(CONFIG_BUSY_GPIO));
    gpio_pullup_dis(static_cast<gpio_num_t>(CONFIG_BUSY_GPIO));
    gpio_set_direction(static_cast<gpio_num_t>(CONFIG_BUSY_GPIO), GPIO_MODE_INPUT); 
    esp_err_t ret;
    //spi_device_handle_t spi;
//    extern "C" {
    spi_bus_config_t buscfg{};
        buscfg.mosi_io_num=CONFIG_MOSI_GPIO;
        buscfg.miso_io_num=CONFIG_MISO_GPIO;
        buscfg.sclk_io_num=CONFIG_CLK_GPIO;
        buscfg.quadwp_io_num=-1;
        buscfg.quadhd_io_num=-1; 
        buscfg.max_transfer_sz=0;
    spi_device_interface_config_t devcfg{};
        devcfg.clock_speed_hz=1000000;               //Clock out at 10 MHz
        devcfg.mode=0;                                //SPI mode 0
        devcfg.spics_io_num=CONFIG_CS_GPIO;               //CS pin
        devcfg.queue_size=7;                          //We want to be able to queue 7 transactions at a time
//        devcfg.pre_cb=lcd_spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line
//    };
 //   }
    //Initialize the SPI bus
    ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    assert(ret==ESP_OK);
    //Attach the LCD to the SPI bus
    ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    assert(ret==ESP_OK);
    return 0;
}

