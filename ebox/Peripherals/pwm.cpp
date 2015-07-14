/*
file   : pwm.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#include "pwm.h"


#define PWM_PIN_NUM 20


#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04

//	PWMpin
PWM::PWM(GPIO* PWMPin)
{
//	if(isPwmPin(PWMpin))
//	{
		_PWMPin = PWMPin;
		_period = PWM_TIM_PERIOD;
		_prescaler = PWM_TIM_PRESCALER;
		
		initInfo(_PWMPin);

		_PWMPin->mode(AF_PP);
		baseInit(_period,_prescaler);

//	}
}
//	
PWM::PWM(GPIO*  PWMPin,uint16_t period,uint16_t prescaler)
{
//	if(isPwmPin(PWMpin))
//	{
		_PWMPin = PWMPin;
		_period = period;
		_prescaler = prescaler;
		initInfo(PWMPin);
		
		_PWMPin->mode(AF_PP);
		baseInit(_period,_prescaler);
//	}
}
void PWM::baseInit(uint16_t period,uint16_t prescaler)
{
	_period = period;
	_prescaler = prescaler;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(_rcc, ENABLE);
	TIM_TimeBaseStructure.TIM_Period=period-1;//ARR
	TIM_TimeBaseStructure.TIM_Prescaler=prescaler-1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //
	TIM_TimeBaseInit(_TIMx, &TIM_TimeBaseStructure);
	
	TIM_ARRPreloadConfig(_TIMx, ENABLE);

	TIM_Cmd(_TIMx, ENABLE); //

}	
void PWM::initInfo(GPIO* PWMPin)
{
	if(PWMPin->port == GPIOA)
	{
		switch(PWMPin->pin)
		{
			case GPIO_Pin_0:
				_TIMx = TIM2;_rcc = RCC_APB1Periph_TIM2;_irq = TIM2_IRQn;_ch = TIMxCH1;
				break;
			case GPIO_Pin_1:
				_TIMx = TIM2;_rcc = RCC_APB1Periph_TIM2;_irq = TIM2_IRQn;_ch = TIMxCH2;
				break;
			case GPIO_Pin_2:
				_TIMx = TIM2;_rcc = RCC_APB1Periph_TIM2;_irq = TIM2_IRQn;_ch = TIMxCH3;
				break;
			case GPIO_Pin_3:
				_TIMx = TIM2;_rcc = RCC_APB1Periph_TIM2;_irq = TIM2_IRQn;_ch = TIMxCH4;
				break;
			
			case GPIO_Pin_6:
				_TIMx = TIM3;_rcc = RCC_APB1Periph_TIM3;_irq = TIM3_IRQn;_ch = TIMxCH1;
				break;
			case GPIO_Pin_7:
				_TIMx = TIM3;_rcc = RCC_APB1Periph_TIM3;_irq = TIM3_IRQn;_ch = TIMxCH2;
				break;
			case GPIO_Pin_10:
				_TIMx = TIM3;_rcc = RCC_APB1Periph_TIM3;_irq = TIM3_IRQn;_ch = TIMxCH3;
				break;
			case GPIO_Pin_11:
				_TIMx = TIM3;_rcc = RCC_APB1Periph_TIM3;_irq = TIM3_IRQn;_ch = TIMxCH4;
				break;
		}
	}
	if(PWMPin->port == GPIOB)
	{
		switch(PWMPin->pin)
		{
			case GPIO_Pin_6:
				_TIMx = TIM4;_rcc = RCC_APB1Periph_TIM4;_irq = TIM4_IRQn;_ch = TIMxCH1;
				break;
			case GPIO_Pin_7:
				_TIMx = TIM4;_rcc = RCC_APB1Periph_TIM4;_irq = TIM4_IRQn;_ch = TIMxCH2;
				break;
			case GPIO_Pin_8:
				_TIMx = TIM4;_rcc = RCC_APB1Periph_TIM4;_irq = TIM4_IRQn;_ch = TIMxCH3;
				break;
			case GPIO_Pin_9:
				_TIMx = TIM4;_rcc = RCC_APB1Periph_TIM4;_irq = TIM4_IRQn;_ch = TIMxCH4;
				break;
		}
	}

}

void PWM::setFrq(uint16_t period,uint16_t prescaler)
{
	_period = period;
	_prescaler = prescaler;
	baseInit(_period,_prescaler);
	setDuty(_duty);

}


//duty:0-1000对应0%-100.0%
void PWM::setDuty(uint16_t duty)
{

			_duty = duty;
	
			uint16_t pulse = 0;
			float percent;
					
			if(_duty>1000)
				_duty = 1000;
			percent = _duty/1000.0;
			
		  pulse = (uint16_t) (( percent * _period ));
			
			TIM_OCInitTypeDef  TIM_OCInitStructure;
			
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			TIM_OCInitStructure.TIM_Pulse = pulse;
				switch(_ch)
				{
					case TIMxCH1:
						TIM_OC1Init(_TIMx, &TIM_OCInitStructure);
						break;
					case TIMxCH2:
						TIM_OC2Init(_TIMx, &TIM_OCInitStructure);
						break;
					case TIMxCH3:
						TIM_OC3Init(_TIMx, &TIM_OCInitStructure);
						break;
					case TIMxCH4:
						TIM_OC4Init(_TIMx, &TIM_OCInitStructure);
						break;
				}		
		
		
}
//duty:0-1000对应0%-100.0%
void analogWrite(GPIO* PWMpin, uint16_t duty) 
{
//	if(isPwmPin(PWMpin))
//	{
			PWM p(PWMpin);
			//p.SetFrq(1000,1);
			p.setDuty(duty);

//	}
//	else
//	{
//	;
//	}
}

//////////////////////////////////////////////////////////////
uint8_t isPwmPin(uint8_t pin)
{

	return 0;
}
//uint8_t isPinNeedRemap(uint8_t pin)
//{
//	int i;

//	for( i = 0; i<255; i++)
//	{
//		if(pinTOTimx[i].pin == pin)
//			
//			return pinTOTimx[i].needremap;
//	}
//		return 0XFF;
//}
