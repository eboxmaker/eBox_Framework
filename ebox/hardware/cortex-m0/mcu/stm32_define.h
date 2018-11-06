/******************************************************************************
程 序 名： stm32_define.h　
编 写 人： cat_li
编写时间： 2017年3月21日
相关平台： STM32F0xx, ebox
接口说明： stm32相关定义
修改日志：　　
	NO.1-  初始版本
	NO.2-  2017/5/16 typedef 定义的struct数据类型统一修改为t后缀,采用驼峰命名法
				   typedef 定义的enum数据类型结尾不带t后缀,采用驼峰命名法
******************************************************************************/

#ifndef __STM32_DEFINE_H_
#define __STM32_DEFINE_H_

#include "stm32f0xx.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_spi.h"
#include "stm32f0xx_ll_adc.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_i2c.h"
#include "stm32f0xx_ll_iwdg.h"
#include "stm32f0xx_hal_flash_ex.h"
#include "ebox_port_gpio.h"

#define NC  0xFFFFFFFF

// 关于GPIO的宏定义
// See stm32f0xx_hal_gpio.h and stm32f0xx_hal_gpio_ex.h for values of MODE, PUPD and OUTPUTTyper
// MODE占用2bit,OTYPER占用2bit,PUPD占用2bit
#define PIN_DATA(MODE,OTYPER,PUPD)  ((uint8_t)(((PUPD) << 4) | (OTYPER)<<2 | ((MODE) << 0)))
//#define SET_AFNUM(X,AFNUM)	(((AFNUM) << 6)| X)
#define GET_MODE(X)   	(((X) >> 0) & 0x03)
#define GET_OTYPER(X) 	(((X) >> 2) & 0x03)
#define GET_PUPD(X)   	(((X) >> 4) & 0x03)
//#define GET_AFNUM(X)  	(((X) >> 6) & 0x0F)
// GPIO mode
#define MODE_INPUT              (LL_GPIO_MODE_INPUT)
#define MODE_OUTPUT		        	(LL_GPIO_MODE_OUTPUT)
#define MODE_AF                 (LL_GPIO_MODE_ALTERNATE)
#define MODE_ANALOG             (LL_GPIO_MODE_ANALOG)
// GPIO OutputTyper
#define	OTYPER_PP				(LL_GPIO_OUTPUT_PUSHPULL)
#define OTYPER_OD				(LL_GPIO_OUTPUT_OPENDRAIN)
// GPIO UP,DOWN
#define PULLNO									(LL_GPIO_PULL_NO)
#define PULLDOWN								(LL_GPIO_PULL_DOWN)
#define PULLUP									(LL_GPIO_PULL_UP)
// 获取Pin,返回值0-15
#define GETPIN(A) 	 	  (uint16_t)(1<<(A&0x0f))
// 获取端口索引，返回0x00,0x10,0x20,0x30,0x40,0x50
#define GETPORTINDEX(A)   					(uint16_t)(((A)&0xf0) >>4)

#define STM_MODE_IT_RISING          (6)
#define STM_MODE_IT_FALLING         (7)
#define STM_MODE_IT_RISING_FALLING  (8)
#define STM_MODE_EVT_RISING         (9)
#define STM_MODE_EVT_FALLING        (10)
#define STM_MODE_EVT_RISING_FALLING (11)
#define STM_MODE_IT_EVT_RESET       (12)
// 定义GPIO时钟使能函数
#define	EnableGPIOClock(A)					LL_AHB1_GRP1_EnableClock(A)

//static const uint32_t GPIO_INFO[] ={
// LL_AHB1_GRP1_PERIPH_GPIOA,
// LL_AHB1_GRP1_PERIPH_GPIOB,
// LL_AHB1_GRP1_PERIPH_GPIOC
//};

/* structure --------------------------------------------------------------- */

/**
 * @brief  GPIO mode相关参数定义
 ********************************************************************************/
//typedef enum
//{
//	OUTPUT_PP  		= PIN_DATA(MODE_OUTPUT,OTYPER_PP,PULLNO),
//	OUTPUT_PP_PD 	= PIN_DATA(MODE_OUTPUT,OTYPER_PP,PULLDOWN),
//	OUTPUT_PP_PU    = PIN_DATA(MODE_OUTPUT,OTYPER_PP,PULLUP),

//	OUTPUT_OD       = PIN_DATA(MODE_OUTPUT,OTYPER_OD,PULLNO),
//	OUTPUT_OD_PD    = PIN_DATA(MODE_OUTPUT,OTYPER_OD,PULLDOWN),
//	OUTPUT_OD_PU    = PIN_DATA(MODE_OUTPUT,OTYPER_OD,PULLUP),

//	AIN             = PIN_DATA(MODE_ANALOG,OTYPER_PP,PULLNO),

//	INPUT           = PIN_DATA(MODE_INPUT,OTYPER_PP,PULLNO),
//	INPUT_PD        = PIN_DATA(MODE_INPUT,OTYPER_PP,PULLDOWN),
//	INPUT_PU        = PIN_DATA(MODE_INPUT,OTYPER_PP,PULLUP),

