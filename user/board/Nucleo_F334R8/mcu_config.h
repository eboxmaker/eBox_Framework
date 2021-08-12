#ifndef __MCU_CONFIG_H
#define __MCU_CONFIG_H


#include "stdint.h"
#include "ebox_type.h"


//用户配置区域
//请查阅ebox_cpu_type.h寻找特定信号CPU的详细配置
#define STM32_TYPE    STM32F334R8
#define STM32_PINS    64
#define STM32_FLASH   64
#define STM32_RAM1    12
#define STM32_RAM2    0
#define STM32_RAM3    0
#define STM32_COMPANY "ST\0"

#define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */



#if (STM32_TYPE == STM32F334R8)
    #ifndef STM32F334x8
        #define STM32F334x8
    #endif
#elif (STM32_TYPE == STM32F373VC)
    #ifndef STM32F373xC
        #define STM32F373xC
    #endif
#endif


//外设配置层


//------------------抽象层宏定义------------------------

#define MCU_TYPE        STM32_TYPE
#define MCU_PINS        STM32_PINS
#define MCU_FLASH       STM32_FLASH
#define MCU_RAM1        STM32_RAM1
#define MCU_RAM2        STM32_RAM2
#define MCU_RAM3        STM32_RAM3
#define MCU_COMPANY     STM32_COMPANY



//RAM 区域定义
#define MCU_SRAM1_SIZE   MCU_RAM1*1024
#define MCU_SRAM1_BEGIN  0x20000000
#define MCU_SRAM1_END    (MCU_SRAM1_BEGIN + MCU_SRAM1_SIZE)

#if STM32_RAM2 != 0
#define MCU_SRAM2_SIZE   MCU_RAM2*1024
#define MCU_SRAM2_BEGIN  0x10000000
#define MCU_SRAM2_END    (MCU_SRAM2_BEGIN + MCU_SRAM2_SIZE)
#endif



#if STM32_RAM3 != 0
#define Bank1_SRAM3_ADDR  ((uint32_t)0x64000000)

#define MCU_SRAM3_SIZE   MCU_RAM3*1024
#define MCU_SRAM3_BEGIN  Bank1_SRAM3_ADDR
#define MCU_SRAM3_END    (MCU_SRAM3_BEGIN + MCU_SRAM3_SIZE)
#endif


//HEAP定义管理区域，用于动态内存管理
#ifdef __CC_ARM
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define MCU_HEAP_BEGIN 	((uint32_t)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="HEAP"
#define MCU_HEAP_BEGIN 	(__segment_end("HEAP"))
#else
extern int __bss_end;
#define MCU_HEAP_BEGIN 	&__bss_end
#endif
#define MCU_HEAP_END        MCU_SRAM1_END
#define MCU_HEAP_SIZE       (MCU_HEAP_END - MCU_HEAP_BEGIN)


#define MCU_SRAM1_USED       (MCU_HEAP_BEGIN - MCU_SRAM1_BEGIN)
#define MCU_SRAM1_REMAIND    (MCU_SRAM1_END - MCU_HEAP_BEGIN)





//FLASH 区域定义
#define MCU_FLASH_SIZE        MCU_FLASH*1024


#ifdef __CC_ARM
extern int SHT$$INIT_ARRAY$$Limit;
#define MCU_FLASH_PRG_END 	((uint32_t)&SHT$$INIT_ARRAY$$Limit)
#endif

#define MCU_FLASH_BEGIN         0x8000000
#define MCU_FLASH_END           (0x8000000+MCU_FLASH_SIZE)


#define MCU_FLASH_USED          (MCU_FLASH_PRG_END - MCU_FLASH_BEGIN)
#define MCU_FLASH_REMAIND       (MCU_FLASH_END - MCU_FLASH_PRG_END)
//--------------------------------------------------------



#include "stm32f3xx.h"
#include "stm32f3xx_ll_system.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_utils.h"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_cortex.h"
#include "stm32f3xx_ll_pwr.h"

#include "stm32f3xx_ll_dma.h"
#include "stm32f3xx_ll_exti.h"
#include "stm32f3xx_ll_usart.h"
#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_spi.h"
#include "stm32f3xx_ll_i2c.h"






#endif
