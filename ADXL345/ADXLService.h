#pragma once
#include "ADXL345.h"
#include <functional>

class ADXLService {
    public: 
        explicit ADXLService(std::function<void()> func);
    private:
        static void ADXLServiceTask(ADXL345& sensor, std::function<void()> cb);
        ADXL345 sensor_;
};
