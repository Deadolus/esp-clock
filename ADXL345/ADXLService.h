#pragma once
#include "ADXL345.h"

class ADXLService {
    public: 
        ADXLService();
    private:
        ADXL345 sensor_;
};
