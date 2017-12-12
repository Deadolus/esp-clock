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
        virtual alarms_t getNextAlarm() override;
    private:
        static std::list<alarms_t> m_alarms;
};
