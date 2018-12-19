
#include "ebox_core.h"
#include "nvic.h"



/**
 *@name     void nvic_priority_group_config(uint32_t NVIC_PriorityGroup)
 *@brief    中断优先级分组配置
 *@param    NVIC_PriorityGroup :  NVIC_PriorityGroup_2是默认分组方案，0-3抢占式优先级，0-3的从优先级
 *@retval   NONE
*/
void nvic_priority_group_config(uint32_t NVIC_PriorityGroup)
{
    //    cotex-M0 没有优先级分组，只有0-3的优先级
}

/**
 *@name     static IRQn_Type dev_to_irqn(uint32_t dev,uint8_t index)
 *@brief    根据设备地址和其对应的第几个中断索引，查找出对应的中断号
 *@param    dev  :  设备地址，如(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1等等
            index:  设备的第几个中断入口。最大支持4个，即0-3，一般设备只有一个，即可填0
 *@retval   中断号
*/
static IRQn_Type dev_to_irqn(uint32_t dev, uint8_t index)
{
    int i;
    for(i = 0; i < sizeof(dev_to_IRQn_table) / sizeof(DevToIRQn_t); i++)
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
            SubPriority:            无效的，和其他芯片保持兼容
 *@retval   NONE
*/
void nvic_dev_set_priority(uint32_t dev, uint8_t index, uint8_t PreemptionPriority, uint8_t SubPriority)
{

    IRQn_Type irq_num = dev_to_irqn(dev, index);
    NVIC_SetPriority(irq_num, PreemptionPriority);
}


/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)
 *@brief    开启设备地址和索引对应的中断号的中断
 *@param    dev  :  设备地址，如(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1等等
            index:  设备的第几个中断入口。最大支持4个，即0-3，一般设备只有一个，即可填0
 *@retval   NONE
*/
void nvic_dev_enable(uint32_t dev, uint8_t index)
{
    IRQn_Type irq_num = dev_to_irqn(dev, index);
    NVIC->ISER[irq_num / 32] = bit_shift(irq_num % 32);//M0只有32个
}

/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)
 *@brief    关闭设备地址和索引对应的中断号的中断
 *@param    dev  :  设备地址，如(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1等等
            index:  设备的第几个中断入口。最大支持4个，即0-3，一般设备只有一个，即可填0
 *@retval   NONE
*/
void nvic_dev_disable(uint32_t dev, uint8_t index)
{
    IRQn_Type irq_num = dev_to_irqn(dev, index);
    NVIC->ICER[irq_num / 32] = bit_shift(irq_num % 32);//M0只有32个
}


//直接输入中断号设置其中断优先级
/**
 *@name     void nvic_irq_set_priority(IRQn_Type irq_num, uint8_t PreemptionPriority,uint8_t SubPriority)
 *@brief    设置中断号的主从优先级
 *@param    irq_num     :  中断号，如TIM2_IRQn,USART1_IRQn
            PreemptionPriority:     抢占式优先级0-3
            SubPriority:            无效的，和其他芯片保持兼容
 *@retval   NONE
*/
void nvic_irq_set_priority(IRQn_Type irq_num, uint8_t PreemptionPriority, uint8_t SubPriority)
{
    NVIC_SetPriority(irq_num, PreemptionPriority);
}
void nvic_irq_get_priority(uint8_t irq_num, uint8_t *PreemptionPriority, uint8_t *SubPriority)
{

    //    irq_num  = (irq_num -  16);
    //    uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;

    //    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700)) >> 0x08;
    //    tmppre = (0x4 - tmppriority);
    //    tmpsub = tmpsub >> tmppriority;

    //    tmppriority = (uint32_t)PreemptionPriority<< tmppre;
    //    tmppriority |=  SubPriority & tmpsub;
    //    tmppriority = tmppriority << 0x04;
    //
    //    NVIC->IP[irq_num] = tmppriority;

    *PreemptionPriority = 		NVIC_GetPriority((IRQn_Type)irq_num);
    *SubPriority = 0;	// f0无效
}
/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)
 *@brief    开启中断号的中断
 *@param    irq_num     :  中断号，如TIM2_IRQn,USART1_IRQn
 *@retval   NONE
*/
void nvic_irq_enable(IRQn_Type irq_num)
{
    NVIC->ISER[irq_num / 32] = bit_shift(irq_num % 32);//M0只有32个
}

/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)
 *@brief    关闭中断号的中断
 *@param    irq_num     :  中断号，如TIM2_IRQn,USART1_IRQn
 *@retval   NONE
*/
void nvic_irq_disable(IRQn_Type irq_num)
{
    NVIC->ICER[irq_num / 32] = bit_shift(irq_num % 32);//M0只有32个
}





