#pragma once

#include "Button.h"

class EspButton : 
    public Button 
{
    public: 
        EspButton() = delete;
        EspButton(unsigned int gpio, bool inverse);
        virtual bool pressed() override;
    private:
        unsigned int gpio_;
        bool inverse_;
};

