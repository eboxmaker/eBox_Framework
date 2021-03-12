
#include "PN532_I2C.h"
#include "debug.h"



#define PN532_I2C_ADDRESS       (0x48)


PN532_I2C::PN532_I2C(I2c &wire)
{
    _wire = &wire;
    command = 0;
}

void PN532_I2C::begin()
{
    _wire->begin();
    PB9.mode(OUTPUT_PP_PU);
    PB9.reset();
    delay_ms(100);
    PB9.set();
    delay_ms(100);
}

void PN532_I2C::wakeup()
{
    _wire->beginTransmission(PN532_I2C_ADDRESS); // I2C start
    _wire->endTransmission();                    // I2C end
        delay_ms(40);

}

int8_t PN532_I2C::writeCommand(const uint8_t buf[], uint8_t len)
{
    int err;
    command = buf[0];
    delay_ms(1);     // or whatever the delay is for waking up the board
    _wire->beginTransmission(PN532_I2C_ADDRESS);
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
    for (uint8_t i = 0; i < len; i++) {
        if (write(buf[i])) {
            sum += buf[i];
            
            DMSG_HEX(buf[i]);
        } else {
            DMSG("\nToo many data to send, I2C doesn't support such a big packet\n");     // I2C max packet: 32 bytes
            return PN532_INVALID_FRAME;
        }
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
    
    err = _wire->endTransmission();
    if(err)
    {
        uart1.printf("[pn532]:write err = %d\n",err);
    }
    DMSG('\n');
    delay_ms(1);     // or whatever the delay is for waking up the board

    return readAckFrame();
}

int16_t PN532_I2C::readResponse(uint8_t buf[], uint8_t len, uint16_t timeout)
{
    uint16_t time = 0;

    do {
        if (_wire->requestFrom(PN532_I2C_ADDRESS, len + 2)) {
            if (read() & 1) {  // check first byte --- status
                break;         // PN532 is ready
            }
        }

        delay_ms(1);
        time++;
        if ((0 != timeout) && (time > timeout)) {
            uart1.printf("read Response timeout(%d,%d)\n",time,timeout);
            return -1;
        }
//        uart1.printf("time:%d\n",millis());

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
        if (_wire->requestFrom(PN532_I2C_ADDRESS,  sizeof(PN532_ACK) + 1)) {
            if (read() & 1) {  // check first byte --- status
                break;         // PN532 is ready
            }
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
