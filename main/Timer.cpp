#include "Timer.h"
#include <chrono>
#include <functional>
#include <thread>
#include "esp_log.h"

static const char *TAG = "timer";

namespace {
    void timerTask(std::chrono::milliseconds time, std::function<void()> callback)
    {
        ESP_LOGI(TAG, "Before sleep");
        std::this_thread::sleep_for(time);
        if(callback)
            callback();
        ESP_LOGI(TAG, "After sleep");
    }
}

Timer::Timer(std::chrono::milliseconds time, std::function<void()> callback) {
    std::thread task(timerTask, time, callback);
    task.detach();
}

Timer::Timer(std::chrono::seconds time, std::function<void()> callback) :
    Timer(std::chrono::milliseconds(time), callback)
{
}
