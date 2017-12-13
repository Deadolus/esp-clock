#include "EspPwmLed.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"
#include <cmath>

static const ledc_timer_t LEDC_TIMER = LEDC_TIMER_0;
static const ledc_mode_t LEDC_MODE{LEDC_LOW_SPEED_MODE};
static const unsigned int LEDC_HS_CH0_CHANNEL = LEDC_CHANNEL_0;
static const unsigned int LEDC_TEST_CH_NUM = 4;
static const char* TAG = "PwmLed";

EspPwmLed::EspPwmLed(unsigned int gpio) {
    ledc_timer_config_t ledc_timer{};
    gpio_pad_select_gpio(CONFIG_LED_GPIO);
    gpio_set_direction(static_cast<gpio_num_t>(CONFIG_LED_GPIO), GPIO_MODE_OUTPUT);
    //ledc_timer.duty_resolution = LEDC_TIMER_13_BIT; // resolution of PWM duty
    ledc_timer.duty_resolution = LEDC_TIMER_10_BIT;
        ledc_timer.freq_hz = 5000;                      // frequency of PWM signal
        ledc_timer.speed_mode = LEDC_MODE;           // timer mode
        ledc_timer.timer_num = LEDC_TIMER;            // timer index
            // Set configuration of timer0 for high speed channels
            ledc_timer_config(&ledc_timer);
            ledChannel_.channel = static_cast<ledc_channel_t>(LEDC_HS_CH0_CHANNEL);
            ledChannel_.duty = 0;
            ledChannel_.gpio_num = gpio;
            ledChannel_.speed_mode = LEDC_MODE;
            ledChannel_.timer_sel = LEDC_TIMER;
            ledc_channel_config(&ledChannel_);
            ledc_fade_func_install(0);
}

void EspPwmLed::setIntensity(unsigned int intensity) {
    unsigned int duty = std::pow(2,10) * intensity/100.0;
    //ESP_LOGI(TAG, "Setting intensity to %u, duty: %u",intensity, duty);
    ledc_set_duty(ledChannel_.speed_mode, ledChannel_.channel, duty);
    ledc_update_duty(ledChannel_.speed_mode, ledChannel_.channel);

}

void EspPwmLed::setOff() {
//    set_duty may only support HS speed
            ledc_set_duty(ledChannel_.speed_mode, ledChannel_.channel, 0);
            ledc_update_duty(ledChannel_.speed_mode, ledChannel_.channel);
}

void EspPwmLed::setOn() {

}
