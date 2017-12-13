#include "EspWifi.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_system.h"
#include "sdkconfig.h"
#include <string>
#include <cstring>


static const char* TAG = "wifi";

/* The event group allows multiple bits for each event,

   but we only care about one event - are we connected
   to the AP with an IP? */
unsigned int EspWifi::connected_bit_{BIT0};
/* FreeRTOS event group to signal when we are connected & ready to make a request */
EventGroupHandle_t EspWifi::eventgroup_;

EspWifi::EspWifi() {}

void EspWifi::init() {
    tcpip_adapter_init();
    eventgroup_ = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    esp_event_loop_init(event_handler, NULL);
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config{};
    memcpy(wifi_config.sta.ssid, CONFIG_WIFI_SSID, strlen(CONFIG_WIFI_SSID)+1);
    memcpy(wifi_config.sta.password, CONFIG_WIFI_PASSWORD, strlen(CONFIG_WIFI_PASSWORD)+1);
    ESP_LOGI(TAG, "Setting WiFi configuration SSID \"%s\"", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
}
void EspWifi::startWifi() {
    ESP_ERROR_CHECK( esp_wifi_start() );
}

void EspWifi::stopWifi() {
    ESP_ERROR_CHECK( esp_wifi_stop() );
}

bool EspWifi::isConnected() {
return (xEventGroupGetBits(eventgroup_) & connected_bit_) == connected_bit_;
}

void EspWifi::waitForConnection() {
    xEventGroupWaitBits(eventgroup_, connected_bit_, false, true, portMAX_DELAY);
}

esp_err_t EspWifi::event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(eventgroup_, connected_bit_);
    ESP_LOGI(TAG, "Connected");
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently
           auto-reassociate. */
        //esp_wifi_connect();
        xEventGroupClearBits(eventgroup_, connected_bit_);
        break;
    default:
        break;
    }
    return ESP_OK;
}
