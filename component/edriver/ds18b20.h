#ifndef __DS18B20_H
#define __DS18B20_H
#include "ebox_core.h"

class Ds18b20
{
public:
    Ds18b20(Gpio *_pin)
    {
        pin = _pin;
    }
    bool begin();
    int reset();
    void write(uint8_t data);
    uint8_t read();
    float get_temp();
    int is_exist();
    void loop();
    bool update_temp();
private:
    Gpio *pin;
    float temperature;
    uint8_t state;
    uint32_t last;
    uint8_t buf[10];

};
#endif















