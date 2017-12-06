
#include "EspSign.h"
#include "imagedata.h"

EspSign::EspSign(Display& display) : display_(display) {}

void EspSign::setClock(bool value)  {
    if(value)
        sendImage(clocksign_filled, 0) ;
    else
        sendImage(clocksign, 0) ;
}

void EspSign::setBluetooth(bool value)  {
    if(value)
        sendImage(bluetooth_filled, 2) ;
    else
        sendImage(bluetooth, 2) ;
}

void EspSign::setWifi(bool value)  {
    if(value)
        sendImage(wifi_filled, 1) ;
    else
        sendImage(wifi, 1) ;
}

void EspSign::sendImage(const unsigned char* image, unsigned int location) {
    display_.setImage(image, x_, y_-size_-location*size_, size_, size_);
}
