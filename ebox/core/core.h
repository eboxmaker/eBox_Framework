#ifndef __CORE_H
#define __CORE_H
#ifdef __cplusplus
extern "C"{
#endif
#include "stm32f10x.h"

//�Ժ�NVIC_PriorityGroupConfig()��������Ҫ�ٱ����á����������Է�NVIC_GROUP_CONFIGֵ������
#define NVIC_GROUP_CONFIG NVIC_PriorityGroup_2   /*!< 2 bits for pre-emption priority
																									 2 bits for subpriority */

#define interrupts() 		__enable_irq()
#define no_interrupts() 	__disable_irq()
	
///////ȫ�ֱ���������///////////////////////////////////////////////
extern __IO uint32_t millis_seconds;
extern __IO uint32_t cpu_calculate_per_sec;

typedef void (*callback_fun_type)(void);	 

extern void ebox_printf(const char *fmt,...);

void ebox_init(void);	
uint32_t millis( void ) ;
void delay_ms(uint32_t ms);
void delay_us(uint16_t us);
void delayus(uint32_t us);
void attch_sys_ticks_interrupt(void (*callback_fun)(void));
    
#ifdef __cplusplus
}
#endif
#endif
