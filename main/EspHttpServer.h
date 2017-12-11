#pragma once
#include "HttpServer.h"
class EspHttpServer : 
    public HttpServer
{
    public:
        virtual void startServer() override;
};
