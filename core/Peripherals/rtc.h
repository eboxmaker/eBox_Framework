#ifndef __RTC_H
#define __RTC_H
#include "ebox.h"


#define RTC_EVENT_OW		((uint16_t)0x0004)  /*!< Overflow interrupt */
#define RTC_EVENT_ALR 	((uint16_t)0x0002)  /*!< Alarm interrupt */
#define RTC_EVENT_SEC		((uint16_t)0x0001)  /*!< Second interrupt */

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
		void setAlarm(uint32_t count);
		uint32_t getCounter();
	
		void setAlarm(uint8_t h,uint8_t m,uint8_t s);
		void getTimeHMS(uint8_t* h,uint8_t* m,uint8_t* s);
		void setTimeHMS(uint8_t h,uint8_t m,uint8_t s);
		
		void interrupt(uint32_t bits,FunctionalState x);
		void attachInterrupt(void (*callbackFun)(void),uint16_t event);
};
extern RTC_CLASS 	rtc;
#endif

