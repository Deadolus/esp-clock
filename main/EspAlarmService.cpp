#include "EspAlarmService.h"
#include "EspAlarm.h"
#include <vector>
#include <iterator>
#include <list>
#include "esp_log.h"

EspAlarmService::EspAlarmService(Alarm& alarm) : alarms_(alarm) {}
static const char* TAG = "AlarmService";

/** Checks for alarm, returns true if one is ringing */
bool EspAlarmService::checkForAlarm() {
    ESP_LOGI(TAG, "Checking for alarm");
    time_t now = 0;
    struct tm timeinfo = {};
    time(&now);
    bool ringing{false};
    for(alarm: alarms_.getAlarms())
    {
    ESP_LOGI(TAG, "Now = %lu, alarm time: %lu", now, alarm.time);
        if( (now == alarm.time) || (alarm.status == AlarmStatus::Ringing) )
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
