#include "softi2c.h"

Softi2c::Softi2c(GPIO* SDApin, GPIO* SCLpin)
{
	_SDApin = SDApin;
	_SCLpin = SCLpin;
}	
void Softi2c::i2cBegin()
{
	setSpeed(SOFT_I2C_SPEED);
	pMode(_SDApin,_OPP);
	pMode(_SCLpin,_OPP);
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
	pMode(_SDApin,_OPP);
	dgWrite(_SDApin,1);
	dgWrite(_SCLpin,1);delay_us(_delayTimes);
	dgWrite(_SDApin,0);delay_us(_delayTimes);
	dgWrite(_SCLpin,0);

}

void Softi2c::stop()
{
	pMode(_SDApin,_OPP);
	dgWrite(_SCLpin,0);
	dgWrite(_SDApin,0);delay_us(_delayTimes);
	dgWrite(_SCLpin,1);delay_us(_delayTimes);
	dgWrite(_SDApin,1);
}
int8_t Softi2c::waitAck()
{
	uint8_t cErrTime = 5;
	pMode(_SDApin,_IPU);
	dgWrite(_SCLpin,1);delay_us(_delayTimes);
	while(dgRead(_SDApin))
	{
		cErrTime--;
		delay_us(_delayTimes);
		if(cErrTime == 0)
		{
			pMode(_SDApin,_OPP);
			stop();
			return -1;
		
		}
	
	}
	dgWrite(_SCLpin,0);delay_us(_delayTimes);
	return 0;
}
void Softi2c::sendAck()
{
	pMode(_SDApin,_OPP);
	dgWrite(_SDApin,0);delay_us(_delayTimes);
	dgWrite(_SCLpin,1);delay_us(_delayTimes);
	dgWrite(_SCLpin,0);delay_us(_delayTimes);

}
void Softi2c::sendNoAck()	
{
	pMode(_SDApin,_OPP);
	dgWrite(_SDApin,1);delay_us(_delayTimes);
	dgWrite(_SCLpin,1);delay_us(_delayTimes);
	dgWrite(_SCLpin,0);delay_us(_delayTimes);

}
void Softi2c::sendByte(uint8_t byte)
{

	uint8_t ii = 8;
	pMode(_SDApin,_OPP);
	while( ii-- )
	{
		dgWrite(_SCLpin,0);
		dgWrite(_SDApin,byte & 0x80);delay_us(_delayTimes);
		byte += byte;//<<1
		dgWrite(_SCLpin,1);delay_us(_delayTimes);
		dgWrite(_SCLpin,0);delay_us(_delayTimes);
	}

}
uint8_t Softi2c::receiveByte(void)
{
	uint8_t i = 8;
	uint8_t byte = 0;
	pMode(_SDApin,_IPU);
	while(i--)
	{
		byte += byte;
		dgWrite(_SCLpin,0);delay_us(_delayTimes);
		dgWrite(_SCLpin,1);delay_us(_delayTimes);
		byte |= dgRead(_SDApin);
	}
	dgWrite(_SCLpin,0);delay_us(_delayTimes);

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
int8_t Softi2c::writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead)
{
		start();
		sendByte(slaveAddress);
    if (waitAck() == -1)
        return -1;
		
		sendByte(regAddress);
    if (waitAck() == -1)
        return -2;

		while(numToRead--)
		{
			sendByte(*data++);
			if (waitAck() == -1)
					return -3;
		}
		
		stop();

    delay_us(10);      
    return 0;
}
int8_t 	Softi2c::readByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data)
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

		*data = receiveByte();
		sendNoAck();
		stop();

	return i;
}
int8_t 	Softi2c::readByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead)
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
			*data++ = receiveByte();
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
