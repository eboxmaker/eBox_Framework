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
 * @file n32g030_flash.c
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g030_flash.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup FLASH
 * @brief FLASH driver modules
 * @{
 */

/** @addtogroup FLASH_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup FLASH_Private_Defines
 * @{
 */

/* Flash Access Control Register bits */
#define AC_LATENCY_MSK  ((uint32_t)0x00000038)
#define AC_PRFTBE_MSK   ((uint32_t)0x0000002F)

/* Flash Access Control Register bits */
#define AC_PRFTBS_MSK   ((uint32_t)0x00000020)

/* Flash Control Register bits */
#define CTRL_Set_PG       ((uint32_t)0x00000001)
#define CTRL_Reset_PG     ((uint32_t)0x00003FFE)
#define CTRL_Set_PER      ((uint32_t)0x00000002)
#define CTRL_Reset_PER    ((uint32_t)0x00003FFD)
#define CTRL_Set_MER      ((uint32_t)0x00000004)
#define CTRL_Reset_MER    ((uint32_t)0x00003FFB)
#define CTRL_Set_OPTPG    ((uint32_t)0x00000010)
#define CTRL_Reset_OPTPG  ((uint32_t)0x00003FEF)
#define CTRL_Set_OPTER    ((uint32_t)0x00000020)
#define CTRL_Reset_OPTER  ((uint32_t)0x00003FDF)
#define CTRL_Set_START    ((uint32_t)0x00000040)
#define CTRL_Set_LOCK     ((uint32_t)0x00000080)

/* FLASH Mask */
#define RDPRTL1_MSK  ((uint32_t)0x00000002)
#define RDPRTL2_MSK  ((uint32_t)0x80000000)
#define OBR_USER_MSK ((uint32_t)0x000000FC)
#define WRP0_MSK     ((uint32_t)0x000000FF)
#define WRP1_MSK     ((uint32_t)0x0000FF00)

/* FLASH Keys */
#define L1_RDP_Key   ((uint32_t)0xFFFF00A5)
#define RDP_USER_Key ((uint32_t)0xFFF000A5)
#define L2_RDP_Key   ((uint32_t)0xFFFF33CC)
#define FLASH_KEY1   ((uint32_t)0x45670123)
#define FLASH_KEY2   ((uint32_t)0xCDEF89AB)

/* Delay definition */
#define EraseTimeout   ((uint32_t)0x000B0000)
#define ProgramTimeout ((uint32_t)0x00002000)
/**
 * @}
 */

/** @addtogroup FLASH_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup FLASH_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup FLASH_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup FLASH_Private_Functions
 * @{
 */

/**
 * @brief  Sets the code latency value.
 * @note   This function can be used for N32G030 devices.
 * @param FLASH_Latency specifies the FLASH Latency value.
 *   This parameter can be one of the following values:
 *     @arg FLASH_LATENCY_0 FLASH Zero Latency cycle
 *     @arg FLASH_LATENCY_1 FLASH One Latency cycle
 *     @arg FLASH_LATENCY_2 FLASH Two Latency cycles
 */
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
    uint32_t tmpregister = 0;

    /* Check the parameters */
    assert_param(IS_FLASH_LATENCY(FLASH_Latency));

    /* Read the ACR register */
    tmpregister = FLASH->AC;

    /* Sets the Latency value */
    tmpregister &= AC_LATENCY_MSK;
    tmpregister |= FLASH_Latency;

    /* Write the ACR register */
    FLASH->AC = tmpregister;
}

/**
 * @brief  Enables or disables the Prefetch Buffer.
 * @note   This function can be used for N32G030 devices.
 * @param FLASH_PrefetchBuf specifies the Prefetch buffer status.
 *   This parameter can be one of the following values:
 *     @arg FLASH_PrefetchBuf_EN FLASH Prefetch Buffer Enable
 *     @arg FLASH_PrefetchBuf_DIS FLASH Prefetch Buffer Disable
 */
void FLASH_PrefetchBufSet(uint32_t FLASH_PrefetchBuf)
{
    /* Check the parameters */
    assert_param(IS_FLASH_PREFETCHBUF_STATE(FLASH_PrefetchBuf));

    /* Enable or disable the Prefetch Buffer */
    FLASH->AC &= AC_PRFTBE_MSK;
    FLASH->AC |= FLASH_PrefetchBuf;
}

/**
 * @brief  Unlocks the FLASH Program Erase Controller.
 * @note   This function can be used for N32G030 devices.
 *         - For N32G030 devices this function unlocks Bank.
 *           to FLASH_Unlock function..
 */
void FLASH_Unlock(void)
{
    /* Unlocks the FLASH Program Erase Controller */
    FLASH->KEY = FLASH_KEY1;
    FLASH->KEY = FLASH_KEY2;
}

/**
 * @brief  Locks the FLASH Program Erase Controller.
 * @note   This function can be used for N32G030 devices.
 *         - For N32G030 devices this function Locks Bank.
 *           to FLASH_Lock function.
 */
