#include "EspHttpServer.h"
#include "Alarm.h"
#include "EspAlarm.h"
#include "Clock.h"
#include "esp_log.h"
//#include <espressif/esp_common.h>
//#include <esp_common.h>
//#include <esp8266.h>
//#include <esp/uart.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
//#include <ssid_config.h>
#include <httpd/httpd.h>
#include <thread>
#include <sstream>
#define LED_PIN 2

static const char* TAG = "HttpServer";
EspHttpServer* EspHttpServer::instance_{};
namespace {
    std::string replaceStr(std::string & str, const std::string & from, const std::string & to)
{
  while(str.find(from) != std::string::npos)
    str.replace(str.find(from), from.length(), to);
  return str;
}

std::string alarmsToHtml(Alarm& alarms) {
    std::stringstream allAlarms{};
    for(auto& alarm: alarms.getAlarms()) {
        tm time = Clock::getTm(alarm.time);
        allAlarms << alarm.name;
        allAlarms << ": ";
        allAlarms << time.tm_hour;
        allAlarms << ":";
        allAlarms << time.tm_min;
        allAlarms << ":";
        allAlarms << time.tm_sec;
        allAlarms << "<br>";
        //allAlarms.append(alarm.name+": "+itoa(time.tm_hour)+":"+itoa(time.tm_min));
    }
    return allAlarms.str();
}
}

enum {
    SSI_UPTIME,
    SSI_FREE_HEAP,
    SSI_LED_STATE,
    SSI_NEXT_ALARM,
    SSI_ALARMS
};

int32_t EspHttpServer::ssi_handler(int32_t iIndex, char *pcInsert, int32_t iInsertLen)
{
    Alarm& alarms = getInstanceAlarms();
    //client send iIndex from pcConfigSSITags in to here - 
    //we populate pcInsert and return it to client
    ESP_LOGI(TAG, "Got request for %i", iIndex);
    switch (iIndex) {
        case SSI_UPTIME:
            snprintf(pcInsert, iInsertLen, "%d",
                    xTaskGetTickCount() * portTICK_PERIOD_MS / 1000);
            break;
        case SSI_FREE_HEAP:
            snprintf(pcInsert, iInsertLen, "%d", (int) xPortGetFreeHeapSize());
            break;
        case SSI_LED_STATE:
            snprintf(pcInsert, iInsertLen, "I don't know");
            //snprintf(pcInsert, iInsertLen, (GPIO.OUT & BIT(LED_PIN)) ? "Off" : "On");
            break;
        case SSI_NEXT_ALARM:
            //snprintf(pcInsert, iInsertLen, "Soon!");
            snprintf(pcInsert, iInsertLen, alarms.getNextAlarm().name.c_str());
            //snprintf(pcInsert, iInsertLen, (GPIO.OUT & BIT(LED_PIN)) ? "Off" : "On");
            break;
        case SSI_ALARMS:
            snprintf(pcInsert, iInsertLen, alarmsToHtml(alarms).c_str());
            break;
        default:
            snprintf(pcInsert, iInsertLen, "N/A");
            break;
    }

    ESP_LOGI(TAG, "Replying to index with %s", pcInsert);
    /* Tell the server how many characters to insert */
    return (strlen(pcInsert));
}

const char* EspHttpServer::newAlarm_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    Alarm& alarms = getInstanceAlarms();
    ESP_LOGI(TAG, "Got request for newAlarm, params: %i, index: %i", iNumParams, iIndex);
    //url handler e.g. /newAalarm?time=xxx&days=xxxx
    alarms_t alarm;

    for (int i = 0; i < iNumParams; i++) {
        if(strcmp(pcParam[i], "time")==0) {
            time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm alarm_tm{};
            alarm_tm = *localtime(&now);
            //alarm_tm.tm_mday++;
            std::string alarmTime = std::string(pcValue[i]);
            if(alarmTime.length() == 5) {
            alarm_tm.tm_hour = atoi(std::string(pcValue[i]).substr(0,2).c_str());
            alarm_tm.tm_min = atoi(std::string(pcValue[i]).substr(3,4).c_str());
            alarm_tm.tm_sec = 0;
            alarm.time = std::chrono::system_clock::from_time_t(std::mktime(&alarm_tm));
            }
            ESP_LOGI(TAG, "time: %s --> %u:%u", pcValue[i], alarm_tm.tm_hour, alarm_tm.tm_min);
        }
        if(strcmp(pcParam[i], "name")==0) {
            ESP_LOGI(TAG, "name: %s", pcValue[i]);
            alarm.name = std::string(pcValue[i]);
            alarm.name = replaceStr(alarm.name, "%20", " ");
            ESP_LOGI(TAG, "name: %s --> %s", pcValue[i], alarm.name.c_str());
        }
        if(strcmp(pcParam[i], "snoozeTime")==0) {
            ESP_LOGI(TAG, "snoozeTime: %s", pcValue[i]);
        }
        if(strcmp(pcParam[i], "repeatingAlarm")==0) {
            ESP_LOGI(TAG, "repeatingAlarm: %s", pcValue[i]);
        }
        if(strcmp(pcParam[i], "days")==0) {
            ESP_LOGI(TAG, "days: %s", pcValue[i]);
            alarm.weekRepeat = std::bitset<7>(pcValue[i]);
            //days is last entry so we can now set alarm
            alarms.setAlarm(alarm);
        }

    }
    return "/newalarm.html";
}

