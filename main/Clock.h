#pragma once 
#include <chrono>

class Clock {
    public: 
        static time_t getCurrentTimeAsTimet();
        static tm getCurrentTimeAsTm();
        static std::chrono::system_clock::time_point getCurrentTimeAsTimePoint();
        static tm getTm(std::chrono::system_clock::time_point time);
        static tm getTm(time_t time);
        static time_t getTimet(std::chrono::system_clock::time_point time);
        static std::chrono::system_clock::time_point convertToTimePoint(time_t time);
        static std::chrono::system_clock::time_point convertToTimePoint(tm time);
};
