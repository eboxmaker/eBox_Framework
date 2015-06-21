#ifndef __ENCODER_H
#define __ENCODER_H
#include "ebox.h"
#include "button.h"
class Encoder
{
	public:
		Encoder(uint8_t Apin,uint8_t Bpin);
		int ReadEncoder();


	private:
		
	uint8_t _Apin;
	uint8_t	_Bpin;
	uint8_t	_Spin;
	
	uint8_t _astate;
	uint8_t _alaststate;

	uint8_t _bstate;
	uint8_t _blaststate;

};
#endif
