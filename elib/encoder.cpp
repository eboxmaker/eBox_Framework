#include "encoder.h"
#include "button.h"

#define PULLUP false


Encoder::Encoder(uint8_t Apin,uint8_t Bpin)
{

	_Apin = Apin;
	_Bpin = Bpin;

	pinMode(_Apin,INPUT_PULLUP);
	pinMode(_Bpin,INPUT_PULLUP);


}
int Encoder::ReadEncoder()
{
	_alaststate = _astate;
	_blaststate = _bstate;
	_astate = digitalRead(_Apin);
	_bstate = digitalRead(_Bpin);

	if((_astate == 0) && _alaststate && _bstate)
	{
			return 1;
	}
		if((_bstate == 0) && _blaststate && _astate)
	{
			return 2;
	}

	return 0;
}
