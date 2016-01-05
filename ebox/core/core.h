#ifndef __CORE_H
#define __CORE_H
#ifdef __cplusplus
extern "C"{
#endif
#include "stm32f10x.h"

//以后NVIC_PriorityGroupConfig()函数不需要再被调用。更不能再以非NVIC_GROUP_CONFIG值填充调用
#define NVIC_GROUP_CONFIG NVIC_PriorityGroup_2   /*!< 2 bits for pre-emption priority
																									 2 bits for subpriority */

#define interrupts() 		__enable_irq()
#define no_interrupts() 	__disable_irq()
	
///////全局变量、函数///////////////////////////////////////////////
extern __IO uint64_t millis_seconds;
extern __IO uint32_t cpu_calculate_per_sec;

typedef void (*callback_fun_type)(void);	 

extern void ebox_printf(const char *fmt,...);

void ebox_init(void);	
uint64_t millis(void);
uint64_t micros(void);
void delay_ms(uint64_t ms);
void delay_us(uint64_t us);
void delayms(uint64_t ms);
void delayus(uint64_t us);
void attch_sys_ticks_interrupt(void (*callback_fun)(void));
    
#ifdef __cplusplus
}
#endif
#endif
