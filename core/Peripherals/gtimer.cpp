#include "gtimer.h"


//默认配置为预分频72分频
//默认配置为溢出值 1000
//默认配置：1ms中断一次
#define PRESCALE 72
#define PEROID	1000


TIM::TIM(TIM_TypeDef* TIMx)
{
	_TIMx = TIMx;
	
	_id = TIMxToID(_TIMx);
	_rcc = TIMxToRCC(TIMx);
	_irq = TIMxToIRQ(TIMx);
	_period = PEROID;
	_prescaler = PRESCALE;
}
TIM::TIM(TIM_TypeDef* TIMx,uint32_t period,uint32_t prescaler)
{

	_TIMx = TIMx;
	
	_id = TIMxToID(_TIMx);
	_rcc = TIMxToRCC(TIMx);
	_irq = TIMxToIRQ(TIMx);
	_period = period;
	_prescaler = prescaler;
}
void TIM::begin(void)
{
	TIMxBaseInit(_period,_prescaler);
	Interrupt(DISABLE);
	stop();
}
void TIM::Interrupt(FunctionalState x)
{
 TIM_ClearITPendingBit(_TIMx , TIM_FLAG_Update);
 TIM_ITConfig(_TIMx,TIM_IT_Update,x);
 NVIC_InitTypeDef NVIC_InitStructure;
 NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);  //使用全局控制值
 NVIC_InitStructure.NVIC_IRQChannel = _irq;//
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// 
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;// 
 NVIC_InitStructure.NVIC_IRQChannelCmd = x;
 NVIC_Init(&NVIC_InitStructure);

}

void TIM::start(void)
{
	 TIM_Cmd(_TIMx, ENABLE); //????
}

void TIM::stop(void)
{
	 TIM_Cmd(_TIMx, DISABLE); //????
}
void TIM::TIMxBaseInit(uint16_t period,uint16_t prescaler)
{

	_period = period;
	_prescaler = prescaler;
	
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
 TIM_DeInit(_TIMx);
 RCC_APB1PeriphClockCmd(_rcc, ENABLE);
 TIM_TimeBaseStructure.TIM_Period=period-1;//ARR??
 TIM_TimeBaseStructure.TIM_Prescaler=prescaler-1;
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
 TIM_TimeBaseInit(_TIMx, &TIM_TimeBaseStructure);
 TIM_ARRPreloadConfig(_TIMx, ENABLE);
 TIM_Cmd(_TIMx, ENABLE); //????

}
void TIM::SetReload(uint16_t Autoreload)
{
	TIM_SetAutoreload(_TIMx,Autoreload);
}
void TIM::clearCount(void)
{
	_TIMx->CNT = 0;
}




