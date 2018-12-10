#ifndef __ADS1271_H
#define __ADS1271_H
#include "ebox_core.h"

class ADS1271
{
public:
    ADS1271(Spi *spi)
    {
        this->spi = spi;
    }
    void begin();
    void process();
    int32_t read();
    double read_voltage()
    {
        int32_t temp;
        double voltage;
        temp = read();
        voltage = temp / 16777216.0;
    }

private:
    Spi         *spi;
    Gpio        *drdy;
    SpiConfig_t config;
};

#endif

