#pragma once

#include "Alarm.h"
#include <string>

class AlarmSerializer {
    public:
        virtual std::string serialize(alarms_t const& alarm) = 0;
        virtual alarms_t deserialize(std::string const& text) = 0;
};
