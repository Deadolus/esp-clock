#pragma once
#include "AlarmService.h"
#include <chrono>
#include <mutex>
#include <functional>
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
        virtual void setAlarmCallback(std::function<void()> callback) override;
    private:
        static void alarmServiceTask(EspAlarmService& alarmService);
        Alarm& alarms_;
        std::chrono::minutes snoozeTime_{};
        std::mutex alarmServiceMutex_{};
        std::function<void()> alarmCallback_{};
};
