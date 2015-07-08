#include "exti.h"

#define EXTI_LINE_NUM 16

callbackFun extiCallbackTable[EXTI_LINE_NUM];

/////////////////////////////////////////////////
typedef struct
{
	uint32_t extiLine;
	uint8_t portSource;
	uint8_t pinSource;
	uint32_t irqch;
	uint8_t  pin;
}PIN_TO_EXTI;

/////////////////////////////////////////////
const PIN_TO_EXTI pinToExti[]=
{
	//{extix,PinSourcex,irqch,APin}
	
	{EXTI_Line0,GPIO_PortSourceGPIOA,GPIO_PinSource0,EXTI0_IRQn,0},
	{EXTI_Line1,GPIO_PortSourceGPIOA,GPIO_PinSource1,EXTI1_IRQn,1},
	{EXTI_Line2,GPIO_PortSourceGPIOA,GPIO_PinSource2,EXTI2_IRQn,2},
	{EXTI_Line3,GPIO_PortSourceGPIOA,GPIO_PinSource3,EXTI3_IRQn,3},
	{EXTI_Line4,GPIO_PortSourceGPIOA,GPIO_PinSource4,EXTI4_IRQn,4},
	{EXTI_Line5,GPIO_PortSourceGPIOA,GPIO_PinSource5,EXTI9_5_IRQn,5},
	{EXTI_Line6,GPIO_PortSourceGPIOA,GPIO_PinSource6,EXTI9_5_IRQn,6},
	{EXTI_Line7,GPIO_PortSourceGPIOA,GPIO_PinSource7,EXTI9_5_IRQn,7},
	{EXTI_Line8,GPIO_PortSourceGPIOA,GPIO_PinSource8,EXTI9_5_IRQn,8},
	{EXTI_Line9,GPIO_PortSourceGPIOA,GPIO_PinSource9,EXTI9_5_IRQn,9},
	{EXTI_Line10,GPIO_PortSourceGPIOA,GPIO_PinSource10,EXTI15_10_IRQn,10},
	{EXTI_Line11,GPIO_PortSourceGPIOA,GPIO_PinSource11,EXTI15_10_IRQn,11},
	{EXTI_Line12,GPIO_PortSourceGPIOA,GPIO_PinSource12,EXTI15_10_IRQn,12},
	{EXTI_Line13,GPIO_PortSourceGPIOA,GPIO_PinSource13,EXTI15_10_IRQn,13},
	{EXTI_Line14,GPIO_PortSourceGPIOA,GPIO_PinSource14,EXTI15_10_IRQn,14},
	{EXTI_Line15,GPIO_PortSourceGPIOA,GPIO_PinSource15,EXTI15_10_IRQn,15},
	
	{EXTI_Line0,GPIO_PortSourceGPIOB,GPIO_PinSource0,EXTI0_IRQn,16},
	{EXTI_Line1,GPIO_PortSourceGPIOB,GPIO_PinSource1,EXTI1_IRQn,17},
	{EXTI_Line2,GPIO_PortSourceGPIOB,GPIO_PinSource2,EXTI2_IRQn,18},
	{EXTI_Line3,GPIO_PortSourceGPIOB,GPIO_PinSource3,EXTI3_IRQn,19},
	{EXTI_Line4,GPIO_PortSourceGPIOB,GPIO_PinSource4,EXTI4_IRQn,20},
	{EXTI_Line5,GPIO_PortSourceGPIOB,GPIO_PinSource5,EXTI9_5_IRQn,21},
	{EXTI_Line6,GPIO_PortSourceGPIOB,GPIO_PinSource6,EXTI9_5_IRQn,22},
	{EXTI_Line7,GPIO_PortSourceGPIOB,GPIO_PinSource7,EXTI9_5_IRQn,23},
	{EXTI_Line8,GPIO_PortSourceGPIOB,GPIO_PinSource8,EXTI9_5_IRQn,24},
	{EXTI_Line9,GPIO_PortSourceGPIOB,GPIO_PinSource9,EXTI9_5_IRQn,25},
	{EXTI_Line10,GPIO_PortSourceGPIOB,GPIO_PinSource10,EXTI15_10_IRQn,26},
	{EXTI_Line11,GPIO_PortSourceGPIOB,GPIO_PinSource11,EXTI15_10_IRQn,27},
	{EXTI_Line12,GPIO_PortSourceGPIOB,GPIO_PinSource12,EXTI15_10_IRQn,28},
	{EXTI_Line13,GPIO_PortSourceGPIOB,GPIO_PinSource13,EXTI15_10_IRQn,29},
	{EXTI_Line14,GPIO_PortSourceGPIOB,GPIO_PinSource14,EXTI15_10_IRQn,30},
	{EXTI_Line15,GPIO_PortSourceGPIOB,GPIO_PinSource15,EXTI15_10_IRQn,31}
	
};
//	pin			: ebox pin
//  trigger	:
// 				 	EXTI_Trigger_Rising 
//  				EXTI_Trigger_Falling  
//  				EXTI_Trigger_Rising_Falling 

