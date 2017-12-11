#include "EspButton.h"
#include "driver/gpio.h"

EspButton::EspButton(unsigned int gpio, bool inverse): gpio_(gpio), inverse_(inverse) {
    gpio_config_t io_conf{};
    //TODO: will probably overwrite existing button configurations...
    io_conf.pin_bit_mask = (1<<gpio_);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = static_cast<gpio_pullup_t>(0);
    gpio_config(&io_conf);
}

bool EspButton::pressed() {
    return gpio_get_level(static_cast<gpio_num_t>(gpio_)) ^ inverse_;

}
