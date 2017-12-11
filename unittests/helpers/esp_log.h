#pragma once
#include <functional>

//std::function<void(const char* tag, const char* string, ...)> ESP_LOGI = [](const char* tag, const char* string, ...) {
auto ESP_LOGI = [](const char* tag, const char* string, ...) {
printf("%s", tag);
};
