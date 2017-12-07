#pragma once

#include "SntpClient.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

class EspSntpClient : 
    public SntpClient {
        public:
            EspSntpClient() = delete;
            EspSntpClient(unsigned int connected_bit, EventGroupHandle_t& eventgroup);
            virtual void getTime(bool turnOffWifiAfterwards) override;
        private:
            unsigned int connected_bit_;
            EventGroupHandle_t& eventgroup_;
    };
