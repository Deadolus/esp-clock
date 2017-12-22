#pragma once 
#include <chrono>

class Clock {
    public: 
        static time_t getCurrentTimeAsTimet();
        static tm getCurrentTimeAsTm();
        static std::chrono::system_clock::time_point getCurrentTimeAsTimePoint();
        static tm getTm(std::chrono::system_clock::time_point const& time);
        static tm getTm(time_t const& time);
        static time_t getTimet(std::chrono::system_clock::time_point const& time);
        static std::chrono::system_clock::time_point convertToTimePoint(time_t const& time);
        static std::chrono::system_clock::time_point convertToTimePoint(tm& time);
};