const char *gpio_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    ESP_LOGI(TAG, "Got request for gpio");
    //url handler, e.g. gpio?off=2
    for (int i = 0; i < iNumParams; i++) {
        if (strcmp(pcParam[i], "on") == 0) {
            //uint8_t gpio_num = atoi(pcValue[i]);
            ////gpio_enable(gpio_num, GPIO_OUTPUT);
            //gpio_set_level(gpio_num, true);
        } else if (strcmp(pcParam[i], "off") == 0) {
            //uint8_t gpio_num = atoi(pcValue[i]);
            //gpio_enable(gpio_num, GPIO_OUTPUT);
            //gpio_set_level(gpio_num, false);
        } else if (strcmp(pcParam[i], "toggle") == 0) {
            //uint8_t gpio_num = atoi(pcValue[i]);
            //gpio_enable(gpio_num, GPIO_OUTPUT);
            //gpio_toggle(gpio_num);
        }
    }
    return "/index.ssi";
}

const char *about_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    ESP_LOGI(TAG, "Got request for about");
    return "/about.html";
}

const char *websocket_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    ESP_LOGI(TAG, "Got request for websocket");
    return "/websockets.html";
}

void websocket_task(void *pvParameter)
{
    ESP_LOGI(TAG, "In websocket task");
    struct tcp_pcb *pcb = (struct tcp_pcb *) pvParameter;

    for (;;) {
        if (pcb == NULL || pcb->state != ESTABLISHED) {
            printf("Connection closed, deleting task\n");
            break;
        }

        int uptime = xTaskGetTickCount() * portTICK_PERIOD_MS / 1000;
        int heap = (int) xPortGetFreeHeapSize();
        int led = false;//!gpio_get_level(LED_PIN);

        /* Generate response in JSON format */
        char response[64];
        int len = snprintf(response, sizeof (response),
                "{\"uptime\" : \"%d\","
                " \"heap\" : \"%d\","
                " \"led\" : \"%d\"}", uptime, heap, led);
        if (len < sizeof (response))
            websocket_write(pcb, (unsigned char *) response, len, WS_TEXT_MODE);

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

/**
 * This function is called when websocket frame is received.
 *
 * Note: this function is executed on TCP thread and should return as soon
 * as possible.
 */
void websocket_cb(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode)
{
    ESP_LOGI(TAG, "In websocket task, len: %u, data: %s", data_len, data);
    //printf("[websocket_callback]:\n%.*s\n", (int) data_len, (char*) data);

    uint8_t response[2];
    uint16_t val;

    switch (data[0]) {
        case 'A': // ADC
            /* This should be done on a separate thread in 'real' applications */
            val=0;
            //val = sdk_system_adc_read();
            break;
        case 'D': // Disable LED
            //gpio_set_level(LED_PIN, true);
            val = 0xDEAD;
            break;
        case 'E': // Enable LED
            //gpio_set_level(LED_PIN, false);
            val = 0xBEEF;
            break;
        default:
            printf("Unknown command\n");
            val = 0;
            break;
    }

    response[1] = (uint8_t) val;
    response[0] = val >> 8;

    websocket_write(pcb, response, 2, WS_BIN_MODE);
}

/**
 * This function is called when new websocket is open and
 * creates a new websocket_task if requested URI equals '/stream'.
 */
void websocket_open_cb(struct tcp_pcb *pcb, const char *uri)
{
    printf("WS URI: %s\n", uri);
    if (!strcmp(uri, "/stream")) {
        ESP_LOGI(TAG, "request for streaming");
        xTaskCreate(&websocket_task, "websocket_task", 256, (void *) pcb, 2, NULL);
    }
}

void httpd_task(void *pvParameters)
{
    tCGI pCGIs[] = {
        {"/gpio", (tCGIHandler) gpio_cgi_handler},
        {"/about", (tCGIHandler) about_cgi_handler},
        {"/websockets", (tCGIHandler) websocket_cgi_handler},
        {"/newalarm", (tCGIHandler) EspHttpServer::newAlarm_cgi_handler},
    };

    //limited to 8chars
    const char *pcConfigSSITags[] = {
        "uptime", // SSI_UPTIME
        "heap",   // SSI_FREE_HEAP
        "led",     // SSI_LED_STATE
        "nalarm",
        "alarms",
        "time",
        "name",
        "snoozeT",
        "repeat",
        "monday",
        "tuesday",
        "wednesda",
        "thursday",
        "friday",
        "saturday",
        "sunday",

    };

    /* register handlers and start the server */
    http_set_cgi_handlers(pCGIs, sizeof (pCGIs) / sizeof (pCGIs[0]));
    http_set_ssi_handler((tSSIHandler) EspHttpServer::ssi_handler, pcConfigSSITags,
            sizeof (pcConfigSSITags) / sizeof (pcConfigSSITags[0]));
    websocket_register_callbacks((tWsOpenHandler) websocket_open_cb,
            (tWsHandler) websocket_cb);
    httpd_init();

    for (;;) {
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

EspHttpServer::EspHttpServer(Alarm& alarms) : alarms_(alarms)  {
    instance_ = this;
}

void EspHttpServer::startServer() {
    ESP_LOGI(TAG, "Starting webserver");
    /* initialize tasks */
    //xTaskCreate(&httpd_task, "HTTP Daemon", 8000, NULL, 2, NULL);
    std::thread server(&httpd_task, nullptr);
    server.detach();
}

EspHttpServer* EspHttpServer::getInstance() {
    return instance_;
}

Alarm& EspHttpServer::getInstanceAlarms() {
    return EspHttpServer::getInstance()->alarms_;
}
