/******************************************************************************
程 序 名： stm32f072_define.h　
编 写 人： cat_li
编写时间： 2017年10月16日
接口说明： stm32f072xb外设定义.
	1个spi,i2c,usart,adc(11ch),15gpios,5tim(4general(3,14,16,17),1advance(1)4kRAM,16kFLASH
修改日志：　　
	NO.1-  初始版本
				2017/10/16 usart info 改用Periph_SS定义，信息中包含不同usart的时钟使能函数
******************************************************************************/

#ifndef __STM32F030_DEFINE_H_
#define __STM32F030_DEFINE_H_

#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_tim.h"
#include "ebox_port_gpio.h"

#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04

typedef struct
{
    uint32_t dev;
    uint32_t rcc;
} DevToRcc_t;

//外设->时钟源查询表
static const DevToRcc_t dev_to_rcc_table[] =
{
    {GPIOA_BASE, LL_AHB1_GRP1_PERIPH_GPIOA},
    {GPIOB_BASE, LL_AHB1_GRP1_PERIPH_GPIOB},
    {GPIOC_BASE, LL_AHB1_GRP1_PERIPH_GPIOC},
    {GPIOD_BASE, LL_AHB1_GRP1_PERIPH_GPIOD},
    {GPIOF_BASE, LL_AHB1_GRP1_PERIPH_GPIOF},


    {ADC1_BASE, LL_APB1_GRP2_PERIPH_ADC1},

    {TIM1_BASE, LL_APB1_GRP2_PERIPH_TIM1},
    {TIM3_BASE, LL_APB1_GRP1_PERIPH_TIM3},

    {SPI1_BASE, LL_APB1_GRP2_PERIPH_SPI1},
  
    {I2C1_BASE, LL_APB1_GRP1_PERIPH_I2C1},


    {USART1_BASE, LL_APB1_GRP2_PERIPH_USART1},

    {DMA1_BASE, LL_AHB1_GRP1_PERIPH_DMA1},
};

typedef struct
{
    uint32_t dev;
    uint8_t irqn[4];
} DevToIRQn_t;
//设备->中断号查询表
const DevToIRQn_t dev_to_IRQn_table[] =
{
    {TIM1_BASE, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_CC_IRQn},

    {TIM3_BASE, TIM3_IRQn},

    {SPI1_BASE, SPI1_IRQn},

    {I2C1_BASE, I2C1_IRQn},


    {LL_DMA_CHANNEL_1, DMA1_Channel1_IRQn},
    {LL_DMA_CHANNEL_2, DMA1_Channel2_3_IRQn},
    {LL_DMA_CHANNEL_3, DMA1_Channel2_3_IRQn},
    {LL_DMA_CHANNEL_4, DMA1_Channel4_5_6_7_IRQn},
    {LL_DMA_CHANNEL_5, DMA1_Channel4_5_6_7_IRQn},



    {USART1_BASE, USART1_IRQn},

    {LL_EXTI_LINE_0, EXTI0_1_IRQn},
    {LL_EXTI_LINE_1, EXTI0_1_IRQn},
    {LL_EXTI_LINE_2, EXTI2_3_IRQn},
    {LL_EXTI_LINE_3, EXTI2_3_IRQn},
    {LL_EXTI_LINE_4, EXTI4_15_IRQn},
    {LL_EXTI_LINE_5, EXTI4_15_IRQn},
    {LL_EXTI_LINE_6, EXTI4_15_IRQn},
    {LL_EXTI_LINE_7, EXTI4_15_IRQn},
    {LL_EXTI_LINE_8, EXTI4_15_IRQn},
    {LL_EXTI_LINE_9, EXTI4_15_IRQn},
    {LL_EXTI_LINE_10, EXTI4_15_IRQn},
    {LL_EXTI_LINE_11, EXTI4_15_IRQn},
    {LL_EXTI_LINE_12, EXTI4_15_IRQn},
    {LL_EXTI_LINE_13, EXTI4_15_IRQn},
    {LL_EXTI_LINE_14, EXTI4_15_IRQn},
    {LL_EXTI_LINE_15, EXTI4_15_IRQn},
};

