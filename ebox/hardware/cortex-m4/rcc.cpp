#include "rcc.h"


typedef struct 
{
    uint32_t dev;
    uint32_t rcc;

}DevToRcc_t;

//外设->时钟源查询表
static const DevToRcc_t dev_to_rcc_table[] = 
{
    {GPIOA_BASE,RCC_AHB1Periph_GPIOA},
    {GPIOB_BASE,RCC_AHB1Periph_GPIOB},
    {GPIOC_BASE,RCC_AHB1Periph_GPIOC},
    {GPIOD_BASE,RCC_AHB1Periph_GPIOD},
    {GPIOE_BASE,RCC_AHB1Periph_GPIOE},
    {GPIOF_BASE,RCC_AHB1Periph_GPIOF},
    {GPIOG_BASE,RCC_AHB1Periph_GPIOG},
    {GPIOH_BASE,RCC_AHB1Periph_GPIOH},
    {GPIOI_BASE,RCC_AHB1Periph_GPIOI},
    
    {ADC1_BASE,RCC_APB2Periph_ADC1},
    {ADC2_BASE,RCC_APB2Periph_ADC2},
    {ADC3_BASE,RCC_APB2Periph_ADC3},
    
    {TIM1_BASE,RCC_APB2Periph_TIM1},
    {TIM2_BASE,RCC_APB1Periph_TIM2},
    {TIM3_BASE,RCC_APB1Periph_TIM3},
    {TIM4_BASE,RCC_APB1Periph_TIM4},
    {TIM5_BASE,RCC_APB1Periph_TIM5},
    {TIM6_BASE,RCC_APB1Periph_TIM6},
    {TIM7_BASE,RCC_APB1Periph_TIM7},
    {TIM8_BASE,RCC_APB2Periph_TIM8},
    {TIM9_BASE,RCC_APB2Periph_TIM9},
    {TIM10_BASE,RCC_APB2Periph_TIM10},
    {TIM11_BASE,RCC_APB2Periph_TIM11},
    
    {SPI1_BASE,RCC_APB2Periph_SPI1},
    {SPI2_BASE,RCC_APB1Periph_SPI2},
    {SPI3_BASE,RCC_APB1Periph_SPI3},
    {SPI4_BASE,RCC_APB2Periph_SPI4},
    {SPI5_BASE,RCC_APB2Periph_SPI5},
    
    {I2C1_BASE,RCC_APB1Periph_I2C1},
    {I2C2_BASE,RCC_APB1Periph_I2C2},
    {I2C3_BASE,RCC_APB1Periph_I2C3},

    {USART1_BASE,RCC_APB2Periph_USART1},
    {USART2_BASE,RCC_APB1Periph_USART2},
    {USART3_BASE,RCC_APB1Periph_USART3},
    {UART4_BASE,RCC_APB1Periph_UART4},
    {UART5_BASE,RCC_APB1Periph_UART5},
    {USART6_BASE,RCC_APB2Periph_USART6}

    #if defined(STM32F427_437xx) || defined(STM32F429_439xx)
    ,{UART7_BASE,RCC_APB1Periph_UART7}
    ,{UART8_BASE,RCC_APB1Periph_UART8}
    #endif
    
};


/**
 *@name     void rcc_clock_cmd(uint32_t dev, FunctionalState state)
 *@brief    根据设备地址开启或关闭时钟
 *@param    dev  :  设备地址，如(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1等等
            state:  ENABLE(开启时钟) 或者 DISABLE(关闭时钟)
 *@retval   NONE
*/
void rcc_clock_cmd(uint32_t dev, FunctionalState state)
{
    uint32_t rcc;
    for(int i = 0; i < sizeof(dev_to_rcc_table)/sizeof(DevToRcc_t); i++)
    {
        if(dev_to_rcc_table[i].dev == dev)
         rcc  = dev_to_rcc_table[i].rcc;
    
    }
    if(dev >= AHB2PERIPH_BASE)
        RCC_AHB2PeriphClockCmd(rcc,state);
    else if(dev >= AHB1PERIPH_BASE)
        RCC_AHB1PeriphClockCmd(rcc,state);
    else if(dev >= APB2PERIPH_BASE)
        RCC_APB2PeriphClockCmd(rcc,state);
    else if(dev >= APB1PERIPH_BASE)
        RCC_APB1PeriphClockCmd(rcc,state);

}