void FLASH_Lock(void)
{
    /* Set the Lock Bit to lock the FLASH Program Erase Controller */
    FLASH->CTRL |= CTRL_Set_LOCK;
}

/**
 * @brief  Erases a specified FLASH page.
 * @note   This function can be used for N32G030 devices.
 * @param Page_Address The page address to be erased.
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_STS or FLASH_TIMEOUT.
 */
FLASH_STS FLASH_EraseOnePage(uint32_t Page_Address)
{
    FLASH_STS status = FLASH_ERR_STS;
    /* Check the parameters */
    assert_param(IS_FLASH_ADDRESS(Page_Address));

    /* Clears the FLASH's pending flags */
    FLASH_ClearFlag(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOpt(EraseTimeout);

    if (status == FLASH_COMPL)
    {
        /* if the previous operation is completed, proceed to erase the page */
        FLASH->CTRL |= CTRL_Set_PER;
        FLASH->ADD = Page_Address;
        FLASH->CTRL |= CTRL_Set_START;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOpt(EraseTimeout);

        /* Disable the PER Bit */
        FLASH->CTRL &= CTRL_Reset_PER;
    }

    /* Return the Erase Status */
    return status;
}

/**
 * @brief  Erases all FLASH pages.
 * @note   This function can be used for all N32G030 devices.
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_STS or FLASH_TIMEOUT.
 */
FLASH_STS FLASH_MassErase(void)
{
    FLASH_STS status = FLASH_ERR_STS;

    /* Clears the FLASH's pending flags */
    FLASH_ClearFlag(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOpt(EraseTimeout);

    if (status == FLASH_COMPL)
    {
        /* if the previous operation is completed, proceed to erase all pages */
        FLASH->CTRL |= CTRL_Set_MER;
        FLASH->CTRL |= CTRL_Set_START;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOpt(EraseTimeout);

        /* Disable the MER Bit */
        FLASH->CTRL &= CTRL_Reset_MER;
    }

    /* Return the Erase Status */
    return status;
}

/**
 * @brief  Erases the FLASH option bytes.
 * @note   This functions erases all option bytes except the Read protection (RDP).
 * @note   This function can be used for N32G030 devices.
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_STS, FLASH_ERR_RDP2 or FLASH_TIMEOUT.
 */
FLASH_STS FLASH_EraseOB(void)
{
    uint32_t rdptmp = L1_RDP_Key;

    FLASH_STS status = FLASH_ERR_STS;

    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_GetReadOutProtectionL2STS() != RESET)
    {
        status = FLASH_ERR_RDP2;
        return status;
    }

    /* Get the actual read protection Option Byte value */
    if (FLASH_GetReadOutProtectionSTS() != RESET)
    {
        rdptmp = (L1_RDP_Key & FLASH_USER_USER);
    }

    /* Clears the FLASH's pending flags */
    FLASH_ClearFlag(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOpt(EraseTimeout);

    if (status == FLASH_COMPL)
    {
        /* Authorize the small information block programming */
        FLASH->OPTKEY = FLASH_KEY1;
        FLASH->OPTKEY = FLASH_KEY2;

        /* if the previous operation is completed, proceed to erase the option bytes */
        FLASH->CTRL |= CTRL_Set_OPTER;
        FLASH->CTRL |= CTRL_Set_START;
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOpt(EraseTimeout);

        if (status == FLASH_COMPL)
        {
            /* Clears the FLASH's pending flags */
            FLASH_ClearFlag(FLASH_STS_CLRFLAG);
            
            /* if the erase operation is completed, disable the OPTER Bit */
            FLASH->CTRL &= CTRL_Reset_OPTER;

            /* Enable the Option Bytes Programming operation */
            FLASH->CTRL |= CTRL_Set_OPTPG;
            /* Restore the last read protection Option Byte value */
            OB->USER_RDP = (uint32_t)rdptmp;
            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOpt(ProgramTimeout);

            if (status != FLASH_TIMEOUT)
            {
                /* if the program operation is completed, disable the OPTPG Bit */
                FLASH->CTRL &= CTRL_Reset_OPTPG;
            }
        }
        else
        {
            if (status != FLASH_TIMEOUT)
            {
                /* Disable the OPTER Bit */
                FLASH->CTRL &= CTRL_Reset_OPTER;
            }
        }
    }
    /* Return the erase status */
    return status;
}


/**
 * @brief  Programs the FLASH User Option Byte: 
 *         RDP1 / IWDG_SW / RST_STOP / RST_PD / nBOOT0 / nBOOT1 / nSWBOOT0
 *         / OB_Data0 / OB_Data1 / WRP_Pages / RDP2.
 * @note   This function can be used for N32G030 devices.
 * @param  OB_RDP1 
 *   This parameter can be one of the following values:
 *     @arg OB_RDP1_ENABLE
 *     @arg OB_RDP1_DISABLE
 * @param  OB_IWDG Selects the IWDG mode
 *   This parameter can be one of the following values:
 *     @arg OB_IWDG_SW Software IWDG selected
 *     @arg OB_IWDG_HW Hardware IWDG selected
 * @param  OB_STOP Reset event when entering STOP2 mode.
 *   This parameter can be one of the following values:
 *     @arg OB_STOP_NORST No reset generated when entering in STOP2
 *     @arg OB_STOP_RST Reset generated when entering in STOP2
 * @param  OB_PD Reset event when entering PowerDown mode.
 *   This parameter can be one of the following values:
 *     @arg OB_PD_NORST No reset generated when entering in PowerDown
 *     @arg OB_PD_RST Reset generated when entering in PowerDown
 * @param  OB_nBOOT0 
 *   This parameter can be one of the following values:
 *     @arg OB_NBOOT0_SET Set nBOOT0
 *     @arg OB_NBOOT0_CLR Clear nBOOT0
 * @param  OB_nBOOT1 
 *   This parameter can be one of the following values:
 *     @arg OB_NBOOT1_SET Set nBOOT1
 *     @arg OB_NBOOT1_CLR Clear nBOOT1
 * @param  OB_nSWBOOT0 
 *   This parameter can be one of the following values:
 *     @arg OB_NSWBOOT0_SET Set nSWBOOT0
 *     @arg OB_NSWBOOT0_CLR Clear nSWBOOT0
 * @param  OB_Data0 
 *   This parameter can be one of the following values:
 *     @arg 0x00 ~ 0xFF
 * @param  OB_Data1 
 *   This parameter can be one of the following values:
 *     @arg 0x00 ~ 0xFF
 * @param WRP_Pages specifies the address of the pages to be write protected.
 *   This parameter can be:
 *     @arg For @b N32G030_devices: value between FLASH_WRPR_Pages0to1 and
 *       FLASH_WRPR_Pages62to63 or FLASH_WRPR_AllPages
 * @param  OB_RDP2 
 *   This parameter can be one of the following values:
 *     @arg OB_RDP2_ENABLE
 *     @arg OB_RDP2_DISABLE
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_STS, FLASH_ERR_RDP2 or FLASH_TIMEOUT.
 */
FLASH_STS FLASH_ConfigALLOptionByte(uint8_t OB_RDP1,     uint8_t OB_IWDG,    uint8_t OB_STOP, 
                                    uint8_t OB_PD,       uint8_t OB_nBOOT0,  uint8_t OB_nBOOT1,
                                    uint8_t OB_nSWBOOT0, uint8_t OB_Data0,   uint8_t OB_Data1,   
                                    uint32_t WRP_Pages,  uint8_t OB_RDP2)
{
    uint32_t rdpuser_tmp, data0data1_tmp, wrp0wrp1_tmp, rdp2_tmp;

    FLASH_STS status = FLASH_ERR_STS;

    /* Check the parameters */
    assert_param(IS_OB_RDP1_SOURCE(OB_RDP1));
    assert_param(IS_OB_IWDG_SOURCE(OB_IWDG));
    assert_param(IS_OB_STOP_SOURCE(OB_STOP));
    assert_param(IS_OB_PD_SOURCE(OB_PD));
    assert_param(IS_FLASH_WRPR_PAGE(WRP_Pages));
    assert_param(IS_OB_RDP2_SOURCE(OB_RDP2));
    assert_param(IS_OB_NBOOT0_SOURCE(OB_nBOOT0));
    assert_param(IS_OB_NBOOT1_SOURCE(OB_nBOOT1));
    assert_param(IS_OB_NSWBOOT0_SOURCE(OB_nSWBOOT0));

    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_GetReadOutProtectionL2STS() != RESET)
    {
        status = FLASH_ERR_RDP2;
        return status;
    }
    
    WRP_Pages      = (uint32_t)(~WRP_Pages);
    rdpuser_tmp    = (((uint32_t)OB_RDP1) | (((uint32_t)(OB_IWDG | OB_STOP | OB_PD | OB_nBOOT0 | OB_nBOOT1 | OB_nSWBOOT0)) << 16));
    data0data1_tmp = (((uint32_t)OB_Data0) | (((uint32_t)OB_Data0) << 16));
    wrp0wrp1_tmp   = ((WRP_Pages & FLASH_WRP0_WRP0) | ((WRP_Pages << 8) & FLASH_WRP1_WRP1));
    rdp2_tmp       = ((uint32_t)OB_RDP2);

    /* Authorize the small information block programming */
    FLASH->OPTKEY = FLASH_KEY1;
    FLASH->OPTKEY = FLASH_KEY2;

    /* Clears the FLASH's pending flags */
    FLASH_ClearFlag(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOpt(EraseTimeout);
    if (status == FLASH_COMPL)
    {
        /* if the previous operation is completed, proceed to erase the option bytes */
        FLASH->CTRL |= CTRL_Set_OPTER;
        FLASH->CTRL |= CTRL_Set_START;
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOpt(EraseTimeout);

        if (status == FLASH_COMPL)
        {
            /* Clears the FLASH's pending flags */
            FLASH_ClearFlag(FLASH_STS_CLRFLAG);
            
            /* if the erase operation is completed, disable the OPTER Bit */
            FLASH->CTRL &= CTRL_Reset_OPTER;

            /* Enable the Option Bytes Programming operation */
            FLASH->CTRL |= CTRL_Set_OPTPG;

            /* Program USER_RDP Option Byte value */
            OB->USER_RDP = (uint32_t)rdpuser_tmp;

            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOpt(ProgramTimeout);

            if (status == FLASH_COMPL)
            {
                /* Clears the FLASH's pending flags */
                FLASH_ClearFlag(FLASH_STS_CLRFLAG);
                
                /* Program Data1_Data0 Option Byte value */
                OB->Data1_Data0 = (uint32_t)data0data1_tmp;

                /* Wait for last operation to be completed */
                status = FLASH_WaitForLastOpt(ProgramTimeout);

                if (status == FLASH_COMPL)
                {
                    /* Clears the FLASH's pending flags */
                    FLASH_ClearFlag(FLASH_STS_CLRFLAG);
                    
                    /* Program WRP1_WRP0 Option Byte value */
                    OB->WRP1_WRP0 = (uint32_t)wrp0wrp1_tmp;

                    /* Wait for last operation to be completed */
                    status = FLASH_WaitForLastOpt(ProgramTimeout);

                    if (status == FLASH_COMPL)
                    {
                        /* Clears the FLASH's pending flags */
                        FLASH_ClearFlag(FLASH_STS_CLRFLAG);

                        /* Program USER2_RDP2 Option Byte value */
                        OB->RDP2 = (uint32_t)rdp2_tmp;

                        /* Wait for last operation to be completed */
                        status = FLASH_WaitForLastOpt(ProgramTimeout);
                    }
                }
            }

            if (status != FLASH_TIMEOUT)
            {
                /* if the program operation is completed, disable the OPTPG Bit */
                FLASH->CTRL &= CTRL_Reset_OPTPG;
            }
        }
        else
        {
            if (status != FLASH_TIMEOUT)
            {
                /* Disable the OPTER Bit */
                FLASH->CTRL &= CTRL_Reset_OPTER;
            }
        }
    }

    /* Return the Option Byte program Status */
    return status;
}

/**
 * @brief  Programs a word at a specified address.
 * @note   This function can be used for N32G030 devices.
 * @param Address specifies the address to be programmed.
 * @param Data specifies the data to be programmed.
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_STS or FLASH_TIMEOUT.
 */
FLASH_STS FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_STS status  = FLASH_ERR_STS;
    __IO uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_FLASH_ADDRESS(Address));

    /* Clears the FLASH's pending flags */
    FLASH_ClearFlag(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOpt(ProgramTimeout);

    if (status == FLASH_COMPL)
    {
        /* if the previous operation is completed, proceed to program the new word */
        FLASH->CTRL |= CTRL_Set_PG;

        *(__IO uint32_t*)Address = (uint32_t)Data;
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOpt(ProgramTimeout);

        /* Disable the PG Bit */
        FLASH->CTRL &= CTRL_Reset_PG;
    }

    /* Return the Program Status */
    return status;
}

/**
 * @brief  Programs a half word at a specified Option Byte Data address.
 * @note   This function can be used for N32G030 devices.
 * @param Address specifies the address to be programmed.
 *   This parameter can be 0x1FFFF604.
 * @param Data specifies the data to be programmed(Data0 and Data1).
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_STS, FLASH_ERR_RDP2 or FLASH_TIMEOUT.
 */
FLASH_STS FLASH_ProgramOBData(uint32_t Address, uint32_t Data)
{
    FLASH_STS status = FLASH_ERR_STS;
    /* Check the parameters */
    assert_param(IS_OB_DATA_ADDRESS(Address));

    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_GetReadOutProtectionL2STS() != RESET)
    {
        status = FLASH_ERR_RDP2;
        return status;
    }

    /* Clears the FLASH's pending flags */
    FLASH_ClearFlag(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOpt(ProgramTimeout);

    if (status == FLASH_COMPL)
    {
        /* Authorize the small information block programming */
        FLASH->OPTKEY = FLASH_KEY1;
        FLASH->OPTKEY = FLASH_KEY2;
        /* Enables the Option Bytes Programming operation */
        FLASH->CTRL |= CTRL_Set_OPTPG;
        *(__IO uint32_t*)Address = (uint32_t)Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOpt(ProgramTimeout);
        if (status != FLASH_TIMEOUT)
        {
            /* if the program operation is completed, disable the OPTPG Bit */
            FLASH->CTRL &= CTRL_Reset_OPTPG;
        }
    }
    /* Return the Option Byte Data Program Status */
    return status;
}

/**
 * @brief  Write protects the desired pages
 * @note   This function can be used for N32G030 devices.
 * @param FLASH_Pages specifies the address of the pages to be write protected.
 *   This parameter can be:
 *     @arg For @b N32G030_devices: value between FLASH_WRPR_Pages0to7 and
 *       FLASH_WRPR_Pages120to127
 *     @arg FLASH_WRPR_AllPages
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_STS, FLASH_ERR_RDP2 or FLASH_TIMEOUT.
 */
FLASH_STS FLASH_EnWriteProtection(uint32_t FLASH_Pages)
{
    uint16_t WRP0_Data = 0xFFFF, WRP1_Data = 0xFFFF;

    FLASH_STS status = FLASH_ERR_STS;

    /* Check the parameters */
    assert_param(IS_FLASH_WRPR_PAGE(FLASH_Pages));

    FLASH_Pages = (uint32_t)(~FLASH_Pages);
    WRP0_Data   = (uint16_t)(FLASH_Pages & WRP0_MSK);
    WRP1_Data   = (uint16_t)((FLASH_Pages & WRP1_MSK) >> 8);

    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_GetReadOutProtectionL2STS() != RESET)
    {
        status = FLASH_ERR_RDP2;
        return status;
    }

    /* Clears the FLASH's pending flags */
    FLASH_ClearFlag(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOpt(ProgramTimeout);

    if (status == FLASH_COMPL)
    {
        /* Authorizes the small information block programming */
        FLASH->OPTKEY = FLASH_KEY1;
        FLASH->OPTKEY = FLASH_KEY2;
        FLASH->CTRL |= CTRL_Set_OPTPG;

        if ((WRP0_Data != 0xFF) || (WRP1_Data != 0xFF))
        {
            OB->WRP1_WRP0 = (((uint32_t)WRP0_Data) | (((uint32_t)WRP1_Data) << 16));

            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOpt(ProgramTimeout);
        }

        if (status != FLASH_TIMEOUT)
        {
            /* if the program operation is completed, disable the OPTPG Bit */
            FLASH->CTRL &= CTRL_Reset_OPTPG;
        }
    }
    /* Return the write protection operation Status */
    return status;
}

/**
 * @brief  Enables or disables the read out protection.
 * @note   If the user has already programmed the other option bytes before calling
 *   this function, he must re-program them since this function erases all option bytes.
 * @note   This function can be used for N32G030 devices.
 * @param Cmd new state of the ReadOut Protection.
 *   This parameter can be: ENABLE or DISABLE.
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_STS, FLASH_ERR_RDP2 or FLASH_TIMEOUT.
 */
FLASH_STS FLASH_ReadOutProtectionL1(FunctionalState Cmd)
{
    uint32_t usertmp;
    FLASH_STS status = FLASH_ERR_STS;

    usertmp = ((OBR_USER_MSK & FLASH->OBR) << 0x0E);
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_GetReadOutProtectionL2STS() != RESET)
    {
        status = FLASH_ERR_RDP2;
        return status;
    }
    /* Clears the FLASH's pending flags */
    FLASH_ClearFlag(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOpt(EraseTimeout);
    
    if (status == FLASH_COMPL)
    {
        /* Authorizes the small information block programming */
        FLASH->OPTKEY = FLASH_KEY1;
        FLASH->OPTKEY = FLASH_KEY2;
        FLASH->CTRL |= CTRL_Set_OPTER;
        FLASH->CTRL |= CTRL_Set_START;
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOpt(EraseTimeout);
        if (status == FLASH_COMPL)
        {
            /* Clears the FLASH's pending flags */
            FLASH_ClearFlag(FLASH_STS_CLRFLAG);
            
            /* if the erase operation is completed, disable the OPTER Bit */
            FLASH->CTRL &= CTRL_Reset_OPTER;
            /* Enable the Option Bytes Programming operation */
            FLASH->CTRL |= CTRL_Set_OPTPG;
            if (Cmd != DISABLE)
            {
                OB->USER_RDP = (FLASH_USER_USER & usertmp);
            }
            else
            {
                OB->USER_RDP = ((L1_RDP_Key & FLASH_RDP_RDP1) | usertmp);
            }
            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOpt(EraseTimeout);

            if (status != FLASH_TIMEOUT)
            {
                /* if the program operation is completed, disable the OPTPG Bit */
                FLASH->CTRL &= CTRL_Reset_OPTPG;
            }
        }
        else
        {
            if (status != FLASH_TIMEOUT)
            {
                /* Disable the OPTER Bit */
                FLASH->CTRL &= CTRL_Reset_OPTER;
            }
        }
    }
    /* Return the protection operation Status */
    return status;
}

/**
 * @brief  Enables or disables the read out protection L2.
 * @note   If the user has already programmed the other option bytes before calling
 *   this function, he must re-program them since this function erases all option bytes.
 * @note   This function can be used for N32G030 devices.
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_RDP2, FLASH_ERR_STS or FLASH_TIMEOUT.
 */
FLASH_STS FLASH_ReadOutProtectionL2_ENABLE(void)
{
    uint32_t usertmp;
    FLASH_STS status = FLASH_ERR_STS;

    usertmp = ((OBR_USER_MSK & FLASH->OBR) << 0x0E);
  
    /* Get the actual read protection L1 Option Byte value */
    if (FLASH_GetReadOutProtectionSTS() == RESET)
    {
        usertmp |= (L1_RDP_Key & FLASH_RDP_RDP1);
    }
    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_GetReadOutProtectionL2STS() != RESET)
    {
        status = FLASH_ERR_RDP2;
        return status;
    }

    /* Clears the FLASH's pending flags */
    FLASH_ClearFlag(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOpt(EraseTimeout);
    
    if (status == FLASH_COMPL)
    {
        /* Authorizes the small information block programming */
        FLASH->OPTKEY = FLASH_KEY1;
        FLASH->OPTKEY = FLASH_KEY2;
        FLASH->CTRL |= CTRL_Set_OPTER;
        FLASH->CTRL |= CTRL_Set_START;
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOpt(EraseTimeout);
        if (status == FLASH_COMPL)
        {
            /* Clears the FLASH's pending flags */
            FLASH_ClearFlag(FLASH_STS_CLRFLAG);
            
            /* if the erase operation is completed, disable the OPTER Bit */
            FLASH->CTRL &= CTRL_Reset_OPTER;
            /* Enable the Option Bytes Programming operation */
            FLASH->CTRL |= CTRL_Set_OPTPG;

            OB->USER_RDP = usertmp;
            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOpt(EraseTimeout);
            
            if (status == FLASH_COMPL)
            {
                /* Clears the FLASH's pending flags */
                FLASH_ClearFlag(FLASH_STS_CLRFLAG);
                
                /* Enables the read out protection L2 */
                OB->RDP2 = L2_RDP_Key;
                
                /* Wait for last operation to be completed */
                status = FLASH_WaitForLastOpt(EraseTimeout);
            }
            
            if (status != FLASH_TIMEOUT)
            {
                /* if the program operation is completed, disable the OPTPG Bit */
                FLASH->CTRL &= CTRL_Reset_OPTPG;
            }
        }
        else
        {
            if (status != FLASH_TIMEOUT)
            {
                /* Disable the OPTER Bit */
                FLASH->CTRL &= CTRL_Reset_OPTER;
            }
        }
    }
    /* Return the protection operation Status */
    return status;
}

/**
 * @brief  Programs the FLASH User Option Byte: IWDG_SW / RST_STOP / RST_STDBY.
 * @note   This function can be used for N32G030 devices.
 * @param OB_IWDG Selects the IWDG mode
 *   This parameter can be one of the following values:
 *     @arg OB_IWDG_SW Software IWDG selected
 *     @arg OB_IWDG_HW Hardware IWDG selected
 * @param OB_STOP Reset event when entering STOP mode.
 *   This parameter can be one of the following values:
 *     @arg OB_STOP_NORST No reset generated when entering in STOP
 *     @arg OB_STOP_RST Reset generated when entering in STOP
 * @param OB_PD Reset event when entering PowerDown mode.
 *   This parameter can be one of the following values:
 *     @arg OB_PD_NORST No reset generated when entering in PowerDown
 *     @arg OB_PD_RST Reset generated when entering in PowerDown
 * @param  OB_nBOOT0 
 *   This parameter can be one of the following values:
 *     @arg OB_NBOOT0_SET Set nBOOT0
 *     @arg OB_NBOOT0_CLR Clear nBOOT0
 * @param  OB_nBOOT1 
 *   This parameter can be one of the following values:
 *     @arg OB_NBOOT1_SET Set nBOOT1
 *     @arg OB_NBOOT1_CLR Clear nBOOT1
 * @param  OB_nSWBOOT0 
 *   This parameter can be one of the following values:
 *     @arg OB_NSWBOOT0_SET Set nSWBOOT0
 *     @arg OB_NSWBOOT0_CLR Clear nSWBOOT0
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_RDP2, FLASH_ERR_STS or FLASH_TIMEOUT.
 */
FLASH_STS FLASH_ConfigUserOB(uint8_t OB_IWDG,   uint8_t OB_STOP,   uint8_t OB_PD,
                             uint8_t OB_nBOOT0, uint8_t OB_nBOOT1, uint8_t OB_nSWBOOT0)
{
    uint32_t rdpuser_tmp = RDP_USER_Key;

    FLASH_STS status = FLASH_ERR_STS;

    /* Check the parameters */
    assert_param(IS_OB_IWDG_SOURCE(OB_IWDG));
    assert_param(IS_OB_STOP_SOURCE(OB_STOP));
    assert_param(IS_OB_PD_SOURCE(OB_PD));
    assert_param(IS_OB_NBOOT0_SOURCE(OB_nBOOT0));
    assert_param(IS_OB_NBOOT1_SOURCE(OB_nBOOT1));
    assert_param(IS_OB_NSWBOOT0_SOURCE(OB_nSWBOOT0));
  
    /* Get the actual read protection L2 Option Byte value */
    if (FLASH_GetReadOutProtectionL2STS() != RESET)
    {
        status = FLASH_ERR_RDP2;
        return status;
    }

    /* Get the actual read protection Option Byte value */
    if (FLASH_GetReadOutProtectionSTS() != RESET)
    {
        rdpuser_tmp = 0xFFF00000;
    }

    /* Authorize the small information block programming */
    FLASH->OPTKEY = FLASH_KEY1;
    FLASH->OPTKEY = FLASH_KEY2;

    /* Clears the FLASH's pending flags */
    FLASH_ClearFlag(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOpt(EraseTimeout);
    
    if (status == FLASH_COMPL)
    {
        /* if the previous operation is completed, proceed to erase the option bytes */
        FLASH->CTRL |= CTRL_Set_OPTER;
        FLASH->CTRL |= CTRL_Set_START;
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOpt(EraseTimeout);

        if (status == FLASH_COMPL)
        {
            /* Clears the FLASH's pending flags */
            FLASH_ClearFlag(FLASH_STS_CLRFLAG);
            
            /* if the erase operation is completed, disable the OPTER Bit */
            FLASH->CTRL &= CTRL_Reset_OPTER;

            /* Enable the Option Bytes Programming operation */
            FLASH->CTRL |= CTRL_Set_OPTPG;
            /* Restore the last read protection Option Byte value */
            OB->USER_RDP =
                (uint32_t)rdpuser_tmp
                | (((uint32_t)(OB_IWDG | OB_STOP | OB_PD | OB_nBOOT0 | OB_nBOOT1 | OB_nSWBOOT0)) << 16);
            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOpt(ProgramTimeout);

            if (status != FLASH_TIMEOUT)
            {
                /* if the program operation is completed, disable the OPTPG Bit */
                FLASH->CTRL &= CTRL_Reset_OPTPG;
            }
        }
        else
        {
            if (status != FLASH_TIMEOUT)
            {
                /* Disable the OPTER Bit */
                FLASH->CTRL &= CTRL_Reset_OPTER;
            }
        }
    }

    /* Return the Option Byte program Status */
    return status;
}

/**
 * @brief  Returns the FLASH User Option Bytes values.
 * @note   This function can be used for N32G030 devices.
 * @return The FLASH User Option Bytes values:IWDG_SW(Bit0), RST_STOP(Bit1)
 *         RST_PD(Bit2), nBOOT0(Bit3), nBOOT1(Bit4) and nSWBOOT0(Bit5).
 */
uint32_t FLASH_GetUserOB(void)
{
    /* Return the User Option Byte */
    return (uint32_t)((FLASH->OBR & OBR_USER_MSK) >> 2);
}

/**
 * @brief  Returns the FLASH Write Protection Option Bytes Register value.
 * @note   This function can be used for N32G030 devices.
 * @return The FLASH Write Protection  Option Bytes Register value
 */
uint32_t FLASH_GetWriteProtectionOB(void)
{
    /* Return the Flash write protection Register value */
    return (uint32_t)(FLASH->WRP);
}

/**
 * @brief  Checks whether the FLASH Read Out Protection Status is set or not.
 * @note   This function can be used for N32G030 devices.
 * @return FLASH ReadOut Protection Status(SET or RESET)
 */
FlagStatus FLASH_GetReadOutProtectionSTS(void)
{
    FlagStatus readoutstatus = RESET;
    if ((FLASH->OBR & RDPRTL1_MSK) != (uint32_t)RESET)
    {
        readoutstatus = SET;
    }
    else
    {
        readoutstatus = RESET;
    }
    return readoutstatus;
}

/**
 * @brief  Checks whether the FLASH Read Out Protection L2 Status is set or not.
 * @note   This function can be used for N32G030 devices.
 * @return FLASH ReadOut Protection L2 Status(SET or RESET)
 */
FlagStatus FLASH_GetReadOutProtectionL2STS(void)
{
    FlagStatus readoutstatus = RESET;
    if ((FLASH->OBR & RDPRTL2_MSK) != (uint32_t)RESET)
    {
        readoutstatus = SET;
    }
    else
    {
        readoutstatus = RESET;
    }
    return readoutstatus;
}

/**
 * @brief  Checks whether the FLASH Prefetch Buffer status is set or not.
 * @note   This function can be used for N32G030 devices.
 * @return FLASH Prefetch Buffer Status (SET or RESET).
 */
FlagStatus FLASH_GetPrefetchBufSTS(void)
{
    FlagStatus bitstatus = RESET;

    if ((FLASH->AC & AC_PRFTBS_MSK) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    /* Return the new state of FLASH Prefetch Buffer Status (SET or RESET) */
    return bitstatus;
}

/**
 * @brief  Enables or disables the specified FLASH interrupts.
 * @note   This function can be used for N32G030 devices.
 * @param FLASH_INT specifies the FLASH interrupt sources to be enabled or disabled.
 *   This parameter can be any combination of the following values:
 *     @arg FLASH_IT_ERROR   FLASH Error Interrupt
 *     @arg FLASH_INT_EOP    FLASH end of operation Interrupt
 * @param Cmd new state of the specified Flash interrupts.
 *   This parameter can be: ENABLE or DISABLE.
 */
void FLASH_INTConfig(uint32_t FLASH_INT, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_FLASH_INT(FLASH_INT));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if (Cmd != DISABLE)
    {
        /* Enable the interrupt sources */
        FLASH->CTRL |= FLASH_INT;
    }
    else
    {
        /* Disable the interrupt sources */
        FLASH->CTRL &= ~(uint32_t)FLASH_INT;
    }
}

/**
 * @brief  Checks whether the specified FLASH flag is set or not.
 * @note   This function can be used for N32G030 devices.
 * @param FLASH_FLAG specifies the FLASH flag to check.
 *   This parameter can be one of the following values:
 *     @arg FLASH_FLAG_BUSY FLASH Busy flag
 *     @arg FLASH_FLAG_PGERR FLASH Program error flag
 *     @arg FLASH_FLAG_WRPERR FLASH Write protected error flag
 *     @arg FLASH_FLAG_EOP FLASH End of Operation flag
 *     @arg FLASH_FLAG_OBERR FLASH Option Byte error flag
 * @return The new state of FLASH_FLAG (SET or RESET).
 */
FlagStatus FLASH_GetFlagSTS(uint32_t FLASH_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG));
    if (FLASH_FLAG == FLASH_FLAG_OBERR)
    {
        if ((FLASH->OBR & FLASH_FLAG_OBERR) != (uint32_t)RESET)
        {
            bitstatus = SET;
        }
        else
        {
            bitstatus = RESET;
        }
    }
    else
    {
        if ((FLASH->STS & FLASH_FLAG) != (uint32_t)RESET)
        {
            bitstatus = SET;
        }
        else
        {
            bitstatus = RESET;
        }
    }

    /* Return the new state of FLASH_FLAG (SET or RESET) */
    return bitstatus;
}

