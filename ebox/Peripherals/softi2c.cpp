#include "softi2c.h"

Softi2c::Softi2c(GPIO* SDAPin, GPIO* SCLPin)
{
	sdaPin = SDAPin;
	sclPin = SCLPin;
}	
void Softi2c::i2cBegin(uint32_t speed)
{
	setSpeed(speed);
	sdaPin->mode(OUTPUT_PP);
	sclPin->mode(OUTPUT_PP);
}
int8_t Softi2c::setSpeed(uint32_t speed)
{
	switch(speed)
	{
		case 400000:
			_delayTimes = 1;
			break;
		case 300000:
			_delayTimes = 2;
			break;
		case 200000:
			_delayTimes = 4;
			break;
		case 100000:
			_delayTimes = 8;
			break;
		default:
			_delayTimes = speed;
			break;
	}
	return 0;
}
void Softi2c::start()
{
	sdaPin->mode(OUTPUT_PP);
	sdaPin->set();
	sclPin->set();delay_us(_delayTimes);
	sdaPin->reset();delay_us(_delayTimes);
	sclPin->reset();

}

void Softi2c::stop()
{
	sdaPin->mode(OUTPUT_PP);
	sclPin->reset();
	sdaPin->reset();delay_us(_delayTimes);
	sclPin->set();delay_us(_delayTimes);
	sdaPin->set();
}
int8_t Softi2c::waitAck()
{
	uint8_t cErrTime = 5;
	sdaPin->mode(INPUT_PU);
	sclPin->set();delay_us(_delayTimes);
	while(sdaPin->read())
	{
		cErrTime--;
		delay_us(_delayTimes);
		if(cErrTime == 0)
		{
			sdaPin->mode(OUTPUT_PP);
			stop();
			return -1;
		
		}
	
	}
	sclPin->reset();delay_us(_delayTimes);
	return 0;
}
void Softi2c::sendAck()
{
	sdaPin->mode(OUTPUT_PP);
	sdaPin->reset();delay_us(_delayTimes);
	sclPin->set();delay_us(_delayTimes);
	sclPin->reset();delay_us(_delayTimes);

}
void Softi2c::sendNoAck()	
{
	sdaPin->mode(OUTPUT_PP);
	sdaPin->set();delay_us(_delayTimes);
	sclPin->set();delay_us(_delayTimes);
	sclPin->reset();delay_us(_delayTimes);

}
void Softi2c::sendByte(uint8_t byte)
{

	uint8_t ii = 8;
	sdaPin->mode(OUTPUT_PP);
	while( ii-- )
	{
		sclPin->reset();
		sdaPin->write(byte & 0x80);delay_us(_delayTimes);
		byte += byte;//<<1
		sclPin->set();delay_us(_delayTimes);
		sclPin->reset();delay_us(_delayTimes);
	}

}
uint8_t Softi2c::receiveByte(void)
{
	uint8_t i = 8;
	uint8_t byte = 0;
	sdaPin->mode(INPUT_PU);
	while(i--)
	{
		byte += byte;
		sclPin->reset();delay_us(_delayTimes);
		sclPin->set();delay_us(_delayTimes);
		byte |= sdaPin->read();
	}
	sclPin->reset();delay_us(_delayTimes);

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
