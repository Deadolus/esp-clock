#pragma once
#include <functional>

class Button {
    public: 
        virtual bool pressed() = 0;
        virtual bool longPress() = 0;
        virtual void setPressCb(std::function<void()>) = 0;
        virtual void setLongPressCb(std::function<void()>) = 0;
};
