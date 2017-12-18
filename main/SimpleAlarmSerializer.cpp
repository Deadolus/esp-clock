#include "SimpleAlarmSerializer.h"

#include "Alarm.h"
#include "Clock.h"

#include "esp_log.h"

#include <sstream>

static const char* TAG = "SimpleSerializer";


std::string SimpleAlarmSerializer::serialize(alarms_t& alarm) {
    std::stringstream retVal;

    retVal << alarm.name <<",";
    retVal << Clock::getTimet(alarm.time) <<",";
    retVal << Clock::getTimet(alarm.snoozeTime) <<",";
    retVal << alarm.weekRepeat.to_string();
    ESP_LOGI(TAG, "%s", retVal.str().c_str());

    return retVal.str();
}
