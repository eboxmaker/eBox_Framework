/*
file   : pwm.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#include "pwm.h"




#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04


	
PWM::PWM(GPIO*  PWMPin,uint32_t Frq)
{
//	if(isPwmPin(PWMpin))
//	{
		pwmPin = PWMPin;
		init_info(PWMPin);
		
		pwmPin->mode(AF_PP);
		set_frq(Frq);

//	}
}
void PWM::base_init(uint16_t Period,uint16_t Prescaler)
{
	period = Period;//更新period

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(rcc, ENABLE);
	TIM_TimeBaseStructure.TIM_Period=Period-1;//ARR
	TIM_TimeBaseStructure.TIM_Prescaler=Prescaler-1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	
	TIM_ARRPreloadConfig(TIMx, ENABLE);

	TIM_Cmd(TIMx, ENABLE); //

}	
void PWM::init_info(GPIO* PWMPin)
{
	if(PWMPin->port == GPIOA)
	{
		switch(PWMPin->pin)
		{
			case GPIO_Pin_0:
				TIMx = TIM2;rcc = RCC_APB1Periph_TIM2;ch = TIMxCH1;//irq = TIM2_IRQn;
				break;
			case GPIO_Pin_1:
				TIMx = TIM2;rcc = RCC_APB1Periph_TIM2;ch = TIMxCH2;//irq = TIM2_IRQn;
				break;
			case GPIO_Pin_2:
				TIMx = TIM2;rcc = RCC_APB1Periph_TIM2;ch = TIMxCH3;//irq = TIM2_IRQn;
				break;
			case GPIO_Pin_3:
				TIMx = TIM2;rcc = RCC_APB1Periph_TIM2;ch = TIMxCH4;//irq = TIM2_IRQn;
				break;
			
			case GPIO_Pin_6:
				TIMx = TIM3;rcc = RCC_APB1Periph_TIM3;ch = TIMxCH1;//irq = TIM3_IRQn;
				break;
			case GPIO_Pin_7:
				TIMx = TIM3;rcc = RCC_APB1Periph_TIM3;ch = TIMxCH2;//irq = TIM3_IRQn;
				break;
			case GPIO_Pin_10:
				TIMx = TIM3;rcc = RCC_APB1Periph_TIM3;ch = TIMxCH3;//irq = TIM3_IRQn;
				break;
			case GPIO_Pin_11:
				TIMx = TIM3;rcc = RCC_APB1Periph_TIM3;ch = TIMxCH4;//irq = TIM3_IRQn;
				break;
		}
	}
	if(PWMPin->port == GPIOB)
	{
		switch(PWMPin->pin)
		{
			case GPIO_Pin_6:
				TIMx = TIM4;rcc = RCC_APB1Periph_TIM4;ch = TIMxCH1;//irq = TIM4_IRQn;
				break;
			case GPIO_Pin_7:
				TIMx = TIM4;rcc = RCC_APB1Periph_TIM4;ch = TIMxCH2;//irq = TIM4_IRQn;
				break;
			case GPIO_Pin_8:
				TIMx = TIM4;rcc = RCC_APB1Periph_TIM4;ch = TIMxCH3;//irq = TIM4_IRQn;
				break;
			case GPIO_Pin_9:
				TIMx = TIM4;rcc = RCC_APB1Periph_TIM4;ch = TIMxCH4;//irq = TIM4_IRQn;
				break;
		}
	}

}


//pwm的频率 = 72M/72/1000;
//
void PWM::set_frq(uint32_t Frq)
{
	uint32_t _period  =0;
	uint32_t _prescaler = 1;
	if(Frq>=720000)Frq = 720000;
	for(;_prescaler <= 0xffff;_prescaler++)
	{
		_period = 72000000/_prescaler/Frq;
		if((0xffff>=_period)&&(_period>=1000))break;
	}
	
	base_init(_period,_prescaler);
	set_duty(duty);

}

//duty:0-1000对应0%-100.0%
void PWM::set_duty(uint16_t Duty)
{

			duty = Duty;
	
			uint16_t pulse = 0;
			float percent;
					
			if(duty>1000)
				duty = 1000;
			percent = duty/1000.0;
			
		  pulse = (uint16_t) (( percent * period ));

			TIM_OCInitTypeDef  TIM_OCInitStructure;
			
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			TIM_OCInitStructure.TIM_Pulse = pulse;
				switch(ch)
				{
					case TIMxCH1:
						TIM_OC1Init(TIMx, &TIM_OCInitStructure);
						break;
					case TIMxCH2:
						TIM_OC2Init(TIMx, &TIM_OCInitStructure);
						break;
					case TIMxCH3:
						TIM_OC3Init(TIMx, &TIM_OCInitStructure);
						break;
					case TIMxCH4:
						TIM_OC4Init(TIMx, &TIM_OCInitStructure);
						break;
				}		
		
		
}
//duty:0-1000对应0%-100.0%
void analog_write(GPIO* pwm_pin, uint16_t duty) 
{
//	if(isPwmPin(PWMpin))
//	{
			PWM p(pwm_pin,1000);
			//p.SetFrq(1000,1);
			p.set_duty(duty);

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
