#include "Timer.h"
#include <chrono>
#include <functional>
#include <thread>

namespace {
    void timerTask(std::chrono::milliseconds time, std::function<void()>& callback)
    {
        std::this_thread::sleep_for(time);
        if(callback)
            callback();
    }
}

Timer::Timer(std::chrono::milliseconds time, std::function<void()> callback) {
    std::thread task(timerTask, time, std::ref(callback));
    task.detach();
}

Timer::Timer(std::chrono::seconds time, std::function<void()> callback) :
    Timer(std::chrono::milliseconds(time), callback)
{
}
