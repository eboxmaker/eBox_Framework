#ifndef __MCU_CONFIG_H
#define __MCU_CONFIG_H


#include "stdint.h"
#include "ebox_type.h"

#define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
//#define STM32F072xB
//#define USE_FULL_LL_DRIVER
//外设配置

#if defined(STM32F030x6)
	//用户配置区域
	//请查阅ebox_cpu_type.h寻找特定信号CPU的详细配置
	#define STM32_TYPE    STM32F030F4
	#define STM32_PINS    15
	#define STM32_FLASH   32			// 标称16k，实际超过16k
	#define STM32_RAM1    4
	#define STM32_COMPANY "ST\0"

#elif defined(STM32F030x8)
	//用户配置区域
	//请查阅ebox_cpu_type.h寻找特定信号CPU的详细配置
	#define STM32_TYPE    STM32F030C8
	#define STM32_PINS    48
	#define STM32_FLASH   64			
	#define STM32_RAM1    8
	#define STM32_COMPANY "ST\0"
#elif defined(STM32F072xB)

	//用户配置区域
	//请查阅ebox_cpu_type.h寻找特定信号CPU的详细配置
	#define STM32_TYPE    STM32F072RB
	#define STM32_PINS    51
	#define STM32_FLASH   128
	#define STM32_RAM1    16
	#define STM32_COMPANY "ST\0"

#else
#error "Please config target STM32F0xx device used in your application (in mcu_config.h file)"
#endif





//------------------抽象层宏定义------------------------

#define MCU_TYPE        STM32_TYPE
#define MCU_PINS        STM32_PINS
#define MCU_COMPANY     STM32_COMPANY



//RAM 区域定义
#define MCU_SRAM1_SIZE   STM32_RAM1*1024
#define MCU_SRAM1_BEGIN  0x20000000
#define MCU_SRAM1_END    (MCU_SRAM1_BEGIN + MCU_SRAM1_SIZE)


#ifdef __CC_ARM
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define MCU_HEAP_BEGIN 	((uint32_t)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="HEAP"
#else
extern int __bss_end;
#endif
#define MCU_HEAP_END        MCU_SRAM1_END
#define MCU_HEAP_SIZE       (MCU_HEAP_END - MCU_HEAP_BEGIN)


#define MCU_SRAM1_USED       (MCU_HEAP_BEGIN - MCU_SRAM1_BEGIN)
#define MCU_SRAM1_REMAIND    (MCU_SRAM1_END - MCU_HEAP_BEGIN)



//FLASH 区域定义
#define MCU_FLASH_SIZE        STM32_FLASH*1024


#ifdef __CC_ARM
extern int SHT$$INIT_ARRAY$$Limit;
#define MCU_FLASH_PRG_END 	((uint32_t)&SHT$$INIT_ARRAY$$Limit)
#endif

#define MCU_FLASH_BEGIN         0x8000000
#define MCU_FLASH_END           (0x8000000+MCU_FLASH_SIZE)


#define MCU_FLASH_USED          (MCU_FLASH_PRG_END - MCU_FLASH_BEGIN)
#define MCU_FLASH_REMAIND       (MCU_FLASH_END - MCU_FLASH_PRG_END)
//--------------------------------------------------------






#include "stm32f0xx.h"



#endif
