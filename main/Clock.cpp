#include "Clock.h"
#include <chrono>


time_t Clock::getCurrentTimeAsTimet() {
    time_t now{0};
    time(&now);
    return now;
}

tm Clock::getCurrentTimeAsTm() {
    return getTm(getCurrentTimeAsTimet());
}

std::chrono::system_clock::time_point Clock::getCurrentTimeAsTimePoint() {
    return std::chrono::system_clock::now();
}

tm Clock::getTm(std::chrono::system_clock::time_point time) {
    tm converted{};
    time_t convertedTimet = getTimet(time);
    converted = *localtime(&convertedTimet);
    return converted;

}
tm Clock::getTm(time_t time) {
    return *localtime(&time);
}

time_t Clock::getTimet(std::chrono::system_clock::time_point time) {
    time_t converted = std::chrono::system_clock::to_time_t(time);
    return converted;
}

std::chrono::system_clock::time_point Clock::convertToTimePoint(time_t time) {
    return std::chrono::system_clock::from_time_t(time);
}

std::chrono::system_clock::time_point Clock::convertToTimePoint(tm time) {
    return convertToTimePoint(std::mktime(&time));
}
