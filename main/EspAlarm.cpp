#include "EspAlarm.h"
#include <list>

std::list<alarms_t> EspAlarm::m_alarms{};

void EspAlarm::setAlarm(alarms_t& time) {
    m_alarms.push_back(time);

}
 std::list<alarms_t>& EspAlarm::getAlarms() const {
     return m_alarms;
 }
