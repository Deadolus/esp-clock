#include "EspButton.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include <thread>

static const char* TAG = "Button";
static const unsigned int SHORT_PRESS = 5;
static const unsigned int LONG_PRESS = 50;
static const unsigned int EXTRA_LONG_PRESS = 200;

void EspButton::buttonTask(void *pvParameters) {
    EspButton* button = static_cast<EspButton*>(pvParameters);
    while(true) {
        if(gpio_get_level(static_cast<gpio_num_t>(button->gpio_)) ^ button->inverse_) {
            if(button->pressedTime<255) button->pressedTime++;
        } else {
            //events should be ordered by long->short presses
            //so that only one event gets fired
            if(button->extraLongPress() && button->longPressCb_)
                button->extraLongPressCb_();
            else if(button->longPress() && button->longPressCb_)
                button->longPressCb_();
            else if(button->pressed() && button->pressCb_)
                button->pressCb_();

            //reset pressed Time
            button->pressedTime = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

EspButton::EspButton(unsigned int gpio, bool inverse): gpio_(gpio), inverse_(inverse) {
    gpio_config_t io_conf{};
    //TODO: will probably overwrite existing button configurations...
    io_conf.pin_bit_mask = (1<<gpio_);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = static_cast<gpio_pullup_t>(0);
    gpio_config(&io_conf);
    std::thread task(EspButton::buttonTask, this);
    task.detach();

    ESP_LOGI(TAG, "Started button task");
    //returned = xTaskCreate(&button_Task, "buttontask", 4000, NULL, 2, &buttonTaskHandle);
    //returned = xTaskCreate(&button_Task, "buttontask", 4000, NULL, 2, NULL);
    /* if(returned != pdPASS) { */
    /*     ESP_LOGI(TAG, "Error! Could not create button task"); */
    /* } */
}

EspButton::~EspButton() {
}

bool EspButton::pressed() {
    //return gpio_get_level(static_cast<gpio_num_t>(gpio_)) ^ inverse_;
    return pressedTime >= SHORT_PRESS;

}

bool EspButton::longPress() {
    return pressedTime >= LONG_PRESS;
}

bool EspButton::extraLongPress() {
    return pressedTime >= EXTRA_LONG_PRESS;
}

void EspButton::setPressCb(std::function<void()> function) {
    pressCb_ = function;
}

void EspButton::setLongPressCb(std::function<void()> function) {
    longPressCb_ = function;
}

void EspButton::setExtraLongPressCb(std::function<void()> function) {
    longPressCb_ = function;
}