EXTIx::EXTIx(uint8_t pin, EXTITrigger_TypeDef trigger)
{

	
	_pin = pin;
	_PortSource = pinToExti[_pin].portSource;
	_PinSource  = pinToExti[_pin].pinSource;
	_ExtiLine		= pinToExti[_pin].extiLine;
	_irq				= pinToExti[_pin].irqch;


	pinMode(_pin,INPUT);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	/* EXTI line(PB0) mode config */
	GPIO_EXTILineConfig(_PortSource, _PinSource); 
  EXTI_InitStructure.EXTI_Line = _ExtiLine;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = trigger; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	
	
	 NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);//使用全局控制值
  
  /* 配置P[A|B|C|D|E]0为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = _irq;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void EXTIx::attachInterrupt(void (*callbackFun)(void))
{
		extiCallbackTable[_PinSource] = callbackFun;
}
extern "C" {
	
	void EXTI0_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line0) != RESET) 
		{
			extiCallbackTable[0]();
			EXTI_ClearITPendingBit(EXTI_Line0);    
		}  	
	}
	void EXTI1_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line1) != RESET)
		{
			extiCallbackTable[1]();

			EXTI_ClearITPendingBit(EXTI_Line1);    
		}  	
	}
	void EXTI2_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line2) != RESET) 
		{

			extiCallbackTable[2]();

			EXTI_ClearITPendingBit(EXTI_Line2);    
		}  	
	}
	void EXTI3_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line3) != RESET) 
		{
			extiCallbackTable[3]();
			EXTI_ClearITPendingBit(EXTI_Line3);     
		}  	
	}
	void EXTI4_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line4) != RESET) 
		{
			extiCallbackTable[4]();
			EXTI_ClearITPendingBit(EXTI_Line4);     
		}  	
	}

	void EXTI9_5_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line5) != RESET) 
		{
			extiCallbackTable[5]();
			EXTI_ClearITPendingBit(EXTI_Line5);     
		}   
		if(EXTI_GetITStatus(EXTI_Line6) != RESET) 
		{
			extiCallbackTable[6]();
			EXTI_ClearITPendingBit(EXTI_Line6);    
		}  
		if(EXTI_GetITStatus(EXTI_Line7) != RESET) 
		{
			extiCallbackTable[7]();
			EXTI_ClearITPendingBit(EXTI_Line7);     
		}   
		if(EXTI_GetITStatus(EXTI_Line8) != RESET) 
		{
			extiCallbackTable[8]();
			EXTI_ClearITPendingBit(EXTI_Line8);    
		}	
		if(EXTI_GetITStatus(EXTI_Line9) != RESET) 
		{
			extiCallbackTable[9]();
			EXTI_ClearITPendingBit(EXTI_Line9);    
		}   

	}

	void EXTI15_10_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line10) != RESET) 
		{
			extiCallbackTable[10]();
			EXTI_ClearITPendingBit(EXTI_Line10);     
		}   
		if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
		{
			extiCallbackTable[11]();
			EXTI_ClearITPendingBit(EXTI_Line11);     
		}  
		if(EXTI_GetITStatus(EXTI_Line12) != RESET) 
		{
			extiCallbackTable[12]();
			EXTI_ClearITPendingBit(EXTI_Line12);     
		}   
		if(EXTI_GetITStatus(EXTI_Line13) != RESET) 
		{
			extiCallbackTable[13]();
			EXTI_ClearITPendingBit(EXTI_Line13);     
		}	
		if(EXTI_GetITStatus(EXTI_Line14) != RESET) 
		{
			extiCallbackTable[14]();
			EXTI_ClearITPendingBit(EXTI_Line14);     
		}   
		if(EXTI_GetITStatus(EXTI_Line15) != RESET) 
		{
			extiCallbackTable[15]();
			EXTI_ClearITPendingBit(EXTI_Line15);    
		}   

	}

}
