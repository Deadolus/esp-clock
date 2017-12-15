#include "EspPersistentStorage.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string>
#include <array>

EspPersistentStorage::EspPersistentStorage(std::string storageHandle) {
    ESP_ERROR_CHECK(nvs_open(storageHandle.c_str(), NVS_READWRITE, &storageHandle_));
}

//uint32_t
template<>
uint32_t EspPersistentStorage::getValue<uint32_t>(std::string name) {
    uint32_t retVal{};
    ESP_ERROR_CHECK(nvs_get_u32(storageHandle_, name.c_str(), &retVal));
    return retVal;
}

template<>
void EspPersistentStorage::setValue<uint32_t>(std::string name, uint32_t& value) {
    ESP_ERROR_CHECK(nvs_set_u32(storageHandle_, name.c_str(), value));
}

//int32_t
template<>
int32_t EspPersistentStorage::getValue<int32_t>(std::string name) {
    int32_t retVal{};
    ESP_ERROR_CHECK(nvs_get_i32(storageHandle_, name.c_str(), &retVal));
    return retVal;
}

template<>
void EspPersistentStorage::setValue<int32_t>(std::string name, int32_t& value) {
    ESP_ERROR_CHECK(nvs_set_i32(storageHandle_, name.c_str(), value));
}

//std::string
template<>
std::string EspPersistentStorage::getValue<std::string>(std::string name) {
    std::string retVal{};
    std::array<char, 64> buffer{};
    size_t requiredSize;
    esp_err_t error{};
    error = nvs_get_str(storageHandle_, name.c_str(), nullptr, &requiredSize);
    assert((error == ESP_OK) || (error == ESP_ERR_NOT_FOUND));
    if(requiredSize<=64) {
    (nvs_get_str(storageHandle_, name.c_str(), buffer.data(), &requiredSize));
    }
    return retVal;
}

template<>
void EspPersistentStorage::setValue<std::string>(std::string name, std::string& value) {
    ESP_ERROR_CHECK(nvs_set_str(storageHandle_, name.c_str(), value.c_str()));
}
 

//TODO: u8, i16, u16, i64, u64, blob
//https://esp-idf.readthedocs.io/en/v1.0/api/nvs_flash.html
