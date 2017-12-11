#pragma once

#include "Button.h"

#include <functional>

class EspButton : 
    public Button 
{
    public: 
        EspButton() = delete;
        EspButton(unsigned int gpio, bool inverse);
        virtual bool pressed() override;
        virtual bool longPress() override;
        virtual void setPressCb(std::function<void()>) override;
        virtual void setLongPressCb(std::function<void()>) override;
    private:
        unsigned int gpio_;
        bool inverse_;
        std::function<void()> pressCb_;
        std::function<void()> longPressCb_;
};

