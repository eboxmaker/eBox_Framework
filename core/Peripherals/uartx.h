#ifndef __UART3_H
#define __UART3_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
	 

#include "ebox.h"
#include <stdio.h>
#include <stdarg.h>  
	 
 class USART
 {
	 public:
		 USART(USART_TypeDef * USARTx)
		 {
			 _USARTx = USARTx;
			 _rcc = USARTxToRCC(_USARTx);
			 _irq = USARTxToIRQ(_USARTx);
			 _id	= USARTxToID(_USARTx);
		 }

	  void begin(uint32_t BaudRate);
		void interrupt(FunctionalState enable);
		void putchar(char ch);
		void putstr(const char *str);
		void printf(const char* fmt,...);			 
		void attachInterrupt(void (*callbackFun)(void),uint8_t IKind)
		{
			if(IKind == 1)
				UARTcbTable[(_id - 1) * 2] = callbackFun;//×¢²árxÖÐ¶Ïº¯Êý
			if(IKind == 2)
				UARTcbTable[(_id - 1) * 2 + 1] = callbackFun;//×¢²átxÖÐ¶Ïº¯Êý
		}
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
#ifdef __cplusplus
}
#endif
#endif
