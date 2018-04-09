#include "ADXLService.h"
#include "ADXL345.h"
#include "esp_system.h"
#include "esp_log.h"

#include <thread>

namespace {
    static const char* TAG = "AlarmService";
}

ADXLService::ADXLService() {
    sensor_.initialize();
    sensor_.setMeasureEnabled(true);
    sensor_.setSleepEnabled(false);
    sensor_.setRange(0);
    ESP_LOGI(TAG, "Test sensor %d", sensor_.testConnection());
#ifndef GOOGLETEST
    std::thread task(ADXLServiceTask, std::ref(sensor_));
    task.detach();
#endif
}

void ADXLService::ADXLServiceTask(ADXL345& sensor) {

    while(true) {
        ESP_LOGI(TAG, "Accel: %d, %d, %d", sensor.getAccelerationX(), sensor.getAccelerationY(), sensor.getAccelerationZ());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
