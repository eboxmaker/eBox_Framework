#ifndef __EBOXTWOWIRE_H
#define __EBOXTWOWIRE_H
#include "ebox_core.h"
#include "stream.h"
#include "i2c.h"
#include "mcu.h"


class mcuI2c : public I2c
{

public:

    
    // public methods
    mcuI2c();
    mcuI2c(I2C_Module *I2Cx, Gpio *sclPin, Gpio *sdaPin);

    virtual void begin(uint8_t address);
    virtual void begin(int address);
    virtual void begin();
    virtual void setClock(Speed_t speed = K100);



private:
    

  // per object data
    I2C_Module 	*_i2cx;		// i2c外设
    Gpio            *_sda;
    Gpio            *_scl;
    uint8_t         _err;
    uint8_t         _err_at;
    uint32_t         _speed;
    I2C_InitType  I2C_InitStructure;



    // private methods
    virtual i2c_err_t _write(const uint8_t *data, size_t);
    virtual i2c_err_t _write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop);
    virtual size_t _read(uint8_t address,uint8_t *data, uint16_t quantity,uint8_t sendStop);


    //基础的i2c功能
    i2c_err_t _start(void);
    i2c_err_t _stop(void);
    i2c_err_t _waitAck();
    i2c_err_t _sendByte( uint8_t data);
    i2c_err_t  _send7bitsAddress(uint8_t slaveAddr, uint8_t WR);
    i2c_err_t _receiveByte(uint8_t *data);
    i2c_err_t _sendAck();
    i2c_err_t _sendNack();
    
        

};
#endif
