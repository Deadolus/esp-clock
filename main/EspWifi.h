#pragma once

#include "Wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event.h"

class EspWifi :
    public Wifi
{
    public:
        EspWifi();
        virtual void startWifi()  override;
        virtual void stopWifi()  override;
        virtual bool isConnected() override;
        virtual void waitForConnection() override;
    private:
        static esp_err_t event_handler(void *ctx, system_event_t *event);
        static unsigned int connected_bit_;
        static EventGroupHandle_t eventgroup_;
};

