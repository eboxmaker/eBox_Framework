#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx.h"
#include "rcc.h"



typedef struct 
{
    uint32_t dev;
    uint32_t rcc;
}DevToRcc_t;

//外设->时钟源查询表
static const DevToRcc_t dev_to_rcc_table[] = 
{
    {GPIOA_BASE,LL_AHB1_GRP1_PERIPH_GPIOA},
    {GPIOB_BASE,LL_AHB1_GRP1_PERIPH_GPIOB},
    {GPIOC_BASE,LL_AHB1_GRP1_PERIPH_GPIOC},
    {GPIOD_BASE,LL_AHB1_GRP1_PERIPH_GPIOD},
    {GPIOE_BASE,LL_AHB1_GRP1_PERIPH_GPIOE},
    {GPIOF_BASE,LL_AHB1_GRP1_PERIPH_GPIOF},
//    {GPIOG_BASE,LL_AHB1_GRP1_PERIPH_GPIOA},
//    {GPIOH_BASE,RCC_APB2Periph_GPIOH},
//    {GPIOI_BASE,RCC_APB2Periph_GPIOI},
    
    {ADC1_BASE,LL_APB1_GRP2_PERIPH_ADC1},
//    {ADC2_BASE,RCC_APB2Periph_ADC2},
//    
    {TIM1_BASE,LL_APB1_GRP2_PERIPH_TIM1},
    {TIM2_BASE,LL_APB1_GRP1_PERIPH_TIM2},
    {TIM3_BASE,LL_APB1_GRP1_PERIPH_TIM3},
//    {TIM4_BASE,LL_APB1_GRP1_PERIPH_TIM4},
//    {TIM5_BASE,RCC_APB1Periph_TIM5},
    {TIM6_BASE,LL_APB1_GRP1_PERIPH_TIM6},
    {TIM7_BASE,LL_APB1_GRP1_PERIPH_TIM7},
//    {TIM8_BASE,RCC_APB2Periph_TIM8},
//    {TIM9_BASE,RCC_APB2Periph_TIM9},
//    {TIM10_BASE,RCC_APB2Periph_TIM10},
//    {TIM11_BASE,RCC_APB2Periph_TIM11},
//    
    {SPI1_BASE,LL_APB1_GRP2_PERIPH_SPI1},
    {SPI2_BASE,LL_APB1_GRP1_PERIPH_SPI2},
//    {SPI3_BASE,RCC_APB1Periph_SPI3},
//    {SPI4_BASE,RCC_APB2Periph_SPI4},
//    {SPI5_BASE,RCC_APB2Periph_SPI5},
    {I2C1_BASE,LL_APB1_GRP1_PERIPH_I2C1},
    {I2C2_BASE,LL_APB1_GRP1_PERIPH_I2C2},
//    {I2C3_BASE,RCC_APB1Periph_I2C3},
    
    {USART1_BASE,LL_APB1_GRP2_PERIPH_USART1},
    {USART2_BASE,LL_APB1_GRP1_PERIPH_USART2},
    {USART3_BASE,LL_APB1_GRP1_PERIPH_USART3},
    {USART4_BASE,LL_APB1_GRP1_PERIPH_USART4},
//    {USART5_BASE,LL_APB1_GRP1_PERIPH_USART5}
    
    {DMA1_BASE,LL_AHB1_GRP1_PERIPH_DMA1},

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
    if(dev >=AHBPERIPH_BASE)
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
