#include "EspAlarmService.h"
#include "EspAlarm.h"
#include <vector>
#include <iterator>
#include <list>
using namespace std;

EspAlarmService::EspAlarmService(Alarm& alarm) : alarms_(alarm) {}

/** Checks for alarm, returns true if one is ringing */
bool EspAlarmService::checkForAlarm() {
    time_t now = 0;
    struct tm timeinfo = {};
    time(&now);
    bool ringing{false};
    for(auto alarm: alarms_.getAlarms())
    {
        if(now == alarm.time)
        {
            alarm.status = AlarmStatus::Ringing;
            ringing = true;
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
