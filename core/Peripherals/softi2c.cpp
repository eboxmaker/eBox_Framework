#include "softi2c.h"


Softi2c::Softi2c(uint8_t SDApin, uint8_t SCLpin)
{
	_SDApin = SDApin;
	_SCLpin = SCLpin;
	pinMode(_SDApin,OUTPUT);
	pinMode(_SCLpin,OUTPUT);
	delaytimes = 4;
}		
void Softi2c::_start()
{
	digitalWrite(_SDApin,1);
	digitalWrite(_SCLpin,1);delay_us(delaytimes);
	digitalWrite(_SDApin,0);delay_us(delaytimes);
	digitalWrite(_SCLpin,0);

}

void Softi2c::_stop()
{
	digitalWrite(_SCLpin,0);
	digitalWrite(_SDApin,0);delay_us(delaytimes);
	digitalWrite(_SCLpin,1);delay_us(delaytimes);
	digitalWrite(_SDApin,1);
}
uint8_t Softi2c::_wait_ack()
{
	uint8_t cErrTime = 5;
	pinMode(_SDApin,INPUT_PULLUP);
	digitalWrite(_SCLpin,1);delay_us(delaytimes);
	while(digitalRead(_SDApin))
	{
		cErrTime--;
		delay_us(delaytimes);
		if(cErrTime == 0)
		{
			pinMode(_SDApin,OUTPUT);
			_stop();
			return 0;
		
		}
	
	}
	pinMode(_SDApin,OUTPUT);
	digitalWrite(_SCLpin,0);delay_us(delaytimes);
	return 1;
}
void Softi2c::_send_ack()
{
	digitalWrite(_SDApin,0);delay_us(delaytimes);
	digitalWrite(_SCLpin,1);delay_us(delaytimes);
	digitalWrite(_SCLpin,0);delay_us(delaytimes);

}
void Softi2c::_send_no_ack()	
{
	digitalWrite(_SDApin,1);delay_us(delaytimes);
	digitalWrite(_SCLpin,1);delay_us(delaytimes);
	digitalWrite(_SCLpin,0);delay_us(delaytimes);

}
void Softi2c::_send_byte(uint8_t byte)
{

	uint8_t ii = 8;
	while( ii-- )
	{
		digitalWrite(_SCLpin,0);
		digitalWrite(_SDApin,byte & 0x80);delay_us(delaytimes);
		byte += byte;//<<1
		digitalWrite(_SCLpin,1);delay_us(delaytimes);
		digitalWrite(_SCLpin,0);delay_us(delaytimes);

	}
}
uint8_t Softi2c::_read_byte()
{
	uint8_t i = 8;
	uint8_t byte = 0;
	pinMode(_SDApin,INPUT_PULLUP);
	while(i--)
	{
		byte += byte;
		digitalWrite(_SCLpin,0);delay_us(delaytimes);
		digitalWrite(_SCLpin,1);delay_us(delaytimes);
		byte |= digitalRead(_SDApin);
	}
	digitalWrite(_SCLpin,0);delay_us(delaytimes);
	pinMode(_SDApin,OUTPUT);

	return byte;
}
