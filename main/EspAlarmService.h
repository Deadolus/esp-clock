#pragma once
#include "AlarmService.h"
class Alarm;
class EspAlarmService :
    public AlarmService
{
    public:
        EspAlarmService() = delete;
        EspAlarmService(Alarm& alarms);
        virtual bool checkForAlarm() override;
        virtual bool alarmRinging() override;
        virtual bool snooze() override;
        virtual bool pacify() override;
    private:
        Alarm& alarms_;
};
