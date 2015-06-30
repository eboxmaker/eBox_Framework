#include "mpu6050.h"



void MPU6050::begin()
{
	writeByte(PWR_MGMT_1,0x00);
	writeByte(SMPLRT_DIV,0x07);
	writeByte(CONFIG,0x06);
	writeByte(GYRO_CONFIG,0x18);
	writeByte(ACCEL_CONFIG,0x08);

}

uint8_t MPU6050::writeByte(uint8_t regAddress,uint8_t regData)
{

			start();
		sendByte(SlaveAddress);
    if (waitAck() == 0)
        return 0;
		
		sendByte(regAddress);
    if (waitAck() == 0)
        return 0;
   
		sendByte(regData);
    if (waitAck() == 0)
        return 0;
		stop();
		
    delay_us(10);   
		
		return 1;
}

int8_t MPU6050::readByte(uint8_t regAddress)
{

	uint8_t readData;
	
		start();
		sendByte(SlaveAddress);
    if (waitAck() == 0)
        return 0;

		sendByte(regAddress);
    if (waitAck() == 0)
        return 0;

		start();
		sendByte(SlaveAddress + 1);
    if (waitAck() == 0)
        return 0;

		readData = receiveByte();
		sendNoAck();
		
		stop();
		
	return readData;
}

int8_t 	MPU6050::readByte(uint8_t regAddress,int8_t* buf,uint8_t numToRead)
{

	int i = 0;
		start();
		sendByte(SlaveAddress);
    if (waitAck() == 0)
        return 0;

		sendByte(regAddress);
    if (waitAck() == 0)
        return 0;

		start();
		sendByte(SlaveAddress + 1);
    if (waitAck() == 0)
        return 0;
		while(numToRead)
		{
			*buf++ = receiveByte();
			numToRead--;
			i++;
			if(numToRead == 0)
			{
				sendNoAck();
				stop();
				return i;
			}
			else
				sendAck();
		}

	return i;
}

int16_t  MPU6050::getData(uint8_t regAddress)
{
	char tmpH,tmpL;
	tmpH = readByte(regAddress);
	tmpL = readByte(regAddress + 1);
	return (tmpH<<8) + tmpL;

}
int8_t  MPU6050::getData(uint8_t regAddress,int16_t* buf,uint8_t numToRead)
{
	uint8_t i,readnum;
	int8_t tmpbuf[20];
	
	readnum = readByte(regAddress,tmpbuf,numToRead*2);
	for(i=0;i<readnum/2;i++)
	{
		*buf++ = (tmpbuf[i*2 + 0]<<8) + (tmpbuf[i*2 + 1]);
	}

	return readnum/2;
}

