#pragma once

class Sign 
{
    public: 
        virtual void setClock(bool value) = 0;
        virtual void setBluetooth(bool value) = 0;
        virtual void setWifi(bool value) = 0;
};
