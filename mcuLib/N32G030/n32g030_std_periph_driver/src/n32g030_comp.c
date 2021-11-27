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
 * @file n32g030_comp.c
 * @author Nations 
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g030_comp.h"
#include "n32g030_rcc.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup COMP
 * @brief COMP driver modules
 * @{
 */

/** @addtogroup COMP_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup COMP_Private_Defines
 * @{
 */

/**
 * @}
 */

/** @addtogroup COMP_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup COMP_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup COMP_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup COMP_Private_Functions
 * @{
 */
#define SetBitMsk(reg, bit, msk) ((reg) = ((reg) & ~(msk) | (bit)))
#define ClrBit(reg, bit)         ((reg) &= ~(bit))
#define SetBit(reg, bit)         ((reg) |= (bit))
#define GetBit(reg, bit)         ((reg) & (bit))
/**
 * @brief  Deinitializes the COMP peripheral registers to their default reset values.
 */
void COMP_DeInit(void)
{

}

/**
 * @brief  Init COMP Struct
 * @note   This function can be used for N32G030 devices.
 * @param  COMP_InitStruct
 * @return none.
 */
void COMP_StructInit(COMP_InitType* COMP_InitStruct)
{
    // COMP_CTRL
    COMP_InitStruct->ClockSelect   = COMP_CTRL_CLKSEL_SYSTEMCLK; // only COMP have this bit
    COMP_InitStruct->LowPoweMode   = COMP_CTRL_PWRMD_NORMAL;     // only COMP have this bit

    COMP_InitStruct->Blking        = COMP_CTRL_BLKING_NO;        /*see @ref COMP_CTRL_BLKING */
    COMP_InitStruct->Hyst          = COMP_CTRL_HYST_NO;          // see @COMPx_CTRL_HYST_MASK
    COMP_InitStruct->PolRev        = COMP_CTRL_POL_NONINVERTED;  // out polarity reverse
    COMP_InitStruct->OutTrg        = COMPx_CTRL_OUTTRG_NC;
    COMP_InitStruct->InpSel        = COMPx_CTRL_INPSEL_VREF;    //Float as same with comp
    COMP_InitStruct->InmSel        = COMPx_CTRL_INMSEL_VREF;    //NC as same with comp 
    // filter
    COMP_InitStruct->SampWindow    = COMPX_FILC_SAMPW_0;
    COMP_InitStruct->Thresh        = COMPX_FILC_THRESH_1;
    COMP_InitStruct->FilterEn      = COMPX_FILC_FILEN_DISABLE;
    // filter psc
    COMP_InitStruct->ClkPsc        = COMP_FILP_CLKPSC_MASK;
}

/**
 * @brief  Init COMP register
 * @note   This function can be used for N32G030 devices.
 * @param  COMPx
 * @param  COMP_InitStruct
 * @return none.
 */
void COMP_Init( COMP_InitType* COMP_InitStruct)
{
    COMP_SingleType* pCS;
    __IO uint32_t tmp;

    pCS = &COMP->Cmp;

    
    // filter
    tmp = pCS->FILC;
    tmp &= (~(uint32_t)(COMP_FILC_SAMPW_MASK | COMP_FILC_THRESH_MASK | COMP_FILC_FILEN_MASK));
    //for remove IAR warning[Pa089]
    tmp |= ((uint32_t)(COMP_InitStruct->SampWindow) | (uint32_t)(COMP_InitStruct->Thresh) | (uint32_t)(COMP_InitStruct->FilterEn));
    pCS->FILC = tmp;

    // filter psc
    pCS->FILP = COMP_InitStruct->ClkPsc;

    // ctrl
    tmp = pCS->CTRL;
    tmp &= (~(uint32_t)(COMP_CTRL_CLKSEL_MASK | COMP_CTRL_PWRMODE_MASK));
    tmp |= ((uint32_t)(COMP_InitStruct->ClockSelect) | (uint32_t)(COMP_InitStruct->LowPoweMode));
    
    tmp &= (~(uint32_t)(COMP_CTRL_BLKING_MASK | COMP_CTRL_HYST_MASK | COMP_CTRL_POL_MASK | COMP_CTRL_OUTTRG_MASK
                      | COMP_CTRL_INPSEL_MASK | COMP_CTRL_INMSEL_MASK | COMP_CTRL_EN_MASK));
    tmp |= ((uint32_t)(COMP_InitStruct->Blking) | (uint32_t)(COMP_InitStruct->Hyst) | (uint32_t)(COMP_InitStruct->PolRev)
             | (uint32_t)(COMP_InitStruct->OutTrg) | (uint32_t)(COMP_InitStruct->InpSel) | (uint32_t)(COMP_InitStruct->InmSel));

    pCS->CTRL = tmp;
}

/**
 * @brief  Enable COMPx x= 1
 * @note   This function can be used for N32G030 devices.
 * @param  COMPx : COMP
 * @param  Cmd   : COMP_CTRL_EN_ENABLE or COMP_CTRL_EN_DISABLE.
 * @return none.
 */
void COMP_Enable(COMP_CTRL_EN_ENUM Cmd)
{
    COMP->Cmp.CTRL &= (~(uint32_t)COMP_CTRL_EN_MASK);
    COMP->Cmp.CTRL |= Cmd;
}

/**
 * @brief  Set INPSEL
 * @note   This function can be used for N32G030 devices.
 * @param  COMPx  : COMP
 * @param  Inpsel : COMP_CTRL_INPSEL.
 * @return none.
 */
void COMP_SetInpSel( COMP_CTRL_INPSEL_ENUM Inpsel)
{
    __IO uint32_t tmp;

    tmp = COMP->Cmp.CTRL;

    tmp &= (~(uint32_t)COMP_CTRL_INPSEL_MASK);
    tmp |= Inpsel;
    
    COMP->Cmp.CTRL = tmp;

}

/**
 * @brief  Set INMSEL
 * @note   This function can be used for N32G030 devices.
 * @param  COMPx  : COMP
 * @param  Inmsel : COMP_CTRL_INMSEL.
 * @return none.
 */
void COMP_SetInmSel( COMP_CTRL_INMSEL_ENUM Inmsel)
{
    __IO uint32_t tmp;


    tmp = COMP->Cmp.CTRL;

    
    tmp &= (~COMP_CTRL_INMSEL_MASK);
    tmp |= Inmsel;
    

    COMP->Cmp.CTRL = tmp;

}

/**
 * @brief  Set OUTTRG
 * @note   This function can be used for N32G030 devices.
 * @param  COMPx  : COMP
 * @param  OutTrig : COMP_CTRL_OUTTRG.
 * @return none.
 */
void COMP_SetOutTrig( COMP_CTRL_OUTTRG_ENUM OutTrig)
{
    __IO uint32_t tmp;


    tmp = COMP->Cmp.CTRL;
    
    tmp &= (~(uint32_t)COMP_CTRL_OUTTRG_MASK);
    tmp |= OutTrig;
    
    COMP->Cmp.CTRL = tmp;
}

/**
 * @brief  Enables or disables the specified COMPx(x = 1) interrupts.
 * @note   This function can be used for N32G030 devices.
 * @param  COMPx_INT : COMP_INTEN_CMP1IEN_CFG.
 * @param  Cmd       : ENABLE or DISABLE.
 * @return none.
 */
void COMP_ConfigInt(COMP_INTEN_CMPXIEN_ENUM COMPx_INT, FunctionalState Cmd)
{
    if(Cmd != DISABLE)
    {
        COMP->INTEN |= (uint32_t)COMPx_INT;
    }
    else
    {
        COMP->INTEN &= (~(uint32_t)COMPx_INT);
    }
}

/**
 * @brief  Checks whether the specified COMPx(x = 1) interrupt has occurred or not
 * @note   This function can be used for N32G030 devices.
 * @param  status : COMP_INTSTS_CMP1IS_STATUS
 * @return The new state of COMP_INT (SET or RESET).
 */
INTStatus COMP_GetIntStatus(COMP_INTSTS_STATUS_ENUM status)
{
    INTStatus bitstatus = RESET;

    if ((COMP->INTSTS & status) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
 * @brief  Checks COMPx(x = 1) out status.
 * @note   This function can be used for N32G030 devices.
 * @param  COMPx : COMP
 * @return The new state of COMPx_CTRL_OUT (SET or RESET).
 */
FlagStatus COMP_GetCompOutStatus(void)
{
    FlagStatus bitstatus = RESET;
  
    __IO uint32_t tmp;

    if ((COMP->Cmp.CTRL & COMP_CTRL_OUT_MASK) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
 * @brief  Config COMPx_CTRL register read-only.
 * @note   This function can be used for N32G030 devices.
 * @param  COMPx_LOCK : COMP_LOCK_CMP1LK_LOCK/COMP_LOCK_CMP1LK_UNLOCK
 * @param  Cmd        : ENABLE or DISABLE.
 * @return none.
 */
void COMP_ConfigCTRLLock(COMP_LOCK_CMPXLK_ENUM COMPx_LOCK, FunctionalState Cmd)
{
    if(Cmd != DISABLE)
    {
        COMP->LOCK |= (uint32_t)COMPx_LOCK;
    }
    else
    {
        COMP->LOCK &= (~(uint32_t)COMPx_LOCK);
    }
}

/**
 * @brief  Config COMPx_CTRL register read-only.
 * @note   This function can be used for N32G030 devices.
 * @param  Vrefx      : VREF
 * @param  COMP_VREFx : COMP_INVREF_VREFXSEL_ENUM
 * @param  Cmd        : ENABLE or DISABLE.
 * @return none.
 */
void COMP_ConfigVREFx(VREFX_ENUM Vrefx, COMP_INVREF_VREFXSEL_ENUM COMP_VREFx, FunctionalState Cmd)
{
    if(Vrefx == VREF)
    {
        COMP->INVREF &= (~COMP_INVREF_VREFSEL_MSK);
        COMP->INVREF |= COMP_VREFx;
        if(Cmd != DISABLE)
        {
            COMP->INVREF |= COMP_INVREF_VREFEN_ENABLE;
        }
        else
        {
            COMP->INVREF &= (~(uint32_t)COMP_INVREF_VREFEN_ENABLE);
        }
    }
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
