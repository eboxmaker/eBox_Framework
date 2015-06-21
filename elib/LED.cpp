#include "led.h"

 uint8_t _LEDNum[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff}; //???

void LED::ShowTimeHMS(uint8_t hour,uint8_t min,uint8_t sec)
{
	int i=0;
		//for( int i = 0; i < 8; i++)
		_LEDBuf[i++] = _LEDNum[16];
		_LEDBuf[i++] = _LEDNum[16];
		_LEDBuf[i++] = _LEDNum[sec%10];
		_LEDBuf[i++] = _LEDNum[sec/10];
		_LEDBuf[i++] = _LEDNum[min%10];
		_LEDBuf[i++] = _LEDNum[min/10];
		_LEDBuf[i++] = _LEDNum[hour%10];
		_LEDBuf[i++] = _LEDNum[hour/10];

		RowOut(_LEDBuf,8);
}
void LED::ShowDateTime(uint16_t year,uint8_t month,uint8_t date)
{
	int i=0;
		//for( int i = 0; i < 8; i++)
		_LEDBuf[i++] = _LEDNum[date%10];
		_LEDBuf[i++] = _LEDNum[date/10];
		_LEDBuf[i++] = _LEDNum[month%10];
		_LEDBuf[i++] = _LEDNum[month/10];
		_LEDBuf[i++] = _LEDNum[year%10];
		_LEDBuf[i++] = _LEDNum[year/10%10];
		_LEDBuf[i++] = _LEDNum[year/100%10];
		_LEDBuf[i++] = _LEDNum[year/1000];


		RowOut(_LEDBuf,8);
}
