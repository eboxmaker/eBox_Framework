#ifndef __MCU_CONFIG_H
#define __MCU_CONFIG_H


#include "stdint.h"
#include "ebox_type.h"

//#define _X1 STM32F
//#define _X2 103
//#define _X3 C
//#define _X4 8

//#define __STM32_TYPE(A,B,C,D) A##B##C##D
//#define _STM32_TYPE(A,B,C,D) __STM32_TYPE(A,B,C,D)


//#define STM32_TYPE    _STM32_TYPE(_X1,_X2,_X3,_X4)
/* MCU CONFIG BEGIN */
#define STM32_TYPE    STM32F030C8
#define STM32_PINS    48
#define STM32_FLASH   64
#define STM32_RAM1    8
#define STM32_COMPANY "ST\0"
#define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
#define STM32F10X_MD
/* MCU CONFIG END */






#define MCU_TYPE        STM32_TYPE
#define MCU_PINS        STM32_PINS
#define MCU_COMPANY     STM32_COMPANY



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






//#define MCU_FLASH_SIZE        STM32_FLASH*1024
#define MCU_FLASH_SIZE        128*1024


#ifdef __CC_ARM
extern int SHT$$INIT_ARRAY$$Limit;
#define MCU_FLASH_PRG_END 	((uint32_t)&SHT$$INIT_ARRAY$$Limit)
#endif

#define MCU_FLASH_BEGIN         0x8000000
#define MCU_FLASH_END           (0x8000000+MCU_FLASH_SIZE)


#define MCU_FLASH_USED          (MCU_FLASH_PRG_END - MCU_FLASH_BEGIN)
#define MCU_FLASH_REMAIND       (MCU_FLASH_END - MCU_FLASH_PRG_END)
//--------------------------------------------------------



#include "stm32f10x.h"




#endif
