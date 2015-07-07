#include "pwm.h"


#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04

typedef struct 
{
	TIM_TypeDef *TIMx;
	uint32_t rcc;
	uint32_t irq;
	uint8_t ch;
	uint8_t needremap;
	uint8_t pin;

}PIN_TO_TIMx;
/////////PWM pin support////////////////////
const PIN_TO_TIMx pinTOTimx[]=
{
	//{TIMx,rcc,irqch,TIMxCHx,needremap,pin}

//		{TIM1,RCC_APB2Periph_TIM1,TIM1_UP_IRQn,TIMxCH1,0,0x08},//暂时不支持
//		{TIM1,RCC_APB2Periph_TIM1,TIM1_UP_IRQn,TIMxCH2,0,0x09},//暂时不支持
//		{TIM1,RCC_APB2Periph_TIM1,TIM1_UP_IRQn,TIMxCH3,0,0x0a},//暂时不支持
//		{TIM1,RCC_APB2Periph_TIM1,TIM1_UP_IRQn,TIMxCH4,0,0x0b},//暂时不支持

		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH1,0,0x00},
		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH2,0,0x01},
		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH3,0,0x02},
		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH4,0,0x03},
		
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH1,0,0x06},
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH2,0,0x07},
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH3,0,0x10},
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH4,0,0x11},
		
		{TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn,TIMxCH1,0,0x16},
		{TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn,TIMxCH2,0,0x17},
		{TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn,TIMxCH3,0,0x18},
		{TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn,TIMxCH4,0,0x19},
		/////一下引脚需要开启ReMaping///////////////////////////
	#if defined (MCUPIN48) || (defined MCUPIN64) || (defined MCUPIN100) || (defined MCUPIN144)

		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH1,1,0x00},//pa0
		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH2,1,0x01},//Pa1
		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH1,1,0x0f},//pa15
		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH2,1,0x1d},//Pb13
		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH3,1,0x1a},//pb10
		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH4,1,0x1b},//PB11
	#if defined (MCUPIN64) || (defined MCUPIN100) || (defined MCUPIN144)
		
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH1,1,0x10},//pb0
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH2,1,0x11},//pb1
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH1,1,0x26},//pc6
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH2,1,0x27},
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH3,1,0x28},
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH4,1,0x29},
		
	#if defined (MCUPIN100) || (defined MCUPIN144)
		{TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn,TIMxCH1,1,0x3c},//pd12
		{TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn,TIMxCH2,1,0x3d},
		{TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn,TIMxCH3,1,0x3e},
		{TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn,TIMxCH4,1,0x3f},
		
	#endif
	#endif
	#endif

};
/////////////////////////////////////////
TIM_TypeDef* 	pinToTIMx(uint8_t pin);
uint8_t 			pinToTIM_ch(uint8_t pin);
uint32_t 			pinToTIM_rcc(uint8_t pin);
uint32_t 			pinToTIM_irq(uint8_t pin);
uint8_t 			isPwmPin(uint8_t pin);
//	PWMpin	: ebox pin
PWM::PWM(uint8_t PWMpin)
{
	if(isPwmPin(PWMpin))
	{
		_pin = PWMpin;
		_period = PWM_TIM_PERIOD;
		_prescaler = PWM_TIM_PRESCALER;
		
		_TIMx = pinToTIMx(_pin);
		_rcc = pinToTIM_rcc(_pin);
		_irq = pinToTIM_irq(_pin);
		_ch = pinToTIM_ch(_pin);
		
		pinMode(_pin,AF_PP);
		baseInit(_period,_prescaler);

	}
}
//	
PWM::PWM(uint8_t PWMpin,uint16_t period,uint16_t prescaler)
{
	if(isPwmPin(PWMpin))
	{
		_pin = PWMpin;
		_period = period;
		_prescaler = prescaler;
		
		_TIMx = pinToTIMx(_pin);
		_rcc = pinToTIM_rcc(_pin);
		_irq = pinToTIM_irq(_pin);
		_ch = pinToTIM_ch(_pin);
		
		pinMode(_pin,AF_PP);
		baseInit(_period,_prescaler);
		digitalWrite(_pin,HIGH);
	}
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
void analogWrite(uint8_t PWMpin, uint16_t duty) 
{
	if(isPwmPin(PWMpin))
	{
			PWM p(PWMpin);
			//p.SetFrq(1000,1);
			p.setDuty(duty);

	}
	else
	{
	;
	}
}

//////////////////////////////////////////////////////////////
TIM_TypeDef* pinToTIMx(uint8_t pin)
{
	int i;
	for(i = 0; i<PWM_PIN_NUM; i++)
	{
		if(pinTOTimx[i].pin == pin)
			
			return pinTOTimx[i].TIMx;
	}
	return 0;
}

uint8_t pinToTIM_ch(uint8_t pin)
{
		int i;

	for( i = 0; i<PWM_PIN_NUM; i++)
	{
		if(pinTOTimx[i].pin == pin)
			
			return pinTOTimx[i].ch;
	}
	return 0;
}
uint32_t pinToTIM_rcc(uint8_t pin)
{
		int i;

	for( i = 0; i<PWM_PIN_NUM; i++)
	{
		if(pinTOTimx[i].pin == pin)
			
			return pinTOTimx[i].rcc;
	}
	return 0;
}
uint32_t pinToTIM_irq(uint8_t pin)
{
		int i;

	for( i = 0; i<PWM_PIN_NUM; i++)
	{
		if(pinTOTimx[i].pin == pin)
			
			return pinTOTimx[i].irq;
	}
	return 0;
}

uint8_t isPwmPin(uint8_t pin)
{
	int i;
	for( i = 0; i<PWM_PIN_NUM; i++)
	{
		if(pinTOTimx[i].pin == pin)
			
			return 1;
	}
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
