#ifndef __TWOWIRE_H
#define __TWOWIRE_H
#include "ebox_port_gpio.h"
#include "stream.h"


#define I2C_BUFFER_LENGTH 128

typedef enum {
    I2C_ERROR_OK,
    I2C_ERROR_DATA_LONG,
    I2C_ERROR_ADDR_NACK_NO_RECV,
    I2C_ERROR_DATA_NACK_NO_RECV,
    I2C_ERROR_BUS,
    I2C_ERROR_TIMEOUT,
} i2c_err_t;

class I2c : public Stream
{

public:
    typedef enum 
    {
        K1000,
        K400,
        K300,
        K200,
        K100,
        K10,
    }Speed_t;

    
    // public methods
    I2c(){};

    virtual void begin(uint8_t address)   = 0;
    virtual void begin(int address)   = 0;
    virtual void begin()   = 0;
    virtual void setClock(Speed_t speed)  = 0;

    void beginTransmission(uint8_t address);
    void beginTransmission(int address);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t sendStop);
    uint8_t requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop);
    
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
  
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);

    inline size_t write(unsigned long n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(long n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(unsigned int n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(int n)
    {
        return write((uint8_t)n);
    }
    using       Print::write;

private:
    // private methods
//    virtual i2c_err_t _write(const uint8_t *data, size_t)  = 0;
    virtual i2c_err_t _write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop)  = 0;
    virtual size_t _read(uint8_t address,uint8_t *data, uint16_t quantity,uint8_t sendStop)  = 0;
    
    uint8_t rxBuffer[I2C_BUFFER_LENGTH];
    uint16_t rxIndex;
    uint16_t rxLength;

    uint8_t txBuffer[I2C_BUFFER_LENGTH];
    uint16_t txIndex;
    uint16_t txLength;

protected:
    uint8_t txAddress;
    uint8_t transmitting;

};
#endif
