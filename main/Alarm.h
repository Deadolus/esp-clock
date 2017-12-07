#pragma once
#include "driver/timer.h"
#include <cstdint>
#include <list>
#include <ctime>
#include <functional>

 struct alarms_t {
    time_t time;
    timer_idx_t timer;
    std::function<void(alarms_t&)> callback;
};

class Alarm 
{
    public: 
        virtual void setAlarm(alarms_t& alarm) = 0;
        virtual std::list<alarms_t> getAlarms() const = 0;
};