#pragma once

#include "Wifi.h"
#include "esp_event.h"

class EspWifi :
    public Wifi
{
    public:
        EspWifi();
        virtual void init() override;
        virtual void startWifi()  override;
        virtual void stopWifi()  override;
        virtual bool isConnected() override;
        virtual void waitForConnection() override;
    private:
        static esp_err_t event_handler(void *ctx, system_event_t *event);
        static bool connected_bit_;
};

