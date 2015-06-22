#ifndef __LCD1602_1_H
#define __LCD1602_1_H

#include "ebox.h"
class LCD1602
{
	
	public:
		LCD1602(uint8_t LEDPin,uint8_t ENPin,uint8_t RWPin,uint8_t RSPin,uint8_t DB0,uint8_t DB1,uint8_t DB2,uint8_t DB3,uint8_t DB4,uint8_t DB5,uint8_t DB6,uint8_t DB7);
		LCD1602(uint8_t LEDPin,uint8_t ENPin,uint8_t RWPin,uint8_t RSPin,uint8_t DB0,uint8_t DB1,uint8_t DB2,uint8_t DB3);
	void begin(void);
	void Setpos(uint8_t row,uint8_t col);
	void DispChar(char ch);
	void Setpos_DispChar(uint8_t row,uint8_t col,char ch);
	void DispString(char str[]);
	void Setpos_DispString(uint8_t row,uint8_t col,char str[]);
	void Dispnum(uint32_t num);
	void Setpos_Dispnum(uint8_t row,uint8_t col,uint32_t num);
	void BackLight(u8 i);
	void TEST(void);
	void ShiftLeft();
	void ShiftRight(void);
	void Clear(void);
	void Return(void);
	void Close(void);
	void Open(void);
	void Flicker(void);
	void FlickerChar(uint8_t row,uint8_t col);
	void CloseFlicker(void);
	void FlickerScreen(void);
	void DispTimeHMS(uint8_t hour,uint8_t min,uint8_t sec);
	void DispTimeYMD(uint32_t year,uint8_t month,uint8_t day);
	void DispDateTime(uint32_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec);
	private:
		uint8_t _LEDPin;
		uint8_t _ENPin;
		uint8_t _RWPin;
		uint8_t _RSPin;
		uint8_t _DB0;
		uint8_t _DB1;
		uint8_t _DB2;
		uint8_t _DB3;
		uint8_t _DB4;
		uint8_t _DB5;
		uint8_t _DB6;
		uint8_t _DB7;
	
		uint16_t nDelay;
		inline void LCDdelay(uint32_t nCount);
		inline uint8_t bz();
		void wcmd(uint8_t cmd);
		void wdat(uint8_t dat) ;
		void pos(uint8_t pos);
		


};


#endif
