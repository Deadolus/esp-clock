#pragma once

#include "AlarmSerializer.h"
#include "Alarm.h"
#include <string>

class SimpleAlarmSerializer :
    public AlarmSerializer
{
    public:
        virtual const std::string serialize(alarms_t const& alarm) override;
        virtual alarms_t deserialize(std::string const& text) override;
};
