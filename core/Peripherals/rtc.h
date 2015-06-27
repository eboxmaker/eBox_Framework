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
		void begin();
		void config();
		void setCounter(uint32_t count);
		uint32_t readCounter()
		{
			return RTC_GetCounter();
		};
		
		void interrupt(FunctionalState x);
		void attachInterrupt(void (*callbackFun)(void))
		{
				rtcCallbackTable[0] = callbackFun;
		}};
extern RTC_CLASS 	rtc;
#endif

