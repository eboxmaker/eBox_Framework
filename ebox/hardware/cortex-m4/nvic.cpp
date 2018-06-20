#include "nvic.h"


  
const DevToIRQn_t dev_to_IRQn_table[] = 
{
    {TIM1_BASE,TIM1_UP_TIM10_IRQn,TIM1_BRK_TIM9_IRQn,TIM1_TRG_COM_TIM11_IRQn,TIM1_CC_IRQn},
    {TIM2_BASE,TIM2_IRQn},
    {TIM3_BASE,TIM3_IRQn},
    {TIM4_BASE,TIM4_IRQn},
    {TIM5_BASE,TIM5_IRQn},
    {TIM6_BASE,TIM6_DAC_IRQn},
    {TIM7_BASE,TIM7_IRQn},
    {TIM8_BASE,TIM8_UP_TIM13_IRQn},
    {TIM9_BASE,TIM1_BRK_TIM9_IRQn},
    {TIM10_BASE,TIM1_UP_TIM10_IRQn},
    {TIM11_BASE,TIM1_TRG_COM_TIM11_IRQn},
    {TIM12_BASE,TIM4_IRQn},
    {TIM13_BASE,TIM8_UP_TIM13_IRQn},
    
    {ADC1_BASE,ADC_IRQn},
    {ADC2_BASE,ADC_IRQn},
    {ADC3_BASE,ADC_IRQn},
    
    {SPI1_BASE,SPI1_IRQn},
    {SPI2_BASE,SPI2_IRQn},
    
    {I2C1_BASE,I2C1_EV_IRQn,I2C1_ER_IRQn},
    {I2C2_BASE,I2C2_EV_IRQn,I2C2_ER_IRQn},
    
    {DMA1_Stream0_BASE,DMA1_Stream0_IRQn},
    {DMA1_Stream1_BASE,DMA1_Stream1_IRQn},
    {DMA1_Stream2_BASE,DMA1_Stream2_IRQn},
    {DMA1_Stream3_BASE,DMA1_Stream3_IRQn},
    {DMA1_Stream4_BASE,DMA1_Stream4_IRQn},
    {DMA1_Stream5_BASE,DMA1_Stream5_IRQn},
    {DMA1_Stream6_BASE,DMA1_Stream6_IRQn},
    {DMA1_Stream7_BASE,DMA1_Stream7_IRQn},
    
   
    {USART1_BASE,USART1_IRQn},
    {USART2_BASE,USART2_IRQn},
    {USART3_BASE,USART3_IRQn},
    


    {DMA2_Stream0_BASE,DMA2_Stream0_IRQn},
    {DMA2_Stream1_BASE,DMA2_Stream1_IRQn},
    {DMA2_Stream2_BASE,DMA2_Stream2_IRQn},
    {DMA2_Stream3_BASE,DMA2_Stream3_IRQn},
    {DMA2_Stream4_BASE,DMA2_Stream4_IRQn},
    {DMA2_Stream5_BASE,DMA2_Stream5_IRQn},
    {DMA2_Stream6_BASE,DMA2_Stream6_IRQn},
    {DMA2_Stream7_BASE,DMA2_Stream7_IRQn},
    
    {SPI3_BASE,SPI3_IRQn},
    
    {UART4_BASE,UART4_IRQn},
    {UART5_BASE,UART5_IRQn},
    
    
};
#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)

void nvic_priority_group_config(uint32_t NVIC_PriorityGroup)
{
  /* Check the parameters */
  assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
  
  /* Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup value */
  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
}

static IRQn_Type dev_to_irqn(uint32_t dev,uint8_t index)
{
    int i;
    for(i = 0; i < sizeof(dev_to_IRQn_table)/sizeof(DevToIRQn_t); i++)
    {
        if(dev == dev_to_IRQn_table[i].dev)
        {
            return (IRQn_Type)dev_to_IRQn_table[i].irqn[index];
            break;
        }
    }
    return IRQn_Type(0);
}





//按照设备基地址设置其中断优先级。index针对一个外设有N个中断入口的第几个中断入口
void nvic_irq_set_priority(uint32_t dev, uint8_t index , uint8_t PreemptionPriority,uint8_t SubPriority) 
{
    
    uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;

    IRQn_Type irq_num = dev_to_irqn(dev,index);
    if(irq_num == 0) return ;
    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;

    tmppriority = (uint32_t)PreemptionPriority<< tmppre;
    tmppriority |=  SubPriority & tmpsub;
    tmppriority = tmppriority << 0x04;
        
    NVIC->IP[irq_num] = tmppriority;
}
//直接输入中断号设置其中断优先级
void nvic_irq_set_priority(IRQn_Type irq_num, uint8_t PreemptionPriority,uint8_t SubPriority) 
{
    
    uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;

    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;

    tmppriority = (uint32_t)PreemptionPriority<< tmppre;
    tmppriority |=  SubPriority & tmpsub;
    tmppriority = tmppriority << 0x04;
        
    NVIC->IP[irq_num] = tmppriority;

}




void nvic_irq_enable(uint32_t dev,uint8_t index) 
{
    IRQn_Type irq_num = dev_to_irqn(dev,index);
    NVIC->ISER[irq_num / 32] = bit_shift(irq_num % 32);
}
void nvic_irq_enable(IRQn_Type irq_num) 
{
    NVIC->ISER[irq_num / 32] = bit_shift(irq_num % 32);
}





void nvic_irq_disable(uint32_t dev,uint8_t index) 
{
    IRQn_Type irq_num = dev_to_irqn(dev,index);
    NVIC->ICER[irq_num / 32] = bit_shift(irq_num % 32);
}
void nvic_irq_disable(IRQn_Type irq_num) 
{
    NVIC->ICER[irq_num / 32] = bit_shift(irq_num % 32);
}





