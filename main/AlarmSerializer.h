#pragma once

#include "Alarm.h"

class AlarmSerializer {
    public:
        virtual std::string serialize(alarms_t& alarm) = 0;
};
