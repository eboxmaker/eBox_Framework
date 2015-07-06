/*
file   : rtc.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#ifndef __UARTX_H
#define __UARTX_H

#include "ebox.h"
#include <stdio.h>
#include <stdarg.h>  

/*
	1.支持串口1,2,3.
	2.支持一个中断事件 rx_it
	3.发送模式采用中断模式，大大节省cpu占用。发送缓存最大为UART_MAX_SEND_BUF
	4.支持强大的printf
	5.暂时不支持引脚的remap
*/

#define UART_MAX_SEND_BUF 128 

class USART
{
	 public:
			char 			sendBuf[UART_MAX_SEND_BUF];
			uint16_t 	count;
			uint16_t 	sendLength;
			uint8_t 	sendMode;
			uint8_t 	sendOver;	 
	 public:
			USART(USART_TypeDef * USARTx);
			void begin(uint32_t BaudRate);
			void interrupt(FunctionalState enable);
			void printf(const char* fmt,...);		
			void printfln(const char *str,uint16_t length);

			void attachInterrupt(void (*callbackFun)(void));
	 
	 private:
		USART_TypeDef * _USARTx;
	 	uint8_t _id;
		uint32_t _rcc;
		uint32_t _irq;
	  uint8_t _initialized;

 
};

extern class USART uart1;
extern class USART uart2;
extern class USART uart3;




#endif
