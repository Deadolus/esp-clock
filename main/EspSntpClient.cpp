#include "EspSntpClient.h"
#include "apps/sntp/sntp.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

static char* NTP_SERVER = "pool.ntp.org";
static const char* TAG = "sntp";

EspSntpClient::EspSntpClient(unsigned int connected_bit, EventGroupHandle_t& eventgroup) : connected_bit_(connected_bit),eventgroup_(eventgroup)  {}

void EspSntpClient::getTime(bool turnOffWifiAfterwards) {
    ESP_LOGI(TAG, "Waiting for CONNECTED_BIT\n");
    xEventGroupWaitBits(eventgroup_, connected_bit_,
            false, true, portMAX_DELAY);

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, NTP_SERVER);
    sntp_init();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = {};
    int retry = 0;
    const int retry_count = 10;
    while(timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }
    if(turnOffWifiAfterwards) {
        ESP_LOGI(TAG, "Stopping WIFI");
        ESP_ERROR_CHECK( esp_wifi_stop() );
    }
}
