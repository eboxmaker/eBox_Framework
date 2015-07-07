#include "gtimer.h"



callbackFun gTimxCallbackTable[TIM_NUM +1];

typedef struct 
{
	uint8_t id ;
	TIM_TypeDef *timx;
	uint32_t rcc;
	uint32_t irq;
} TIMx_INFO ;

////////////外设及其附属属性对应表///////////////////////////
const TIMx_INFO TIMxInfo[]=
{
	{1,TIM1,RCC_APB2Periph_TIM1,TIM1_UP_IRQn},//暂时不支持
	{2,TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn},
	{3,TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn},
	{4,TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn},
	#if defined (MCUPIN64) || (defined MCUPIN100) || (defined MCUPIN144)
		{5,TIM5,RCC_APB1Periph_TIM5,NULL},
		{6,TIM6,RCC_APB1Periph_TIM6,NULL},
		{7,TIM7,RCC_APB1Periph_TIM7,NULL}
	#endif
};

//////////////////////////////////////
uint32_t TIMxToRCC(TIM_TypeDef* TIMx);
uint32_t TIMxToIRQ(TIM_TypeDef* TIMx);
uint8_t TIMxToID(TIM_TypeDef* TIMx);


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


///////////////////////////////////////////////////////////////////////////////////////
uint32_t  TIMxToRCC(TIM_TypeDef* TIMx)
{
	int i;
	uint32_t rcc;
	for(i=0;i<TIM_NUM;i++)
	{
		if(TIMxInfo[i].timx == TIMx)
		{
					rcc = TIMxInfo[i].rcc;
			break;

		}
	
	}
	return rcc;
}
uint32_t TIMxToIRQ(TIM_TypeDef* TIMx)
{
	uint32_t irq;
	int i;
	for(i=0;i<TIM_NUM;i++)
	{
		if(TIMxInfo[i].timx == TIMx)
		{
			irq = TIMxInfo[i].irq;
			break;
		}
	
	}	

	return irq;
}
uint8_t TIMxToID(TIM_TypeDef* TIMx)
{
	uint32_t id;
	int i;
	for(i=0;i<TIM_NUM;i++)
	{
		if(TIMxInfo[i].timx == TIMx)
		{
			id = TIMxInfo[i].id;
			break;
		}
	
	}	

	return id;
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

