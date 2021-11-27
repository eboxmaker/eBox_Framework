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
 * @file n32g030_opamp.h
 * @author Nations 
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __N32G030_OPAMPMP_H__
#define __N32G030_OPAMPMP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g030.h"
#include <stdbool.h>

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup OPAMP
 * @{
 */

/** @addtogroup OPAMP_Exported_Constants
 * @{
 */

//bit:20~17
typedef enum
{
    OPAMP_CS_VPSSEL_PF0    = ((uint32_t)0x00000000),
    OPAMP_CS_VPSSEL_PA0    = ((uint32_t)0x00020000),
    OPAMP_CS_VPSSEL_PA1    = ((uint32_t)0x00040000),
    OPAMP_CS_VPSSEL_PA4    = ((uint32_t)0x00060000),  
    OPAMP_CS_VPSSEL_PA7    = ((uint32_t)0x00080000),
    OPAMP_CS_VPSSEL_PB0    = ((uint32_t)0x000A0000),
    OPAMP_CS_VPSSEL_PB14   = ((uint32_t)0x000C0000),
} OPAMP_CS_VPSSEL;

//bit:16:14
typedef enum
{
    OPAMP_CS_VMSSEL_PA2        =  ((uint32_t)0x00000000),
    OPAMP_CS_VMSSEL_PA5        =  ((uint32_t)0x00004000),
    OPAMP_CS_VMSSEL_PB1        =  ((uint32_t)0x00008000),
    OPAMP_CS_VMSSEL_PB2        =  ((uint32_t)0x0000C000),
} OPAMP_CS_VMSSEL;

//bit:12~9
typedef enum
{
    OPAMP_CS_VPSEL_PF0     =  ((uint32_t)0x00000000),
    OPAMP_CS_VPSEL_PA0     =  ((uint32_t)0x00000200),
    OPAMP_CS_VPSEL_PA1     =  ((uint32_t)0x00000400),
    OPAMP_CS_VPSEL_PA4     =  ((uint32_t)0x00000600),
    OPAMP_CS_VPSEL_PA7     =  ((uint32_t)0x00000800),
    OPAMP_CS_VPSEL_PB0     =  ((uint32_t)0x00000A00),
    OPAMP_CS_VPSEL_PB14    =  ((uint32_t)0x00000C00),
} OPAMP_CS_VPSEL;

//bit:8~6
typedef enum
{
    OPAMP_CS_VMSEL_PA2        =  ((uint32_t)0x00000000),
    OPAMP_CS_VMSEL_PA5        =  ((uint32_t)0x00004000),
    OPAMP_CS_VMSEL_PB1        =  ((uint32_t)0x00008000),
    OPAMP_CS_VMSEL_PB2        =  ((uint32_t)0x0000C000),
} OPAMP_CS_VMSEL;

//bit:5~3
typedef enum
{
    OPAMP_CS_PGA_GAIN_2  = ((uint32_t)0x00000000),
    OPAMP_CS_PGA_GAIN_4  = ((uint32_t)0x00000008),
    OPAMP_CS_PGA_GAIN_8  = ((uint32_t)0x00000010),
    OPAMP_CS_PGA_GAIN_16 = ((uint32_t)0x00000018),
    OPAMP_CS_PGA_GAIN_32 = ((uint32_t)0x00000020),
} OPAMP_CS_PGA_GAIN;

//bit:2:1
typedef enum
{
    OPAMP_CS_EXT_OPAMP = (0x00 << 1),
    OPAMP_CS_PGA_EN    = (0x02 << 1),
    OPAMP_CS_FOLLOW    = (0x03 << 1),
} OPAMP_CS_MOD;

// bit mask
#define OPAMP_CS_EN_MASK           (0x01L << 0)
#define OPAMP_CS_MOD_MASK          (0x03L << 1)
#define OPAMP_CS_PGA_GAIN_MASK     (0x07L << 3)
#define OPAMP_CS_VMSEL_MASK        (0x07L << 6)
#define OPAMP_CS_VPSEL_MASK        (0x0FL << 9)
#define OPAMP_CS_TCMEN_MASK        (0x01L << 13)
#define OPAMP_CS_VMSEL_SECOND_MASK (0x07L << 14)
#define OPAMP_CS_VPSEL_SECOND_MASK (0x0FL << 17)
/** @addtogroup OPAMP_LOCK
 * @{
 */
#define OPAMP_LOCK 0x01L
/**
 * @}
 */
/**
 * @}
 */

/**
 * @brief  OPAMP Init structure definition
 */

typedef struct
{
    FunctionalState TimeAutoMuxEn; /*call ENABLE or DISABLE */
    OPAMP_CS_PGA_GAIN Gain; /*see @EM_PGA_GAIN */
    OPAMP_CS_MOD Mod; /*see @EM_OPAMP_MOD*/
} OPAMP_InitType;

/** @addtogroup OPAMP_Exported_Functions
 * @{
 */

void OPAMP_DeInit(void);
void OPAMP_StructInit(OPAMP_InitType* OPAMP_InitStruct);
void OPAMP_Init(OPAMP_InitType* OPAMP_InitStruct);
void OPAMP_Enable(FunctionalState en);
void OPAMP_SetPgaGain(OPAMP_CS_PGA_GAIN Gain);
void OPAMP_SetVpSecondSel(OPAMP_CS_VPSSEL VpSSel);
void OPAMP_SetVmSecondSel(OPAMP_CS_VMSSEL VmSSel);
void OPAMP_SetVpSel(OPAMP_CS_VPSEL VpSel);
void OPAMP_SetVmSel(OPAMP_CS_VMSEL VmSel);
void OPAMP_SetLock(uint32_t Lock); // see @OPAMP_LOCK
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /*__N32G030_ADC_H */
       /**
        * @}
        */
       /**
        * @}
        */
