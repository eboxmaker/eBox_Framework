/*
file   : uartx.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

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
#define USE_DMA 1
#define UART_MAX_SEND_BUF 128 

class USART
{
	 public:
			USART(USART_TypeDef * USARTx,GPIO* tx_pin,GPIO* rx_pin);
	 
			void begin(uint32_t baud_rate);
			void attach_interrupt(void (*callback_fun)(void));
			void interrupt(FunctionalState enable);
			uint16_t receive();
	 
			int 	put_char(char ch);
			void 	put_string(const char *str);
			void 	put_string(const char *str,uint16_t length);

			void printf(const char* fmt,...);		
			void printf_length(const char *str,uint16_t length);
			void wait_busy();
	 
	 private:
		USART_TypeDef* 				_USARTx;
	  DMA_Channel_TypeDef*	_DMA1_Channelx;
		char 									send_buf[UART_MAX_SEND_BUF];
};






#endif
