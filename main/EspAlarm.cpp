#include "EspAlarm.h"
#include "esp_log.h"
#include "SimpleAlarmSerializer.h"
#include "Clock.h"
#include <list>
#include <string>
static const char* TAG = "Alarm";
static unsigned int MAX_ALARMS = 10;

std::vector<alarms_t> EspAlarm::m_alarms{};

void EspAlarm::setAlarm(alarms_t const& alarm) {
    ESP_LOGI(TAG, "Got new alarm, name: %s", alarm.name.c_str())
        if(m_alarms.size() <= MAX_ALARMS) {
            m_alarms.push_back(alarm);
            saveToPersistentStorage();
        }
}
 std::vector<alarms_t>& EspAlarm::getAlarms() const {
     return m_alarms;
 }

const alarms_t EspAlarm::getNextAlarm() const {
    std::chrono::system_clock::time_point maxTime{std::chrono::system_clock::time_point::max()};
    std::chrono::system_clock::time_point now{std::chrono::system_clock::now()};
    alarms_t nextAlarm;
    nextAlarm.name = "No alarm";
    nextAlarm.time = maxTime;
    auto alarmComparison = [&](alarms_t& alarm)->bool {
        return (alarm.time < nextAlarm.time)
            && (alarm.time > now);
    };
    auto repeatedComparison = [](alarms_t& alarm)->bool {
        tm alarmTime = Clock::getTm(alarm.time);
        alarmTime.tm_wday
        
        return true;
    };
    for(auto& alarm: m_alarms)
    {
        if( alarmComparison(alarm) || repeatedComparison(alarm) ) 
           {
            nextAlarm = alarm;
           }
    }
    return nextAlarm;
}

void EspAlarm::deleteAlarm(alarms_t& alarm) {
    auto equalToRemovingAlarm =
        [alarm](const alarms_t& value)->bool { return value == alarm;};
    //m_alarms.remove_if(equalToRemovingAlarm);
    //Todo: use m_alarms.erase
    saveToPersistentStorage();
}

void EspAlarm::deleteAlarm(unsigned int position) {
    auto it = m_alarms.begin();
    std::advance(it, position);
    if(it != m_alarms.end()) {
        std::string alarmNr = std::string("Alarm")+std::to_string(std::distance(m_alarms.begin(), it));
        //persistentStorage_.erase(alarmNr);
        m_alarms.erase(it);
        saveToPersistentStorage();
    }
}

void EspAlarm::saveToPersistentStorage() {
    ESP_LOGI(TAG, "Going to save %u alarms", m_alarms.size());
    persistentStorage_.setValue<uint32_t>("count", static_cast<uint32_t>(m_alarms.size()));
    //size_t i{};
    SimpleAlarmSerializer serializer;
    std::string alarmString{"Alarm"};
    //for(auto const& alarm: m_alarms) {
    for(size_t i=0; i<m_alarms.size(); i++) {
        //std::string alarmNr = std::string("Alarm");
        //alarmNr.append(std::to_string(i));
        std::string alarmNr = alarmString+std::to_string(i);
        std::string serializedAlarm = serializer.serialize(m_alarms.at(i));
         ESP_LOGI(TAG, "Saving to storage..."); 
        persistentStorage_.setValue<std::string>(alarmNr.c_str(), serializedAlarm.c_str());
        ESP_LOGI(TAG, "Serialized and saved alarm: %s/%s", alarmNr.c_str(), serializedAlarm.c_str()); 
    }
}

void EspAlarm::loadFromPeristentStorage() {
    //persistentStorage_.setValue<uint32_t>(std::string("count"), 1);
    //persistentStorage_.setValue<std::string>(std::string("Alarm0"), "Alarm,1337,10,0");
    size_t alarms = persistentStorage_.getValue<uint32_t>(std::string("count"));
    ESP_LOGI(TAG, "There were %u saved alarms", alarms);
    for (size_t i=0; i<alarms; i++) {
        SimpleAlarmSerializer serializer;
        std::string alarmNr = std::string("Alarm")+std::to_string(i);
        std::string current = persistentStorage_.getValue<std::string>(alarmNr.c_str());
        if(current.compare("")!=0) {
        ESP_LOGI(TAG, "Deserializing alarm: %s", current.c_str());
        m_alarms.push_back(serializer.deserialize(current));
        ESP_LOGI(TAG, "Loaded alarm: %s", m_alarms.back().name.c_str());
        } else {
            ESP_LOGI(TAG, "Detected empty alarm %s", alarmNr.c_str());
        }
    }

}
void EspAlarm::saveAlarms() {
    saveToPersistentStorage();
}
