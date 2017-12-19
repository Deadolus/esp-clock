#pragma once
#include "HttpServer.h"
#include "Alarm.h"
#include <cstdint>
class EspHttpServer : 
    public HttpServer
{
    public:
        EspHttpServer() = delete;
        EspHttpServer(Alarm& alarms);
        virtual void startServer() override;
        static int32_t ssi_handler(int32_t iIndex, char *pcInsert, int32_t iInsertLen);
        static const char *newAlarm_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
    private:
        static EspHttpServer* getInstance();
        static Alarm& getInstanceAlarms();
        Alarm& alarms_;
        static EspHttpServer* instance_;
};
