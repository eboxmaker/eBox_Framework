
#include "ebox_core.h"
#include "nvic.h"



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
void nvic_dev_set_priority(uint32_t dev, uint8_t index, uint8_t PreemptionPriority, uint8_t SubPriority)
{

    IRQn_Type irq_num = dev_to_irqn(dev, index);
    NVIC_SetPriority(irq_num, PreemptionPriority);
}


/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)
 *@brief    �����豸��ַ��������Ӧ���жϺŵ��ж�
 *@param    dev  :  �豸��ַ����(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1�ȵ�
            index:  �豸�ĵڼ����ж���ڡ����֧��4������0-3��һ���豸ֻ��һ����������0
 *@retval   NONE
*/
void nvic_dev_enable(uint32_t dev, uint8_t index)
{
    IRQn_Type irq_num = dev_to_irqn(dev, index);
    NVIC->ISER[irq_num / 32] = bit_shift(irq_num % 32);//M0ֻ��32��
}

/**
 *@name     void nvic_dev_enable(uint32_t dev,uint8_t index)
 *@brief    �ر��豸��ַ��������Ӧ���жϺŵ��ж�
 *@param    dev  :  �豸��ַ����(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1�ȵ�
            index:  �豸�ĵڼ����ж���ڡ����֧��4������0-3��һ���豸ֻ��һ����������0
 *@retval   NONE
*/
void nvic_dev_disable(uint32_t dev, uint8_t index)
{
    IRQn_Type irq_num = dev_to_irqn(dev, index);
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
    *SubPriority = 0;	// f0��Ч
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





