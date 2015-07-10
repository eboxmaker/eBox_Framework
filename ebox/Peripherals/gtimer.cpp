#include "gtimer.h"

//#define TIM_NUM 7

callbackFun gTimxCallbackTable[3];

//////////////////////////////////////


TIM::TIM(TIM_TypeDef* TIMx)
{
	_TIMx = TIMx;
}

void TIM::begin(uint32_t period,uint32_t prescaler)
{
	config(period,prescaler);
}
void TIM::interrupt(FunctionalState x)
{
 TIM_ClearITPendingBit(_TIMx , TIM_FLAG_Update);//必须加，否则开启中断会立即产生一次中断
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
void TIM::config(uint16_t period,uint16_t prescaler)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_DeInit(_TIMx);
	switch((uint32_t)_TIMx)
	{
		case (uint32_t)TIM2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//
			break;
		case (uint32_t)TIM3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//
			break;
		case (uint32_t)TIM4:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//
			break;
	}
	
	TIM_TimeBaseStructure.TIM_Period=period-1;//ARR寄存器
	TIM_TimeBaseStructure.TIM_Prescaler=prescaler-1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //单边斜坡
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(_TIMx, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(_TIMx, ENABLE);	//控制ARR寄存器是否使用影子寄存器
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;// 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

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
	switch((uint32_t)_TIMx)
	{
		case (uint32_t)TIM2:
			gTimxCallbackTable[0] = callback;
			break;
		case (uint32_t)TIM3:
			gTimxCallbackTable[1] = callback;
			break;
		case (uint32_t)TIM4:
			gTimxCallbackTable[2] = callback;
			break;
	}
}


extern "C"{
	void TIM2_IRQHandler(void)
	{

	 if(TIM_GetITStatus(TIM2 , TIM_IT_Update) == SET)
	 {
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
			gTimxCallbackTable[0]();
	 }
	}
	void TIM3_IRQHandler(void)
	{
	 if(TIM_GetITStatus(TIM3 , TIM_IT_Update) == SET)
	 {
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
			gTimxCallbackTable[1]();
	 }
	}
	void TIM4_IRQHandler(void)
	{
	 if(TIM_GetITStatus(TIM4 , TIM_IT_Update) == SET)
	 {
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
			gTimxCallbackTable[2]();
	 }
	}
}

