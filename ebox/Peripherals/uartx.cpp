#include "uartx.h"

#define USART_NUM 3

class USART uart1(USART1);
class USART uart2(USART2);
class USART uart3(USART3);

#define SEND_MODE_P 	0
#define SEND_MODE_LN 	1	

#define BUSY 	1
#define FREE 	0

//uint8_t uart1State,uart2State,uart3State;

callbackFun UARTCallbackTable[USART_NUM];//支持串口的rx中断

typedef struct 
{
	uint8_t id ;
	USART_TypeDef *usart;
	uint32_t rcc;
	uint32_t irq;
} USARTx_INFO ;

////////////////////////////////////////////////////////////
const USARTx_INFO USARTxInfo[]=
{
	{1,USART1,RCC_APB2Periph_USART1,USART1_IRQn},//暂时不支持
	{2,USART2,RCC_APB1Periph_USART2,USART2_IRQn},
	{3,USART3,RCC_APB1Periph_USART3,USART3_IRQn},
};
//////////////////////////
uint32_t  USARTxToRCC(USART_TypeDef* USARTx);
uint32_t 	USARTxToIRQ(USART_TypeDef* USARTx);
uint8_t 	USARTxToID(USART_TypeDef* USARTx);

USART::USART(USART_TypeDef * USARTx)
{
	_USARTx = USARTx;
	_rcc = USARTxToRCC(_USARTx);
	_irq = USARTxToIRQ(_USARTx);
	_id	= USARTxToID(_USARTx);
	state = FREE;
}


void USART::begin(uint32_t BaudRate)
{
   USART_InitTypeDef USART_InitStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;
			/* ADC1  DMA1 Channel Config */
	 DMA_InitTypeDef DMA_InitStructure;

	
	 NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

		if(_id == 1)
		{
			RCC_APB2PeriphClockCmd(_rcc,ENABLE); //??????
			pinMode(0x09,AF_PP);
			pinMode(0x0a,INPUT);
			
			_DMA1_Channelx = DMA1_Channel4;
		}
		else if(_id == 2)
		{
			RCC_APB1PeriphClockCmd(_rcc,ENABLE); //?????
			pinMode(0x02,AF_PP);
			pinMode(0x03,INPUT);
			
			_DMA1_Channelx = DMA1_Channel7;
		}
		else if(_id == 3)
		{

			RCC_APB1PeriphClockCmd(_rcc,ENABLE); //?????
			pinMode(0x1a,AF_PP);
			pinMode(0x1B,INPUT);
			
			_DMA1_Channelx = DMA1_Channel2;
			
		}
		#if defined USE_DMA
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
		
//			DMA_DeInit(_DMA1_Channelx);   //将DMA的通道1寄存器重设为缺省值
			DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&_USARTx->DR;  //DMA外设ADC基地址
			DMA_InitStructure.DMA_MemoryBaseAddr = (u32)sendBuf;  //DMA内存基地址
			DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //
			DMA_InitStructure.DMA_BufferSize = 128;  //DMA通道的DMA缓存的大小
			DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //
			DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //
			DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //
			DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; //
			DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在循环缓存模式
			DMA_InitStructure.DMA_Priority = DMA_Priority_High; //
			DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
			DMA_Init(_DMA1_Channelx, &DMA_InitStructure);  //
			if(_id == 1)
			{
					
				NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;  
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
				NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
				NVIC_Init(&NVIC_InitStructure);  	

			 DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  
			}
			else if(_id == 2)
			{
							
				NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;  
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
				NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
				NVIC_Init(&NVIC_InitStructure);  	
			

			 DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);  
			}
			else if(_id == 3)
			{
				NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;  
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
				NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
				NVIC_Init(&NVIC_InitStructure);  	

				 DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);  
			}
		#endif

    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(_USARTx, &USART_InitStructure);
	
		
		NVIC_InitStructure.NVIC_IRQChannel = _irq;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		
		USART_DMACmd(_USARTx,USART_DMAReq_Tx,ENABLE);     
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

int USART::putChar(char ch)
{
	USART_SendData(_USARTx,ch);
	while(USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	return ch;
}
void USART::putString(const char *str)
{
	while(*str!='\0')
	{
		USART_SendData(_USARTx,*str++);
		while(USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	}
}	
void USART::putString(const char *str,uint16_t length)
{
	while(length--)
	{
		USART_SendData(_USARTx,*str++);
		while(USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	}
}	

void USART::printfln(const char *str,uint16_t length)
{	
	#if defined USE_DMA
		uint16_t i = 0;
		uint16_t tmpln;
		tmpln = length;
		while(state);
		while(tmpln--)
		{
			sendBuf[i++] = *str++;
		};
		state = BUSY;
		DMA_SetCurrDataCounter(_DMA1_Channelx,length);  
		DMA_Cmd(_DMA1_Channelx,ENABLE); 
	#else
		putString(str,length);
	#endif
}
void USART::printf(const char* fmt,...)
{
	 __IO uint16_t length = 0;
	while(state);
	va_list va_params;   
	va_start(va_params,fmt);   
	length = vsprintf(sendBuf,fmt,va_params);   
	va_end(va_params); 
  
	#if defined USE_DMA
		state = BUSY;
		DMA_SetCurrDataCounter(_DMA1_Channelx,length);  
		DMA_Cmd(_DMA1_Channelx,ENABLE);  
	#else
		putString(sendBuf);
	#endif
	
}

////////////////////////////////////////////////////
uint32_t  USARTxToRCC(USART_TypeDef* USARTx)
{
	int i;
	uint32_t rcc;
	for(i=0;i<USART_NUM;i++)
	{
		if(USARTxInfo[i].usart == USARTx)
		{
					rcc = USARTxInfo[i].rcc;
			break;

		}
	
	}
	return rcc;
}
uint32_t USARTxToIRQ(USART_TypeDef* USARTx)
{
	uint32_t irq;
	int i;
	for(i=0;i<USART_NUM;i++)
	{
		if(USARTxInfo[i].usart == USARTx)
		{
			irq = USARTxInfo[i].irq;
			break;
		}
	
	}	

	return irq;
}
uint8_t USARTxToID(USART_TypeDef* USARTx)
{
	uint32_t id;
	int i;
	for(i=0;i<USART_NUM;i++)
	{
		if(USARTxInfo[i].usart == USARTx)
		{
			id = USARTxInfo[i].id;
			break;
		}
	
	}	

	return id;
}



extern "C"{
	
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		UARTCallbackTable[0]();
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		UARTCallbackTable[1]();
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}
void USART3_IRQHandler(void)
{

	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		UARTCallbackTable[2]();
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
}
void DMA1_Channel2_IRQHandler(void)  
{  

  DMA_Cmd(DMA1_Channel2,DISABLE);  
  uart3.state = FREE;   
	DMA_ClearFlag(DMA1_FLAG_TC2); 

}  
void DMA1_Channel4_IRQHandler(void)  
{  

  DMA_Cmd(DMA1_Channel4,DISABLE);  
  uart1.state = FREE;  
	DMA_ClearFlag(DMA1_FLAG_TC4); 

}	 
void DMA1_Channel7_IRQHandler(void)  
{  
  DMA_Cmd(DMA1_Channel7,DISABLE);  
  uart2.state = FREE;  
	DMA_ClearFlag(DMA1_FLAG_TC7); 
}	 

}


