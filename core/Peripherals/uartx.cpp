#include "uartx.h"

#define  TxBufLn 128

void USART::begin(uint32_t BaudRate)
{
    USART_InitTypeDef USART_InitStructure;

		if(_id == 1)
		{
			RCC_APB2PeriphClockCmd(_rcc,ENABLE); //??????
			pinMode(0x09,AF_PP);
			pinMode(0x0a,INPUT);
	
		}
		else if(_id == 2)
		{
			RCC_APB1PeriphClockCmd(_rcc,ENABLE); //?????
			pinMode(0x02,AF_PP);
			pinMode(0x03,INPUT);
		
		}
		else if(_id == 3)
		{
			RCC_APB1PeriphClockCmd(_rcc,ENABLE); //?????
			pinMode(0x1a,AF_PP);
			pinMode(0x1B,INPUT);
		
		}
		
  
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(_USARTx, &USART_InitStructure);
		
 	  interrupt(DISABLE);

     
    USART_Cmd(_USARTx, ENABLE);
		USART_ClearFlag(_USARTx, USART_FLAG_TC); 

		if(_id == 1)
		{
			printf(" \r\nusart1 ok!\r\n");
		}
		else if(_id == 2)
		{
			printf(" \r\nusart2 ok!\r\n");
		
		}
		else if(_id == 3)
		{
			printf(" \r\nusart3 ok!\r\n");
		
		}

}
void USART::interrupt(FunctionalState enable)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);
  
  /* ??P[A|B|C|D|E]0???? */
  NVIC_InitStructure.NVIC_IRQChannel = _irq;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = enable;
  NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(_USARTx,USART_IT_RXNE,enable);

}

void USART::putchar(char ch)
{
	USART_SendData(_USARTx,ch);
	while(USART_GetFlagStatus(_USARTx, USART_FLAG_TC) != SET);
}

void USART::putstr(const char *str)
{
	while(*str!='\0')
	{
		USART_SendData(_USARTx,*str++);
		while(USART_GetFlagStatus(_USARTx, USART_FLAG_TC) != SET);
	}
}	


void USART::printf(const char* fmt,...)
{

	char buffer[TxBufLn];   
	va_list va_params;   
	va_start(va_params,fmt);   
	vsprintf(buffer,fmt,va_params);   
	va_end(va_params);   
	putstr(buffer);
}
class USART uart1(USART1);
class USART uart2(USART2);
class USART uart3(USART3);
