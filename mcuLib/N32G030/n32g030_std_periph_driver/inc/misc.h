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
 * @file misc.h
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __MISC_H__
#define __MISC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g030.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup MISC
 * @{
 */

/** @addtogroup MISC_Exported_Types
 * @{
 */

/**
 * @brief  NVIC Init Structure definition
 */

typedef struct
{
    IRQn_Type NVIC_IRQChannel; /*!< Specifies the IRQ channel to be enabled or disabled.
                                  This parameter can be a value of @ref IRQn_Type
                                  (For the complete N32G030 Devices IRQ Channels list, please
                                   refer to n32g030.h file) */

    uint8_t NVIC_IRQChannelPriority; /*!< Specifies the priority for the IRQ channel
                                          specified in NVIC_IRQChannel. This parameter can be a value
                                          between 0 and 3 */

    FunctionalState NVIC_IRQChannelCmd; /*!< Specifies whether the IRQ channel defined in NVIC_IRQChannel
                                             will be enabled or disabled.
                                             This parameter can be set either to ENABLE or DISABLE */
} NVIC_InitType;

/**
 * @}
 */

/** @addtogroup MISC_Exported_Constants
 * @{
 */

/**
 * @}
 */

/** @addtogroup Preemption_Priority_Group
 * @{
 */

#define IS_NVIC_PRIORITY(PRIORITY) ((PRIORITY) < 0x04)

#define IS_NVIC_OFFSET(OFFSET) ((OFFSET) < 0x000FFFFF)

/**
 * @}
 */

/** @addtogroup SysTick_clock_source
 * @{
 */

#define SysTick_CLKSource_HCLK_Div8 ((uint32_t)0xFFFFFFFB)
#define SysTick_CLKSource_HCLK      ((uint32_t)0x00000004)
#define IS_SYSTICK_CLK_SOURCE(SOURCE)                                                                                  \
    (((SOURCE) == SysTick_CLKSource_HCLK) || ((SOURCE) == SysTick_CLKSource_HCLK_Div8))
/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup MISC_Exported_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup MISC_Exported_Functions
 * @{
 */

void NVIC_Init(NVIC_InitType* NVIC_InitStruct);
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource);

#ifdef __cplusplus
}
#endif

#endif /* __MISC_H__ */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
