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
 * @file misc.c
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "misc.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup MISC
 * @brief MISC driver modules
 * @{
 */

/** @addtogroup MISC_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup MISC_Private_Defines
 * @{
 */

#define AIRCR_VECTKEY_MASK ((uint32_t)0x05FA0000)
/**
 * @}
 */

/** @addtogroup MISC_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup MISC_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup MISC_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup MISC_Private_Functions
 * @{
 */

/**
 * @brief  Initializes the NVIC peripheral according to the specified
 *         parameters in the NVIC_InitStruct.
 * @param NVIC_InitStruct pointer to a NVIC_InitType structure that contains
 *         the configuration information for the specified NVIC peripheral.
 */
void NVIC_Init(NVIC_InitType* NVIC_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
    assert_param(IS_NVIC_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPriority));

    /* Set Interrupt Priority */
    NVIC_SetPriority(NVIC_InitStruct->NVIC_IRQChannel, NVIC_InitStruct->NVIC_IRQChannelPriority);

    if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
    {
        /* Enable the Selected IRQ Channels --------------------------------------*/
        NVIC_EnableIRQ(NVIC_InitStruct->NVIC_IRQChannel);
    }
    else
    {
        /* Disable the Selected IRQ Channels -------------------------------------*/
        NVIC_DisableIRQ(NVIC_InitStruct->NVIC_IRQChannel);
    }
}

/**
 * @brief  Configures the SysTick clock source.
 * @param SysTick_CLKSource specifies the SysTick clock source.
 *   This parameter can be one of the following values:
 *     @arg SysTick_CLKSource_HCLK_Div8 AHB clock divided by 8 selected as SysTick clock source.
 *     @arg SysTick_CLKSource_HCLK AHB clock selected as SysTick clock source.
 */
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource)
{
    /* Check the parameters */
    assert_param(IS_SYSTICK_CLK_SOURCE(SysTick_CLKSource));
    if (SysTick_CLKSource == SysTick_CLKSource_HCLK)
    {
        SysTick->CTRL |= SysTick_CLKSource_HCLK;
    }
    else
    {
        SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;
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
