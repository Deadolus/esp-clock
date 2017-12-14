#include "EspAlarmService.h"
#include "EspAlarm.h"
#include "Clock.h"
#include "esp_log.h"
#include <vector>
#include <iterator>
#include <list>
#include <chrono>

namespace {
static const char* TAG = "AlarmService";


bool correctDayOfWeek(std::bitset<7>& days) {
    time_t now = Clock::getCurrentTimeAsTimet();
    //tm_wday is days since sundays
    tm now_tm = *localtime(&now);

    //ESP_LOGI(TAG, "Now is %u, comparison: %d, t%d f%d, %s", now_tm.tm_wday, days.test(now_tm.tm_wday), true, false, days.to_string().c_str());
    return days.test(now_tm.tm_wday);

}
bool correctTime(std::chrono::system_clock::time_point alarmTime) {
    tm alarm_tm = Clock::getTm(alarmTime);
    tm now_tm = Clock::getCurrentTimeAsTm();
    return (now_tm.tm_hour == alarm_tm.tm_hour) && (now_tm.tm_min == alarm_tm.tm_min) && (now_tm.tm_sec == alarm_tm.tm_sec);
}

bool alarmShouldRing(alarms_t& alarm, std::chrono::minutes snoozeTime) {
        std::chrono::system_clock::time_point snoozePoint = alarm.snoozeTime+snoozeTime;
        if( 
                correctDayOfWeek(alarm.weekRepeat) &&  (
                correctTime(alarm.time)
                || (Clock::getCurrentTimeAsTimePoint() == snoozePoint)
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
    std::lock_guard<std::mutex> guard(alarmServiceMutex);
    //ESP_LOGI(TAG, "Checking for alarm");
    bool ringing{false};
    for(auto& alarm: alarms_.getAlarms())
    {
        //ESP_LOGI(TAG, "Now = %lu, alarm time: %lu, Status: %u", std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), std::chrono::system_clock::to_time_t(alarm.time), static_cast<unsigned int>(alarm.status));
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
    std::lock_guard<std::mutex> guard(alarmServiceMutex);
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
    std::lock_guard<std::mutex> guard(alarmServiceMutex);
    for(auto& alarm: alarms_.getAlarms())
    {
        if(alarm.status == AlarmStatus::Ringing)
        {
            alarm.status = AlarmStatus::Snoozed;
            alarm.snoozeTime = Clock::getCurrentTimeAsTimePoint();
        }
    }
    return true;
}

bool EspAlarmService::pacify() {
    std::lock_guard<std::mutex> guard(alarmServiceMutex);
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
    std::lock_guard<std::mutex> guard(alarmServiceMutex);
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

