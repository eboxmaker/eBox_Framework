#ifndef __MCU_CONFIG_H
#define __MCU_CONFIG_H


#include "stdint.h"
#include "stm32f4xx.h"
#include "ebox_type.h"
/*
MCU_TYPE        MCU_PINS   CLOCK    FLASH   RAM     
STM32F050C4     LQFP 48     48      16      4
STM32F050C6     LQFP 48     48      32      4
STM32F050F4     TSSOP 20    48      16      4
STM32F050F6     TSSOP 20    48      32      4
STM32F050G4     UFQFPN 28   48      16      4
STM32F050G6     UFQFPN 28   48      32      4
STM32F050K4     UFQFPN 32   48      16      4
STM32F050K6     UFQFPN 32 	48      32      4
STM32F051C4     LQFP 48     48      16      4
STM32F051C6     LQFP 48     48      32      4
STM32F051C8     LQFP 48     48      64      8
STM32F051K4     LQFP 32     48      16      4
STM32F051K6     LQFP 32     48      32      4
STM32F051K8     LQFP 32     48      64      8
STM32F051R4     LQFP 64     48      16      4
STM32F051R6     LQFP 64     48      32      4
STM32F051R8     LQFP 64     48      64      8

MCU_TYPE        MCU_PINS   CLOCK    FLASH   RAM     
STM32F103C4      48         72      16      6
STM32F103C6      48         72      32      10
STM32F103C8      48         72      64      20
STM32F103CB      48         72      128     20
STM32F103R4      64         72      16      6
STM32F103R6      64         72      32      10
STM32F103R8      64         72      64      20
STM32F103RB      64         72      128     20
STM32F103RC      64         72      256     48
STM32F103RD      64         72      384     64
STM32F103RE      64         72      512     64
STM32F103RF      64         72      768     96
STM32F103RG      64         72      1024    96
STM32F103T4      36         72      16      6
STM32F103T6      36         72      32      10
STM32F103T8      36         72      64      20
STM32F103TB      36         72      128     20
STM32F103V8      100        72      64      20
STM32F103VB      100        72      128     20
STM32F103VC      100        72      256     48
STM32F103VD      100        72      384     64
STM32F103VE      100        72      512     64
STM32F103VF      100        72      768     96
STM32F103VG      100        72      1024    96
STM32F103ZC      144        72      256     48
STM32F103ZD      144        72      384     64
STM32F103ZE      144        72      512     64
STM32F103ZF      144        72      768     96
STM32F103ZG      144        72      1024    96


MCU_TYPE        MCU_PINS   CLOCK    FLASH   RAM     LMD
STM32F401CB     UFQFPN 48   84      128     64
STM32F401CC     UFQFPN 48   84      256     64
STM32F401RB     LQFP 64     84      128     64
STM32F401RC     LQFP 64     84      128     64
STM32F401VB     LQFP 100    84      128     64
STM32F401VC     LQFP 100    84      256     64
STM32F405OE     WLSCP 90    168     512     (128+64)
STM32F405OG     WLSCP 90    168     1024    (128+64)
STM32F405RG     LQFP 64     168     1024    (128+64)
STM32F405VG     LQFP 100    168     1024    (128+64)
STM32F405ZG     LQFP 144    168     1024    (128+64)
STM32F407IE     BGA 176;    168     512     (128+64)
STM32F407IG     BGA 176;    168     1024    (128+64)
STM32F407VE     LQFP 100    168     512     (128+64)
STM32F407VG     LQFP 100    168     1024    (128+64)
STM32F407ZE     LQFP 144    168     512     (128+64)
STM32F407ZG     LQFP 144    168     1024    (128+64)
STM32F415OG     WLSCP 90    168     1024    (128+64)
STM32F415RG     LQFP 64     168     1024    (128+64)
STM32F415VG     LQFP 100    168     1024    (128+64)
STM32F415ZG     LQFP 144    168     1024    (128+64)
STM32F417IE     BGA 176;    168     512     (128+64)
STM32F417IG     BGA 176;    168     1024    (128+64)
STM32F417VE     LQFP 100    168     512     (128+64)
STM32F417VG     LQFP 100    168     1024    (128+64)
*/

#define STM32_TYPE    STM32F417VG
#define STM32_PINS    176 
#define STM32_FLASH   1024
#define STM32_RAM1    128
#define STM32_RAM2    0
#define STM32_RAM3    0
#define STM32_COMPANY "ST\0"




#if ((STM32_TYPE >= STM32F401CB) && (STM32_TYPE <= STM32F417VG))
    #define STM32F40_41xxx
#endif


//外设配置层

#define USE_DMA 1


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









#endif
