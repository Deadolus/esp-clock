#pragma once
#include "nvs.h"
#include <cassert>
#include <cstdint>
#include <cstddef>
typedef unsigned int esp_err_t;
#define ESP_OK 0
#define ESP_ERR_NVS_NOT_FOUND 1
#define ESP_ERR_NVS_NO_FREE_PAGES 1
enum nvs_open_mode {NVS_READONLY, NVS_READWRITE};

esp_err_t nvs_set_i32(nvs_handle handle, const char *key, int8_t value);
esp_err_t nvs_get_i32(nvs_handle handle, const char *key, int32_t *out_value);
esp_err_t nvs_set_str(nvs_handle handle, const char *key, const char *value);
esp_err_t nvs_set_u32(nvs_handle handle, const char *key, uint32_t value);
esp_err_t nvs_get_u32(nvs_handle handle, const char *key, uint32_t *out_value);
esp_err_t nvs_erase_key(nvs_handle handle, const char *key);
esp_err_t nvs_commit(nvs_handle handle);
void nvs_close(nvs_handle handle);
esp_err_t nvs_flash_erase();
esp_err_t nvs_flash_init(void);
esp_err_t nvs_open(const char *name, nvs_open_mode open_mode, nvs_handle *out_handle);
esp_err_t nvs_get_str(nvs_handle handle, const char *key, char *out_value, size_t *length);
void ESP_ERROR_CHECK(esp_err_t );
//bool assert(bool) {return true;}
