#include "ADXLService.h"
#include "ADXL345.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include <thread>
#include <mutex>

namespace {
    static const char* TAG = "AlarmService";
    static std::mutex interruptMutex_{};
}

static void IRAM_ATTR myISR(void *arg) {
    interruptMutex_.unlock();
}
ADXLService::ADXLService() {
    sensor_.initialize();
    sensor_.setMeasureEnabled(true);
    sensor_.setSleepEnabled(false);
    sensor_.setRange(0);
    sensor_.setInterruptMode(0);
    sensor_.setTapThreshold(1/0.0625); //1g tap threshold
    sensor_.setTapDuration(10000.0/625); //10ms tap duration
    sensor_.setTapAxisXEnabled(true);
    sensor_.setTapAxisYEnabled(true);
    sensor_.setIntSingleTapPin(0);//INT1 pin
    //sensor_.setIntSingleTapEnabled(true);
    //sensor_.setTapAxisZEnabled(true);
    gpio_config_t io_conf;
    //interrupt of rising edge
    io_conf.intr_type = static_cast<gpio_int_type_t>(GPIO_PIN_INTR_POSEDGE);
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = 1<<5;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = static_cast<gpio_pullup_t>(0);
    gpio_config(&io_conf);
    gpio_set_intr_type(static_cast<gpio_num_t>(5), GPIO_INTR_POSEDGE);
    //gpio_intr_enable(static_cast<gpio_num_t>(5));
    gpio_install_isr_service(ESP_INTR_FLAG_EDGE|ESP_INTR_FLAG_LOWMED);
    gpio_isr_handler_add(static_cast<gpio_num_t>(5), myISR,nullptr);
    ESP_LOGI(TAG, "Test sensor %d", sensor_.testConnection());
#ifndef GOOGLETEST
    std::thread task(ADXLServiceTask, std::ref(sensor_));
    task.detach();
#endif
}

void ADXLService::ADXLServiceTask(ADXL345& sensor) {

    while(true) {
//        ESP_LOGI(TAG, "Accel: %d, %d, %d, : %f, %f, %f", sensor.getAccelerationX(), sensor.getAccelerationY(), sensor.getAccelerationZ(), sensor.getGX(), sensor.getGY(), sensor.getGZ());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if(!interruptMutex_.try_lock())
        {
        ESP_LOGI(TAG, "Got interrupt!");
        sensor.getIntSingleTapSource(); //clear interrupt

        }
    }
}
