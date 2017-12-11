#pragma once
#include "Alarm.h"
#include <list>
#include <ctime>

class EspAlarm :
    public Alarm
{
    public: 
        virtual void setAlarm(alarms_t& time);
        virtual std::list<alarms_t>& getAlarms() const;
    private:
        static std::list<alarms_t> m_alarms;
};
