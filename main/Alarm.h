#pragma once
#include "driver/timer.h"
#include <cstdint>
#include <list>
#include <chrono>
#include <functional>
#include <bitset>

enum class AlarmStatus {
    Disabled = 0,
    Pacified,
    Ringing,
    Snoozed
};

struct alarms_t {
    std::chrono::system_clock::time_point time{};
    std::chrono::system_clock::time_point snoozeTime{};
    timer_idx_t timer{};
    AlarmStatus status{AlarmStatus::Disabled};
    std::string name{};
    //!days of week, bit 0 is sunday, bit 6 saturday
    std::bitset<7> weekRepeat{};
    std::function<void(alarms_t&)> callback{};
};

class Alarm 
{
    public: 
        virtual void setAlarm(alarms_t& alarm) = 0;
        virtual std::list<alarms_t>& getAlarms() const = 0;
        virtual alarms_t getNextAlarm() = 0;
};
