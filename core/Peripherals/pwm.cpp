#include "pwm.h"

//	PWMpin	: ebox pin
PWM::PWM(uint8_t PWMpin)
{
	if(IsPwmPin(PWMpin))
	{
		_pin = PWMpin;
		_period = PWM_TIM_PERIOD;
		_prescaler = PWM_TIM_PRESCALER;
		
		_TIMx = PinToTIMx(_pin);
		_rcc = PinToTIM_rcc(_pin);
		_irq = PinToTIM_irq(_pin);
		_ch = PinToTIM_ch(_pin);
		
		pinMode(_pin,AF_PP);
		TIMxBaseInit(_period,_prescaler);

	}
}
//	PWMpin	: arduino pin

PWM::PWM(uint8_t PWMpin,uint16_t period,uint16_t prescaler)
{
	if(IsPwmPin(PWMpin))
	{
		_pin = PWMpin;
		_period = period;
		_prescaler = prescaler;
		
		_TIMx = PinToTIMx(_pin);
		_rcc = PinToTIM_rcc(_pin);
		_irq = PinToTIM_irq(_pin);
		_ch = PinToTIM_ch(_pin);
		
		pinMode(_pin,AF_PP);
		TIMxBaseInit(_period,_prescaler);
		digitalWrite(_pin,HIGH);
	}
}
void PWM::TIMxBaseInit(uint16_t period,uint16_t prescaler)
{
	_period = period;
	_prescaler = prescaler;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(_rcc, ENABLE);
	TIM_TimeBaseStructure.TIM_Period=period-1;//ARR??
	TIM_TimeBaseStructure.TIM_Prescaler=prescaler-1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	TIM_TimeBaseInit(_TIMx, &TIM_TimeBaseStructure);
	
	//TIM_ARRPreloadConfig(_TIMx, ENABLE);

	TIM_Cmd(_TIMx, ENABLE); //

}		
void PWM::SetFrq(uint16_t period,uint16_t prescaler)
{
	_period = period;
	_prescaler = prescaler;
	TIMxBaseInit(_period,_prescaler);
	SetDuty(_duty);

}


//duty:0-1000对应0%-100.0%
void PWM::SetDuty(uint16_t duty)
{

			_duty = duty;
	
			uint16_t Pulse = 0;
			float percent;
					
			if(_duty>1000)
				_duty = 1000;
			percent = _duty/1000.0;
			
		  Pulse = (uint16_t) (( percent * _period ));
			
			TIM_OCInitTypeDef  TIM_OCInitStructure;
			
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			TIM_OCInitStructure.TIM_Pulse = Pulse;
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
	if(IsPwmPin(PWMpin))
	{
			PwmPinStatu[PWMpin] = 1;
			PWM p(PWMpin);
			//p.SetFrq(1000,1);
			p.SetDuty(duty);

	}
	else
	{
	;
	}
}
