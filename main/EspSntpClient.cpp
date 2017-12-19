#include "EspSntpClient.h"
#include "apps/sntp/sntp.h"
#include "esp_log.h"
#include "EspWifi.h"
#include <thread>

static char* NTP_SERVER = "pool.ntp.org";
static const char* TAG = "sntp";

EspSntpClient::EspSntpClient(Wifi& wifi) : wifi_(wifi) {}

void EspSntpClient::getTime(bool turnOffWifiAfterwards) {
    ESP_LOGI(TAG, "Waiting for connection...\n");
    wifi_.waitForConnection();

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, NTP_SERVER);
    sntp_init();

    // wait for time to be set
    int retry = 0;
    const int retry_count = 10;
    while(!timeSet() && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    if(turnOffWifiAfterwards) {
        ESP_LOGI(TAG, "Stopping WIFI");
        wifi_.stopWifi();
    }
}

bool EspSntpClient::timeSet() {
    time_t now = 0;
    struct tm timeinfo = {};
    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    return timeinfo.tm_year > (2016 - 1900);
}
