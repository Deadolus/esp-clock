#pragma once
#include "Alarm.h"
#include "EspPersistentStorage.h"
#include <list>
#include <ctime>

class EspAlarm :
    public Alarm
{
    public: 
        virtual void setAlarm(alarms_t& time);
        virtual std::list<alarms_t>& getAlarms() const;
        virtual alarms_t getNextAlarm() override;
        virtual void deleteAlarm(alarms_t& alarm) override;
    private:
        static std::list<alarms_t> m_alarms;
        EspPersistentStorage persistentStorage_{"Alarms"};
};
