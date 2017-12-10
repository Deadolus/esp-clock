#include "EspAlarmService.h"
#include "EspAlarm.h"
#include <vector>
#include <iterator>
#include <list>
#include "esp_log.h"
#include <chrono>

namespace {

    time_t getCurrentTime() {
       std::chrono::system_clock::now();
        time_t now{0};
        time(&now);
        return now;
    }

    bool alarmShouldRing(alarms_t& alarm, std::chrono::minutes snoozeTime) {
        //std::chrono::system_clock snoozePoint = std::chrono::system_clock(now)+snoozeTime;
        std::chrono::system_clock::time_point snoozePoint = alarm.snoozeTime+snoozeTime;
        if( 
                (std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()) == std::chrono::time_point_cast<std::chrono::seconds>(alarm.time)) 
                || (std::chrono::system_clock::now() == snoozePoint)
                || (alarm.status == AlarmStatus::Ringing)
          )
        {
            return true;
        }
        return false;
    }
}
EspAlarmService::EspAlarmService(Alarm& alarm, std::chrono::minutes snoozeTime) : alarms_(alarm), snoozeTime_(snoozeTime) {}
static const char* TAG = "AlarmService";

/** Checks for alarm, returns true if one is ringing */
bool EspAlarmService::checkForAlarm() {
    ESP_LOGI(TAG, "Checking for alarm");
    bool ringing{false};
    for(alarm: alarms_.getAlarms())
    {
        ESP_LOGI(TAG, "Now = %lu, alarm time: %lu", std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), std::chrono::system_clock::to_time_t(alarm.time));
        if(alarmShouldRing(alarm, snoozeTime_))
        {
            alarm.status = AlarmStatus::Ringing;
            ringing = true;
            ESP_LOGI(TAG, "Alarm ringing");
        }
    }
    return ringing;
}

bool EspAlarmService::alarmRinging() {
    for(auto alarm: alarms_.getAlarms())
    {
        if(alarm.status == AlarmStatus::Ringing)
        {
            return true;
        }
    }
    return false;
}

bool EspAlarmService::snooze() {
    for(auto alarm: alarms_.getAlarms())
    {
        if(alarm.status == AlarmStatus::Ringing)
        {
            alarm.status = AlarmStatus::Snoozed;
            alarm.snoozeTime = std::chrono::system_clock::now();
        }
    }
    return true;
}

bool EspAlarmService::pacify() {
    for(auto alarm: alarms_.getAlarms())
    {
        if(alarm.status >= AlarmStatus::Ringing)
        {
            alarm.status = AlarmStatus::Pacified;
        }
    }
    return true;
}
