#pragma once


class SntpClient {
    public:
        virtual void getTime(bool turnOffWifiAfterwards) = 0;
};
