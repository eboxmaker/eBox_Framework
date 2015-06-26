#ifndef __TIMERONE_H
#define __TIMERONE_H
#include "ebox.h"
class TIMERONE
{

	private:
		uint16_t _period;
		uint16_t _prescaler;
	
//		uint8_t _id;
//		TIM_TypeDef *_TIMx;
//		uint32_t _rcc;
//		uint32_t _irq;
	public:
		TIMERONE();
		void begin();
		void start();
		void stop();
		void baseInit(uint16_t period,uint16_t prescaler);
		void setReload(uint16_t Autoreload);
		void clearCount(void);
		void interrupt(FunctionalState x);
		void attachInterrupt(void(*callback)(void))
		{
			 TimOnecbTable[0] = callback;
		}
};
#endif
