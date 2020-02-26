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
#include "Timer.h"

#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
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

#include "imagedata.h"
extern "C" void app_main()
{
ESP_LOGI(TAG, "Hello to ESP Clock");
    ESP_ERROR_CHECK( nvs_flash_init() );
    EspDisplay display{};
    display.init();
    EspSign espsign(display);
    ESP_LOGI(TAG, "Start writing to display");
    //display.setImage(wifi_filled, 32, 32, 32, 32);
    //espsign.setClock(true);
    //display.write("Hello world", 128/2,296/2,Font::Font24);
    //display.write("Hello world", 24,24,Font::Font24);
    ESP_LOGI(TAG, "Wrote to display");
    //std::this_thread::sleep_for(std::chrono::seconds(1000));
    EspWifi wifi{};
    EspSntpClient sntp{wifi};
    EspAlarm alarm{};
    alarm.loadFromPeristentStorage();
    EspAlarmService alarms{alarm, std::chrono::minutes(10)};
    //EspAudioPlayer audioplayer{};
    //EspPwmLed pwmLed{CONFIG_LED_GPIO};
    EspButton button{CONFIG_BUTTON_GPIO, false};
    //EspHttpServer httpserver{alarm, pwmLed};
    auto pacifyFunction =  [&]() {
            ESP_LOGI(TAG, "Pacify function called");
            alarms.pacify();
            //audioplayer.stopAudio();
            //display.fullUpdate();
            //pwmLed.setIntensity(0);
            //alarm.saveAlarms();
    };
    auto sensorFunction = [&](){
        pacifyFunction();
        //pwmLed.setIntensity(80);
        //Timer timer{std::chrono::seconds(5), [&]()->void{pwmLed.setIntensity(0);}};
    };
    //button.setPressCb(pacifyFunction);
    button.setPressCb([&](){
        ESP_LOGI(TAG, "Button pressed");
        });
    button.setLongPressCb([&](){
        ESP_LOGI(TAG, "Long pressed");
            if(wifi.isConnected())
            wifi.stopWifi();
            else
            wifi.startWifi();
            });
    button.setExtraLongPressCb([&](){
        ESP_LOGI(TAG, "Extra long pressed");
        });
    alarms.setAlarmCallback([&](){
            //audioplayer.startAudio();
            //pwmLed.setIntensity(100);
    });
    //ADXLService sensorService{sensorFunction};
    wifi.init();
    wifi.startWifi();
    EspDisplayService displayService{display, espsign, alarm, alarms, wifi, sntp, 5000};
    setTimezone();
    sntp.getTime(false);
    //httpserver.startServer();
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
	setenv("TZ", "CET-1", 1);
	tzset();
}
