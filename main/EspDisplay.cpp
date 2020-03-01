#include <string>
#include "EspDisplay.h"
#include "Clock.h"
#include "Alarm.h"
#ifdef RED_DISPLAY
#include <epd2in9b.h>
#else
#include <epd1in54.h>
#endif
#include <epdpaint.h>
#include "esp_log.h"
#include "EspSntpClient.h"
#include <chrono>
#include <mutex>

static const uint8_t COLORED = 0;
static const uint8_t UNCOLORED = 1;
static bool display_initialized{false};
static const int NEXT_ALARM_LINE = EPD_HEIGHT-32-2*Font24.Height;
static const int NEXT_ALARM_TIME_LINE = NEXT_ALARM_LINE+Font24.Height;
static const int NEXT_ALARM_COUNTDOWN_LINE = NEXT_ALARM_TIME_LINE;
static const int ALARM_LINE = NEXT_ALARM_LINE-Font24.Height;
static const char* TAG = "Display";

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
            case Font::Font36:
                return Font36;
        }
        return Font8;
    }

}

EspDisplay::EspDisplay() : paint_(image_, 0, 0)
{
    init();
}

void EspDisplay::setImage(const unsigned char* image, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    std::lock_guard<std::mutex> lock(displayMutex);
#ifdef RED_DISPLAY
    epd_.SetPartialWindowBlack(image, x, y, width, height);
#else
    epd_.SetFrameMemory(image, x, y, width, height);
#endif
    //epd_.DisplayFrame();
    //epd_.SetFrameMemory(image, x, y, width, height);
    //epd_.DisplayFrame();
}
void EspDisplay::setNextAlarmName(const std::string& name, const std::chrono::system_clock::time_point time) {
    //tm alarmTime = Clock::getTm(time);
    std::string alarmName = name.substr(0,10);
    //ESP_LOGI(TAG, "Got name: %s, converted to: %s", name.c_str(), alarmName.c_str());
    clearNextAlarm();
    write((std::string("N:")+alarmName), NEXT_ALARM_LINE, 0, Font::Font24);
}

void EspDisplay::clearNextAlarmName() {
    clearLine(Font24, NEXT_ALARM_LINE);
}
void EspDisplay::clearNextAlarmTime() {
    clearLine(Font24, NEXT_ALARM_TIME_LINE);
}

void EspDisplay::clearNextAlarmCountdown() {
    clearLine(Font24, NEXT_ALARM_COUNTDOWN_LINE);
}

void EspDisplay::setAlarm(std::string alarm) {
    clearAlarm();
    if(alarm.size() > 12){
    alarm = alarm.substr(12);
    }
    write(alarm.c_str(), ALARM_LINE, 0, Font::Font24);
}

void EspDisplay::clearAlarm() {
    clearLine(Font24, ALARM_LINE);
}

void EspDisplay::clearNextAlarm() {
    clearNextAlarmName();
    clearNextAlarmTime();
    clearNextAlarmCountdown();
}

void EspDisplay::showNextAlarmInfo(alarms_t alarm) {
    clearLine(Font24, NEXT_ALARM_COUNTDOWN_LINE);
    if( alarm.nextAlarm() != std::chrono::system_clock::time_point::max()) {
        char buf[16];
        tm alarmTime = Clock::getTm(alarm.nextAlarm());
        sprintf(buf, "%d:%02d", alarmTime.tm_hour, alarmTime.tm_min);
        write(std::string(buf), NEXT_ALARM_TIME_LINE, 100, Font::Font24);
        //ESP_LOGI(TAG, "%s", buf);
    std::chrono::system_clock::time_point now = Clock::getCurrentTimeAsTimePoint();
    auto duration = alarm.nextAlarm() - now;
    //tm alarmTime = Clock::getTm(duration);
    long seconds = std::chrono::duration_cast<std::chrono::minutes>(duration).count()+1;
    sprintf(buf, "%ld:%02ld", seconds/60, seconds%60);
    write(std::string(buf), NEXT_ALARM_COUNTDOWN_LINE, 0, Font::Font24);
    }

}
void EspDisplay::init() {
    std::lock_guard<std::mutex> lock(displayMutex);
    if(!display_initialized) {
#ifdef RED_DISPLAY
        epd_.Init();
#else
        epd_.SpiInit();
        epd_.Init(lut_full_update);
#endif
        display_initialized = true;
    }
  /**
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to clear the frame memory twice.
   */
#ifdef RED_DISPLAY
    epd_.ClearFrame();   // bit set = white, bit reset = black
    epd_.DisplayFrame();
    epd_.ClearFrame();   // bit set = white, bit reset = black
#else
    epd_.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    epd_.DisplayFrame();
    epd_.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
#endif
    epd_.DisplayFrame();
  paint_.SetRotate(ROTATE_0);
}

void EspDisplay::partialUpdate(){
    std::lock_guard<std::mutex> lock(displayMutex);
#ifndef RED_DISPLAY
  if (epd_.Init(lut_partial_update) != 0) {
      printf("e-Paper init failed");
      return;
  }
#endif
}

void EspDisplay::fullUpdate(){
    std::lock_guard<std::mutex> lock(displayMutex);
#ifdef RED_DISPLAY
if(epd_.Init() != 0) {
#else
  if (epd_.Init(lut_full_update) != 0) {
#endif
      printf("e-Paper init failed");
      return;
  }
}

void EspDisplay::write(const std::string& text, unsigned int x, unsigned int y, Font _font) {
    std::lock_guard<std::mutex> lock(displayMutex);

    sFONT font = getFont(_font);
    paint_.Clear(UNCOLORED);
    paint_.SetWidth(font.Height);
    paint_.SetHeight(text.length()*font.Width);
    paint_.SetRotate(ROTATE_270);
    paint_.DrawStringAt(0, font.Height/8, text.c_str(), &font, COLORED);
#ifdef RED_DISPLAY
    epd_.SetPartialWindowBlack(paint_.GetImage(), x-font.Height, y, paint_.GetWidth(), paint_.GetHeight());
#else
    epd_.SetFrameMemory(paint_.GetImage(), x-font.Height, y, paint_.GetWidth(), paint_.GetHeight());
#endif
    //epd_.DisplayFrame();
}

/** sends whole paint_ image to display */
void EspDisplay::send() {
    std::lock_guard<std::mutex> lock(displayMutex);
  epd_.DisplayFrame();

}
void EspDisplay::sleep() {
    std::lock_guard<std::mutex> lock(displayMutex);
    epd_.Sleep();
}

void EspDisplay::clearLine(sFONT font, unsigned int x) {
    std::string temp;
    for(size_t i{0}; i<=(EPD_WIDTH/font.Width); i++)
        temp.append(" ");
    write(temp, x, 0, Font::Font24);
    }

void EspDisplay::setTime(EspSntpClient& sntp) {
    char strftime_buf[64];
    struct tm timeinfo = Clock::getCurrentTimeAsTm();
    sFONT font = getFont(Font::Font36);
    //std::strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    if(sntp.timeSet())
        strftime(strftime_buf, sizeof(strftime_buf), "%R", &timeinfo);
    else
        sprintf(strftime_buf, "00:00");
    ESP_LOGI(TAG, "Current time: %s", strftime_buf);

    //write(std::string(strftime_buf), 200, 0, Font::Font36);
    write(std::string(strftime_buf), (EPD_WIDTH-font.Height)/2, (EPD_HEIGHT-(5*font.Width))/2, Font::Font36);
}
