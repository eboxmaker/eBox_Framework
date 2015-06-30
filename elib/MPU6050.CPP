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

int16_t  MPU6050::getData(uint8_t regAddress)
{
	char tmpH,tmpL;
	tmpH = readByte(regAddress);
	tmpL = readByte(regAddress + 1);
	return (tmpH<<8) + tmpL;

}

