/*
file   : uartx.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#ifndef __UARTX_H
#define __UARTX_H

#include "common.h"
#include <stdio.h>
#include <stdarg.h>  


/*
	1.֧�ִ���1,2,3.
	2.֧��һ���ж��¼� rx_it
	3.����ģʽ����DMA�Զ�����ģʽ������ʡcpuռ�á����ͻ������ΪUART_MAX_SEND_BUF
	4.֧��ǿ���printf
	5.��ʱ��֧�����ŵ�remap
*/
//�û�����//////////////
//#define USE_UART1
//#define USE_UART2
#define USE_UART3
#define USE_DMA
#define UART_MAX_SEND_BUF 128 

class USART
{
	 public:
			char 			sendBuf[UART_MAX_SEND_BUF];
			uint8_t 	state;	 
	 public:
			USART(USART_TypeDef * USARTx,GPIO* txPin,GPIO* rxPin);
			void begin(uint32_t BaudRate);
			void attachInterrupt(void (*callbackFun)(void));
			void interrupt(FunctionalState enable);
	 

			void printf(const char* fmt,...);		
			void printfln(const char *str,uint16_t length);


	 
	 private:
		USART_TypeDef* 			_USARTx;
	  DMA_Channel_TypeDef* _DMA1_Channelx;
	 	uint8_t _id;
		uint32_t _rcc;
		uint32_t _irq;
		 GPIO* _txPin;
		 GPIO* _rxPin;
	  uint8_t _initialized;
		int 	putChar(char ch);
		void 	putString(const char *str);
		void 	putString(const char *str,uint16_t length);

 
};






#endif
