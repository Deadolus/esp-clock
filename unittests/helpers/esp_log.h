#pragma once

#include <functional>
#include <stdarg.h>

auto ESP_LOGI = [](const char* tag, const char* message, ...) {
    va_list args;
    va_start(args, message);
    printf("%s: ", tag);
    //printf("%s: %s\n", tag, message, args);
    vprintf(message, args);
    printf("\n");
    va_end(args);
};

