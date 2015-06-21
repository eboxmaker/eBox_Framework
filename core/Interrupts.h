#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include  "ebox.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#define NVIC_GROUP_CONFIG NVIC_PriorityGroup_2   /*!< 2 bits for pre-emption priority
                                                 2 bits for subpriority */
	
	
typedef void (*callbackFun)(void);
	
extern callbackFun gTimxcbTable[];
extern callbackFun ExticbTable[];
extern callbackFun UARTcbTable[];


	
	
	
	
	
	
#ifdef __cplusplus
}
#endif
	
	

#endif

