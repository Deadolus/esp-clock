#pragma once

class AlarmService
{
    public:
        virtual bool checkForAlarm() = 0;
        virtual bool alarmRinging() = 0;
        virtual bool snooze() = 0;
        virtual bool pacify() = 0;
};
