#include "exti.h"

callbackFun extiCallbackTable[16];

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