#include "EspPersistentStorage.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include <string>
#include <array>
#include <mutex>

static const char* TAG = "PersistentStorage";
std::mutex EspPersistentStorage::storageMutex_{};

EspPersistentStorage::EspPersistentStorage(std::string flashNamespace) {
    std::lock_guard<std::mutex> lock(storageMutex_);
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
    ESP_ERROR_CHECK(nvs_open(flashNamespace.c_str(), NVS_READWRITE, &storageHandle_));
    ESP_LOGI(TAG, "Initialized flash");
}

EspPersistentStorage::~EspPersistentStorage() {
    std::lock_guard<std::mutex> lock(storageMutex_);
    nvs_close(storageHandle_);
}

//uint32_t
template<>
uint32_t EspPersistentStorage::getValue<uint32_t>(std::string const& name) {
    std::lock_guard<std::mutex> lock(storageMutex_);
    uint32_t retVal{};
    esp_err_t error{};
    error = nvs_get_u32(storageHandle_, name.c_str(), &retVal);
    ESP_LOGI(TAG, "Error is %d, retVal %u", error, retVal);
    assert((error == ESP_OK) || (error == ESP_ERR_NVS_NOT_FOUND));
    return retVal;
}

template<>
void EspPersistentStorage::setValue<uint32_t>(std::string const& name, uint32_t const& value) {
    std::lock_guard<std::mutex> lock(storageMutex_);
    ESP_ERROR_CHECK(nvs_set_u32(storageHandle_, name.c_str(), value));
    ESP_ERROR_CHECK(nvs_commit(storageHandle_));
}

//int32_t
template<>
int32_t EspPersistentStorage::getValue<int32_t>(std::string const& name) {
    std::lock_guard<std::mutex> lock(storageMutex_);
    int32_t retVal{};
    esp_err_t error{};
    error = nvs_get_i32(storageHandle_, name.c_str(), &retVal);
    assert((error == ESP_OK) || (error == ESP_ERR_NVS_NOT_FOUND));
    return retVal;
}

template<>
void EspPersistentStorage::setValue<int32_t>(std::string const& name, int32_t const& value) {
    std::lock_guard<std::mutex> lock(storageMutex_);
    ESP_ERROR_CHECK(nvs_set_i32(storageHandle_, name.c_str(), value));
    ESP_ERROR_CHECK(nvs_commit(storageHandle_));
}

//std::string
template<>
std::string EspPersistentStorage::getValue<std::string>(std::string const& name) {
    std::lock_guard<std::mutex> lock(storageMutex_);
    ESP_LOGI(TAG, "Getting key %s", name.c_str());
    std::array<char, 64> buffer{};
    size_t requiredSize{};
    esp_err_t error{};
    error = nvs_get_str(storageHandle_, name.c_str(), nullptr, &requiredSize);
    assert((error == ESP_OK) || (error == ESP_ERR_NVS_NOT_FOUND));
    if(requiredSize<=64) {
    (nvs_get_str(storageHandle_, name.c_str(), buffer.data(), &requiredSize));
    }
    //ESP_LOGI(TAG, "loaded %s/%s, required size: %u, notfound: %d",name.c_str(), buffer.data(), requiredSize, error==ESP_ERR_NVS_NOT_FOUND);
    return std::string(buffer.data());
}

template<>
void EspPersistentStorage::setValue<std::string>(std::string const& name, std::string const& value) {
    std::lock_guard<std::mutex> lock(storageMutex_);
    ESP_LOGI(TAG, "Saving key %s/%s", name.c_str(), value.c_str());
    ESP_ERROR_CHECK(nvs_set_str(storageHandle_, name.c_str(), value.c_str()));
    ESP_ERROR_CHECK(nvs_commit(storageHandle_));
}

template <>
void EspPersistentStorage::erase<std::string>(std::string const& name) {
    std::lock_guard<std::mutex> lock(storageMutex_);
    esp_err_t error{};
    ESP_LOGI(TAG, "Erasing key %s", name.c_str());
    error = nvs_erase_key(storageHandle_, name.c_str());
    assert((error == ESP_OK) || (error == ESP_ERR_NVS_NOT_FOUND));
    ESP_ERROR_CHECK(nvs_commit(storageHandle_));

}


//TODO: u8, i16, u16, i64, u64, blob
//https://esp-idf.readthedocs.io/en/v1.0/api/nvs_flash.html
