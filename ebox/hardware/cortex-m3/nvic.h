#ifndef __NVIC_H
#define __NVIC_H


#include "ebox_core.h"
#include "mcu.h"

typedef struct
{
    uint32_t dev;
    uint8_t irqn[4];
}DevToIRQn_t;

extern const DevToIRQn_t dev_to_IRQn_table[];


void nvic_priority_group_config(uint32_t NVIC_PriorityGroup);


void nvic_irq_set_priority(uint32_t dev, uint8_t index ,uint8_t PreemptionPriority,uint8_t SubPriority) ;
void nvic_irq_set_priority(IRQn_Type irq_num, uint8_t PreemptionPriority,uint8_t SubPriority) ;

void nvic_irq_enable(uint32_t dev,uint8_t index ) ;
void nvic_irq_enable(IRQn_Type irq_num) ;

void nvic_irq_disable(uint32_t dev,uint8_t index ) ;
void nvic_irq_disable(IRQn_Type irq_num) ;




#endif
