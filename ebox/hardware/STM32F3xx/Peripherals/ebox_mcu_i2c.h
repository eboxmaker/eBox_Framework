#ifndef __EBOXTWOWIRE_H
#define __EBOXTWOWIRE_H
#include "i2c.h"
#include "mcu.h"

#define USE_I2C1 1
#define USE_I2C2 0
#define USE_I2C3 0

class mcuI2c : public I2c
{
public:
    // public methods
    mcuI2c();
    mcuI2c(I2C_TypeDef *I2Cx, Gpio *sclPin, Gpio *sdaPin);

    virtual void begin(uint8_t address);
    virtual void begin(int address);
    virtual void begin();
    virtual void setClock(Speed_t speed = K100);

private:
  // per object data
    I2C_TypeDef 	*_i2cx;		// i2cÕ‚…Ë
    Gpio            *_sda;
    Gpio            *_scl;
    uint32_t         _speed;
    
    // private methods
    virtual i2c_err_t _write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop);
    virtual size_t _read(uint8_t address,uint8_t *data, uint16_t quantity,uint8_t sendStop);
};
#endif
