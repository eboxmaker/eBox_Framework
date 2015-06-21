#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
extern "C"{
	#endif

#include "ebox.h"

	#define PWM_TIM_PERIOD 		1000
	#define PWM_TIM_PRESCALER	72
	
	
class PWM
{
	private:
		uint8_t _pin;
	  uint16_t _duty;
	
	
		uint8_t _ch;
		uint8_t _needremap;
	
		uint16_t _period;
		uint16_t _prescaler;
		TIM_TypeDef *_TIMx;
		uint32_t _rcc;
		uint32_t _irq;

	
	public:
		//PWMPin:有定时器oc通道的io
		//period：定时器的周期。溢出值
		//prescaler:定时器预分频器
		PWM(uint8_t PWMpin);
		PWM(uint8_t PWMpin,uint16_t period,uint16_t prescaler);
		void TIMxBaseInit(uint16_t period,uint16_t prescaler);
		void SetFrq(uint16_t period,uint16_t prescaler);
		void SetDuty(uint16_t duty);

};
void analogWrite(uint8_t pin, uint16_t duty);

#ifdef __cplusplus
}
#endif
#endif
