#pragma once

#include "Alarm.h"
#include "EspSntpClient.h"
#include <string>
#include <chrono>

enum class Font {
    Font8, 
    Font16, 
    Font24
};
class Display 
{
    public:
        virtual void init() = 0;
        virtual void setImage(const unsigned char* image, unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;
        virtual void setNextAlarmName(std::string alarm, std::chrono::system_clock::time_point time) = 0;
        virtual void clearNextAlarmName() = 0;
        virtual void setAlarm(std::string alarm) = 0;
        virtual void clearAlarm() = 0;
        virtual void showNextAlarmInfo(alarms_t alarm) = 0;
        virtual void setTime(EspSntpClient& sntp) = 0;
        virtual void partialUpdate() = 0;
        virtual void fullUpdate() = 0;
        virtual void write(const std::string& text, unsigned int x, unsigned int y, Font font) = 0;
        virtual void send() = 0;
        virtual void sleep() = 0;
};
