#include "EspAlarm.h"
#include "esp_log.h"
#include "SimpleAlarmSerializer.h"
#include <list>
#include <string>
static const char* TAG = "Alarm";

std::list<alarms_t> EspAlarm::m_alarms{};

void EspAlarm::setAlarm(alarms_t& time) {
    ESP_LOGI(TAG, "Got new alarm, name: %s", time.name.c_str())
    m_alarms.push_back(time);
    saveToPersistentStorage();
}
 std::list<alarms_t>& EspAlarm::getAlarms() const {
     return m_alarms;
 }

alarms_t EspAlarm::getNextAlarm() {
    std::chrono::system_clock::time_point maxTime{std::chrono::system_clock::time_point::max()};
    std::chrono::system_clock::time_point now{std::chrono::system_clock::now()};
    alarms_t nextAlarm;
    nextAlarm.name = "No alarm";
    nextAlarm.time = maxTime;
    for(auto& alarm: m_alarms)
    {
        if( (alarm.time < nextAlarm.time) 
                && (alarm.time > now)
          ) {
            nextAlarm = alarm;
        }
    }
    return nextAlarm;
}

void EspAlarm::deleteAlarm(alarms_t& alarm) {
    auto equalToRemovingAlarm =
        [alarm](const alarms_t& value)->bool { return value == alarm;};
    m_alarms.remove_if(equalToRemovingAlarm);
    saveToPersistentStorage();
}

void EspAlarm::saveToPersistentStorage() {
    persistentStorage_.setValue<uint32_t>(std::string("count"), static_cast<uint32_t>(m_alarms.size()));
    size_t i = 0;
    for(auto& alarm: m_alarms) {
        SimpleAlarmSerializer serializer;

        persistentStorage_.setValue<std::string>(std::string("Alarm")+std::to_string(i), serializer.serialize(alarm));
        i++;
    }
}

void EspAlarm::loadFromPeristentStorage() {
    size_t alarms = persistentStorage_.getValue<uint32_t>(std::string("count"));
    for (size_t i; i<alarms; i++) {
        std::string current = persistentStorage_.getValue<std::string>(std::string("Alarm")+std::to_string(i));

    }

}
