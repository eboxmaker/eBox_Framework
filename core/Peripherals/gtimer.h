#ifndef __GTIMER_H
#define __GTIMER_H

#include "ebox.h"
#include "interrupts.h"
class TIM
{
	protected:
		

	private:

		uint16_t _period;
		uint16_t _prescaler;
	
		uint8_t _id;
		TIM_TypeDef *_TIMx;
		uint32_t _rcc;
		uint32_t _irq;
	
		void baseInit(uint16_t period,uint16_t prescaler);
	
	public:


		TIM(TIM_TypeDef* TIMx);
		TIM(TIM_TypeDef* TIMx,uint32_t period,uint32_t prescaler);
	
		void begin(void);
		void interrupt(FunctionalState x);
		void start(void);
		void stop(void);
		void setReload(uint16_t Autoreload);
		void clearCount(void);
		void attachInterrupt(void(*callback)(void))
		{
				gTimxCallbackTable[_id] = callback;
		}
};
#endif
