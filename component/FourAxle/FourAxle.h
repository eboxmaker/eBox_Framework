#ifndef __FOURAXLE_H
#define __FOURAXLE_H

#include "ebox_core.h"
#include "stream.h"

class FourAxle
{
    public:
        FourAxle(){};
        void begin(uint16_t baud);
        void begin(Stream &s);
            
        uint8_t send1Channel(int16_t value);
        uint8_t sendNChannel(int16_t *buf,uint8_t len);
        uint8_t addSum(uint8_t *buf);
        uint8_t getFrameLength(uint8_t *buf);

    public:
        Stream *frameStream;
        uint8_t sendBuffer[24];
};
extern FourAxle test;
#endif

