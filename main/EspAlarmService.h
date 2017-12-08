#pragma once
#include "AlarmService.h"
#include <chrono>
class Alarm;
class EspAlarmService :
    public AlarmService
{
    public:
        EspAlarmService() = delete;
        EspAlarmService(Alarm& alarms, std::chrono::minutes snoozeTime);
        virtual bool checkForAlarm() override;
        virtual bool alarmRinging() override;
        virtual bool snooze() override;
        virtual bool pacify() override;
    private:
        Alarm& alarms_;
        std::chrono::minutes snoozeTime_;
};
