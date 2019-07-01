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

#include "mcu_config.h"
#include "stm32f0xx.h"
#include "ebox_port_gpio.h"

// 外设时钟源
typedef struct
{
    uint32_t dev;
    uint32_t rcc;
} DevToRcc_t;
// 外设中断源
typedef struct
{
    uint32_t dev;
    uint8_t irqn[4];
} DevToIRQn_t;
// 外设 pin 信息
typedef struct
{
    PinId_t	_pinId;		//pin_id
    PinMode_t	_pinMode;	//pin 参数， mode，outputtyper,updown
    uint8_t		_pinAf;		//af功能
    uint32_t	_periph;	//外设名或通道号
}AF_FUN_S;

#if defined(STM32F030x6)
#include "stm32f030_define.h"
#elif defined(STM32F030x8)
#include "stm32f030x8_define.h"
#elif defined(STM32F072xB)
  #include "stm32f072_define.h"
#else
#error "Please add and include the target STM32F0xx device define (in mcu_define.h file)"
#endif

/**
 *@brief    根据Pin_id和periph获取索引
 *@param    val：1：输出高电平；0：输出低电平
 *@retval   NONE
*/
//__STATIC_INLINE
__STATIC_INLINE uint8_t getIndex(PinId_t pin_id, const AF_FUN_S *emap, uint32_t periph)
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
__STATIC_INLINE uint8_t getIndex(PinId_t pin_id, const AF_FUN_S *emap)
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
