#ifndef __MCU_CONFIG_H
#define __MCU_CONFIG_H


#include "stdint.h"


#define MCU_PINS    48 //48, 64, 100, 144
#define MCU_TYPE    STM32F103C8T6
#define MCU_COMPANY "ST\0"

#ifdef __CC_ARM
    extern int Image$$RW_IRAM1$$ZI$$Limit;
    #define STM32_SRAM_BEGIN 	(&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
    #pragma section="HEAP"
#else
    extern int __bss_end;
#endif

#define STM32_SRAM_SIZE         20
#define STM32_SRAM_END          (0x20000000 + STM32_SRAM_SIZE * 1024)

#if defined (STM32_EX_SRAM)
    #define STM32_EX_SRAM_BEGIN    0x68000000 /* the begining address of external SRAM */
    #define STM32_EX_SRAM_END      0x68080000 /* the end address of external SRAM */
#endif

#endif
