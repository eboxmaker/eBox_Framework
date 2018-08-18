#ifndef __NVIC_H
#define __NVIC_H

#include "mcu_config.h"



#ifdef __cplusplus
extern "C" {
#endif
    
#include "mcu_config.h"


void nvic_priority_group_config(uint32_t NVIC_PriorityGroup);


void nvic_dev_set_priority(uint32_t dev, uint8_t index ,uint8_t PreemptionPriority,uint8_t SubPriority) ;
void nvic_dev_enable(uint32_t dev,uint8_t index ) ;
void nvic_dev_disable(uint32_t dev,uint8_t index ) ;

void nvic_irq_set_priority(IRQn_Type irq_num, uint8_t PreemptionPriority,uint8_t SubPriority) ;
void nvic_irq_enable(IRQn_Type irq_num) ;
void nvic_irq_disable(IRQn_Type irq_num) ;

#ifdef __cplusplus
}
#endif
#endif
