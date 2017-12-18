#pragma once

#include "AlarmSerializer.h"
#include "Alarm.h"
#include <string>

class SimpleAlarmSerializer : 
    public AlarmSerializer 
{
    public:
        virtual std::string serialize(alarms_t& alarm) override;
        virtual alarms_t deserialize(std::string const& text) override;
};
