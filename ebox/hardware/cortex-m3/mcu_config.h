#ifndef __MCU_CONFIG_H
#define __MCU_CONFIG_H


#include "stdint.h"
#include "stm32f10x.h"

//�û���������
#define STM32_TYPE    STM32F103C8
#define STM32_PINS    48 
#define STM32_FLASH   64
#define STM32_RAM1    20
#define STM32_COMPANY "ST\0"



//����KEIL�����б���ĺ궨��ᵼ�´�ѡ����Ч������Ҫ����keil��deviceѡ�ѡ����ȷ��MCU
#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD) && !defined (STM32F10X_HD_VL) && !defined (STM32F10X_XL) && !defined (STM32F10X_CL) 
    #if (STM32_FLASH <= 32)
        #define STM32F10X_LD   
    #elif (STM32_FLASH <= 128)
        #define STM32F10X_MD   
    #elif (MCU_FLASH <= 1024)
        #define STM32F10X_HD   
    #endif
#endif



//��������

#define USE_DMA 1












//------------------�����궨��------------------------

#define MCU_TYPE        STM32_TYPE
#define MCU_PINS        STM32_PINS
#define MCU_COMPANY     STM32_COMPANY



//RAM ������
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






//FLASH ������
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







#endif
