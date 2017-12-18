#include "EspPersistentStorage.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include <string>
#include <array>

static const char* TAG = "PersistentStorage";

EspPersistentStorage::EspPersistentStorage(std::string flashNamespace) {
    // Initialize NVS
}

EspPersistentStorage::~EspPersistentStorage() {
}

//uint32_t
template<>
uint32_t EspPersistentStorage::getValue<uint32_t>(std::string name) {
    return true;
}

template<>
void EspPersistentStorage::setValue<uint32_t>(std::string name, uint32_t const& value) {
}

//int32_t
template<>
int32_t EspPersistentStorage::getValue<int32_t>(std::string name) {
    return true;
}

template<>
void EspPersistentStorage::setValue<int32_t>(std::string name, int32_t const& value) {
}

//std::string
template<>
std::string EspPersistentStorage::getValue<std::string>(std::string name) {
    return "";
}

template<>
void EspPersistentStorage::setValue<std::string>(std::string name, std::string const& value) {
}


//TODO: u8, i16, u16, i64, u64, blob
//https://esp-idf.readthedocs.io/en/v1.0/api/nvs_flash.html
