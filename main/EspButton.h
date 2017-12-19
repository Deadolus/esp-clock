#pragma once

#include "Button.h"

#include <functional>

class EspButton : 
    public Button 
{
    public: 
        EspButton() = delete;
        EspButton(unsigned int gpio, bool inverse);
        ~EspButton();
        virtual bool pressed() override;
        virtual bool longPress() override;
        virtual bool extraLongPress() override;
        virtual void setPressCb(std::function<void()>) override;
        virtual void setLongPressCb(std::function<void()>) override;
        virtual void setExtraLongPressCb(std::function<void()>) override;
    private:
        unsigned int gpio_{};
        bool inverse_{};
        uint8_t pressedTime{};
        std::function<void()> pressCb_{};
        std::function<void()> longPressCb_{};
        std::function<void()> extraLongPressCb_{};
        static void buttonTask(void *pvParameters);
};

