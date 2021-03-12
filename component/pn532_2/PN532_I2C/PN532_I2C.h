
#ifndef __PN532_I2C_H__
#define __PN532_I2C_H__

#include <i2c.h>
#include "PN532Interface.h"

class PN532_I2C : public PN532Interface {
public:
    PN532_I2C(I2c &wire);
    
    void begin();
    void wakeup();
    int8_t writeCommand(const uint8_t buf[], uint8_t len);
    int16_t readResponse(uint8_t buf[], uint8_t len, uint16_t timeout);
    
private:
    I2c* _wire;
    uint8_t command;
    
    int8_t readAckFrame();
    
    inline uint8_t write(uint8_t data) {
            return _wire->write(data);
    }
    
    inline uint8_t read() {
            return _wire->read();
    }
};

#endif
