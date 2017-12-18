#pragma once
#include "Alarm.h"
#include "EspPersistentStorage.h"
#include <list>
#include <ctime>

class EspAlarm :
    public Alarm
{
    public: 
        virtual void setAlarm(alarms_t& time) override;
        virtual std::list<alarms_t>& getAlarms() const override;
        virtual alarms_t getNextAlarm() override;
        virtual void deleteAlarm(alarms_t& alarm) override;
        virtual void loadFromPeristentStorage() override;
    private:
        void saveToPersistentStorage();
        static std::list<alarms_t> m_alarms;
        EspPersistentStorage persistentStorage_{"Alarms"};
};
