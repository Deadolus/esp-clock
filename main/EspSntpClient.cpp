#include "EspSntpClient.h"
#include "lwip/apps/sntp.h"
#include "esp_log.h"
#include "EspWifi.h"
#include <thread>

static const char* NTP_SERVER = "pool.ntp.org";
static const char* TAG = "sntp";

EspSntpClient::EspSntpClient(Wifi& wifi) : wifi_(wifi) {}

void EspSntpClient::getTimeTask(const bool turnOffWifiAfterwards, void* pvParameters) {
  //Wifi* wifi_ = static_cast<Wifi*>(pvParameters);
  EspSntpClient* espSntpClient = static_cast<EspSntpClient*>(pvParameters);
  ESP_LOGI(TAG, "Waiting for connection...\n");
  //wifi_.waitForConnection();

  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, const_cast<char*>(NTP_SERVER));
  sntp_init();

  // wait for time to be set
  int retry = 0;
  const int retry_count = 10;
  while(!espSntpClient->timeSet() && ++retry < retry_count) {
    ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
  if(turnOffWifiAfterwards) {
    ESP_LOGI(TAG, "Stopping WIFI");
    espSntpClient->wifi_.stopWifi();
  }
  ESP_LOGI(TAG, "%s time getting", espSntpClient->timeSet() ? "Sucessful" : "Unsucessful" );
}

void EspSntpClient::getTime(bool turnOffWifiAfterwards) {
  std::thread task(EspSntpClient::getTimeTask, turnOffWifiAfterwards, this);
  task.detach();
}

bool EspSntpClient::timeSet() {
  time_t now = 0;
  struct tm timeinfo = {};
  time(&now);
  localtime_r(&now, &timeinfo);
  // Is time set? If not, tm_year will be (1970 - 1900).
  return timeinfo.tm_year > (2016 - 1900);
}
