#pragma once

#include "PwmLed.h"

#include "driver/ledc.h"

class EspPwmLed :
    public PwmLed
{
    public:
        EspPwmLed() = delete;
        EspPwmLed(unsigned int gpio);
        /** set intensity in percentage (0-100) */
        virtual void setIntensity(unsigned int intensity) override;
        virtual void setOff() override;
        virtual void setOn() override;
        virtual unsigned int getIntensity() override;
    private:
        unsigned int intensity_{};
        ledc_channel_config_t ledChannel_{};
};
