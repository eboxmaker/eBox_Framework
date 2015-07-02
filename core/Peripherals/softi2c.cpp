#include "softi2c.h"

#define DELAYTIMES 3
Softi2c::Softi2c(uint8_t SDApin, uint8_t SCLpin)
{
	_SDApin = SDApin;
	_SCLpin = SCLpin;
	_delayTimes = DELAYTIMES;//
}	
void Softi2c::i2cBegin()
{
	setSpeed(100000);
	pinMode(_SDApin,OUTPUT);
	pinMode(_SCLpin,OUTPUT);
}
int8_t Softi2c::setSpeed(uint32_t speed)
{
	switch(speed)
	{
		case 400000:
			_delayTimes = 0;
			break;
		case 300000:
			_delayTimes = 1;
			break;
		case 200000:
			_delayTimes = 2;
			break;
		case 100000:
			_delayTimes = 3;
			break;
		default:
			_delayTimes = speed;
			break;
	}
	return 0;
}
void Softi2c::start()
{
	pinMode(_SDApin,OUTPUT);
	digitalWrite(_SDApin,1);
	digitalWrite(_SCLpin,1);delay_us(_delayTimes);
	digitalWrite(_SDApin,0);delay_us(_delayTimes);
	digitalWrite(_SCLpin,0);

}

void Softi2c::stop()
{
	pinMode(_SDApin,OUTPUT);
	digitalWrite(_SCLpin,0);
	digitalWrite(_SDApin,0);delay_us(_delayTimes);
	digitalWrite(_SCLpin,1);delay_us(_delayTimes);
	digitalWrite(_SDApin,1);
}
int8_t Softi2c::waitAck()
{
	uint8_t cErrTime = 5;
	pinMode(_SDApin,INPUT_PULLUP);
	digitalWrite(_SCLpin,1);delay_us(_delayTimes);
	while(digitalRead(_SDApin))
	{
		cErrTime--;
		delay_us(_delayTimes);
		if(cErrTime == 0)
		{
			pinMode(_SDApin,OUTPUT);
			stop();
			return -1;
		
		}
	
	}
	digitalWrite(_SCLpin,0);delay_us(_delayTimes);
	return 0;
}
void Softi2c::sendAck()
{
	pinMode(_SDApin,OUTPUT);
	digitalWrite(_SDApin,0);delay_us(_delayTimes);
	digitalWrite(_SCLpin,1);delay_us(_delayTimes);
	digitalWrite(_SCLpin,0);delay_us(_delayTimes);

}
void Softi2c::sendNoAck()	
{
	pinMode(_SDApin,OUTPUT);
	digitalWrite(_SDApin,1);delay_us(_delayTimes);
	digitalWrite(_SCLpin,1);delay_us(_delayTimes);
	digitalWrite(_SCLpin,0);delay_us(_delayTimes);

}
void Softi2c::sendByte(uint8_t byte)
{

	uint8_t ii = 8;
	pinMode(_SDApin,OUTPUT);
	while( ii-- )
	{
		digitalWrite(_SCLpin,0);
		digitalWrite(_SDApin,byte & 0x80);delay_us(_delayTimes);
		byte += byte;//<<1
		digitalWrite(_SCLpin,1);delay_us(_delayTimes);
		digitalWrite(_SCLpin,0);delay_us(_delayTimes);
	}

}
uint8_t Softi2c::receiveByte()
{
	uint8_t i = 8;
	uint8_t byte = 0;
	pinMode(_SDApin,INPUT_PULLUP);
	while(i--)
	{
		byte += byte;
		digitalWrite(_SCLpin,0);delay_us(_delayTimes);
		digitalWrite(_SCLpin,1);delay_us(_delayTimes);
		byte |= digitalRead(_SDApin);
	}
	digitalWrite(_SCLpin,0);delay_us(_delayTimes);

	return byte;
}
int8_t Softi2c::writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t data)
{
		start();
		sendByte(slaveAddress);
    if (waitAck() == -1)
        return -1;
		
		sendByte(regAddress);
    if (waitAck() == -1)
        return -2;

   
		sendByte(data);
    if (waitAck() == -1)
        return -3;
		stop();

    delay_us(10);      
    return 0;
}
int8_t 	Softi2c::readByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* buf,uint8_t numToRead)
{

	int i = 0;
		start();
		sendByte(slaveAddress);
    if (waitAck() == -1)
        return -1;

		sendByte(regAddress);
    if (waitAck() == -1)
        return -2;

		start();
		sendByte(slaveAddress + 1);
    if (waitAck() == -1)
        return -3;
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
