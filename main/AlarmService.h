#pragma once
#include "Alarm.h"
#include <list>

class AlarmService
{
    public:
        virtual bool checkForAlarm() = 0;
        virtual bool alarmRinging() = 0;
        virtual std::list<alarms_t> getRingingAlarms() = 0;
        virtual bool snooze() = 0;
        virtual bool pacify() = 0;
};
