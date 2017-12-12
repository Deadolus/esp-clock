#pragma once
#include "HttpServer.h"
class EspHttpServer : 
    public HttpServer
{
    public:
        //EspHttpServer() = delete;
        //EspHttpServer(Alarm& alarms);
        virtual void startServer() override;
    private:
        //Alarm& alarms_;
};
