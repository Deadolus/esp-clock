#pragma once
#include "HttpServer.h"
#include "Alarm.h"
#include <cstdint>
#include <mutex>
#include "PwmLed.h"
class EspHttpServer :
    public HttpServer
{
    public:
        EspHttpServer() = delete;
        EspHttpServer(Alarm& alarms, PwmLed& led);
        virtual void startServer() override;
        static int32_t ssi_handler(int32_t iIndex, char *pcInsert, int32_t iInsertLen);
        static const char *newAlarm_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
        static const char *delete_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
        static PwmLed& getInstanceLed();
    private:
        static Alarm& getInstanceAlarms();
        static EspHttpServer* getInstance();
        Alarm& alarms_;
        PwmLed& led_;
        static EspHttpServer* instance_;
};
