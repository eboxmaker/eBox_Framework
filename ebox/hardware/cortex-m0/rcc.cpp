#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx.h"
#include "rcc.h"
#include "mcu_define.h"


/**
 *@name     void rcc_clock_cmd(uint32_t dev, FunctionalState state)
 *@brief    �����豸��ַ������ر�ʱ��
 *@param    dev  :  �豸��ַ����(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1�ȵ�
            state:  ENABLE(����ʱ��) ���� DISABLE(�ر�ʱ��)
 *@retval   NONE
*/
void rcc_clock_cmd(uint32_t dev, FunctionalState state)
{
    uint32_t rcc;
    for(int i = 0; i < sizeof(dev_to_rcc_table) / sizeof(DevToRcc_t); i++)
    {
        if(dev_to_rcc_table[i].dev == dev)
            rcc  = dev_to_rcc_table[i].rcc;
    }
    if(dev >= AHBPERIPH_BASE)
    {
        state ? LL_AHB1_GRP1_EnableClock(rcc) : LL_AHB1_GRP1_DisableClock(rcc);
    }
    else if(dev >= APBPERIPH_BASE + 0x00010000)
    {
        state ? LL_APB1_GRP2_EnableClock(rcc) : LL_APB1_GRP2_DisableClock(rcc);
    }
    else if(dev >= APBPERIPH_BASE)
    {
        state ? LL_APB1_GRP1_EnableClock(rcc) : LL_APB1_GRP1_DisableClock(rcc);
    }

}
