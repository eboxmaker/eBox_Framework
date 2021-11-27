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
 * @file n32g030_dbg.c
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g030_dbg.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup DBG
 * @brief DBG driver modules
 * @{
 */

/** @addtogroup DBGMCU_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup DBGMCU_Private_Defines
 * @{
 */
/**
 * @}
 */

/** @addtogroup DBGMCU_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup DBGMCU_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup DBGMCU_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup DBGMCU_Private_Functions
 * @{
 */


/**
 * @brief  Returns the UCID.
 * @return UCID
 */

void GetUCID(uint8_t *UCIDbuf)
{
	uint8_t num=0;
	
	for (num = 0; num < UCID_LENGTH; num ++)
    {
         UCIDbuf[num] = *(__IO uint8_t*)(UCID_BASE + num); 
    }
}


/**
 * @brief  Returns the UID.
 * @return UID
 */

void GetUID(uint8_t *UIDbuf)
{
	uint8_t num=0;
	
	for (num = 0; num < UID_LENGTH; num ++)
    {
         UIDbuf[num] = *(__IO uint8_t*)(UID_BASE + num); 
    }
}


/**
 * @brief  Returns the DBGMCU_ID.
 * @return DBGMCU_ID
 */

void GetDBGMCU_ID(uint8_t *DBGMCU_IDbuf)
{
    uint8_t num=0;
	
    for (num = 0; num < DBGMCU_ID_LENGTH; num ++)
    {
         DBGMCU_IDbuf[num] = *(__IO uint8_t*)(DBGMCU_ID_BASE + num); 
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
