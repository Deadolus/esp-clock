/**
 *  @filename   :   epd1in54-demo.ino
 *  @brief      :   1.54inch e-paper display demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 5 2017
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

//#include <SPI.h>
#include "EspDisplay.h"
#include "EspSign.h"
#include "EspSntpClient.h"
#include "EspWifi.h"
#include "EspAlarmService.h"
#include "EspAlarm.h"
#include "EspAudioPlayer.h"
#include "EspButton.h"
#include "EspPwmLed.h"
#include "EspHttpServer.h"
#include "EspPersistentStorage.h"
#include "Clock.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

#include <chrono>
#include <cstring>
#include <thread>
#include <iomanip>


extern "C" {
}

/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO
#define TIMER_DIVIDER         16  //  Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds
#define TIMER_INTERVAL0_SEC   (1) // sample test interval for the first timer
#define TIMER_INTERVAL1_SEC   (5.78)   // sample test interval for the second timer
#define TEST_WITHOUT_RELOAD   0        // testing will be done without auto reload
#define TEST_WITH_RELOAD 1 // testing will be done with auto reload
static unsigned int timer_variable = 0;

static const char *TAG = "clock";
/* Variable holding number of times ESP32 restarted since first boot.
 * It is placed into RTC memory using RTC_DATA_ATTR and
 * maintains its value when ESP32 wakes from deep sleep.
 */
//RTC_DATA_ATTR static int boot_count = 0; 
//static void obtain_time();
static void obtain_time(void *);
static void initialise_wifi();
static void example_tg0_timer_init(timer_idx_t timer_idx, bool auto_reload, double timer_interval_sec);
void IRAM_ATTR timer_group0_isr(void *para);
void blink_task(void *pvParameter);
void display_test(void *pvParameter);
void updateTime(EspDisplay& display, EspSign& espsign);

unsigned long time_start_ms;
unsigned long time_now_s;


extern "C" void app_main()
{
    static EspHttpServer httpserver;
    EspPersistentStorage storage{"storage"};
    uint32_t test = storage.getValue<uint32_t>("test");
    int32_t test2 = storage.getValue<int32_t>("test");
    //std::string test3 = storage.getValue<std::string>("test");
    //ESP_LOGI(TAG, "Test: %u %d %s", test, test2, test3.c_str());
    if(esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER)
    {
    ESP_ERROR_CHECK( nvs_flash_init() );
    //xTaskCreate(&blink_task, "display_test", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    //xTaskCreate(&display_test, "display_test", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
   // xTaskCreate(&initialise_wifi, "initialise_wifi", 4048, NULL, 5, NULL);
    //init_time();
    xTaskCreate(&display_test, "display_test", 8000, NULL, 5, NULL);
    std::thread wifi(initialise_wifi);
    wifi.detach();
    //xTaskCreate(&obtain_time, "obtain_time", 2000, NULL, 5, NULL);
    std::thread obtainTime(obtain_time, nullptr);
    obtainTime.detach();
    //xTaskCreate(&obtain_time, "obtain_time", 2048, NULL, 5, NULL);
    //example_tg0_timer_init(TIMER_0, TEST_WITHOUT_RELOAD, TIMER_INTERVAL0_SEC);
    /* wifi.join(); */
    /* obtainTime.join(); */
    httpserver.startServer();
    }
    ESP_LOGI(TAG, "Everything started...");
}

//void obtain_time()
void obtain_time(void *pvParameters)
{
    EspWifi wifi{};
    EspSntpClient sntpClient{ wifi };
    sntpClient.getTime(true);
    //test alarm
    EspAlarm alarm;
    alarms_t soon{};
    soon.time = Clock::getCurrentTimeAsTimePoint()+std::chrono::seconds(4);
    soon.weekRepeat = 0b0111111; // Mo, tue, thu, fr
    soon.name = "Soon Alarm";
    alarms_t wakeup{};

    time_t now = Clock::getCurrentTimeAsTimet();
    std::tm wakeup_tm{};
    wakeup_tm = *localtime(&now);
    wakeup_tm.tm_mday++;
    wakeup_tm.tm_hour = 8;
    wakeup_tm.tm_min = 0;
    wakeup.time = Clock::convertToTimePoint(wakeup_tm);
    wakeup.weekRepeat = 0b1111111;
    wakeup.name = "Wakeup";
    //alarms_t soon{std::chrono::system_clock::now()+std::chrono::seconds(4),std::chrono::system_clock::from_time_t(0), static_cast<timer_idx_t>(0), AlarmStatus::Pacified, [](alarms_t){} };

    //alarm.setAlarm(soon);
    alarm.setAlarm(wakeup);
}

static void initialise_wifi()
{
    EspWifi wifi{};
    wifi.init();
    wifi.startWifi();
}

