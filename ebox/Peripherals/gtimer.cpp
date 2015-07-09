#include "gtimer.h"

#define TIM_NUM 7

callbackFun gTimxCallbackTable[TIM_NUM +1];



//////////////////////////////////////


TIM::TIM(TIM_TypeDef* TIMx)
{
	
	initInfo(TIMx);
	_period = PEROID;
	_prescaler = PRESCALE;
}
TIM::TIM(TIM_TypeDef* TIMx,uint32_t period,uint32_t prescaler)
{

	initInfo(TIMx);
	_period = period;
	_prescaler = prescaler;
}
void TIM::begin(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	baseInit(_period,_prescaler);
	
	NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);  //使用全局控制值
	NVIC_InitStructure.NVIC_IRQChannel = _irq;//
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;// 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	interrupt(DISABLE);
	stop();
	//start();
}
void TIM::interrupt(FunctionalState x)
{
 TIM_ClearITPendingBit(_TIMx , TIM_FLAG_Update);
 TIM_ITConfig(_TIMx,TIM_IT_Update,x);

}

void TIM::start(void)
{
	 TIM_Cmd(_TIMx, ENABLE); //????
}

void TIM::stop(void)
{
	 TIM_Cmd(_TIMx, DISABLE); //????
}
void TIM::baseInit(uint16_t period,uint16_t prescaler)
{

	_period = period;
	_prescaler = prescaler;
	
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
 TIM_DeInit(_TIMx);
 RCC_APB1PeriphClockCmd(_rcc, ENABLE);
	
 TIM_TimeBaseStructure.TIM_Period=period-1;//ARR寄存器
 TIM_TimeBaseStructure.TIM_Prescaler=prescaler-1;
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //单边斜坡
 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

 TIM_TimeBaseInit(_TIMx, &TIM_TimeBaseStructure);
 TIM_ARRPreloadConfig(_TIMx, ENABLE);	//控制ARR寄存器是否使用影子寄存器


}
void TIM::initInfo(TIM_TypeDef* TIMx)
{
	switch((uint32_t)TIMx)
	{
		case (uint32_t)TIM1:
			_TIMx = TIM1;_id = 1;_rcc = RCC_APB2Periph_TIM1;_irq = TIM1_UP_IRQn;//暂时不支持
			break;
		case (uint32_t)TIM2:
			_TIMx = TIM2;_id = 2;_rcc = RCC_APB1Periph_TIM2;_irq = TIM2_IRQn;
			break;
		case (uint32_t)TIM3:
			_TIMx = TIM3;_id = 3;_rcc = RCC_APB1Periph_TIM3;_irq = TIM3_IRQn;
			break;
		case (uint32_t)TIM4:
			_TIMx = TIM4;_id = 4;_rcc = RCC_APB1Periph_TIM4;_irq = TIM4_IRQn;
			break;
	}

}

void TIM::setReload(uint16_t Autoreload)
{
	TIM_SetAutoreload(_TIMx,Autoreload);
}
void TIM::clearCount(void)
{
	_TIMx->CNT = 0;
}
void TIM::attachInterrupt(void(*callback)(void))
{
		gTimxCallbackTable[_id] = callback;
}


extern "C"{
	void TIM2_IRQHandler(void)
	{

	 if(TIM_GetITStatus(TIM2 , TIM_IT_Update) == SET)
	 {
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
			gTimxCallbackTable[2]();
	 }
	}
	void TIM3_IRQHandler(void)
	{
	 if(TIM_GetITStatus(TIM3 , TIM_IT_Update) == SET)
	 {
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
			gTimxCallbackTable[3]();
	 }
	}
	void TIM4_IRQHandler(void)
	{
	 if(TIM_GetITStatus(TIM4 , TIM_IT_Update) == SET)
	 {
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
			gTimxCallbackTable[4]();
	 }
	}
}

