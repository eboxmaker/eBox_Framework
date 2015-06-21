#ifndef __EXTI_H
#define __EXTI_H
#include "ebox.h"
#include "interrupts.h"
class EXTIx
{
	private:
		uint32_t _ExtiLine;
		uint8_t _PortSource;
		uint8_t _PinSource;
		uint32_t _irqch;
		uint8_t  _pin;

	public:
		EXTIx(uint8_t pin,EXTITrigger_TypeDef trigger);
	
		void attachInterrupt(void (*callbackFun)(void))
		{
				ExticbTable[_PinSource] = callbackFun;
		}
	
};

#endif
