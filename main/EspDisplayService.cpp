#include "EspDisplayService.h"
#include "EspDisplay.h"
#include "EspSign.h"
#include "EspAlarm.h"
#include "EspAlarmService.h"
#include "EspWifi.h"
#include "EspSntpClient.h"
#include "EspSntpClient.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <thread>

static const char* TAG = "DisplayService";

EspDisplayService::EspDisplayService(EspDisplay& display, EspSign& espsign, EspAlarm& alarm, EspAlarmService& alarms, EspWifi& wifi, EspSntpClient& sntp, unsigned int delay=5000) 
{
    std::thread task(displayServiceTask, delay, display, espsign, alarm, alarms, wifi, sntp);
    //task.detach();
}

void EspDisplayService::displayServiceTask(unsigned int delay, EspDisplay& display, EspSign& espsign, EspAlarm& alarm, EspAlarmService& alarms, EspWifi& wifi, EspSntpClient& sntp ) {
    while(true) {
        ESP_LOGI(TAG, "Hello from display-service");
        alarms_t nextAlarm = alarm.getNextAlarm();
        display.setNextAlarmName(nextAlarm.name.c_str(), alarm.getNextAlarm().time);
        display.showNextAlarmInfo(nextAlarm);
        if(alarms.alarmRinging()) {
            auto ringingAlarms = alarms.getRingingAlarms();
            display.setAlarm(ringingAlarms.front().name.c_str());
        }
        else {
            display.partialUpdate();
            //erasing "Alarm!"
            display.clearAlarm();
        }
        espsign.setWifi(wifi.isConnected());
        espsign.setClock(sntp.timeSet());
        display.setTime(sntp);
        display.send();
        display.sleep();
        vTaskDelay(delay / portTICK_PERIOD_MS);
    }
}