void display_test(void *pvParameter)
 {
    EspDisplay display;
    EspSign espsign(display);
    while(true) {
        //all of the framebuffer is updated on every updateTime
        //thus no need to call it twice
        updateTime(display, espsign);
        display.sleep();
        //esp_sleep_enable_timer_wakeup(0.2 * 1000000);
        //esp_light_sleep_start();
        //esp_deep_sleep_start();
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
 }

void updateTime(EspDisplay& display, EspSign& espsign) {
    static EspWifi wifi;
    static EspSntpClient sntp{wifi};
    static EspAlarm alarm{};
    static EspAlarmService alarms{alarm, std::chrono::minutes(10)};
    static EspAudioPlayer audioplayer;
    static EspPwmLed pwmLed{CONFIG_LED_GPIO};
    static EspButton button{0, true};
    button.setPressCb([&](){
            ESP_LOGI(TAG, "Button pressed!");
            alarms.pacify();
            audioplayer.stopAudio();
            display.fullUpdate();
            pwmLed.setIntensity(0);
            if(!wifi.isConnected()) wifi.startWifi();
            });
    button.setLongPressCb([](){
            ESP_LOGI(TAG, "Button long pressed!");
            if(wifi.isConnected()) wifi.stopWifi();
            });
    espsign.setWifi(wifi.isConnected());
    time_t now{};
    struct tm timeinfo{};
    char strftime_buf[64];

    espsign.setWifi(wifi.isConnected());
    time(&now);
    localtime_r(&now, &timeinfo);
    espsign.setClock(sntp.timeSet());
    setenv("TZ", "CET-1", 1);
    tzset();
    std::strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    //std::put_time(std::localtime(&now), "The current date/time in Zuerich is: %s");
    //ESP_LOGI(TAG, "The current date/time in Zuerich is: %s", strftime_buf);
    if(sntp.timeSet())
        strftime(strftime_buf, sizeof(strftime_buf), "%R", &timeinfo);
    else
        sprintf(strftime_buf, "--:--");
    display.setNextAlarmName(alarm.getNextAlarm().name.c_str(), alarm.getNextAlarm().time);
    if(alarms.checkForAlarm()) {
        auto ringingAlarms = alarms.getRingingAlarms();
        display.setAlarm(ringingAlarms.front().name.c_str());
        audioplayer.startAudio();
        pwmLed.setIntensity(50);
    }
    else
    {
        display.partialUpdate();
        //erasing "Alarm!"
        display.clearAlarm();
        //pwmLed.setIntensity(0);
    }

    //strftime(strftime_buf, sizeof(strftime_buf), "%r", &timeinfo);
    display.write(std::string(strftime_buf), 200, 0, Font::Font24);
    display.send();
}

/*
 * Initialize selected timer of the timer group 0
 *
 * timer_idx - the timer number to initialize
 * auto_reload - should the timer auto reload on alarm?
 * timer_interval_sec - the interval of alarm to set
 */
static void example_tg0_timer_init(timer_idx_t timer_idx, 
    bool auto_reload, double timer_interval_sec)
{
    /* Select and initialize basic parameters of the timer */
    timer_config_t config;
    config.divider = TIMER_DIVIDER;
    config.counter_dir = TIMER_COUNT_UP;
    config.counter_en = TIMER_PAUSE;
    config.alarm_en = TIMER_ALARM_EN;
    config.intr_type = TIMER_INTR_LEVEL;
    config.auto_reload = auto_reload;
    timer_init(TIMER_GROUP_0, timer_idx, &config);

    /* Timer's counter will initially start from value below.
       Also, if auto_reload is set, this value will be automatically reload on alarm */
    timer_set_counter_value(TIMER_GROUP_0, timer_idx, 0x00000000ULL);

    /* Configure the alarm value and the interrupt on alarm. */
    timer_set_alarm_value(TIMER_GROUP_0, timer_idx, timer_interval_sec * TIMER_SCALE);
    timer_enable_intr(TIMER_GROUP_0, timer_idx);
    timer_isr_register(TIMER_GROUP_0, timer_idx, timer_group0_isr, 
        (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);

    timer_start(TIMER_GROUP_0, timer_idx);
}
/*
 * Timer group0 ISR handler
 *
 * Note:
 * We don't call the timer API here because they are not declared with IRAM_ATTR.
 * If we're okay with the timer irq not being serviced while SPI flash cache is disabled,
 * we can allocate this interrupt without the ESP_INTR_FLAG_IRAM flag and use the normal API.
 */
void IRAM_ATTR timer_group0_isr(void *para)
{
    //timer_idx_t timer_idx = (int) para;
    timer_idx_t timer_idx = static_cast<timer_idx_t>((int)para);

    /* Retrieve the interrupt status and the counter value
       from the timer that reported the interrupt */
    uint32_t intr_status = TIMERG0.int_st_timers.val;
    TIMERG0.hw_timer[timer_idx].update = 1;
    uint64_t timer_counter_value = 
        ((uint64_t) TIMERG0.hw_timer[timer_idx].cnt_high) << 32
        | TIMERG0.hw_timer[timer_idx].cnt_low;

    /* Clear the interrupt
       and update the alarm time for the timer with without reload */
    if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_0) {
        /* evt.type = TEST_WITHOUT_RELOAD; */
        TIMERG0.int_clr_timers.t0 = 1;
        timer_counter_value += (uint64_t) (TIMER_INTERVAL0_SEC * TIMER_SCALE);
        TIMERG0.hw_timer[timer_idx].alarm_high = (uint32_t) (timer_counter_value >> 32);
        TIMERG0.hw_timer[timer_idx].alarm_low = (uint32_t) timer_counter_value;
    timer_variable = timer_counter_value;
    } else if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_1) {
        /* evt.type = TEST_WITH_RELOAD; */
        TIMERG0.int_clr_timers.t1 = 1;
    } else {
        /* evt.type = -1; // not supported even type */
    }

    /* After the alarm has been triggered
      we need enable it again, so it is triggered the next time */
    TIMERG0.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;

}
