#ifndef __FOURAXLE_H
#define __FOURAXLE_H

#include "ebox_core.h"
#include "stream.h"
typedef struct
{
    int16_t roll;
    int16_t pitch;
    int16_t yaw;
    int32_t alt;
    uint8_t mode;
    uint8_t armed;
    
}NiMingStatus_t;

class FourAxle
{
    public:
        FourAxle(){};
        void begin(uint16_t baud);
        void begin(Stream &s);
            
        uint8_t send1Channel(int16_t value);
        uint8_t sendNChannel(uint8_t ch,int16_t *buf,uint8_t len);
        uint8_t sendNMStatues(NiMingStatus_t &status);
        uint8_t addSum(uint8_t *buf);
        uint8_t getFrameLength(uint8_t *buf);

    public:
        Stream *frameStream;
        uint8_t sendBuffer[32];
};
extern FourAxle test;
#endif

