#include "ADXLService.h"
#include "ADXL345.h"
#include "esp_system.h"
#include "esp_log.h"

namespace {
static const char* TAG = "AlarmService";
}

ADXLService::ADXLService() {
    ESP_LOGI(TAG, "Sensor Device Id: %u", sensor_.getDeviceID());
    ESP_LOGI(TAG, "Tap threshold: %u", sensor_.getTapThreshold());
}
