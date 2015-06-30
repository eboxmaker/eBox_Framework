#include "softi2c.h"

#define DELAYTIMES 1
Softi2c::Softi2c(uint8_t SDApin, uint8_t SCLpin)
{
	_SDApin = SDApin;
	_SCLpin = SCLpin;
	pinMode(_SDApin,OUTPUT);
	pinMode(_SCLpin,OUTPUT);
	_delayTimes = DELAYTIMES;//
}	
void Softi2c::setSpeed(uint32_t speed)
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
uint8_t Softi2c::waitAck()
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
			return 0;
		
		}
	
	}
	digitalWrite(_SCLpin,0);delay_us(_delayTimes);
	return 1;
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
