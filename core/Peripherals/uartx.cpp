#include "uartx.h"
class USART uart1(USART1);
class USART uart2(USART2);
class USART uart3(USART3);

#define SEND_MODE_P 	0
#define SEND_MODE_LN 	1	


callbackFun UARTCallbackTable[USART_NUM];//支持串口的rx和tx中断


USART::USART(USART_TypeDef * USARTx)
{
	_USARTx = USARTx;
	_rcc = USARTxToRCC(_USARTx);
	_irq = USARTxToIRQ(_USARTx);
	_id	= USARTxToID(_USARTx);
	sendOver = 1;
}


void USART::begin(uint32_t BaudRate)
{
    USART_InitTypeDef USART_InitStructure;
		  NVIC_InitTypeDef NVIC_InitStructure;

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
	
		
  
		NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);
		
		NVIC_InitStructure.NVIC_IRQChannel = _irq;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
			
    
    USART_Cmd(_USARTx, ENABLE);
		
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
void USART::interrupt(FunctionalState x)
{
	USART_ITConfig(_USARTx,USART_IT_RXNE,x);
}
void USART::attachInterrupt(void (*callbackFun)(void))
{
		UARTCallbackTable[_id - 1] = callbackFun;//注册rx中断函数
}

void USART::printfln(const char *str,uint16_t length)
{
	uint16_t i;
	length = length % UART_MAX_SEND_BUF;
	sendMode = SEND_MODE_LN;
	sendLength = length;
	while(sendOver == 0);
	do
	{
		sendBuf[i++] = *str++;
	}while(length--);
	for(;i < UART_MAX_SEND_BUF; i++)
	{
		sendBuf[i] = '\0';
	}
	sendOver = 0;
	USART_ITConfig(_USARTx, USART_IT_TXE, ENABLE); 

}
void USART::printf(const char* fmt,...)
{
	
	while(sendOver == 0);
	va_list va_params;   
	va_start(va_params,fmt);   
	vsprintf(sendBuf,fmt,va_params);   
	va_end(va_params);   
	
	
	sendMode = SEND_MODE_P;
	sendOver = 0;
	
	USART_ITConfig(_USARTx, USART_IT_TXE, ENABLE); 
}

extern "C"{
	
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		UARTCallbackTable[0]();
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		return;
	}
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {
		if(uart1.sendMode == SEND_MODE_P)
		{
			if(uart1.sendBuf[uart1.count] == '\0')
			{
				USART_ClearITPendingBit(USART1	,USART_IT_TXE);
				USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
				USART_ITConfig(USART1, USART_IT_TC, ENABLE);
				uart1.count = 0;
				uart1.sendOver = 1;
			}
			else
				USART_SendData(USART1, uart1.sendBuf[uart3.count++]);
		}
		else if(uart1.sendMode == SEND_MODE_LN)
		{
			if(uart1.count == uart1.sendLength)
			{
				USART_ClearITPendingBit(USART1	,USART_IT_TXE);
				USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
				USART_ITConfig(USART1, USART_IT_TC, ENABLE);
				uart1.count = 0;
				uart1.sendOver = 1;
			}
			else
				USART_SendData(USART1, uart1.sendBuf[uart1.count++]);
		}
  }
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
	 USART_ClearITPendingBit(USART1,USART_IT_TC);
	 USART_ITConfig(USART1, USART_IT_TC, DISABLE);
	}
}
void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		UARTCallbackTable[1]();
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		return;
	}
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
  {
		if(uart2.sendMode == SEND_MODE_P)
		{
			if(uart2.sendBuf[uart2.count] == '\0')
			{
				USART_ClearITPendingBit(USART2	,USART_IT_TXE);
				USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
				USART_ITConfig(USART2, USART_IT_TC, ENABLE);
				uart2.count = 0;
				uart2.sendOver = 1;
			}
			else
				USART_SendData(USART2, uart2.sendBuf[uart2.count++]);
		}
		else if(uart2.sendMode == SEND_MODE_LN)
		{
			if(uart2.count == uart2.sendLength)
			{
				USART_ClearITPendingBit(USART2	,USART_IT_TXE);
				USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
				USART_ITConfig(USART2, USART_IT_TC, ENABLE);
				uart2.count = 0;
				uart2.sendOver = 1;
			}
			else
				USART_SendData(USART2, uart2.sendBuf[uart2.count++]);
		}
  }
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
	 USART_ClearITPendingBit(USART2,USART_IT_TC);
	 USART_ITConfig(USART2, USART_IT_TC, DISABLE);
	}
}
void USART3_IRQHandler(void)
{

	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		UARTCallbackTable[2]();
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		return;
	}
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
  {
		if(uart3.sendMode == SEND_MODE_P)
		{
			if(uart3.sendBuf[uart3.count] == '\0')
			{
				USART_ClearITPendingBit(USART3	,USART_IT_TXE);
				USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
				USART_ITConfig(USART3, USART_IT_TC, ENABLE);
				uart3.count = 0;
				uart3.sendOver = 1;
			}
			else
				USART_SendData(USART3, uart3.sendBuf[uart3.count++]);
		}
		else if(uart3.sendMode == SEND_MODE_LN)
		{
			if(uart3.count == uart3.sendLength)
			{
				USART_ClearITPendingBit(USART3	,USART_IT_TXE);
				USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
				USART_ITConfig(USART3, USART_IT_TC, ENABLE);
				uart3.count = 0;
				uart3.sendOver = 1;
			}
			else
				USART_SendData(USART3, uart3.sendBuf[uart3.count++]);
		}
  }
	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
	 USART_ClearITPendingBit(USART3,USART_IT_TC);
	 USART_ITConfig(USART3, USART_IT_TC, DISABLE);
	}
}

	
}


