


#include "FourAxle.h"

void FourAxle::begin(uint16_t baud)
{

}
void FourAxle::begin(Stream &s)
{
    frameStream = &s;
}

uint8_t FourAxle::send1Channel(int16_t value)
{
    int i = 0;
    DataU16_t data;
    data.value = value;

    sendBuffer[i++] = 0xaa;
    sendBuffer[i++] = 0x55;
    sendBuffer[i++] = 1;//fun
    sendBuffer[i++] = 2;//len
    sendBuffer[i++] = data.byte[1];
    sendBuffer[i++] = data.byte[0];
    addSum(sendBuffer);
    frameStream->write(sendBuffer, 7);
    return 7;
}
uint8_t FourAxle::sendNChannel(uint8_t ch, int16_t *buf, uint8_t len)
{
    int i = 0;
    DataU16_t data;

    sendBuffer[i++] = 0xaa;
    sendBuffer[i++] = 0xaa;
    sendBuffer[i++] = ch;//fun
    sendBuffer[i++] = len * 2; //len
    for(int k = 0 ; k < len; k++)
    {
        data.value = *buf++;
        sendBuffer[i++] = data.byte[1];
        sendBuffer[i++] = data.byte[0];
    }
    addSum(sendBuffer);
    frameStream->write(sendBuffer, i + 1);
    return i + 1;
}
uint8_t FourAxle::sendNMStatues(NiMingStatus_t &status)
{
    int i = 0;

    Data16_t tempi16;
    DataU32_t tempu32;
    sendBuffer[i++] = 0xaa;
    sendBuffer[i++] = 0xaa;
    sendBuffer[i++] = 1;//fun
    sendBuffer[i++] = 12;//len

    tempi16.value = status.roll;
    sendBuffer[i++] = tempi16.byte[1];
    sendBuffer[i++] = tempi16.byte[0];

    tempi16.value = status.pitch;
    sendBuffer[i++] = tempi16.byte[1];
    sendBuffer[i++] = tempi16.byte[0];

    tempi16.value = status.yaw;
    sendBuffer[i++] = tempi16.byte[1];
    sendBuffer[i++] = tempi16.byte[0];

    tempu32.value = status.alt;
    sendBuffer[i++] = tempu32.byte[3];
    sendBuffer[i++] = tempu32.byte[2];
    sendBuffer[i++] = tempu32.byte[1];
    sendBuffer[i++] = tempu32.byte[0];

    sendBuffer[i++] = status.mode;
    sendBuffer[i++] = status.armed;

    addSum(sendBuffer);
    frameStream->write(sendBuffer, i + 1);
    return i + 1;
}
uint8_t FourAxle::sendNMSenser(NiMingSenser_t &senser)
{

    int i = 0;

    Data16_t tempi16;
    sendBuffer[i++] = 0xaa;
    sendBuffer[i++] = 0xaa;
    sendBuffer[i++] = 2;//fun
    sendBuffer[i++] = 18;//len

    tempi16.value = senser.ax;
    sendBuffer[i++] = tempi16.byte[1];
    sendBuffer[i++] = tempi16.byte[0];

    tempi16.value = senser.ay;
    sendBuffer[i++] = tempi16.byte[1];
    sendBuffer[i++] = tempi16.byte[0];

    tempi16.value = senser.az;
    sendBuffer[i++] = tempi16.byte[1];
    sendBuffer[i++] = tempi16.byte[0];

    tempi16.value = senser.gx;
    sendBuffer[i++] = tempi16.byte[1];
    sendBuffer[i++] = tempi16.byte[0];

    tempi16.value = senser.gy;
    sendBuffer[i++] = tempi16.byte[1];
    sendBuffer[i++] = tempi16.byte[0];

    tempi16.value = senser.gz;
    sendBuffer[i++] = tempi16.byte[1];
    sendBuffer[i++] = tempi16.byte[0];

    tempi16.value = senser.magx;
    sendBuffer[i++] = tempi16.byte[1];
    sendBuffer[i++] = tempi16.byte[0];

    tempi16.value = senser.magy;
    sendBuffer[i++] = tempi16.byte[1];
    sendBuffer[i++] = tempi16.byte[0];

    tempi16.value = senser.magz;
    sendBuffer[i++] = tempi16.byte[1];
    sendBuffer[i++] = tempi16.byte[0];


    addSum(sendBuffer);
    frameStream->write(sendBuffer, i + 1);
    return i + 1;
}


uint8_t FourAxle::addSum(uint8_t *buf)
{
    uint8_t len = getFrameLength(buf);
    uint8_t sum = 0;
    for(int i = 0; i < len; i++)
    {
        sum += buf[i];
    }
    buf[len++] = sum;
    return sum;

}
uint8_t FourAxle::getFrameLength(uint8_t *buf)
{
    uint8_t len = buf[3] + 4;
    return len;
}
