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
	
	public:


		TIM(TIM_TypeDef* TIMx);
		TIM(TIM_TypeDef* TIMx,uint32_t period,uint32_t prescaler);
		TIM(TIM_TypeDef* TIMx,uint8_t nothing);
	
		void TIMxBaseInit(uint16_t period,uint16_t prescaler);
		void begin(void);
		void Interrupt(FunctionalState x);
		void start(void);
		void stop(void);
		void SetReload(uint16_t Autoreload);
		void clearCount(void);
		void attachInterrupt(void(*callback)(void))
		{
				gTimxcbTable[_id] = callback;
		}
};
#endif
