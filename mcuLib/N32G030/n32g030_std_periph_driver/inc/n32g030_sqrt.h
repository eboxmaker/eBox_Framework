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
 * @file n32g030_sqrt.h
 * @author Nations 
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __N32G030_SQRT_H__
#define __N32G030_SQRT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g030.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup SQRT
 * @{
 */

/** @addtogroup SQRT_Exported_Types
 * @{
 */

/**
 * @}
 */

/** @addtogroup SQRT_Exported_Constants
 * @{
 */

/** @addtogroup SQRTEN
 * @{
 */

#define SQRT_ENABLE  ((uint32_t)0x00000001)
#define SQRT_DISABLE ((uint32_t)0x00000000)

#define IS_SQRT_EN(CONFIG)                                                          \
     (((CONFIG) == SQRT_ENABLE) || ((CONFIG) == SQRT_DISABLE))

/** @addtogroup SQRTSTART
 * @{
 */

#define SQRT_START_ENABLE  ((uint32_t)0x00000002)
#define SQRT_START_DISABLE ((uint32_t)0x00000000)

#define IS_SQRT_START(CONFIG)                                                          \
     (((CONFIG) == SQRT_START_ENABLE) || ((CONFIG) == SQRT_START_DISABLE))

/** @addtogroup SQRT_Flag
 * @{
 */

#define SQRT_FLAG_BUSY    ((uint32_t)0x00000004)
#define SQRT_FLAG_DF      ((uint32_t)0x00000008)
#define SQRT_FLAG_INTF    ((uint32_t)0x00000010)
#define IS_SQRT_FLAG(FLAG)                                                             \
    (((FLAG) == SQRT_FLAG_BUSY) || ((FLAG) == SQRT_FLAG_DF) || ((FLAG) == SQRT_FLAG_INTF))

/** @addtogroup SQRTIEN
 * @{
 */

#define SQRT_INT_ENABLE  ((uint32_t)0x00000020)
#define SQRT_INT_DISABLE ((uint32_t)0x00000000)

#define IS_SQRT_INT(CONFIG)                                                            \
     (((CONFIG) == SQRT_INT_ENABLE) || ((CONFIG) == SQRT_INT_DISABLE))

/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup SQRT_Exported_Macros
 * @{
 */
/**
 * @}
 */

/** @addtogroup SQRT_Exported_Functions
 * @{
 */

void SQRT_ConfigInt(FunctionalState Cmd);
void SQRT_ClrIntPendingBit(void);
INTStatus SQRT_GetIntStatus(void);
FlagStatus SQRT_GetFlagStatus(uint32_t SQRT_FLAG);
void SQRT_SetRADICANT_Data(uint32_t data);
uint32_t SQRT_GetRADICANT_Data(void);
uint32_t SQRT_GetROOT_Data(void);
void SQRT_Start(FunctionalState Cmd);

#ifdef __cplusplus
}
#endif

#endif /* __N32G030_SQRT_H__ */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
