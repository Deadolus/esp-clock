#pragma once
#include "ADXL345.h"

class ADXLService {
    public: 
        ADXLService();
    private:
        static void ADXLServiceTask(ADXL345& sensor);
        ADXL345 sensor_;
};
