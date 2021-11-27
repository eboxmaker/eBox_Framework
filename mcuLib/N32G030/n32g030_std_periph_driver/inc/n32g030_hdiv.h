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
 * @file n32g030_hdiv.h
 * @author Nations 
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __N32G030_HDIV_H__
#define __N32G030_HDIV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g030.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup HDIV
 * @{
 */

/** @addtogroup HDIV_Exported_Types
 * @{
 */

/**
 * @}
 */

/** @addtogroup HDIV_Exported_Constants
 * @{
 */

/** @addtogroup HDIVEN
 * @{
 */

#define HDIV_ENABLE  ((uint32_t)0x00000001)
#define HDIV_DISABLE ((uint32_t)0x00000000)

#define IS_HDIV_EN(CONFIG)                                                          \
     (((CONFIG) == HDIV_ENABLE) || ((CONFIG) == HDIV_DISABLE))

/** @addtogroup HDIVSTART
 * @{
 */

#define HDIV_START_ENABLE  ((uint32_t)0x00000002)
#define HDIV_START_DISABLE ((uint32_t)0x00000000)

#define IS_HDIV_START(CONFIG)                                                          \
     (((CONFIG) == HDIV_START_ENABLE) || ((CONFIG) == HDIV_START_DISABLE))

/** @addtogroup HDIV_Flag
 * @{
 */

#define HDIV_FLAG_BUSY    ((uint32_t)0x00000004)
#define HDIV_FLAG_DF      ((uint32_t)0x00000008)
#define HDIV_FLAG_INTF    ((uint32_t)0x00000010)
#define HDIV_FLAG_DIVBY0  ((uint32_t)0x00000001)
#define IS_HDIV_FLAG(FLAG)                                                             \
    (((FLAG) == HDIV_FLAG_BUSY) || ((FLAG) == HDIV_FLAG_DF) || ((FLAG) == HDIV_FLAG_INTF) || ((FLAG) == HDIV_FLAG_DIVBY0))

/** @addtogroup HDIVIEN
 * @{
 */

#define HDIV_INT_ENABLE  ((uint32_t)0x00000020)
#define HDIV_INT_DISABLE ((uint32_t)0x00000000)

#define IS_HDIV_INT(CONFIG)                                                            \
     (((CONFIG) == HDIV_INT_ENABLE) || ((CONFIG) == HDIV_INT_DISABLE))

/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup HDIV_Exported_Macros
 * @{
 */
/**
 * @}
 */

/** @addtogroup HDIV_Exported_Functions
 * @{
 */

void HDIV_ConfigInt(FunctionalState Cmd);
void HDIV_ClrIntPendingBit(void);
INTStatus HDIV_GetIntStatus(void);
FlagStatus HDIV_GetFlagStatus(uint32_t HDIV_FLAG);
void HDIV_SetDIVIDEND_Data(uint32_t data);
void HDIV_SetDIVISOR_Data(uint32_t data);
uint32_t HDIV_GetDIVIDEND_Data(void);
uint32_t HDIV_GetDIVISOR_Data(void);
uint32_t HDIV_GetQUOTIENT_Data(void);
uint32_t HDIV_GetREMAINDER_Data(void);
void HDIV_Start(FunctionalState Cmd);

#ifdef __cplusplus
}
#endif

#endif /* __N32G030_HDIV_H__ */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
