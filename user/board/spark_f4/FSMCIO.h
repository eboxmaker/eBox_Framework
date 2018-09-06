#ifndef __FSMCIO_H__
#define __FSMCIO_H__

#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

    void fsmc_init_sram1_8bit();
    void fsmc_init_sram1_16bit();

    void fsmc_init_sram2_8bit();
    void fsmc_init_sram2_16bit();

    void fsmc_init_sram3_8bit();
    void fsmc_init_sram3_16bit();

    void fsmc_init_sram4_8bit();
    void fsmc_init_sram4_16bit();
#ifdef __cplusplus
}
#endif // __cplusplus

#endif 

