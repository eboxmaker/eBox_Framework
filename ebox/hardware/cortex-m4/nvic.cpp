

#include "ebox_core.h"
#include "nvic.h"


//设备->中断号查询表
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
    
    
   
    {USART1_BASE,USART1_IRQn},
    {USART2_BASE,USART2_IRQn},
    {USART3_BASE,USART3_IRQn},
    
    {EXTI_Line0,EXTI0_IRQn},
    {EXTI_Line1,EXTI1_IRQn},
    {EXTI_Line2,EXTI2_IRQn},
    {EXTI_Line3,EXTI3_IRQn},
    {EXTI_Line4,EXTI4_IRQn},
    {EXTI_Line5,EXTI9_5_IRQn},
    {EXTI_Line6,EXTI9_5_IRQn},
    {EXTI_Line7,EXTI9_5_IRQn},
    {EXTI_Line8,EXTI9_5_IRQn},
    {EXTI_Line9,EXTI9_5_IRQn},
    {EXTI_Line10,EXTI15_10_IRQn},
    {EXTI_Line11,EXTI15_10_IRQn},
    {EXTI_Line12,EXTI15_10_IRQn},
    {EXTI_Line13,EXTI15_10_IRQn},
    {EXTI_Line14,EXTI15_10_IRQn},
    {EXTI_Line15,EXTI15_10_IRQn},
//    {EXTI_Line16,EXTI15_10_IRQn},
//    {EXTI_Line17,EXTI15_10_IRQn},
//    {EXTI_Line18,EXTI15_10_IRQn},
//    {EXTI_Line19,EXTI15_10_IRQn},
//    {EXTI_Line20,EXTI15_10_IRQn},
//    {EXTI_Line21,EXTI15_10_IRQn},
//    {EXTI_Line22,EXTI15_10_IRQn},
//    {EXTI_Line23,EXTI15_10_IRQn},

    {DMA1_Stream0_BASE,DMA1_Stream0_IRQn},
    {DMA1_Stream1_BASE,DMA1_Stream1_IRQn},
    {DMA1_Stream2_BASE,DMA1_Stream2_IRQn},
    {DMA1_Stream3_BASE,DMA1_Stream3_IRQn},
    {DMA1_Stream4_BASE,DMA1_Stream4_IRQn},
    {DMA1_Stream5_BASE,DMA1_Stream5_IRQn},
    {DMA1_Stream6_BASE,DMA1_Stream6_IRQn},
    {DMA1_Stream7_BASE,DMA1_Stream7_IRQn},

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
    {USART6_BASE,USART6_IRQn}
    
    
    #if defined(STM32F427_437xx) || defined(STM32F429_439xx)
    ,{UART7_BASE,UART7_IRQn}
    ,{UART8_BASE,UART8_IRQn}
    #endif
    
};

/**
 *@name     void nvic_priority_group_config(uint32_t NVIC_PriorityGroup)
 *@brief    中断优先级分组配置
 *@param    NVIC_PriorityGroup :  NVIC_PriorityGroup_2是默认分组方案，0-3抢占式优先级，0-3的从优先级
 *@retval   NONE
*/
#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)
void nvic_priority_group_config(uint32_t NVIC_PriorityGroup)
{
  /* Check the parameters */
  assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
  
  /* Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup value */
  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
}

/**
 *@name     static IRQn_Type dev_to_irqn(uint32_t dev,uint8_t index)
 *@brief    根据设备地址和其对应的第几个中断索引，查找出对应的中断号
 *@param    dev  :  设备地址，如(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1等等
            index:  设备的第几个中断入口。最大支持4个，即0-3，一般设备只有一个，即可填0
 *@retval   中断号
*/
static IRQn_Type dev_to_irqn(uint32_t dev,uint8_t index)
{
    int i;
    for(i = 0; i < sizeof(dev_to_IRQn_table)/sizeof(DevToIRQn_t); i++)
    {
        if(dev == dev_to_IRQn_table[i].dev)
        {
            return (IRQn_Type)dev_to_IRQn_table[i].irqn[index];
        }
    }
    return IRQn_Type(0);
}





