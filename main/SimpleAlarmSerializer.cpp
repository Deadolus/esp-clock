#include "SimpleAlarmSerializer.h"

#include "Alarm.h"
#include "Clock.h"

#include "esp_log.h"

#include <sstream>
#include <string>
#include <vector>

namespace {
static const char* TAG = "SimpleSerializer";
    //https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string
    std::vector<std::string> split(const std::string& text, const std::string& delims)
    {
        std::vector<std::string> tokens;
        std::size_t start = text.find_first_not_of(delims), end = 0;

        while((end = text.find_first_of(delims, start)) != std::string::npos)
        {
            tokens.push_back(text.substr(start, end - start));
            start = text.find_first_not_of(delims, end);
        }
        if(start != std::string::npos)
            tokens.push_back(text.substr(start));

        return tokens;
    }
}



const std::string SimpleAlarmSerializer::serialize(alarms_t const& alarm) {
    std::stringstream retVal;

    retVal << alarm.name <<",";
    retVal << Clock::getTimet(alarm.time) <<",";
    retVal << Clock::getTimet(alarm.snoozeTime) <<",";
    retVal << std::to_string(alarm.weekRepeat.to_ulong())<<",";
    retVal << std::to_string(alarm.singleShot);
    ESP_LOGI(TAG, "Serialized: %s", retVal.str().c_str());

    return retVal.str();
}

alarms_t SimpleAlarmSerializer::deserialize(std::string const& text) {
    alarms_t retVal{};
    std::vector<std::string> fields = split(text, ",");
    if(fields.size() == 5) {
    retVal.name = fields.at(0);
    try {
    retVal.time = Clock::convertToTimePoint(static_cast<time_t>(std::stoi(fields.at(1))));
    retVal.snoozeTime = Clock::convertToTimePoint(static_cast<time_t>(std::stoi(fields.at(2))));
    retVal.weekRepeat = std::bitset<7>(std::stoi(fields.at(3)));
    retVal.singleShot = std::stoi(fields.at(4));
    } catch(std::exception& e) {}
    }
    return retVal;
}
