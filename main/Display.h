#pragma once

#include <string>

enum class Font {
    Font8, 
    Font16, 
    Font24
};
class Display 
{
    public:
        virtual void init() = 0;
        virtual void setImage(const unsigned char* image, unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;
        virtual void partialUpdate() = 0;
        virtual void fullUpdate() = 0;
        virtual void write(const std::string& text, unsigned int x, unsigned int y, Font font) = 0;
        virtual void send() = 0;
};
