#include "exti.h"
//	pin			: ebox pin
//  trigger	:
// 				 	EXTI_Trigger_Rising 
//  				EXTI_Trigger_Falling  
//  				EXTI_Trigger_Rising_Falling 
EXTIx::EXTIx(uint8_t pin, EXTITrigger_TypeDef trigger)
{
//		uint32_t _ExtiLine;
//		uint8_t _PortSource;
//		uint8_t _PinSource;
//		uint32_t _irqch;
//		uint8_t  _pin;
	_pin = pin;
	_PortSource = PinToExti[_pin].PortSource;
	_PinSource  = PinToExti[_pin].PinSource;
	_ExtiLine		= PinToExti[_pin].ExtiLine;
	_irqch			= PinToExti[_pin].irqch;


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
  NVIC_InitStructure.NVIC_IRQChannel = _irqch;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
