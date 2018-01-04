#ifndef __NVIC_H
#define __NVIC_H


#include "ebox_core.h"

void nvic_priority_group_config(uint32_t NVIC_PriorityGroup);
void nvic_irq_enable(IRQn_Type irq_num) ;
void nvic_irq_disable(IRQn_Type irq_num) ;
void nvic_irq_set_priority(IRQn_Type irqn, uint8_t PreemptionPriority,uint8_t SubPriority) ;



#endif
