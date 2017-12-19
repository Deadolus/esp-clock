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
#include "EspDisplayService.h"
#include "Clock.h"
#include "SimpleAlarmSerializer.h"

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

static const char *TAG = "clock";

void setTimezone();

extern "C" void app_main()
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    EspWifi wifi;
    EspSntpClient sntp{wifi};
    EspDisplay display;
    EspSign espsign(display);
    EspAlarm alarm{};
    EspHttpServer httpserver{alarm};
    alarm.loadFromPeristentStorage();
    EspAlarmService alarms{alarm, std::chrono::minutes(10)};
    EspAudioPlayer audioplayer;
    EspPwmLed pwmLed{CONFIG_LED_GPIO};
    EspButton button{0, true};
    button.setPressCb([&](){
            ESP_LOGI(TAG, "Button pressed!");
            alarms.pacify();
            audioplayer.stopAudio();
            display.fullUpdate();
            pwmLed.setIntensity(0);
            if(!wifi.isConnected()) wifi.startWifi();
            });
    button.setLongPressCb([&](){
            ESP_LOGI(TAG, "Button long pressed!");
            if(wifi.isConnected()) wifi.stopWifi();
            });
    alarms.setAlarmCallback([&](){
            audioplayer.startAudio();
            pwmLed.setIntensity(100);
    });
    wifi.init();
    wifi.startWifi();
    setTimezone();
    sntp.getTime(false);
    httpserver.startServer();
    EspDisplayService displayService{display, espsign, alarm, alarms, wifi, sntp, 5000};
    ESP_LOGI(TAG, "Everything started...");

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void setTimezone() {
	setenv("TZ", "CET-1", 1);
	tzset();
}
