#include "mpu6050.h"



void MPU6050::begin()
{
	i2cBegin();
	writeByte(SlaveAddress,PWR_MGMT_1,0x00);
	writeByte(SlaveAddress,SMPLRT_DIV,0x07);
	writeByte(SlaveAddress,CONFIG,0x06);
	writeByte(SlaveAddress,GYRO_CONFIG,0x18);
	writeByte(SlaveAddress,ACCEL_CONFIG,0x01);

}
void MPU6050::getID(uint8_t* id)
{
	readByte(SlaveAddress,WHO_AM_I,id);
};

int16_t  MPU6050::getData(uint8_t regAddress)
{
	uint8_t tmpH,tmpL;
	
	readByte(SlaveAddress,regAddress,&tmpH);
	readByte(SlaveAddress,regAddress,&tmpL);

	return ((tmpH<<8) + tmpL);

}
int8_t  MPU6050::getData(uint8_t regAddress,int16_t* buf,uint8_t numToRead)
{
	uint8_t i,readnum;
	uint8_t tmpbuf[20];
	
	readnum = readByte(SlaveAddress,regAddress,tmpbuf,numToRead*2);
	for(i=0;i<readnum/2;i++)
	{
		*buf++ = (tmpbuf[i*2 + 0]<<8) + (tmpbuf[i*2 + 1]);
	}
	return readnum/2;
}

