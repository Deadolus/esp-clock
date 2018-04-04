#pragma once
#include "Alarm.h"
#include "EspPersistentStorage.h"
#include <list>
#include <vector>
#include <ctime>

class EspAlarm :
    public Alarm
{
    public: 
        virtual void setAlarm(alarms_t const& time) override;
        //virtual std::list<alarms_t>& getAlarms() const override;
        virtual std::vector<alarms_t>& getAlarms() const override;
        virtual const alarms_t getNextAlarm() const override;
        virtual void deleteAlarm(alarms_t& alarm) override;
        virtual void deleteAlarm(unsigned int position) override;
        virtual void loadFromPeristentStorage() override;
        virtual void saveAlarms() override;
    private:
        void saveToPersistentStorage();
        //static std::list<alarms_t> m_alarms;
        static std::vector<alarms_t> m_alarms;
        EspPersistentStorage persistentStorage_{"Alarms"};
};