//按照设备基地址设置其中断优先级。index针对一个外设有N个中断入口的第几个中断入口
/**
 *@name     void nvic_dev_set_priority(uint32_t dev, uint8_t index , uint8_t PreemptionPriority,uint8_t SubPriority) 
 *@brief    根据设备地址和其对应的第几个中断索引，设置对应的中断号的主从优先级
 *@param    dev  :  设备地址，如(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1等等
            index:  设备的第几个中断入口。最大支持4个，即0-3，一般设备只有一个，即可填0
            PreemptionPriority:     抢占式优先级0-3
            SubPriority:            从优先级0-3
 *@retval   NONE
*/
void nvic_dev_set_priority(uint32_t dev, uint8_t index , uint8_t PreemptionPriority,uint8_t SubPriority) 
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

/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)  
 *@brief    开启设备地址和索引对应的中断号的中断
 *@param    dev  :  设备地址，如(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1等等
            index:  设备的第几个中断入口。最大支持4个，即0-3，一般设备只有一个，即可填0
 *@retval   NONE
*/
void nvic_dev_enable(uint32_t dev,uint8_t index) 
{
    IRQn_Type irq_num = dev_to_irqn(dev,index);
    NVIC->ISER[irq_num / 32] = bit_shift(irq_num % 32);
}

/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)  
 *@brief    关闭设备地址和索引对应的中断号的中断
 *@param    dev  :  设备地址，如(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1等等
            index:  设备的第几个中断入口。最大支持4个，即0-3，一般设备只有一个，即可填0
 *@retval   NONE
*/
void nvic_dev_disable(uint32_t dev,uint8_t index) 
{
    IRQn_Type irq_num = dev_to_irqn(dev,index);
    NVIC->ICER[irq_num / 32] = bit_shift(irq_num % 32);
}





//直接输入中断号设置其中断优先级
/**
 *@name     void nvic_irq_set_priority(IRQn_Type irq_num, uint8_t PreemptionPriority,uint8_t SubPriority) 
 *@brief    设置中断号的主从优先级
 *@param    irq_num     :  中断号，如TIM2_IRQn,USART1_IRQn
            PreemptionPriority:     抢占式优先级0-3
            SubPriority:            从优先级0-3
 *@retval   NONE
*/
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
void nvic_irq_get_priority(uint8_t irq_num, uint8_t *PreemptionPriority,uint8_t *SubPriority) 
{
    
    irq_num  = (irq_num -  16);
    uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;

    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;

//    tmppriority = (uint32_t)PreemptionPriority<< tmppre;
//    tmppriority |=  SubPriority & tmpsub;
//    tmppriority = tmppriority << 0x04;
//        
//    NVIC->IP[irq_num] = tmppriority;
    
    *PreemptionPriority = (NVIC->IP[irq_num] >> 4) >> tmppre;
    *SubPriority = ((uint8_t)((NVIC->IP[irq_num] >> 4) << (8 - tmppre))) >>(8 - tmppre);
}

/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)  
 *@brief    开启中断号的中断
 *@param    irq_num     :  中断号，如TIM2_IRQn,USART1_IRQn
 *@retval   NONE
*/
void nvic_irq_enable(IRQn_Type irq_num) 
{
    NVIC->ISER[irq_num / 32] = bit_shift(irq_num % 32);
}

/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)  
 *@brief    关闭中断号的中断
 *@param    irq_num     :  中断号，如TIM2_IRQn,USART1_IRQn
 *@retval   NONE
*/
void nvic_irq_disable(IRQn_Type irq_num) 
{
    NVIC->ICER[irq_num / 32] = bit_shift(irq_num % 32);
}





