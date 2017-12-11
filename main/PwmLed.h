#pragma once

class PwmLed {
    public:
        virtual void setIntensity(unsigned int intensity) = 0;
        virtual void setOff() = 0;
        virtual void setOn() = 0;
};
