
#include "ebox_core.h"
#include "nvic.h"

typedef struct
{
    uint32_t dev;
    uint8_t irqn[2];
}DevToIRQn_t;
  
//�豸->�жϺŲ�ѯ��
const DevToIRQn_t dev_to_IRQn_table[] = 
{
    {TIM1_BASE,TIM1_BRK_UP_TRG_COM_IRQn,TIM1_CC_IRQn},
    {TIM2_BASE,TIM2_IRQn},
    {TIM3_BASE,TIM3_IRQn},
//    {TIM4_BASE,TIM4_IRQn},
    
//    {ADC1_BASE,ADC1_2_IRQn},
//    {ADC2_BASE,ADC1_2_IRQn},
    
    {SPI1_BASE,SPI1_IRQn},
    {SPI2_BASE,SPI2_IRQn},
    
    {I2C1_BASE,I2C1_IRQn},
    {I2C2_BASE,I2C2_IRQn},
    
    {LL_DMA_CHANNEL_1,DMA1_Channel1_IRQn},
    {LL_DMA_CHANNEL_2,DMA1_Channel2_3_IRQn},
    {LL_DMA_CHANNEL_3,DMA1_Channel2_3_IRQn},
    {LL_DMA_CHANNEL_4,DMA1_Channel4_5_6_7_IRQn},
    {LL_DMA_CHANNEL_5,DMA1_Channel4_5_6_7_IRQn},
    {LL_DMA_CHANNEL_6,DMA1_Channel4_5_6_7_IRQn},
    {LL_DMA_CHANNEL_7,DMA1_Channel4_5_6_7_IRQn},
    
   
    {USART1_BASE,USART1_IRQn},
    {USART2_BASE,USART2_IRQn},
    {USART3_BASE,USART3_4_IRQn},
    {USART4_BASE,USART3_4_IRQn},
    
    {LL_EXTI_LINE_0,EXTI0_1_IRQn},
    {LL_EXTI_LINE_1,EXTI0_1_IRQn},
    {LL_EXTI_LINE_2,EXTI2_3_IRQn},
    {LL_EXTI_LINE_3,EXTI2_3_IRQn},
    {LL_EXTI_LINE_4,EXTI4_15_IRQn},
    {LL_EXTI_LINE_5,EXTI4_15_IRQn},
    {LL_EXTI_LINE_6,EXTI4_15_IRQn},
    {LL_EXTI_LINE_7,EXTI4_15_IRQn},
    {LL_EXTI_LINE_8,EXTI4_15_IRQn},
    {LL_EXTI_LINE_9,EXTI4_15_IRQn},
    {LL_EXTI_LINE_10,EXTI4_15_IRQn},
    {LL_EXTI_LINE_11,EXTI4_15_IRQn},
    {LL_EXTI_LINE_12,EXTI4_15_IRQn},
    {LL_EXTI_LINE_13,EXTI4_15_IRQn},
    {LL_EXTI_LINE_14,EXTI4_15_IRQn},
    {LL_EXTI_LINE_15,EXTI4_15_IRQn},
    
    #ifdef STM32F10X_HD
    
    {TIM5_BASE,TIM5_IRQn},
    {TIM6_BASE,TIM6_IRQn},
    {TIM7_BASE,TIM7_IRQn},
    {TIM8_BASE,TIM8_BRK_IRQn,TIM8_UP_IRQn,TIM8_TRG_COM_IRQn,TIM8_CC_IRQn},

    {DMA2_Channel1_BASE,DMA2_Channel1_IRQn},
    {DMA2_Channel2_BASE,DMA2_Channel2_IRQn},
    {DMA2_Channel3_BASE,DMA2_Channel3_IRQn},
    {DMA2_Channel4_BASE,DMA2_Channel4_IRQn},
    {DMA2_Channel5_BASE,DMA2_Channel5_IRQn},
    
    {SPI3_BASE,SPI3_IRQn},
    
    {UART4_BASE,UART4_IRQn},
    {UART5_BASE,UART5_IRQn},
    
    #endif
    
};
/**
 *@name     void nvic_priority_group_config(uint32_t NVIC_PriorityGroup)
 *@brief    �ж����ȼ���������
 *@param    NVIC_PriorityGroup :  NVIC_PriorityGroup_2��Ĭ�Ϸ��鷽����0-3��ռʽ���ȼ���0-3�Ĵ����ȼ�
 *@retval   NONE
*/
void nvic_priority_group_config(uint32_t NVIC_PriorityGroup)
{
//    cotex-M0 û�����ȼ����飬ֻ��0-3�����ȼ�
}

