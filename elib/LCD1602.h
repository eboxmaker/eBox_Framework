/*
file   : lcd1602.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#ifndef __LCD1602_1_H
#define __LCD1602_1_H

#include "ebox.h"
class LCD1602
{
	
	public:
		LCD1602(GPIO* LEDPin,GPIO* ENPin,GPIO* RWPin,GPIO* RSPin,GPIO* DB0,GPIO* DB1,GPIO* DB2,GPIO* DB3,GPIO* DB4,GPIO* DB5,GPIO* DB6,GPIO* DB7);
		LCD1602(GPIO* LEDPin,GPIO* ENPin,GPIO* RWPin,GPIO* RSPin,GPIO* DB0,GPIO* DB1,GPIO* DB2,GPIO* DB3);
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
		GPIO* _LEDPin;
		GPIO* _ENPin;
		GPIO* _RWPin;
		GPIO* _RSPin;
		GPIO* _DB0;
		GPIO* _DB1;
		GPIO* _DB2;
		GPIO* _DB3;
		GPIO* _DB4;
		GPIO* _DB5;
		GPIO* _DB6;
		GPIO* _DB7;
	
		uint16_t nDelay;
		inline void LCDdelay(uint32_t nCount);
		inline uint8_t bz();
		void wcmd(uint8_t cmd);
		void wdat(uint8_t dat) ;
		void pos(uint8_t pos);
		


};


#endif