typedef struct
{
    PIN_ID_t	_pinId;		//pin_id
    PIN_MODE	_pinMode;	    //pin 参数， mode，outputtyper,updown
    uint8_t		_pinAf;		//af功能
    uint32_t	_periph;	//外设名或通道号
} AF_FUN_S;

// PA2,9,14 tx; PA3,10 rx
static const AF_FUN_S UART_MAP[] =
{
//    PA2_ID, AF_PP_PU, LL_GPIO_AF_1, USART2_BASE,
//    PA3_ID, AF_PP_PU, LL_GPIO_AF_1, USART2_BASE,
    PA9_ID, AF_PP_PU, LL_GPIO_AF_1, USART1_BASE,
    PA10_ID, AF_PP_PU, LL_GPIO_AF_1, USART1_BASE,
    P_NC
};

// PB9 scl,PB9 SDA
static const AF_FUN_S I2C_MAP[] =
{
    // i2c1
    PB8_ID, AF_PP_PU, LL_GPIO_AF_1, I2C1_BASE,
    PB9_ID, AF_PP_PU, LL_GPIO_AF_1, I2C1_BASE,
    (PIN_ID_t)0xff
};

// sck,miso,mosi
static const AF_FUN_S SPI_MAP[] =
{
    PA5_ID, AF_PP_PU, 0, SPI1_BASE,
    PA6_ID, AF_PP_PU, 0, SPI1_BASE,
    PA7_ID, AF_PP_PU, 0, SPI1_BASE,
    PB3_ID, AF_PP_PU, 0, SPI1_BASE,
    P_NC
};

static const AF_FUN_S TIM_MAP[] =
{
    // TIM3,CH1,CH2,CH4
    PA6_ID, AF_PP, LL_GPIO_AF_1, TIM3_BASE + TIMxCH1,
    PA7_ID, AF_PP, LL_GPIO_AF_1, TIM3_BASE + TIMxCH2,
    PB1_ID, AF_PP, LL_GPIO_AF_1, TIM3_BASE + TIMxCH4,
    // TIM16,CH1
    PA6_ID, AF_PP, LL_GPIO_AF_5, TIM16_BASE + TIMxCH1,
    // TIM17,CH1
    PA7_ID, AF_PP, LL_GPIO_AF_5, TIM17_BASE + TIMxCH1,
    // TIM14,CH1
    PA4_ID, AF_PP, LL_GPIO_AF_4, TIM14_BASE + TIMxCH1,
    // TIM1,CH2,CH3
    PA8_ID, AF_PP, LL_GPIO_AF_2, TIM1_BASE + TIMxCH1,
    PA9_ID, AF_PP, LL_GPIO_AF_2, TIM1_BASE + TIMxCH2,
    PA10_ID, AF_PP, LL_GPIO_AF_2, TIM1_BASE + TIMxCH3,
    (PIN_ID_t)0xff
};

/**
 *@brief    根据Pin_id和periph获取索引
 *@param    val：1：输出高电平；0：输出低电平
 *@retval   NONE
*/
//__STATIC_INLINE
__STATIC_INLINE uint8_t getIndex(PIN_ID_t pin_id, const AF_FUN_S *emap, uint32_t periph)
{
    uint8_t i = 0;
    while (!((0xffffff00 & (emap + i)->_periph) == periph) || !((emap + i)->_pinId == pin_id))
    {
        if ((emap + i)->_pinId == 0xff)
        {
            return (uint8_t)0xff;
        }
        i++;
    }
    return i;
}

/**
 *@brief    根据Pin_id获取对应外设索引
 *@param    pin_id：pin_id     *emap  外设里列表
 *@retval   NONE
*/
//__STATIC_INLINE
__STATIC_INLINE uint8_t getIndex(PIN_ID_t pin_id, const AF_FUN_S *emap)
{
    uint8_t i = 0;
    while (!((emap + i)->_pinId  == pin_id ))
    {
        if ((emap + i)->_pinId == 0xff)
        {
            return (uint8_t)0xff;
        }
        i++;
    }
    return i;
}

#endif
