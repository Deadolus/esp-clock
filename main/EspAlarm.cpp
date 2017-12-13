#include "EspAlarm.h"
#include <list>

std::list<alarms_t> EspAlarm::m_alarms{};

void EspAlarm::setAlarm(alarms_t& time) {
    m_alarms.push_back(time);

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
