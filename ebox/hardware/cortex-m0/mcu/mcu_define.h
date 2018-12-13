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



#if defined(STM32F030x6)
  #include "stm32f030_define.h"
#elif defined(STM32F072xB)
  #include "stm32f072_define.h"
#else
#error "Please select first the target STM32F0xx device used in your application (in stm32f0xx.h file)"
#endif

#endif
