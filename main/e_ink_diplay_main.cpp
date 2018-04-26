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
#include "ADXLService.h"

#include "driver/gpio.h"
#include "sdkconfig.h"
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
    EspWifi wifi{};
    EspSntpClient sntp{wifi};
    EspDisplay display{};
    display.init();
    EspSign espsign(display);
    EspAlarm alarm{};
    alarm.loadFromPeristentStorage();
    EspHttpServer httpserver{alarm};
    EspAlarmService alarms{alarm, std::chrono::minutes(10)};
    EspAudioPlayer audioplayer{};
    EspPwmLed pwmLed{CONFIG_LED_GPIO};
    EspButton button{0, true};
    auto pacifyFunction =  [&]() {
            ESP_LOGI(TAG, "Pacify function called");
            alarms.pacify();
            audioplayer.stopAudio();
            display.fullUpdate();
            pwmLed.setIntensity(0);
            //alarm.saveAlarms();
    };
    auto sensorFunction = [&](){
        pacifyFunction();
        pwmLed.setIntensity(80);
        sleep(5);
        pwmLed.setIntensity(0);
    };
    button.setPressCb(pacifyFunction);
    button.setLongPressCb([&](){
            if(wifi.isConnected()) 
            wifi.stopWifi();
            else
            wifi.startWifi();

            });
    alarms.setAlarmCallback([&](){
            audioplayer.startAudio();
            pwmLed.setIntensity(100);
    });
    ADXLService sensorService{sensorFunction};
    wifi.init();
    wifi.startWifi();
    EspDisplayService displayService{display, espsign, alarm, alarms, wifi, sntp, 5000};
    setTimezone();
    sntp.getTime(false);
    httpserver.startServer();
    ESP_LOGI(TAG, "Everything started...");
    alarms_t testAlarm;
    testAlarm.time = std::chrono::system_clock::now() + std::chrono::seconds(1);
    testAlarm.singleShot = true;
    testAlarm.name="MyTest";
    testAlarm.weekRepeat = 0xff;
    //alarm.setAlarm(testAlarm );


    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void setTimezone() {
	setenv("TZ", "CET-2", 1);
	tzset();
}
