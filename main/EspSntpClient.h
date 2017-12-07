#pragma once

#include "SntpClient.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
class Wifi;

class EspSntpClient : 
    public SntpClient {
        public:
            EspSntpClient() = delete;
            EspSntpClient(Wifi& wifi);
            virtual void getTime(bool turnOffWifiAfterwards) override;
            virtual bool timeSet() override;
        private:
            Wifi& wifi_;
    };
