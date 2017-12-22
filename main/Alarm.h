#pragma once
#include "driver/timer.h"
#include <cstdint>
#include <list>
#include <vector>
#include <chrono>
#include <functional>
#include <bitset>

enum class AlarmStatus {
    Disabled = 0,
    Set,
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
    bool singleShot{false};
    friend bool operator==(const alarms_t& lhs, const alarms_t& rhs) {
        return std::tie(lhs.time, lhs.name, lhs.weekRepeat) == std::tie(rhs.time, rhs.name, rhs.weekRepeat);
    }
    friend bool operator<(const alarms_t& lhs, const alarms_t& rhs) {
        return std::tie(lhs.time, lhs.name) < std::tie(rhs.time, rhs.name);
    }
};

class Alarm 
{
    public: 
        virtual void setAlarm(alarms_t const& alarm) = 0;
        //virtual std::list<alarms_t>& getAlarms() const = 0;
        virtual std::vector<alarms_t>& getAlarms() const = 0;
        virtual alarms_t getNextAlarm() = 0;
        virtual void deleteAlarm(alarms_t& alarm) = 0;
        virtual void deleteAlarm(unsigned int position) = 0;
        virtual void loadFromPeristentStorage() = 0;
        virtual void saveAlarms()  = 0;
};
