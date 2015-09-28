#include "24c02.h"
int8_t EEPROM::byteWrite(uint16_t byteAddr,uint8_t byte)
{
	i2c->writeByte(SLAVE_ADDR,byteAddr,byte);
	return 0;
}

int8_t EEPROM::byteWrite(uint16_t byteAddr,uint8_t* buf,uint16_t numToWrite)
{
	int8_t ret = 0;

	for(uint16_t i = 0; i < numToWrite; i++)
	{
			byteWrite(byteAddr++,buf[i]);
		  //ret = i2c->waitBusy(SLAVE_ADDR);//如果有错误再加此句
	
	}
//	i2c->writeByte(SLAVE_ADDR,byteAddr,buf,numToWrite);//部分器件个别的位置相应比较慢
	return ret;
}

uint8_t EEPROM::byteRead(uint16_t byteAddr)
{
	uint8_t byte;	
	i2c->readByte(SLAVE_ADDR,byteAddr,&byte);
	return byte;
}

int8_t	EEPROM::byteRead(uint16_t byteAddr,uint8_t* buf,uint16_t numToRead)
{
	int8_t ret = 0;

//	for(uint16_t i = 0; i < numToRead; i++)
//	{
//			buf[i] = byteRead(byteAddr++);
//	}
	ret = i2c->readByte(SLAVE_ADDR,byteAddr,buf,numToRead);
	if(ret != 0)
		uart1.printf("\r\nerr = %d\r\n",ret);
	return 0;
}




int8_t EEPROM::pageWrite(uint8_t* buf,uint32_t addr,uint8_t numToWrite)
{

	return 0;
}


