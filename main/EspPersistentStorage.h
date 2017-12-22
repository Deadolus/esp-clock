#pragma once

#include "PersistentStorage.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string>
#include <cstdint>
#include <mutex>

class EspPersistentStorage //: 
//    public PersistentStorage 
{
    public:
        EspPersistentStorage() = delete;
        EspPersistentStorage(std::string flashNamespace);
        virtual ~EspPersistentStorage();
        template <class T>
            T getValue(std::string const& name) ;
        template <class T>
            void setValue(std::string const& name, T const& value) ;
        template <class T>
            void erase(T const& value);
    private:
        nvs_handle storageHandle_;
        static std::mutex storageMutex_;
};
