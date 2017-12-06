#include "EspDisplay.h"
#include <string>
#include <epd1in54.h>
#include <epdpaint.h>

static const uint8_t COLORED = 0;
static const uint8_t UNCOLORED = 1;
static bool display_initialized{false};

EspDisplay::EspDisplay() : paint_(image_, 0, 0)
{
    init();
}

void EspDisplay::setImage(const unsigned char* image, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    epd_.SetFrameMemory(image, x, y, width, height);
    epd_.DisplayFrame();
    epd_.SetFrameMemory(image, x, y, width, height);
    epd_.DisplayFrame();
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

void EspDisplay::write(const std::string& text, unsigned int x, unsigned int y, Font font) {

  paint_.Clear(UNCOLORED);
  paint_.SetWidth(32);
  paint_.SetHeight(96);
  paint_.SetRotate(ROTATE_270);
  paint_.DrawStringAt(0, 4, text.c_str(), &Font24, COLORED);
  epd_.SetFrameMemory(paint_.GetImage(), 130, 0, paint_.GetWidth(), paint_.GetHeight());
  epd_.DisplayFrame();
  epd_.SetFrameMemory(paint_.GetImage(), 130, 0, paint_.GetWidth(), paint_.GetHeight());
  epd_.DisplayFrame();
}

void EspDisplay::send() {
  epd_.DisplayFrame();
  epd_.DisplayFrame();
}
