#include "softi2c.h"


Softi2c::Softi2c(uint8_t SDApin, uint8_t SCLpin)
{
	_SDApin = SDApin;
	_SCLpin = SCLpin;
	pinMode(_SDApin,OUTPUT);
	pinMode(_SCLpin,OUTPUT);
	_delaytimes = 4;
}		
void Softi2c::start()
{
	digitalWrite(_SDApin,1);
	digitalWrite(_SCLpin,1);delay_us(_delaytimes);
	digitalWrite(_SDApin,0);delay_us(_delaytimes);
	digitalWrite(_SCLpin,0);

}

void Softi2c::stop()
{
	digitalWrite(_SCLpin,0);
	digitalWrite(_SDApin,0);delay_us(_delaytimes);
	digitalWrite(_SCLpin,1);delay_us(_delaytimes);
	digitalWrite(_SDApin,1);
}
uint8_t Softi2c::wait_ack()
{
	uint8_t cErrTime = 5;
	pinMode(_SDApin,INPUT_PULLUP);
	digitalWrite(_SCLpin,1);delay_us(_delaytimes);
	while(digitalRead(_SDApin))
	{
		cErrTime--;
		delay_us(_delaytimes);
		if(cErrTime == 0)
		{
			pinMode(_SDApin,OUTPUT);
			stop();
			return 0;
		
		}
	
	}
	pinMode(_SDApin,OUTPUT);
	digitalWrite(_SCLpin,0);delay_us(_delaytimes);
	return 1;
}
void Softi2c::send_ack()
{
	digitalWrite(_SDApin,0);delay_us(_delaytimes);
	digitalWrite(_SCLpin,1);delay_us(_delaytimes);
	digitalWrite(_SCLpin,0);delay_us(_delaytimes);

}
void Softi2c::send_no_ack()	
{
	digitalWrite(_SDApin,1);delay_us(_delaytimes);
	digitalWrite(_SCLpin,1);delay_us(_delaytimes);
	digitalWrite(_SCLpin,0);delay_us(_delaytimes);

}
void Softi2c::send_byte(uint8_t byte)
{

	uint8_t ii = 8;
	while( ii-- )
	{
		digitalWrite(_SCLpin,0);
		digitalWrite(_SDApin,byte & 0x80);delay_us(_delaytimes);
		byte += byte;//<<1
		digitalWrite(_SCLpin,1);delay_us(_delaytimes);
		digitalWrite(_SCLpin,0);delay_us(_delaytimes);

	}
}
uint8_t Softi2c::read_byte()
{
	uint8_t i = 8;
	uint8_t byte = 0;
	pinMode(_SDApin,INPUT_PULLUP);
	while(i--)
	{
		byte += byte;
		digitalWrite(_SCLpin,0);delay_us(_delaytimes);
		digitalWrite(_SCLpin,1);delay_us(_delaytimes);
		byte |= digitalRead(_SDApin);
	}
	digitalWrite(_SCLpin,0);delay_us(_delaytimes);
	pinMode(_SDApin,OUTPUT);

	return byte;
}
