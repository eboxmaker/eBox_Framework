
#include "PN532_I2C.h"
#include "debug.h"

#define PN532_I2C_ADDRESS       (0x48 >> 1)


PN532_I2C::PN532_I2C(I2c *wire,uint16_t slaveAddr)
{
    command = 0;
    this->_wire = wire;
    this->slaveAddr = PN532_I2C_ADDRESS;
    cfg.regAddrBits = I2c::BIT8;
    cfg.speed = I2c::K100;
}

void PN532_I2C::begin()
{
    _wire->begin(&cfg);
}

void PN532_I2C::wakeup()
{
//    PB7.mode(OUTPUT_OD_PU);
//    PB7.set();
//    delay_ms(10);
//    PB7.reset();
//    delay_ms(40);
//    PB7.set();
//    delay_ms(10);
    
    _wire->beginTransmission(0x48);
    delay_ms(20);
    _wire->endTransmission();                    // I2C end
}

int8_t PN532_I2C::writeCommand(const uint8_t buf[], uint8_t len)
{
    command = buf[0];
    
    write(PN532_PREAMBLE);
    write(PN532_STARTCODE1);
    write(PN532_STARTCODE2);
    
    uint8_t length = len + 1;   // length of data field: TFI + DATA
    write(length);
    write(~length + 1);         // checksum of length
    
    write(PN532_HOSTTOPN532);
    uint8_t sum = PN532_HOSTTOPN532;    // sum of TFI + DATA
    
    DMSG("write: ");
    
#if 1    
//    uint8_t mcuI2c::write_buf(uint16_t slaveAddr,uint8_t *data, uint16_t nWrite)

    _wire->write_buf(slaveAddr,(uint8_t*)buf,(uint16_t)len);
    for (uint8_t i = 0; i < len; i++) {
            sum += buf[i];
            DMSG_HEX(buf[i]);

    }
#else
    uint8_t bytes = 6;
    for (uint8_t i = 0; i < len; i++) {
        write(buf[i]);
        sum += buf[i];   
        DMSG_HEX(buf[i]);
        
        bytes++;
        if (bytes >= 30) {
            _wire->endTransmission(false);   // keep I2C connection active.
            _wire->beginTransmission(PN532_I2C_ADDRESS);
            bytes = 0;
        }
    }
    
#endif
  
    uint8_t checksum = ~sum + 1;            // checksum of TFI + DATA
    write(checksum);
    write(PN532_POSTAMBLE);
    
    
    DMSG('\n');

    return readAckFrame();
}

int16_t PN532_I2C::readResponse(uint8_t buf[], uint8_t len, uint16_t timeout)
{
    uint16_t time = 0;

    do {
        if (read() & 1) {  // check first byte --- status
            break;         // PN532 is ready
        }
        delay_ms(1);
        time++;
        if ((0 != timeout) && (time > timeout)) {
            return -1;
        }
    } while (1); 
    
    if (0x00 != read()      ||       // PREAMBLE
            0x00 != read()  ||       // STARTCODE1
            0xFF != read()           // STARTCODE2
        ) {
        
        return PN532_INVALID_FRAME;
    }
    
    uint8_t length = read();
    if (0 != (uint8_t)(length + read())) {   // checksum of length
        return PN532_INVALID_FRAME;
    }
    
    uint8_t cmd = command + 1;               // response command
    if (PN532_PN532TOHOST != read() || (cmd) != read()) {
        return PN532_INVALID_FRAME;
    }
    
    length -= 2;
    if (length > len) {
        return PN532_NO_SPACE;  // not enough space
    }
    
    DMSG("read:  ");
    DMSG_HEX(cmd);
    
    uint8_t sum = PN532_PN532TOHOST + cmd;
    for (uint8_t i = 0; i < length; i++) {
        buf[i] = read();
        sum += buf[i];
        
        DMSG_HEX(buf[i]);
    }
    DMSG('\n');
    
    uint8_t checksum = read();
    if (0 != (uint8_t)(sum + checksum)) {
        DMSG("checksum is not ok\n");
        return PN532_INVALID_FRAME;
    }
    read();         // POSTAMBLE
    
    return length;
}

int8_t PN532_I2C::readAckFrame()
{
    const uint8_t PN532_ACK[] = {0, 0, 0xFF, 0, 0xFF, 0};
    uint8_t ackBuf[sizeof(PN532_ACK)];
    
    DMSG("wait for ack at : ");
    DMSG(millis());
    DMSG('\n');
    
    uint16_t time = 0;
    do {
        uint8_t value = read();
        if (value & 1) {  // check first byte --- status
            break;         // PN532 is ready
        }
        delay_ms(1);
        time++;
        if (time > PN532_ACK_WAIT_TIME) {
            DMSG("Time out when waiting for ACK\n");
            return PN532_TIMEOUT;
        }
    } while (1); 
    
    DMSG("ready at : ");
    DMSG(millis());
    DMSG('\n');
    

    for (uint8_t i = 0; i < sizeof(PN532_ACK); i++) {
        ackBuf[i] = read();
    }
    
    if (memcmp(ackBuf, PN532_ACK, sizeof(PN532_ACK))) {
        DMSG("Invalid ACK\n");
        return PN532_INVALID_ACK;
    }
    
    return 0;
}
