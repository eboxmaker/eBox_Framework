#include "nvic.h"
#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)

void nvic_priority_group_config(uint32_t NVIC_PriorityGroup)
{
  /* Check the parameters */
  assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
  
  /* Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup value */
  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
}



void nvic_irq_enable(IRQn_Type irq_num) 
{

    NVIC->ISER[irq_num / 32] = bit_shift(irq_num % 32);
}


void nvic_irq_disable(IRQn_Type irq_num) 
{

    NVIC->ICER[irq_num / 32] = bit_shift(irq_num % 32);
}



void nvic_irq_set_priority(IRQn_Type irqn, uint8_t PreemptionPriority,uint8_t SubPriority) 
{
    
    uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;

    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;

    tmppriority = (uint32_t)PreemptionPriority<< tmppre;
    tmppriority |=  SubPriority & tmpsub;
    tmppriority = tmppriority << 0x04;
        
    NVIC->IP[irqn] = tmppriority;

}