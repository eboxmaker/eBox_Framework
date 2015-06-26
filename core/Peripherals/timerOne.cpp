#include "timerone.h"

//默认配置为预分频72分频
//默认配置为溢出值 1000
//默认配置：1ms中断一次
#define PRESCALE 72
#define PEROID	1000

TIMERONE::TIMERONE()
{

	_period = PEROID;
	_prescaler = PRESCALE;
}
void TIMERONE::begin(void)
{
	BaseInit(_period,_prescaler);
	Interrupt(DISABLE);
	stop();
}
void TIMERONE::Interrupt(FunctionalState x)
{
 TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
 TIM_ITConfig(TIM1,TIM_IT_Update,x);
 NVIC_InitTypeDef NVIC_InitStructure;
 NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);  //使用全局控制值
 NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;//
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// 
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;// 
 NVIC_InitStructure.NVIC_IRQChannelCmd = x;
 NVIC_Init(&NVIC_InitStructure);

}

void TIMERONE::start(void)
{
	 TIM_Cmd(TIM1, ENABLE); //????
}

void TIMERONE::stop(void)
{
	 TIM_Cmd(TIM1, DISABLE); //????
}
void TIMERONE::BaseInit(uint16_t period,uint16_t prescaler)
{
	_period = period;
	_prescaler = prescaler;
	
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_DeInit(TIM1);
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler-1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = period-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	////此处和通用定时器不一样 控制定时器溢出多少次产生一次中断
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0 ;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	//控制ARR寄存器是否使用影子寄存器
// TIM_ARRPreloadConfig(TIM1, ENABLE);
	
}
void TIMERONE::SetReload(uint16_t Autoreload)
{
	TIM_SetAutoreload(TIM1,Autoreload);
}
void TIMERONE::clearCount(void)
{
	TIM1->CNT = 0;
}
