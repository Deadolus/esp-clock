#pragma once

#include "PersistentStorage.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string>
#include <cstdint>

class EspPersistentStorage //: 
//    public PersistentStorage 
{
    public:
        EspPersistentStorage() = delete;
        EspPersistentStorage(std::string storageHandle);
        template <class T>
            T getValue(std::string name) ;
        template <class T>
            void setValue(std::string name, T& value) ;
    private:
        nvs_handle storageHandle_;
};
