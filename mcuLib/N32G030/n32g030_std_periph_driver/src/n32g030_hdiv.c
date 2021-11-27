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
 * @file n32g030_hdiv.c
 * @author Nations 
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g030_hdiv.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup HDIV
 * @brief HDIV driver modules
 * @{
 */

/** @addtogroup HDIV_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup HDIV_Private_Defines
 * @{
 */

/**
 * @}
 */

/** @addtogroup HDIV_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup HDIV_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup HDIV_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup HDIV_Private_Functions
 * @{
 */

/**
 * @brief  Enables or disables the specified HDIV interrupts.
 * @param Cmd new state of the specified HDIV interrupts.
 *   This parameter can be: ENABLE or DISABLE.
 */
void HDIV_ConfigInt(FunctionalState Cmd)
{
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if (Cmd != DISABLE)
    {
        HDIV->CTRLSTS |= HDIV_INT_ENABLE;
    }
    else
    {
        HDIV->CTRLSTS &= (~HDIV_INT_ENABLE);
    }
}

/**
 * @brief  Clears the HDIV interrupt pending bits.
 * @param  None
 */
void HDIV_ClrIntPendingBit(void)
{
    HDIV->CTRLSTS |= HDIV_FLAG_INTF;
}

/**
 * @brief  Checks whether the specified HDIV interrupt has occurred or not.
 * @param  None
 * @return The new state of HDIV_CTRLSTS_HDIVIF (SET or RESET).
 */
INTStatus HDIV_GetIntStatus(void)
{
    INTStatus bitstatus = RESET;
    
    if ((HDIV->CTRLSTS & HDIV_CTRLSTS_HDIVIF) != (uint32_t)RESET)
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
 * @brief  Checks whether the specified HDIV flag is set or not.
 * @param  HDIV_FLAG specifies the flag to check.
 *   This parameter can be one of the following values:
 *     @arg HDIV_FLAG_BUSY   HDIV busy flag
 *     @arg HDIV_FLAG_DF     HDIV dong flag
 *     @arg HDIV_FLAG_INTF     HDIV dong flag
 *     @arg HDIV_FLAG_DIVBY0 The divisor is 0 flag
 * @return The new state of HDIV_FLAG (SET or RESET).
 */
FlagStatus HDIV_GetFlagStatus(uint32_t HDIV_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_HDIV_FLAG(HDIV_FLAG));

    if(HDIV_FLAG==HDIV_FLAG_DIVBY0)
    {
        if ((HDIV->DIVBY0 & HDIV_FLAG) != (uint32_t)RESET)
        {
                bitstatus = SET;
        }
        else
        {
                bitstatus = RESET;
        }
    }else{
        if ((HDIV->CTRLSTS & HDIV_FLAG) != (uint32_t)RESET)
        {
            bitstatus = SET;
        }
        else
        {
            bitstatus = RESET;
        }
    }
    return bitstatus;
}

/**
 * @brief  Set 32bit unsigned interger dividend data through the HDIV peripheral.
 * @param  Data The data to set.
 */
void HDIV_SetDIVIDEND_Data(uint32_t data)
{
    /* 32bit unsigned interger dividend Data */
    HDIV->DIVIDEND = data;
}

/**
 * @brief  Get 32bit unsigned interger dividend data through the HDIV peripheral.
 * @param  None.
 * @return The register data.
 */
uint32_t HDIV_GetDIVIDEND_Data(void)
{
    /* 32bit unsigned interger dividend Data */
    return HDIV->DIVIDEND;
}

/**
 * @brief  Set 32bit unsigned interger divisor data through the HDIV peripheral.
 * @param  Data the data to set.
 */
void HDIV_SetDIVISOR_Data(uint32_t data)
{
    /* 32bit unsigned interger divisor Data */
    HDIV->DIVISOR = data;
}

/**
 * @brief  Get 32bit unsigned interger divisor data through the HDIV peripheral.
 * @param  None.
 * @return The register data.
 */
uint32_t HDIV_GetDIVISOR_Data(void)
{
    /* 32bit unsigned interger divisor Data */
    return HDIV->DIVISOR;
}

/**
 * @brief  Get Quotient calculated by the divider data through the HDIV peripheral.
 * @param  None.
 * @return The register data.
 */
uint32_t HDIV_GetQUOTIENT_Data(void)
{
    /* Quotient calculated by the divider Data */
    return HDIV->QUOTIENT;
}

/**
 * @brief  Get Remainder calculated by the divider data through the HDIV peripheral.
 * @param  None.
 * @return The register data.
 */
uint32_t HDIV_GetREMAINDER_Data(void)
{
    /* Remainder calculated by the divider */
    return HDIV->REMAINDER;
}

/**
 * @brief  Start or End the HDIV Calculation.
 * @param  Cmd new state of the HDIV Calculation.
 *         This parameter can be: ENABLE or DISABLE.
 */
void HDIV_Start(FunctionalState Cmd)
{
    if (Cmd != DISABLE)
    {
        /* Start HDIV Calculation */
        HDIV->CTRLSTS |= (HDIV_ENABLE | HDIV_START_ENABLE);
    }
    else
    {
        /* End HDIV Calculation */
        HDIV->CTRLSTS &= (~(HDIV_ENABLE | HDIV_START_ENABLE));
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
