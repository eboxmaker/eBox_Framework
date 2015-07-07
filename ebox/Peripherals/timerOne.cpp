#include "timerone.h"


callbackFun timOneCallbackTable[1];

TIMERONE::TIMERONE()
{

	_period = PEROID;
	_prescaler = PRESCALE;
}
void TIMERONE::begin(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	baseInit(_period,_prescaler);
	
	NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);  //ʹ��ȫ�ֿ���ֵ
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;//
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;// 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	interrupt(DISABLE);
	stop();
}
void TIMERONE::interrupt(FunctionalState x)
{
 TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
 TIM_ITConfig(TIM1,TIM_IT_Update,x);

}

void TIMERONE::start(void)
{
	 TIM_Cmd(TIM1, ENABLE); //????
}

void TIMERONE::stop(void)
{
	 TIM_Cmd(TIM1, DISABLE); //????
}
void TIMERONE::baseInit(uint16_t period,uint16_t prescaler)
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
	////�˴���ͨ�ö�ʱ����һ�� ���ƶ�ʱ��������ٴβ���һ���ж�
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0 ;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	//����ARR�Ĵ����Ƿ�ʹ��Ӱ�ӼĴ���
// TIM_ARRPreloadConfig(TIM1, ENABLE);
	
}
void TIMERONE::setReload(uint16_t Autoreload)
{
	TIM_SetAutoreload(TIM1,Autoreload);
}
void TIMERONE::clearCount(void)
{
	TIM1->CNT = 0;
}
void TIMERONE::attachInterrupt(void(*callback)(void))
{
	 timOneCallbackTable[0] = callback;
}
extern "C" {
	void TIM1_UP_IRQHandler(void)
	{

	 if(TIM_GetITStatus(TIM1 , TIM_IT_Update) == SET)
	 {
		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
			timOneCallbackTable[0]();
	 }
	}
}
