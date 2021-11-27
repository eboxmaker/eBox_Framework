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
 * @file n32g030_opamp.c
 * @author Nations 
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g030_opamp.h"
#include "n32g030_rcc.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup OPAMP
 * @brief OPAMP driver modules
 * @{
 */

/** @addtogroup OPAMP_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup OPAMP_Private_Defines
 * @{
 */

/**
 * @}
 */

/** @addtogroup OPAMP_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup OPAMP_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup OPAMP_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup OPAMP_Private_Functions
 * @{
 */
#define SetBitMsk(reg, bit, msk) ((reg) = ((reg) & ~(msk) | (bit)))
#define ClrBit(reg, bit)         ((reg) &= ~(bit))
#define SetBit(reg, bit)         ((reg) |= (bit))
#define GetBit(reg, bit)         ((reg) & (bit))
/**
 * @brief  Deinitializes the OPAMP peripheral registers to their default reset values.
 */
void OPAMP_DeInit(void)
{

}
void OPAMP_StructInit(OPAMP_InitType* OPAMP_InitStruct)
{
    OPAMP_InitStruct->Gain              = OPAMP_CS_PGA_GAIN_2;
    OPAMP_InitStruct->TimeAutoMuxEn     = DISABLE;
    OPAMP_InitStruct->Mod               = OPAMP_CS_PGA_EN;
}
void OPAMP_Init(OPAMP_InitType* OPAMP_InitStruct)
{
    __IO uint32_t* pCs = &OPAMP->CS;
    __IO uint32_t tmp  = *pCs;
    
    SetBitMsk(tmp, OPAMP_InitStruct->Gain, OPAMP_CS_PGA_GAIN_MASK);
    if(OPAMP_InitStruct->TimeAutoMuxEn!=DISABLE)
        SetBitMsk(tmp, OPAMP_CS_TCMEN_MASK, OPAMP_CS_TCMEN_MASK);
    else
        SetBitMsk(tmp, 0, OPAMP_CS_TCMEN_MASK);
    SetBitMsk(tmp, OPAMP_InitStruct->Mod, OPAMP_CS_MOD_MASK);
    *pCs = tmp;
}
void OPAMP_Enable(FunctionalState en)
{
    __IO uint32_t* pCs = &OPAMP->CS;
    if (en)
        SetBit(*pCs, OPAMP_CS_EN_MASK);
    else
        ClrBit(*pCs, OPAMP_CS_EN_MASK);
}

void OPAMP_SetPgaGain(OPAMP_CS_PGA_GAIN Gain)
{
    __IO uint32_t* pCs = &OPAMP->CS;
    __IO uint32_t tmp  = *pCs;
    SetBitMsk(tmp, Gain, OPAMP_CS_PGA_GAIN_MASK);
    *pCs = tmp;
}
void OPAMP_SetVpSecondSel(OPAMP_CS_VPSSEL VpSSel)
{
    __IO uint32_t* pCs = &OPAMP->CS;
    __IO uint32_t tmp  = *pCs;
    SetBitMsk(tmp, VpSSel, OPAMP_CS_VPSEL_SECOND_MASK);
    *pCs = tmp;
}
void OPAMP_SetVmSecondSel(OPAMP_CS_VMSSEL VmSSel)
{
    __IO uint32_t* pCs = &OPAMP->CS;
    __IO uint32_t tmp  = *pCs;
    SetBitMsk(tmp, VmSSel, OPAMP_CS_VMSEL_SECOND_MASK);
    *pCs = tmp;
}
void OPAMP_SetVpSel(OPAMP_CS_VPSEL VpSel)
{
    __IO uint32_t* pCs = &OPAMP->CS;
    __IO uint32_t tmp  = *pCs;
    SetBitMsk(tmp, VpSel, OPAMP_CS_VPSEL_MASK);
    *pCs = tmp;
}
void OPAMP_SetVmSel(OPAMP_CS_VMSEL VmSel)
{
    __IO uint32_t* pCs = &OPAMP->CS;
    __IO uint32_t tmp  = *pCs;
    SetBitMsk(tmp, VmSel, OPAMP_CS_VMSEL_MASK);
    *pCs = tmp;
}

// Lock see @OPAMP_LOCK
void OPAMP_SetLock(uint32_t Lock)
{
    OPAMP->LOCK = Lock;
}
/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */
