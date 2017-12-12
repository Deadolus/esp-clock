#include "EspAlarmService.h"
#include "EspAlarm.h"
#include <vector>
#include <iterator>
#include <list>
#include "esp_log.h"
#include <chrono>

namespace {
static const char* TAG = "AlarmService";

    time_t getCurrentTime() {
       std::chrono::system_clock::now();
        time_t now{0};
        time(&now);
        return now;
    }

bool correctDayOfWeek(std::bitset<7>& days) {
    time_t now = getCurrentTime();
    //tm_wday is days since sundays
    tm now_tm = *localtime(&now);

    //ESP_LOGI(TAG, "Now is %u, comparison: %d, t%d f%d, %s", now_tm.tm_wday, days.test(now_tm.tm_wday), true, false, days.to_string().c_str());
    return days.test(now_tm.tm_wday);

}
bool correctTime(std::chrono::system_clock::time_point alarmTime) {
    time_t alarm_time = std::chrono::system_clock::to_time_t(alarmTime);
    tm alarm_tm = *localtime(&alarm_time);
    time_t now = getCurrentTime();
    tm now_tm = *localtime(&now);
    return (now_tm.tm_hour == alarm_tm.tm_hour) && (now_tm.tm_min == alarm_tm.tm_min) && (now_tm.tm_sec == alarm_tm.tm_sec);
}

    bool alarmShouldRing(alarms_t& alarm, std::chrono::minutes snoozeTime) {
        std::chrono::system_clock::time_point snoozePoint = alarm.snoozeTime+snoozeTime;
        if( 
                correctDayOfWeek(alarm.weekRepeat) &&  (
                correctTime(alarm.time)
                || (std::chrono::system_clock::now() == snoozePoint)
                || (alarm.status == AlarmStatus::Ringing)
          ))
        {
            return true;
        }
        return false;
    }
}
EspAlarmService::EspAlarmService(Alarm& alarm, std::chrono::minutes snoozeTime) : alarms_(alarm), snoozeTime_(snoozeTime) {}

/** Checks for alarm, returns true if one is ringing */
bool EspAlarmService::checkForAlarm() {
    ESP_LOGI(TAG, "Checking for alarm");
    bool ringing{false};
    for(auto& alarm: alarms_.getAlarms())
    {
        ESP_LOGI(TAG, "Now = %lu, alarm time: %lu, Status: %u", std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), std::chrono::system_clock::to_time_t(alarm.time), static_cast<unsigned int>(alarm.status));
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
    for(auto& alarm: alarms_.getAlarms())
    {
        if(alarm.status == AlarmStatus::Ringing)
        {
            return true;
        }
    }
    return false;
}

bool EspAlarmService::snooze() {
    for(auto& alarm: alarms_.getAlarms())
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
    for(auto& alarm: alarms_.getAlarms())
    {
        if(alarm.status >= AlarmStatus::Ringing)
        {
            ESP_LOGI(TAG, "Pacifying alarm");
            alarm.status = AlarmStatus::Pacified;
        }
    }
    return true;
}
std::list<alarms_t> EspAlarmService::getRingingAlarms() {
    std::list<alarms_t> ringingAlarms;
    for(auto& alarm: alarms_.getAlarms())
    {
        if(alarm.status == AlarmStatus::Ringing)
        {
            ringingAlarms.push_back(alarm);
        }
    }
    return ringingAlarms;
}

alarms_t EspAlarmService::getNextAlarm() {
    //std::chrono::system_clock::time_point next{std::chrono::system_clock::time_point::max()};
    alarms_t nextAlarm = alarms_.getAlarms().front();
    for(auto& alarm: alarms_.getAlarms())
    {
        if(alarm.time < nextAlarm.time)
            nextAlarm = alarm;
    }
    return nextAlarm;
}