//	AF_OD           = PIN_DATA(MODE_AF,OTYPER_OD,PULLNO),  //10
//	AF_OD_PD        = PIN_DATA(MODE_AF,OTYPER_OD,PULLDOWN),
//	AF_OD_PU        = PIN_DATA(MODE_AF,OTYPER_OD,PULLUP),

//	AF_PP           = PIN_DATA(MODE_AF,OTYPER_PP,PULLNO),  //11
//	AF_PP_PD        = PIN_DATA(MODE_AF,OTYPER_PP,PULLDOWN),
//	AF_PP_PU        = PIN_DATA(MODE_AF,OTYPER_PP,PULLUP),
//}E_PinMode;///<gpio的模式

/**
  * @brief  GPIO枚举，利用Pin_ID将端口和pin合并，方便使用。
  * 保持一致
 ********************************************************************************/
//typedef enum{
//	E_PORTA = 0x0,
//	E_PORTB = 0x1,
//	E_PORTC = 0x2,
//	E_PORTD = 0x3,
//	E_PORTE = 0x4,
//	E_PORTF = 0x5
//}E_Port;

//typedef enum{
//	PA_0 = (E_PORTA<<4)|0, PA_1,PA_2,PA_3,PA_4,PA_5,PA_6,PA_7,PA_8,PA_9,PA_10,PA_11,PA_12,PA_13,PA_14,PA_15,
//	PB_0 = (E_PORTB<<4)|0, PB_1,PB_2,PB_3,PB_4,PB_5,PB_6,PB_7,PB_8,PB_9,PB_10,PB_11,PB_12,PB_13,PB_14,PB_15,
//	PC_0 = (E_PORTC<<4)|0, PC_1,PC_2,PC_3,PC_4,PC_5,PC_6,PC_7,PC_8,PC_9,PC_10,PC_11,PC_12,PC_13,PC_14,PC_15,
//	PD_0 = (E_PORTD<<4)|0, PD_1,PD_2,PD_3,PD_4,PD_5,PD_6,PD_7,PD_8,PD_9,PD_10,PD_11,PD_12,PD_13,PD_14,PD_15,
//	PE_0 = (E_PORTE<<4)|0, PE_1,PE_2,PE_3,PE_4,PE_5,PE_6,PE_7,PE_8,PE_9,PE_10,PE_11,PE_12,PE_13,PE_14,PE_15,
//	PF_0 = (E_PORTF<<4)|0, PF_1,PF_2,PF_3,PF_4,PF_5,PF_6,PF_7,PF_8,PF_9,PF_10,PF_11,PF_12,PF_13,PF_14,PF_15,
//	P_NC = (int)0xff
//}E_PinID;

typedef struct
{
    PIN_ID_t	_pinId;		//pin_id
    PIN_MODE	_pinMode;	    //pin 参数， mode，outputtyper,updown
    uint8_t		_pinAf;		//af功能
    uint32_t	_periph;	//外设名或通道号
} AF_FUN_S;

// 中断索引信息,用于中断初始化
typedef enum
{
    Irq1 = 0,
    Irq2,
    Irq3,
    Irq4,
    Irq5,
    Irq6,
} IrqIndex_t;

// 外设信息：外设基地址，时钟，中断，中断索引（中断索引需要与外设号对应，比如uart1对应irq1,i2c2对应irq2）
typedef struct
{
    uint32_t 	_periph_base;
    uint32_t 	_rcc;
    IRQn_Type	_irq;
    IrqIndex_t 	_irqIndex;
} Periph_S;

//typedef struct{
//	uint32_t 	_base;
//	fun_onePara_t	_EnableClock;	// 指向时钟使能函数
//	uint32_t 	_rcc;				// 外设时钟
//	IRQn_Type	_irq;				// 外设中断
//	IrqIndex_t 	_irqIndex;			// 中断索引
//}Periph_SS;

/**
 *@brief    根据Pin_id获取对应外设索引
 *@param    Periph 外设基地址，类似 USART1_BASE  *emap 保存外设信息的数组
 *@retval   对应外设在数组中的索引；0xff 外设信息不存在
*/
__STATIC_INLINE uint8_t getPeriphIndex(uint32_t periph_base, const Periph_S *emap)
{
    uint8_t i = 0;
    while (!((emap + i)->_periph_base  == periph_base ))
    {
        if ((emap + i)->_periph_base == NC)
        {
            return (uint8_t)NC;
        }
        i++;
    }
    return i;
}

//__STATIC_INLINE uint8_t getPeriphIndex1(uint32_t periph_base,const Periph_SS *emap)
//{
//	uint8_t i = 0;
//	while (!((emap+i)->_base  == periph_base ))
//	{
//		if ((emap+i)->_base == NC){
//			return (uint8_t)NC;
//		}
//		i++;
//	}
//	return i;
//}

/**
 *@brief    根据Pin_id获取对应外设索引
 *@param    val：1：输出高电平；0：输出低电平
 *@retval   NONE
*/
__STATIC_INLINE uint8_t getIndex(PIN_ID_t pin_id, const AF_FUN_S *emap)
{
    uint8_t i = 0;
    while (!((emap + i)->_pinId  == pin_id ))
    {
        if ((emap + i)->_pinId == 0xff)
        {
            return (uint8_t)NC;
        }
        i++;
    }
    return i;
}


#endif
