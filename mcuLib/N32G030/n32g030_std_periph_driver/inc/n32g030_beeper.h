/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file n32g030_beeper.h
 * @author Nations
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __N32G030_BEEPER_H__
#define __N32G030_BEEPER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g030.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup BEEPER
 * @{
 */

/** @addtogroup BEEPER_Exported_Types
 * @{
 */

/**
 * @}
 */

/** @addtogroup BEEPER_Exported_Constants
 * @{
 */

/** @addtogroup BUZZER_CLKSEL
 * @{
 */

#define BEEPER_BUZZER_CLKSEL_L1 ((uint32_t)0x00000001)
#define BEEPER_BUZZER_CLKSEL_L2 ((uint32_t)0x00000002)
#define BEEPER_BUZZER_CLKSEL_L3 ((uint32_t)0x00000003)
#define BEEPER_BUZZER_CLKSEL_L4 ((uint32_t)0x00000004)
#define BEEPER_BUZZER_CLKSEL_L5 ((uint32_t)0x00000005)
#define BEEPER_BUZZER_CLKSEL_L6 ((uint32_t)0x00000006)
#define BEEPER_BUZZER_CLKSEL_L7 ((uint32_t)0x00000007)

#define BEEPER_BUZZER_CLKSEL_M1 ((uint32_t)0x00000008)
#define BEEPER_BUZZER_CLKSEL_M2 ((uint32_t)0x00000009)
#define BEEPER_BUZZER_CLKSEL_M3 ((uint32_t)0x0000000A)
#define BEEPER_BUZZER_CLKSEL_M4 ((uint32_t)0x0000000B)
#define BEEPER_BUZZER_CLKSEL_M5 ((uint32_t)0x0000000C)
#define BEEPER_BUZZER_CLKSEL_M6 ((uint32_t)0x0000000D)
#define BEEPER_BUZZER_CLKSEL_M7 ((uint32_t)0x0000000E)

#define BEEPER_BUZZER_CLKSEL_H1 ((uint32_t)0x0000000F)
#define BEEPER_BUZZER_CLKSEL_H2 ((uint32_t)0x00000010)
#define BEEPER_BUZZER_CLKSEL_H3 ((uint32_t)0x00000011)
#define BEEPER_BUZZER_CLKSEL_H4 ((uint32_t)0x00000012)
#define BEEPER_BUZZER_CLKSEL_H5 ((uint32_t)0x00000013)
#define BEEPER_BUZZER_CLKSEL_H6 ((uint32_t)0x00000014)
#define BEEPER_BUZZER_CLKSEL_H7 ((uint32_t)0x00000015)

#define IS_BEEPER_BUZZER_CLKSEL(CLKSEL)                                                   \
     (((CLKSEL) >= BEEPER_BUZZER_CLKSEL_L1) && ((CLKSEL) <= BEEPER_BUZZER_CLKSEL_H7))

/** @addtogroup BUZZER_EN
 * @{
 */

#define BEEPER_BUZZER_BUZZER_ENABLE  ((uint32_t)0x00000020)
#define BEEPER_BUZZER_BUZZER_DISABLE ((uint32_t)0x00000000)

#define IS_BEEPER_BUZZER_EN(CONFIG)                                                       \
     (((CONFIG) == BEEPER_BUZZER_BUZZER_ENABLE) || ((CONFIG) == BEEPER_BUZZER_BUZZER_DISABLE))

/** @addtogroup INV_EN
 * @{
 */

#define BEEPER_INV_ENABLE  ((uint32_t)0x00000040)
#define BEEPER_INV_DISABLE ((uint32_t)0x00000000)

#define IS_BEEPER_INV_EN(CONFIG)                                                          \
     (((CONFIG) == BEEPER_INV_ENABLE) || ((CONFIG) == BEEPER_INV_DISABLE))

/** @addtogroup BUZZ_LPRUN
 * @{
 */

#define BEEPER_LPRUN_MODE  ((uint32_t)0x00002000)
#define BEEPER_NORMAL_MODE ((uint32_t)0x00000000)

#define IS_BEEPER_RUN_MODE(CONFIG)                                                         \
     (((CONFIG) == BEEPER_LPRUN_MODE) || ((CONFIG) == BEEPER_NORMAL_MODE))

/** @addtogroup FREQ_SEL
 * @{
 */

#define BEEPER_FREQ_APBCLK_00 ((uint32_t)0x00000000)
#define BEEPER_FREQ_APBCLK_01 ((uint32_t)0x00000080)
#define BEEPER_FREQ_APBCLK_02 ((uint32_t)0x00000100)
#define BEEPER_FREQ_APBCLK_03 ((uint32_t)0x00000180)
#define BEEPER_FREQ_APBCLK_04 ((uint32_t)0x00000200)
#define BEEPER_FREQ_APBCLK_05 ((uint32_t)0x00000280)
#define BEEPER_FREQ_APBCLK_06 ((uint32_t)0x00000300)
#define BEEPER_FREQ_APBCLK_07 ((uint32_t)0x00000380)
#define BEEPER_FREQ_APBCLK_08 ((uint32_t)0x00000400)
#define BEEPER_FREQ_APBCLK_09 ((uint32_t)0x00000480)
#define BEEPER_FREQ_APBCLK_10 ((uint32_t)0x00000500)
#define BEEPER_FREQ_APBCLK_11 ((uint32_t)0x00000580)
#define BEEPER_FREQ_APBCLK_12 ((uint32_t)0x00000600)
#define BEEPER_FREQ_APBCLK_13 ((uint32_t)0x00000680)
#define BEEPER_FREQ_APBCLK_14 ((uint32_t)0x00000700)
#define BEEPER_FREQ_APBCLK_15 ((uint32_t)0x00000780)
#define BEEPER_FREQ_APBCLK_16 ((uint32_t)0x00000800)
#define BEEPER_FREQ_APBCLK_17 ((uint32_t)0x00000880)
#define BEEPER_FREQ_APBCLK_18 ((uint32_t)0x00000900)
#define BEEPER_FREQ_APBCLK_19 ((uint32_t)0x00000980)
#define BEEPER_FREQ_APBCLK_20 ((uint32_t)0x00000A00)
#define BEEPER_FREQ_APBCLK_21 ((uint32_t)0x00000A80)
#define BEEPER_FREQ_APBCLK_22 ((uint32_t)0x00000B00)
#define BEEPER_FREQ_APBCLK_23 ((uint32_t)0x00000B80)

#define BEEPER_FREQ_APBCLK_24 ((uint32_t)0x00000C00)
#define BEEPER_FREQ_APBCLK_25 ((uint32_t)0x00000C80)
#define BEEPER_FREQ_APBCLK_26 ((uint32_t)0x00000D00)
#define BEEPER_FREQ_APBCLK_27 ((uint32_t)0x00000D80)
#define BEEPER_FREQ_APBCLK_28 ((uint32_t)0x00000E00)
#define BEEPER_FREQ_APBCLK_29 ((uint32_t)0x00000E80)
#define BEEPER_FREQ_APBCLK_30 ((uint32_t)0x00000F00)
#define BEEPER_FREQ_APBCLK_31 ((uint32_t)0x00000F80)
#define BEEPER_FREQ_APBCLK_32 ((uint32_t)0x00001000)
#define BEEPER_FREQ_APBCLK_33 ((uint32_t)0x00001080)
#define BEEPER_FREQ_APBCLK_34 ((uint32_t)0x00001100)
#define BEEPER_FREQ_APBCLK_35 ((uint32_t)0x00001180)
#define BEEPER_FREQ_APBCLK_36 ((uint32_t)0x00001200)
#define BEEPER_FREQ_APBCLK_37 ((uint32_t)0x00001280)
#define BEEPER_FREQ_APBCLK_38 ((uint32_t)0x00001300)
#define BEEPER_FREQ_APBCLK_39 ((uint32_t)0x00001380)
#define BEEPER_FREQ_APBCLK_40 ((uint32_t)0x00001400)
#define BEEPER_FREQ_APBCLK_41 ((uint32_t)0x00001480)
#define BEEPER_FREQ_APBCLK_42 ((uint32_t)0x00001500)
#define BEEPER_FREQ_APBCLK_43 ((uint32_t)0x00001580)
#define BEEPER_FREQ_APBCLK_44 ((uint32_t)0x00001600)
#define BEEPER_FREQ_APBCLK_45 ((uint32_t)0x00001680)
#define BEEPER_FREQ_APBCLK_46 ((uint32_t)0x00001700)
#define BEEPER_FREQ_APBCLK_47 ((uint32_t)0x00001780)
#define BEEPER_FREQ_APBCLK_48 ((uint32_t)0x00001800)

#define IS_BEEPER_FREQ_APBCLK(CLKSEL)                                                          \
     (((CLKSEL) >= (BEEPER_FREQ_APBCLK_00 >> 7)) && ((CLKSEL) <= (BEEPER_FREQ_APBCLK_48 >> 7)))

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup BEEPER_Exported_Macros
 * @{
 */
/**
 * @}
 */

/** @addtogroup BEEPER_Exported_Functions
 * @{
 */

void BEEPER_Init(uint32_t inv_en, uint32_t buzzer_clksel,uint32_t freq_sel);
void BEEPER_Config(FunctionalState Cmd,uint32_t buzz_lprun);

#ifdef __cplusplus
}
#endif

#endif /* __N32G030_BEEPER_H__ */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
