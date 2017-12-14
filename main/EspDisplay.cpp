#include <string>
#include "EspDisplay.h"
#include "Clock.h"
#include "Alarm.h"
#include <epd1in54.h>
#include <epdpaint.h>
#include <chrono>

static const uint8_t COLORED = 0;
static const uint8_t UNCOLORED = 1;
static bool display_initialized{false};
static const int NEXT_ALARM_LINE = EPD_HEIGHT-32;
static const int ALARM_LINE = 150;
namespace {
    sFONT getFont(Font font) {
        switch(font) 
        {
            case Font::Font8:
                return Font8;
            case Font::Font16:
                return Font16;
            case Font::Font24:
                return Font24;
        }
        return Font8;
    }

}

EspDisplay::EspDisplay() : paint_(image_, 0, 0)
{
    init();
}

void EspDisplay::setImage(const unsigned char* image, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    //paint_.SetFrameMemory(image, x, y, width, height);
    epd_.SetFrameMemory(image, x, y, width, height);
    //epd_.DisplayFrame();
    //epd_.SetFrameMemory(image, x, y, width, height);
    //epd_.DisplayFrame();
}
void EspDisplay::setNextAlarmName(std::string alarm, std::chrono::system_clock::time_point time) {
    tm alarmTime = Clock::getTm(time);
    clearNextAlarmName();
    if(alarm.size() > 10){
    alarm = alarm.substr(10);
    }
    char buf[10];
    sprintf(buf, "%d:%d", alarmTime.tm_hour, alarmTime.tm_min);
    //alarm.insert(0, std::string(buf));
    //std::to_string is not found somehow
    write(std::string(buf), NEXT_ALARM_LINE-Font24.Height, 0, Font::Font24);
    write((std::string("N:")+alarm).c_str(), NEXT_ALARM_LINE, 0, Font::Font24);
}

void EspDisplay::clearNextAlarmName() {
    clearLine(Font24, EPD_HEIGHT-32);
}

void EspDisplay::setAlarm(std::string alarm) {
    clearAlarm();
    if(alarm.size() > 12){
    alarm = alarm.substr(12);
    }
    write(alarm.c_str(), 150, 0, Font::Font24);
}

void EspDisplay::clearAlarm() {
    clearLine(Font24, 150);
}

void EspDisplay::showNextAlarmInfo(alarms_t alarm) {
    char buf[10];
    tm alarmTime = Clock::getTm(alarm.time);
    sprintf(buf, "%d:%d", alarmTime.tm_hour, alarmTime.tm_min);
    write(std::string(buf), NEXT_ALARM_LINE-2*Font24.Height, 0, Font::Font24);

}
void EspDisplay::init() {
    if(!display_initialized) {
        epd_.SpiInit();
        epd_.Init(lut_full_update);
        display_initialized = true;
    }
  /** 
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to clear the frame memory twice.
   */
  epd_.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd_.DisplayFrame();
  epd_.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd_.DisplayFrame();
  paint_.SetRotate(ROTATE_0); 
}

void EspDisplay::partialUpdate(){
  if (epd_.Init(lut_partial_update) != 0) {
      printf("e-Paper init failed");
      return;
  }
}

void EspDisplay::fullUpdate(){
  if (epd_.Init(lut_full_update) != 0) {
      printf("e-Paper init failed");
      return;
  }
}

void EspDisplay::write(const std::string& text, unsigned int x, unsigned int y, Font _font) {

    sFONT font = getFont(_font);
    paint_.Clear(UNCOLORED);
    paint_.SetWidth(font.Height);
    paint_.SetHeight(text.length()*font.Width);
    paint_.SetRotate(ROTATE_270);
    paint_.DrawStringAt(0, font.Height/8, text.c_str(), &font, COLORED);
    //epd_.SetFrameMemory(paint_.GetImage(), x-font.Height, y, paint_.GetWidth(), paint_.GetHeight());
    //epd_.DisplayFrame();
    epd_.SetFrameMemory(paint_.GetImage(), x-font.Height, y, paint_.GetWidth(), paint_.GetHeight());
    //epd_.DisplayFrame();
}

/** sends whole paint_ image to display */
void EspDisplay::send() {
  epd_.DisplayFrame();

}
void EspDisplay::sleep() {
    epd_.Sleep();
}

void EspDisplay::clearLine(sFONT font, unsigned int x) {
    std::string temp;
    for(size_t i{0}; i<=(EPD_WIDTH/font.Width); i++)
        temp.append(" ");
    write(temp, x, 0, Font::Font24);
    }
