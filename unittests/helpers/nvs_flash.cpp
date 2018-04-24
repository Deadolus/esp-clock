#include "nvs_flash.h"

esp_err_t nvs_set_i32(nvs_handle handle, const char *key, int8_t value) { return 0;}
esp_err_t nvs_get_i32(nvs_handle handle, const char *key, int32_t *out_value) { return 0;}
esp_err_t nvs_set_str(nvs_handle handle, const char *key, const char *value) { return 0;}
esp_err_t nvs_set_u32(nvs_handle handle, const char *key, uint32_t value) { return 0;}
esp_err_t nvs_get_u32(nvs_handle handle, const char *key, uint32_t *out_value) { return 0;}
esp_err_t nvs_erase_key(nvs_handle handle, const char *key) { return 0;}
esp_err_t nvs_commit(nvs_handle handle) { return 0;}
void nvs_close(nvs_handle handle) {}
esp_err_t nvs_flash_erase() {return 0;}
esp_err_t nvs_flash_init(void) {return 0;}
esp_err_t nvs_open(const char *name, nvs_open_mode open_mode, nvs_handle *out_handle) { return 0;}
esp_err_t nvs_get_str(nvs_handle handle, const char *key, char *out_value, size_t *length) { return 0;}
void ESP_ERROR_CHECK(esp_err_t ) {}
//bool assert(bool) {return true;}