/**
 * @brief  Clears the FLASH's pending flags.
 * @note   This function can be used for N32G030 devices.
 * @param FLASH_FLAG specifies the FLASH flags to clear.
 *   This parameter can be any combination of the following values:
 *     @arg FLASH_FLAG_PGERR FLASH Program error flag
 *     @arg FLASH_FLAG_WRPERR FLASH Write protected error flag
 *     @arg FLASH_FLAG_EOP FLASH End of Operation flag
 */
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
    /* Check the parameters */
    assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG));

    /* Clear the flags */
    FLASH->STS |= FLASH_FLAG;
}

/**
 * @brief  Returns the FLASH Status.
 * @note   This function can be used for N32G030 devices, it is equivalent
 *         to FLASH_GetBank1Status function.
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_STS or FLASH_TIMEOUT.
 */
FLASH_STS FLASH_GetSTS(void)
{
    FLASH_STS flashstatus = FLASH_ERR_STS;

    if ((FLASH->STS & FLASH_FLAG_BUSY) == FLASH_FLAG_BUSY)
    {
        flashstatus = FLASH_BUSY;
    }
    else
    {
        if ((FLASH->STS & FLASH_FLAG_PGERR) != 0)
        {
            flashstatus = FLASH_ERR_PG;
        }
        else
        {
            if ((FLASH->STS & FLASH_FLAG_WRPERR) != 0)
            {
                flashstatus = FLASH_ERR_WRP;
            }
            else
            {
                flashstatus = FLASH_COMPL;
            }
        }
    }

    /* Return the Flash Status */
    return flashstatus;
}

/**
 * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
 * @note   This function can be used for N32G030 devices,
 *         it is equivalent to FLASH_WaitForLastBank1Operation..
 * @param Timeout FLASH programming Timeout
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_STS or FLASH_TIMEOUT.
 */
FLASH_STS FLASH_WaitForLastOpt(uint32_t Timeout)
{
    FLASH_STS status = FLASH_ERR_STS;

    /* Check for the Flash Status */
    status = FLASH_GetSTS();
    /* Wait for a Flash operation to complete or a TIMEOUT to occur */
    while ((status == FLASH_BUSY) && (Timeout != 0x00))
    {
        status = FLASH_GetSTS();
        Timeout--;
    }
    if (Timeout == 0x00)
    {
        status = FLASH_TIMEOUT;
    }
    /* Return the operation status */
    return status;
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