/**
 *@name     static IRQn_Type dev_to_irqn(uint32_t dev,uint8_t index)
 *@brief    �����豸��ַ�����Ӧ�ĵڼ����ж����������ҳ���Ӧ���жϺ�
 *@param    dev  :  �豸��ַ����(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1�ȵ�
            index:  �豸�ĵڼ����ж���ڡ����֧��4������0-3��һ���豸ֻ��һ����������0
 *@retval   �жϺ�
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




//�����豸����ַ�������ж����ȼ���index���һ��������N���ж���ڵĵڼ����ж����
/**
 *@name     void nvic_dev_set_priority(uint32_t dev, uint8_t index , uint8_t PreemptionPriority,uint8_t SubPriority) 
 *@brief    �����豸��ַ�����Ӧ�ĵڼ����ж����������ö�Ӧ���жϺŵ��������ȼ�
 *@param    dev  :  �豸��ַ����(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1�ȵ�
            index:  �豸�ĵڼ����ж���ڡ����֧��4������0-3��һ���豸ֻ��һ����������0
            PreemptionPriority:     ��ռʽ���ȼ�0-3
            SubPriority:            ��Ч�ģ�������оƬ���ּ���
 *@retval   NONE
*/
void nvic_dev_set_priority(uint32_t dev, uint8_t index , uint8_t PreemptionPriority,uint8_t SubPriority) 
{
    
    IRQn_Type irq_num = dev_to_irqn(dev,index);
    NVIC_SetPriority(irq_num,PreemptionPriority);
}


/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)  
 *@brief    �����豸��ַ��������Ӧ���жϺŵ��ж�
 *@param    dev  :  �豸��ַ����(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1�ȵ�
            index:  �豸�ĵڼ����ж���ڡ����֧��4������0-3��һ���豸ֻ��һ����������0
 *@retval   NONE
*/
void nvic_dev_enable(uint32_t dev,uint8_t index) 
{
    IRQn_Type irq_num = dev_to_irqn(dev,index);
    NVIC->ISER[irq_num / 32] = bit_shift(irq_num % 32);//M0ֻ��32��
}

/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)  
 *@brief    �ر��豸��ַ��������Ӧ���жϺŵ��ж�
 *@param    dev  :  �豸��ַ����(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1�ȵ�
            index:  �豸�ĵڼ����ж���ڡ����֧��4������0-3��һ���豸ֻ��һ����������0
 *@retval   NONE
*/
void nvic_dev_disable(uint32_t dev,uint8_t index) 
{
    IRQn_Type irq_num = dev_to_irqn(dev,index);
    NVIC->ICER[irq_num / 32] = bit_shift(irq_num % 32);//M0ֻ��32��
}








//ֱ�������жϺ��������ж����ȼ�
/**
 *@name     void nvic_irq_set_priority(IRQn_Type irq_num, uint8_t PreemptionPriority,uint8_t SubPriority) 
 *@brief    �����жϺŵ��������ȼ�
 *@param    irq_num     :  �жϺţ���TIM2_IRQn,USART1_IRQn
            PreemptionPriority:     ��ռʽ���ȼ�0-3
            SubPriority:            ��Ч�ģ�������оƬ���ּ���
 *@retval   NONE
*/
void nvic_irq_set_priority(IRQn_Type irq_num, uint8_t PreemptionPriority,uint8_t SubPriority) 
{
    NVIC_SetPriority(irq_num,PreemptionPriority);
}

/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)  
 *@brief    �����жϺŵ��ж�
 *@param    irq_num     :  �жϺţ���TIM2_IRQn,USART1_IRQn
 *@retval   NONE
*/
void nvic_irq_enable(IRQn_Type irq_num) 
{
    NVIC->ISER[irq_num / 32] = bit_shift(irq_num % 32);//M0ֻ��32��
}

/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)  
 *@brief    �ر��жϺŵ��ж�
 *@param    irq_num     :  �жϺţ���TIM2_IRQn,USART1_IRQn
 *@retval   NONE
*/
void nvic_irq_disable(IRQn_Type irq_num) 
{
    NVIC->ICER[irq_num / 32] = bit_shift(irq_num % 32);//M0ֻ��32��
}





