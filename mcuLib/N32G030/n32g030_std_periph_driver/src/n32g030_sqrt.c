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
 * @file n32g030_sqrt.c
 * @author Nations 
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g030_sqrt.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup SQRT
 * @brief SQRT driver modules
 * @{
 */

/** @addtogroup SQRT_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup SQRT_Private_Defines
 * @{
 */

/**
 * @}
 */

/** @addtogroup SQRT_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup SQRT_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup SQRT_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup SQRT_Private_Functions
 * @{
 */

/**
 * @brief  Enables or disables the specified SQRT interrupts.
 * @param Cmd new state of the specified SQRT interrupts.
 *   This parameter can be: ENABLE or DISABLE.
 */
void SQRT_ConfigInt(FunctionalState Cmd)
{
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if (Cmd != DISABLE)
    {
        SQRT->CTRLSTS |= SQRT_INT_ENABLE;
    }
    else
    {
        SQRT->CTRLSTS &= (~SQRT_INT_ENABLE);
    }
}

/**
 * @brief  Clears the SQRT interrupt pending bits.
 * @param  None
 */
void SQRT_ClrIntPendingBit(void)
{
    SQRT->CTRLSTS |= SQRT_FLAG_INTF;
}

/**
 * @brief  Checks whether the specified SQRT interrupt has occurred or not.
 * @param  None
 * @return The new state of SQRT_CTRLSTS_SQRTIF (SET or RESET).
 */
INTStatus SQRT_GetIntStatus(void)
{
    INTStatus bitstatus = RESET;
    
    if ((SQRT->CTRLSTS & SQRT_CTRLSTS_SQRTIF) != (uint32_t)RESET)
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
 * @brief  Checks whether the specified SQRT flag is set or not.
 * @param  SQRT_FLAG specifies the flag to check.
 *   This parameter can be one of the following values:
 *     @arg SQRT_FLAG_BUSY   SQRT busy flag
 *     @arg SQRT_FLAG_DF     SQRT dong flag
 *     @arg SQRT_FLAG_INTF The divisor is 0 flag
 * @return The new state of SQRT_FLAG (SET or RESET).
 */
FlagStatus SQRT_GetFlagStatus(uint32_t SQRT_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_SQRT_FLAG(SQRT_FLAG));

    if ((SQRT->CTRLSTS & SQRT_FLAG) != (uint32_t)RESET)
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
 * @brief  Set 32bit unsigned interger dividend data through the SQRT peripheral.
 * @param  Data The data to set.
 */
void SQRT_SetRADICANT_Data(uint32_t data)
{
    /* 32bit unsigned interger dividend Data */
    SQRT->RADICANT = data;
}

/**
 * @brief  Get 32bit unsigned interger dividend data through the SQRT peripheral.
 * @param  None.
 * @return The register data.
 */
uint32_t SQRT_GetRADICANT_Data(void)
{
    /* 32bit unsigned interger dividend Data */
    return SQRT->RADICANT;
}

/**
 * @brief  Get Remainder calculated by the divider data through the SQRT peripheral.
 * @param  None.
 * @return The register data.
 */
uint32_t SQRT_GetROOT_Data(void)
{
    /* Remainder calculated by the divider */
    return SQRT->ROOT;
}

/**
 * @brief  Start or End the SQRT Calculation.
 * @param  Cmd new state of the SQRT Calculation.
 *         This parameter can be: ENABLE or DISABLE.
 */
void SQRT_Start(FunctionalState Cmd)
{
    if (Cmd != DISABLE)
    {
        /* Start SQRT Calculation */
        SQRT->CTRLSTS |= (SQRT_ENABLE | SQRT_START_ENABLE);
    }
    else
    {
        /* End SQRT Calculation */
        SQRT->CTRLSTS &= (~(SQRT_ENABLE | SQRT_START_ENABLE));
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
