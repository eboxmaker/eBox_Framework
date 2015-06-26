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
		void BaseInit(uint16_t period,uint16_t prescaler);
		void SetReload(uint16_t Autoreload);
		void clearCount(void);
		void Interrupt(FunctionalState x);
		void attachInterrupt(void(*callback)(void))
		{
			 TimionecbTable[0] = callback;
		}
};
#endif