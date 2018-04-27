#pragma once
#include <chrono>
#include <functional>

class Timer {
    public:
        Timer(std::chrono::milliseconds dur, std::function<void()>& callback);
        Timer(std::chrono::seconds dur, std::function<void()>& callback);
    private:
        Timer() = delete;
};
