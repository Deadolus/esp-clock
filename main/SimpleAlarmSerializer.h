#pragma once

#include "AlarmSerializer.h"
#include "Alarm.h"

class SimpleAlarmSerializer : 
    public AlarmSerializer 
{
    public:
        virtual std::string serialize(alarms_t& alarm) override;
};
