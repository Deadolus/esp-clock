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
    //std::chrono::system_clock::time_point next{std::chrono::system_clock::time_point::max()};
    alarms_t nextAlarm = m_alarms.front();
    for(auto& alarm: m_alarms)
    {
        if(alarm.time < nextAlarm.time)
            nextAlarm = alarm;
    }
    return nextAlarm;
}
