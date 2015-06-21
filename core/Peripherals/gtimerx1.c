#include "gtimerx.h"

uint16_t _period;
uint16_t _prescaler;

TIM_TypeDef *_TIMx;
uint32_t _rcc;
uint32_t _irq;

#define PRESCALE 72
#define PEROID	1000





	///////////////////////////////////////////////////////////////////////////////////////
uint32_t  TIMxToRCC(TIM_TypeDef* TIMx)
{
	int i;
	uint32_t rcc;
	for(i=0;i<10;i++)
	{
		if(_A_TIMx[i].timx == TIMx)
		{
					rcc = _A_TIMx[i].rcc;
			break;

		}
	
	}
	return rcc;
}
uint32_t TIMxToIRQ(TIM_TypeDef* TIMx)
{
	uint32_t irq;
	int i;
	for(i=0;i<10;i++)
	{
		if(_A_TIMx[i].timx == TIMx)
		{
			irq = _A_TIMx[i].irq;
			break;
		}
	
	}	

	return irq;
}	

void Frequence(uint16_t period,uint16_t prescaler)
{
	
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	_period = period;
	_prescaler = prescaler;
// TIM_DeInit(_TIMx);
 RCC_APB1PeriphClockCmd(_rcc, ENABLE);
 TIM_TimeBaseStructure.TIM_Period=period-1;//ARR??
 TIM_TimeBaseStructure.TIM_Prescaler=prescaler-1;
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
 TIM_TimeBaseInit(_TIMx, &TIM_TimeBaseStructure);
 TIM_ARRPreloadConfig(_TIMx, ENABLE);
	
 Interrupt(ENABLE);
	
 TIM_Cmd(_TIMx, ENABLE); //????

}
void Interrupt(FunctionalState x)
{
 NVIC_InitTypeDef NVIC_InitStructure;
	
 TIM_ClearITPendingBit(_TIMx , TIM_FLAG_Update);
 TIM_ITConfig(_TIMx,TIM_IT_Update,x);
	
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);   // ??????? 
 NVIC_InitStructure.NVIC_IRQChannel = _irq;//?????
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;// ????????1
 NVIC_InitStructure.NVIC_IRQChannelCmd = x;
 NVIC_Init(&NVIC_InitStructure);

}
void CMD_Enable(FunctionalState x)
{
	 TIM_Cmd(_TIMx, x); //????
}

void TIMx_DeInit(TIM_TypeDef* TIMx)
{
	 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;

	_TIMx = TIMx;
	_rcc = TIMxToRCC(TIMx);
	_irq = TIMxToIRQ(TIMx);
//		_period = period;
//		_prescaler = prescaler;
	 TIM_DeInit(_TIMx);
	 RCC_APB1PeriphClockCmd(_rcc, ENABLE);
	 TIM_TimeBaseStructure.TIM_Period=PEROID;//ARR??
	 TIM_TimeBaseStructure.TIM_Prescaler=PRESCALE;
	 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	 TIM_TimeBaseInit(_TIMx, &TIM_TimeBaseStructure);
	 TIM_ARRPreloadConfig(_TIMx, ENABLE);
	 TIM_ITConfig(_TIMx,TIM_IT_Update,ENABLE);
	
	 TIM_ClearITPendingBit(_TIMx , TIM_FLAG_Update);
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);   // ??????? 
	 NVIC_InitStructure.NVIC_IRQChannel = _irq;//?????
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;// ????????1
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
	 
	 TIM_Cmd(_TIMx, ENABLE); //????

}