
#pragma once
#include "Sign.h"
#include "Display.h"

class EspSign :
    public Sign
{
    public:
        EspSign(Display& display);
        EspSign() = delete;
        virtual void setClock(bool value) override;
        virtual void setBluetooth(bool value) override;
        virtual void setWifi(bool value) override;
    private:
        void sendImage(const unsigned char* image, unsigned int location);
        Display& display_;
#ifdef RED_DISPLAY
        unsigned int x_ {100}; //max 128
        unsigned int y_ {3*32}; //max 296
#else
        unsigned int x_ {168};
        unsigned int y_ {200};
#endif
        unsigned int size_ {32};
};
