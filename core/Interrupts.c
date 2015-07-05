#include "interrupts.h"
#include "mcuconfig.h"
callbackFun timOneCallbackTable[5];
callbackFun gTimxCallbackTable[TIM_NUM +1];
callbackFun extiCallbackTable[EXTI_LINE_NUM + 1];
callbackFun UARTCallbackTable[USART_NUM * 2];//支持串口的rx和tx中断
callbackFun rtcCallbackTable[3];//


void RTC_IRQHandler(void)
{
	if (RTC_GetITStatus(RTC_IT_OW) != RESET)
	{	
		rtcCallbackTable[0]();
		RTC_ClearITPendingBit(RTC_IT_OW);
	}	
	if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
	{	
		rtcCallbackTable[1]();
		RTC_ClearITPendingBit(RTC_IT_ALR);
	}	
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{	

		rtcCallbackTable[2]();
		RTC_ClearITPendingBit(RTC_IT_SEC);
	}
}

void TIM1_UP_IRQHandler(void)
{

 if(TIM_GetITStatus(TIM1 , TIM_IT_Update) == SET)
 {
  TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
		timOneCallbackTable[0]();
 }
}
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
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[0]();
    EXTI_ClearITPendingBit(EXTI_Line0);     //清除中断标志位
  }  	
}
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[1]();

    EXTI_ClearITPendingBit(EXTI_Line1);     //清除中断标志位
  }  	
}
void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET) //确保是否产生了EXTI Line中断
  {

		extiCallbackTable[2]();

    EXTI_ClearITPendingBit(EXTI_Line2);     //清除中断标志位
  }  	
}
void EXTI3_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line3) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[3]();
    EXTI_ClearITPendingBit(EXTI_Line3);     //清除中断标志位
  }  	
}
void EXTI4_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line4) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[4]();
    EXTI_ClearITPendingBit(EXTI_Line4);     //清除中断标志位
  }  	
}

void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[5]();
    EXTI_ClearITPendingBit(EXTI_Line5);     //清除中断标志位
  }   
	if(EXTI_GetITStatus(EXTI_Line6) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[6]();
    EXTI_ClearITPendingBit(EXTI_Line6);     //清除中断标志位
  }  
  if(EXTI_GetITStatus(EXTI_Line7) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[7]();
    EXTI_ClearITPendingBit(EXTI_Line7);     //清除中断标志位
  }   
	if(EXTI_GetITStatus(EXTI_Line8) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[8]();
    EXTI_ClearITPendingBit(EXTI_Line8);     //清除中断标志位
  }	
  if(EXTI_GetITStatus(EXTI_Line9) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[9]();
    EXTI_ClearITPendingBit(EXTI_Line9);     //清除中断标志位
  }   

}

void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line10) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[10]();
    EXTI_ClearITPendingBit(EXTI_Line10);     //清除中断标志位
  }   
	if(EXTI_GetITStatus(EXTI_Line11) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[11]();
    EXTI_ClearITPendingBit(EXTI_Line11);     //清除中断标志位
  }  
  if(EXTI_GetITStatus(EXTI_Line12) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[12]();
    EXTI_ClearITPendingBit(EXTI_Line12);     //清除中断标志位
  }   
	if(EXTI_GetITStatus(EXTI_Line13) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[13]();
    EXTI_ClearITPendingBit(EXTI_Line13);     //清除中断标志位
  }	
  if(EXTI_GetITStatus(EXTI_Line14) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[14]();
    EXTI_ClearITPendingBit(EXTI_Line14);     //清除中断标志位
  }   
  if(EXTI_GetITStatus(EXTI_Line15) != RESET) //确保是否产生了EXTI Line中断
  {
		extiCallbackTable[15]();
    EXTI_ClearITPendingBit(EXTI_Line15);     //清除中断标志位
  }   

}
void USART1_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		UARTCallbackTable[0]();
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		return;
	}
	if(USART_GetITStatus(USART1,USART_IT_TXE)==SET)
	{
		UARTCallbackTable[1]();
		USART_ClearITPendingBit(USART1,USART_IT_TXE);
	}
}
void USART2_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		UARTCallbackTable[2]();
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		return;
	}	
	if(USART_GetITStatus(USART2,USART_IT_TXE)==SET)
	{
		UARTCallbackTable[3]();
		USART_ClearITPendingBit(USART2,USART_IT_TXE);
	}
}
void USART3_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		UARTCallbackTable[4]();
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		return;
	}
	if(USART_GetITStatus(USART3,USART_IT_TXE)==SET)
	{
		UARTCallbackTable[5]();
		USART_ClearITPendingBit(USART3,USART_IT_TXE);
	}
}
