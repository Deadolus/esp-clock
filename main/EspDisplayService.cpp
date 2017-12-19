#include "EspDisplayService.h"
#include "EspDisplay.h"
#include "EspSign.h"
#include "EspAlarm.h"
#include "EspAlarmService.h"
#include "EspWifi.h"
#include "EspSntpClient.h"
#include "EspSntpClient.h"
#include "esp_log.h"
#include <thread>

static const char* TAG = "DisplayService";
void test(unsigned int delay, EspDisplay& display) {
ESP_LOGI(TAG, "TEST");
}

EspDisplayService::EspDisplayService(EspDisplay& display, EspSign& espsign, EspAlarm& alarm, EspAlarmService& alarms, EspWifi& wifi, EspSntpClient& sntp, unsigned int delay=5000) 
{
    std::thread task(displayServiceTask, delay, std::ref(display), std::ref(espsign), std::ref(alarm), std::ref(alarms), std::ref(wifi), std::ref(sntp));
    task.detach();
    ESP_LOGI(TAG, "Started display task");
}

void EspDisplayService::displayServiceTask(unsigned int delay, EspDisplay& display, EspSign& espsign, EspAlarm& alarm, EspAlarmService& alarms, EspWifi& wifi, EspSntpClient& sntp ) {
    while(true) {
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
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}
