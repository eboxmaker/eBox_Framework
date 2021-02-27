#ifndef __TWOWIRE_H
#define __TWOWIRE_H
#include "ebox_core.h"
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

class TwoWire : public Stream
{

public:
    typedef enum 
    {
        K400,
        K300,
        K200,
        K100,
        K10,
    }Speed_t;

    
    // public methods
    TwoWire();
    TwoWire(Gpio *sclPin, Gpio *sdaPin);

    void begin();
    void begin(Speed_t speed);
    void setClock(Speed_t speed);

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
    


    uint8_t rxBuffer[I2C_BUFFER_LENGTH];
    uint16_t rxIndex;
    uint16_t rxLength;

    uint8_t txBuffer[I2C_BUFFER_LENGTH];
    uint16_t txIndex;
    uint16_t txLength;
    uint8_t txAddress;

    uint8_t transmitting;

  // per object data
    Gpio            *_sda;
    Gpio            *_scl;
	uint8_t   	 	_bitDelay;	// i2c ±–Ú
    uint16_t        _bitTimeout;



    // private methods

    int i2c_start(void);
    void i2c_stop(void);
    i2c_err_t i2c_waitAck();
    i2c_err_t i2c_write( uint8_t c );
    i2c_err_t i2c_write(const uint8_t *, size_t);
    i2c_err_t i2c_write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop);
    uint8_t i2c_read();
    size_t i2c_read(uint8_t address,uint8_t *data, uint16_t quantity,uint8_t sendStop);
    int8_t i2c_sendAck();
    int8_t i2c_sendNack();
    

};
#endif
