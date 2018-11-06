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

} NiMingStatus_t;
typedef struct
{
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t gx;
    int16_t gy;
    int16_t gz;
    int16_t magx;
    int16_t magy;
    int16_t magz;

} NiMingSenser_t;
class FourAxle
{
public:
    FourAxle() {};
    void begin(uint16_t baud);
    void begin(Stream &s);

    uint8_t send1Channel(int16_t value);
    uint8_t sendNChannel(uint8_t ch, int16_t *buf, uint8_t len);
    uint8_t sendNMStatues(NiMingStatus_t &status);
    uint8_t sendNMSenser(NiMingSenser_t &status);
    uint8_t addSum(uint8_t *buf);
    uint8_t getFrameLength(uint8_t *buf);

public:
    Stream *frameStream;
    uint8_t sendBuffer[32];
};
#endif

