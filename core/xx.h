#ifndef __XX_H
#define __XX_H

#include "ebox.h"
#include "stm32f10x.h"                  // Device header
#include "eboxconfig.h"
#include "ebox_pin.h" 

#define INPUT 0x0
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x3
	
#define OUTPUT 0x1
	
#define AF_PP 0x4
#define AF_OD 0x5
#define AIN 0x6


#include "exti.h"

#endif
