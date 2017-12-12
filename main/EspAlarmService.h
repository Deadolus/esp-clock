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
        virtual std::list<alarms_t> getRingingAlarms() override;
        virtual bool snooze() override;
        virtual bool pacify() override;
        virtual alarms_t getNextAlarm() override;
    private:
        Alarm& alarms_;
        std::chrono::minutes snoozeTime_;
};
