#ifndef __DS18B20_H
#define __DS18B20_H 
#include "ebox.h" 
#include "crc.h"
                                      
             
class Ds18b20
{
    public:
        Ds18b20(Gpio *_pin){pin = _pin;}
        int begin();
        int reset();
        void write(uint8_t data);
        uint8_t read();
        float get_temp();
        int is_exist();
    private:
        Gpio *pin;

};
#endif















