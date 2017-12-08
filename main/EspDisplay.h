#pragma once
#include "Display.h"

#include <epd1in54.h>
#include <epdpaint.h>

class EspDisplay :
    public Display
{
    public:
        EspDisplay();
        virtual void init() override;
        virtual void setImage(const unsigned char* image, unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
        virtual void partialUpdate() override;
        virtual void fullUpdate() override;
        virtual void write(const std::string& text, unsigned int x, unsigned int y, Font font) override;
        virtual void send() override;
        virtual void sleep() override;
    private:
        unsigned char image_[1024];
        Paint paint_{image_, 0, 0};    // width should be the multiple of 8 
        Epd epd_;
};
