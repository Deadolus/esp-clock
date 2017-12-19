#pragma once

#include "SntpClient.h"
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
