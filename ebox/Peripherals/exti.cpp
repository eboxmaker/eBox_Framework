#include "exti.h"

#define EXTI_LINE_NUM 16

callbackFun extiCallbackTable[EXTI_LINE_NUM];


//  trigger	:
// 				 	EXTI_Trigger_Rising 
//  				EXTI_Trigger_Falling  
//  				EXTI_Trigger_Rising_Falling 

EXTIx::EXTIx(GPIO* extiPin, EXTITrigger_TypeDef trigger)
{

	
//	_pin = pin;
//	_PortSource = pinToExti[_pin].portSource;
//	_PinSource  = pinToExti[_pin].pinSource;
//	_ExtiLine		= pinToExti[_pin].extiLine;
//	_irq				= pinToExti[_pin].irqch;

	extiPin->mode(INPUT);
	initInfo(extiPin);

	
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
void EXTIx::initInfo(GPIO* extiPin)
{
	switch((uint32_t)extiPin->port)
	{
		case (uint32_t)GPIOA: _PortSource = GPIO_PortSourceGPIOA;break;
		case (uint32_t)GPIOB: _PortSource = GPIO_PortSourceGPIOB;break;
		case (uint32_t)GPIOC: _PortSource = GPIO_PortSourceGPIOC;break;
		case (uint32_t)GPIOD: _PortSource = GPIO_PortSourceGPIOD;break;
		case (uint32_t)GPIOE: _PortSource = GPIO_PortSourceGPIOE;break;
		case (uint32_t)GPIOF: _PortSource = GPIO_PortSourceGPIOF;break;
	}
	switch(extiPin->pin)
	{
		case GPIO_Pin_0:
			_PinSource = GPIO_PinSource0;_ExtiLine = EXTI_Line0;_irq = EXTI0_IRQn;
			break;
		
		case GPIO_Pin_1:
			_PinSource = GPIO_PinSource1;_ExtiLine = EXTI_Line1;_irq = EXTI1_IRQn;
			break;
		
		case GPIO_Pin_2:
			_PinSource = GPIO_PinSource2;_ExtiLine = EXTI_Line2;_irq = EXTI2_IRQn;
			break;
		
		case GPIO_Pin_3:
			_PinSource = GPIO_PinSource3;_ExtiLine = EXTI_Line3;_irq = EXTI3_IRQn;
			break;
		
		case GPIO_Pin_4:
			_PinSource = GPIO_PinSource4;_ExtiLine = EXTI_Line4;_irq = EXTI4_IRQn;
			break;
		
		case GPIO_Pin_5:
			_PinSource = GPIO_PinSource5;_ExtiLine = EXTI_Line5;_irq = EXTI9_5_IRQn;
			break;
		
		case GPIO_Pin_6:
			_PinSource = GPIO_PinSource6;_ExtiLine = EXTI_Line6;_irq = EXTI9_5_IRQn;
			break;
		
		case GPIO_Pin_7:
			_PinSource = GPIO_PinSource7;_ExtiLine = EXTI_Line7;_irq = EXTI9_5_IRQn;
			break;
		
		case GPIO_Pin_8:
			_PinSource = GPIO_PinSource8;_ExtiLine = EXTI_Line8;_irq = EXTI9_5_IRQn;
			break;
		
		case GPIO_Pin_9:
			_PinSource = GPIO_PinSource9;_ExtiLine = EXTI_Line9;_irq = EXTI9_5_IRQn;
			break;
		
		case GPIO_Pin_10:
			_PinSource = GPIO_PinSource10;_ExtiLine = EXTI_Line10;_irq = EXTI15_10_IRQn;
			break;
		
		case GPIO_Pin_11:
			_PinSource = GPIO_PinSource11;_ExtiLine = EXTI_Line11;_irq = EXTI15_10_IRQn;
			break;
		
		case GPIO_Pin_12:
			_PinSource = GPIO_PinSource12;_ExtiLine = EXTI_Line12;_irq = EXTI15_10_IRQn;
			break;
		
		case GPIO_Pin_13:
			_PinSource = GPIO_PinSource13;_ExtiLine = EXTI_Line13;_irq = EXTI15_10_IRQn;
			break;
		
		case GPIO_Pin_14:
			_PinSource = GPIO_PinSource14;_ExtiLine = EXTI_Line14;_irq = EXTI15_10_IRQn;
			break;
		
		case GPIO_Pin_15:
			_PinSource = GPIO_PinSource15;_ExtiLine = EXTI_Line15;_irq = EXTI15_10_IRQn;
			break;
		
	}

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
