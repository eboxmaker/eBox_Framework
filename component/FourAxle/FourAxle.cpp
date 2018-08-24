


#include "FourAxle.h"
FourAxle test;

void FourAxle::begin(uint16_t baud)
{

}
void FourAxle::begin(Stream &s)
{
    frameStream = &s;
}
            
uint8_t FourAxle::send1Channel(int16_t value)
{
    int i=0;
    DataU16_t data;
    data.value = value;
    
    sendBuffer[i++] = 0xaa;
    sendBuffer[i++] = 0x55;
    sendBuffer[i++] = 1;//fun
    sendBuffer[i++] = 2;//len
    sendBuffer[i++] = data.byte[1];
    sendBuffer[i++] = data.byte[0];
    addSum(sendBuffer);
    frameStream->write(sendBuffer,7);
    return 7;
}
uint8_t FourAxle::sendNChannel(int16_t *buf,uint8_t len)
{
    int i=0;
    DataU16_t data;
    
    sendBuffer[i++] = 0xaa;
    sendBuffer[i++] = 0x55;
    sendBuffer[i++] = 1;//fun
    sendBuffer[i++] = 4;//len
    for(int k = 0 ; k < len; k++)
    {
        data.value = *buf++;
        sendBuffer[i++] = data.byte[1];
        sendBuffer[i++] = data.byte[0];
    }
    addSum(sendBuffer);
    frameStream->write(sendBuffer,i+1);
    return i+1;
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
    uint8_t len = buf[3]+4;
}
