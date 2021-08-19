#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H
#include "i2c.h"


class SoftI2c : public I2c
{

public:
    // public methods
    SoftI2c();
    SoftI2c(Gpio *sclPin, Gpio *sdaPin);

    virtual void begin(uint8_t address);
    virtual void begin(int address);
    virtual void begin();
    virtual void setClock(Speed_t speed = K100);

private:
  // per object data
    Gpio            *_sda;
    Gpio            *_scl;
	uint8_t   	 	_bitDelay;	// i2c时序
    uint8_t         _err_at;

    // private methods
    virtual i2c_err_t _write(const uint8_t *data, size_t);
    virtual i2c_err_t _write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop);
    virtual size_t _read(uint8_t address,uint8_t *data, uint16_t quantity,uint8_t sendStop);

    //基础的i2c功能
    i2c_err_t _start(void);
    i2c_err_t _stop(void);
    i2c_err_t _waitAck();
    i2c_err_t _sendByte( uint8_t data);
    i2c_err_t _sendByte_first( uint8_t data);
    i2c_err_t  _send7bitsAddress(uint8_t slaveAddr, uint8_t WR);
    i2c_err_t _receiveByte(uint8_t *data);
    i2c_err_t _sendAck();
    i2c_err_t _sendNack();
};
#endif
