#pragma once

class Wifi 
{
    public:
        virtual void init() = 0;
        virtual void startWifi() = 0;
        virtual void stopWifi() = 0;
        virtual bool isConnected() = 0;
        virtual void waitForConnection() = 0;
};
