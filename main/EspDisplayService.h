#pragma once

#include "DisplayService.h"

class EspDisplay;
class EspSign;
class EspAlarm;
class EspAlarmService;
class EspWifi;
class EspSntpClient;

class EspDisplayService : 
    public DisplayService
{
    public:
        EspDisplayService() = delete;
        EspDisplayService(EspDisplay& display, EspSign& espsign, EspAlarm& alarm, EspAlarmService& alarms, EspWifi& wifi, EspSntpClient& sntp, unsigned int delay );
    private:
        static void displayServiceTask(unsigned int delay, EspDisplay& display, EspSign& espsign, EspAlarm& alarm, EspAlarmService& alarms, EspWifi& wifi, EspSntpClient& sntp );
            unsigned int delay_;
};
