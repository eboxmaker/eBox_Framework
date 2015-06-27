#ifndef __RTC_H
#define __RTC_H
#include "ebox.h"

class RTC_CLASS
{



		
	private:
		uint8_t isConfig()
		{
			uint8_t flag = 0;
			if(BKP_ReadBackupRegister(BKP_DR1) == 0xA5A5)
				flag = 1;
			return flag;
		}
		void setConfigFlag()
		{
				BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
		}
	public:
		uint8_t sec;
		uint8_t min;
		uint8_t hour;
	public:
		void begin();
		void config();
		void setCounter(uint32_t count);
		uint32_t getCounter()
		{
			return RTC_GetCounter();
		};
		void getTimeHMS(uint8_t* h,uint8_t* m,uint8_t* s)
		{
			uint32_t tmp = 0;
			tmp = getCounter() % 0x15180;
			*h = (tmp / 3600);
			*m = (tmp % 3600)/60;
			*s = (tmp % 3600) %60;
		};
		void setTimeHMS(uint8_t h,uint8_t m,uint8_t s)
		{
			uint32_t tmp = 0;
			tmp = h*3600 + m*60 + s;
			setCounter(tmp);
		}
		
		void interrupt(FunctionalState x);
		void attachInterrupt(void (*callbackFun)(void))
		{
				rtcCallbackTable[0] = callbackFun;
		};
};
extern RTC_CLASS 	rtc;
#endif

