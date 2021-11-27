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
 * @file n32g030_flash.h
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __N32G030_FLASH_H__
#define __N32G030_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g030.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup FLASH
 * @{
 */

/** @addtogroup FLASH_Exported_Types
 * @{
 */

/**
 * @brief  FLASH Status
 */

typedef enum
{
    FLASH_BUSY = 1,
    FLASH_ERR_PG,
    FLASH_ERR_WRP,
    FLASH_COMPL,
    FLASH_ERR_RDP2,
    FLASH_TIMEOUT,
    FLASH_ERR_STS
} FLASH_STS;

/**
 * @brief  FLASH_SMPSEL
 */

typedef enum
{
    FLASH_SMP1 = 0,
    FLASH_SMP2
} FLASH_SMPSEL;

/**
 * @brief  FLASH_HSICLOCK
 */

typedef enum
{
    FLASH_HSICLOCK_ENABLE = 0,
    FLASH_HSICLOCK_DISABLE
} FLASH_HSICLOCK;

/**
 * @}
 */

/** @addtogroup FLASH_Exported_Constants
 * @{
 */

/** @addtogroup Flash_Latency
 * @{
 */

#define FLASH_LATENCY_0 ((uint32_t)0x00000000) /*!< FLASH Zero Latency cycle */
#define FLASH_LATENCY_1 ((uint32_t)0x00000001) /*!< FLASH One Latency cycle */
#define FLASH_LATENCY_2 ((uint32_t)0x00000002) /*!< FLASH Two Latency cycles */
#define IS_FLASH_LATENCY(LATENCY)                                                                                      \
    (((LATENCY) == FLASH_LATENCY_0) || ((LATENCY) == FLASH_LATENCY_1) || ((LATENCY) == FLASH_LATENCY_2))
/**
 * @}
 */

/** @addtogroup Prefetch_Buffer_Enable_Disable
 * @{
 */

#define FLASH_PrefetchBuf_EN              ((uint32_t)0x00000010) /*!< FLASH Prefetch Buffer Enable */
#define FLASH_PrefetchBuf_DIS             ((uint32_t)0x00000000) /*!< FLASH Prefetch Buffer Disable */
#define IS_FLASH_PREFETCHBUF_STATE(STATE) (((STATE) == FLASH_PrefetchBuf_EN) || ((STATE) == FLASH_PrefetchBuf_DIS))
/**
 * @}
 */

/** @addtogroup iCache_Enable_Disable
 * @{
 */

#define FLASH_iCache_EN              ((uint32_t)0x00000080) /*!< FLASH iCache Enable */
#define FLASH_iCache_DIS             ((uint32_t)0x00000000) /*!< FLASH iCache Disable */
#define IS_FLASH_ICACHE_STATE(STATE) (((STATE) == FLASH_iCache_EN) || ((STATE) == FLASH_iCache_DIS))
/**
 * @}
 */

/** @addtogroup Low Voltage Mode
 * @{
 */

#define FLASH_LVM_EN              ((uint32_t)0x00000200) /*!< FLASH Low Voltage Mode Enable */
#define FLASH_LVM_DIS             ((uint32_t)0x00000000) /*!< FLASH Low Voltage Mode Disable */
#define IS_FLASH_LVM(STATE)       (((STATE) == FLASH_LVM_EN) || ((STATE) == FLASH_LVM_DIS))
/**
 * @}
 */

/** @addtogroup FLASH Sleep Mode
 * @{
 */

#define FLASH_SLM_EN              ((uint32_t)0x00000800) /*!< FLASH Sleep Mode Enable */
#define FLASH_SLM_DIS             ((uint32_t)0x00000000) /*!< FLASH Sleep Mode Disable */
#define IS_FLASH_SLM(STATE)       (((STATE) == FLASH_SLM_EN) || ((STATE) == FLASH_SLM_DIS))
/**
 * @}
 */

/** @addtogroup SMPSEL_SMP1_SMP2
 * @{
 */

#define FLASH_SMPSEL_SMP1            ((uint32_t)0x00000000) /*!< FLASH SMPSEL SMP1 */
#define FLASH_SMPSEL_SMP2            ((uint32_t)0x00000100) /*!< FLASH SMPSEL SMP2 */
#define IS_FLASH_SMPSEL_STATE(STATE) (((STATE) == FLASH_SMPSEL_SMP1) || ((STATE) == FLASH_SMPSEL_SMP2))
/**
 * @}
 */

/* Values to be used with N32G030 devices */
#define FLASH_WRPR_Pages0to7                                                                                           \
    ((uint32_t)0x00000001) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 0 to 7 */
#define FLASH_WRPR_Pages8to15                                                                                          \
    ((uint32_t)0x00000002) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 8 to 15 */
#define FLASH_WRPR_Pages16to23                                                                                         \
    ((uint32_t)0x00000004) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 16 to 23 */
#define FLASH_WRPR_Pages24to31                                                                                         \
    ((uint32_t)0x00000008) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 24 to 31 */
#define FLASH_WRPR_Pages32to39                                                                                         \
    ((uint32_t)0x00000010) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 32 to 39 */
#define FLASH_WRPR_Pages40to47                                                                                         \
    ((uint32_t)0x00000020) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 40 to 47 */
#define FLASH_WRPR_Pages48to55                                                                                         \
    ((uint32_t)0x00000040) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 48 to 55 */
#define FLASH_WRPR_Pages56to63                                                                                         \
    ((uint32_t)0x00000080) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 56 to 63 */
#define FLASH_WRPR_Pages64to71                                                                                         \
    ((uint32_t)0x00000100) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 64 to 71 */
#define FLASH_WRPR_Pages72to79                                                                                         \
    ((uint32_t)0x00000200) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 72 to 79 */
#define FLASH_WRPR_Pages80to87                                                                                         \
    ((uint32_t)0x00000400) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 80 to 87 */
#define FLASH_WRPR_Pages88to95                                                                                         \
    ((uint32_t)0x00000800) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 88 to 95 */
#define FLASH_WRPR_Pages96to103                                                                                        \
    ((uint32_t)0x00001000) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 96 to 103 */
#define FLASH_WRPR_Pages104to111                                                                                       \
    ((uint32_t)0x00002000) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 104 to 111 */
#define FLASH_WRPR_Pages112to119                                                                                       \
    ((uint32_t)0x00004000) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 112 to 119 */
#define FLASH_WRPR_Pages120to127                                                                                       \
    ((uint32_t)0x00008000) /*!< N32G030 devices:                                                                       \
                                  Write protection of page 120 to 127 */

#define FLASH_WRPR_AllPages ((uint32_t)0x0000FFFF) /*!< Write protection of all Pages */

#define IS_FLASH_WRPR_PAGE(PAGE) (((PAGE) <= FLASH_WRPR_AllPages))

#define IS_FLASH_ADDRESS(ADDRESS) (((ADDRESS) >= 0x08000000) && ((ADDRESS) < 0x0800FFFF))

#define IS_OB_DATA_ADDRESS(ADDRESS) ((ADDRESS) == 0x1FFFF604)

/**
 * @}
 */

/** @addtogroup Option_Bytes_RDP1
 * @{
 */

#define OB_RDP1_ENABLE            ((uint8_t)0x00) /*!< Enable RDP1 */
#define OB_RDP1_DISABLE           ((uint8_t)0xA5) /*!< DISABLE RDP1 */
#define IS_OB_RDP1_SOURCE(SOURCE) (((SOURCE) == OB_RDP1_ENABLE) || ((SOURCE) == OB_RDP1_DISABLE))

/**
 * @}
 */

/** @addtogroup Option_Bytes_IWatchdog
 * @{
 */

#define OB_IWDG_SW                ((uint8_t)0x01) /*!< Software IWDG selected */
#define OB_IWDG_HW                ((uint8_t)0x00) /*!< Hardware IWDG selected */
#define IS_OB_IWDG_SOURCE(SOURCE) (((SOURCE) == OB_IWDG_SW) || ((SOURCE) == OB_IWDG_HW))

/**
 * @}
 */

/** @addtogroup Option_Bytes_nRST_STOP
 * @{
 */

#define OB_STOP_NORST             ((uint8_t)0x02) /*!< No reset generated when entering in STOP */
#define OB_STOP_RST               ((uint8_t)0x00) /*!< Reset generated when entering in STOP */
#define IS_OB_STOP_SOURCE(SOURCE) (((SOURCE) == OB_STOP_NORST) || ((SOURCE) == OB_STOP_RST))

/**
 * @}
 */

/** @addtogroup Option_Bytes_nRST_PD
 * @{
 */

#define OB_PD_NORST                ((uint8_t)0x04) /*!< No reset generated when entering in PowerDown */
#define OB_PD_RST                  ((uint8_t)0x00) /*!< Reset generated when entering in PowerDown */
#define IS_OB_PD_SOURCE(SOURCE)    (((SOURCE) == OB_PD_NORST) || ((SOURCE) == OB_PD_RST))

/**
 * @}
 */

/** @addtogroup Option_Bytes_RDP2
 * @{
 */

#define OB_RDP2_ENABLE            ((uint8_t)0x33) /*!< Enable RDP2 */
#define OB_RDP2_DISABLE           ((uint8_t)0x00) /*!< Disable RDP2 */
#define IS_OB_RDP2_SOURCE(SOURCE) (((SOURCE) == OB_RDP2_ENABLE) || ((SOURCE) == OB_RDP2_DISABLE))

/**
 * @}
 */

/** @addtogroup Option_Bytes_nBOOT0
 * @{
 */

#define OB_NBOOT0_SET                ((uint8_t)0x08) /*!< Set nBOOT0 */
#define OB_NBOOT0_CLR                ((uint8_t)0x00) /*!< Clear nBOOT0 */
#define IS_OB_NBOOT0_SOURCE(SOURCE)  (((SOURCE) == OB_NBOOT0_SET) || ((SOURCE) == OB_NBOOT0_CLR))

/**
 * @}
 */

/** @addtogroup Option_Bytes_nBOOT1
 * @{
 */

#define OB_NBOOT1_SET                ((uint8_t)0x10) /*!< Set nBOOT1 */
#define OB_NBOOT1_CLR                ((uint8_t)0x00) /*!< Clear nBOOT1 */
#define IS_OB_NBOOT1_SOURCE(SOURCE)  (((SOURCE) == OB_NBOOT1_SET) || ((SOURCE) == OB_NBOOT1_CLR))

/**
 * @}
 */

/** @addtogroup Option_Bytes_nSWBOOT0
 * @{
 */

#define OB_NSWBOOT0_SET                ((uint8_t)0x20) /*!< Set nSWBOOT0 */
#define OB_NSWBOOT0_CLR                ((uint8_t)0x00) /*!< Clear nSWBOOT0 */
#define IS_OB_NSWBOOT0_SOURCE(SOURCE)  (((SOURCE) == OB_NSWBOOT0_SET) || ((SOURCE) == OB_NSWBOOT0_CLR))

/**
 * @}
 */
/** @addtogroup FLASH_Interrupts
 * @{
 */
#define FLASH_INT_ERR      ((uint32_t)0x00000400) /*!< PGERR WRPRTERR ERROR error interrupt source */
#define FLASH_INT_EOP      ((uint32_t)0x00001000) /*!< End of FLASH Operation Interrupt source */

#define IS_FLASH_INT(IT) ((((IT) & (uint32_t)0xFFFFEBFF) == 0x00000000) && (((IT) != 0x00000000)))

/**
 * @}
 */

/** @addtogroup FLASH_Flags
 * @{
 */
#define FLASH_FLAG_BUSY     ((uint32_t)0x00000001) /*!< FLASH Busy flag */
#define FLASH_FLAG_PGERR    ((uint32_t)0x00000004) /*!< FLASH Program error flag */
#define FLASH_FLAG_WRPERR   ((uint32_t)0x00000010) /*!< FLASH Write protected error flag */
#define FLASH_FLAG_EOP      ((uint32_t)0x00000020) /*!< FLASH End of Operation flag */
#define FLASH_FLAG_OBERR    ((uint32_t)0x00000001) /*!< FLASH Option Byte error flag */

#define IS_FLASH_CLEAR_FLAG(FLAG) (((FLAG & ((uint32_t)0x00000034)) != 0) && ((FLAG & ((uint32_t)0xFFFFFFCB)) == 0))

#define IS_FLASH_GET_FLAG(FLAG)                                                                                      \
    (((FLAG) == FLASH_FLAG_BUSY) || ((FLAG) == FLASH_FLAG_PGERR) || ((FLAG) == FLASH_FLAG_WRPERR)                    \
  || ((FLAG) == FLASH_FLAG_EOP)  || ((FLAG) == FLASH_FLAG_OBERR))

/**
 * @}
 */

/** @addtogroup FLASH_STS_CLRFLAG
 * @{
 */
#define FLASH_STS_CLRFLAG   (FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR | FLASH_FLAG_EOP)

/**
 * @}
 */

/** @addtogroup FLASH_Exported_Functions
 * @{
 */

/*------------ Functions used for N32G030 devices -----*/
void FLASH_SetLatency(uint32_t FLASH_Latency);
void FLASH_PrefetchBufSet(uint32_t FLASH_PrefetchBuf);
void FLASH_Unlock(void);
void FLASH_Lock(void);
FLASH_STS FLASH_EraseOnePage(uint32_t Page_Address);
FLASH_STS FLASH_MassErase(void);
FLASH_STS FLASH_EraseOB(void);
FLASH_STS FLASH_ProgramWord(uint32_t Address, uint32_t Data);
FLASH_STS FLASH_ProgramOBData(uint32_t Address, uint32_t Data);
FLASH_STS FLASH_EnWriteProtection(uint32_t FLASH_Pages);
FLASH_STS FLASH_ReadOutProtectionL1(FunctionalState Cmd);
FLASH_STS FLASH_ReadOutProtectionL2_ENABLE(void);
FLASH_STS FLASH_ConfigALLOptionByte(uint8_t OB_RDP1,     uint8_t OB_IWDG,    uint8_t OB_STOP, 
                                    uint8_t OB_PD,       uint8_t OB_nBOOT0,  uint8_t OB_nBOOT1,
                                    uint8_t OB_nSWBOOT0, uint8_t OB_Data0,   uint8_t OB_Data1,   
                                    uint32_t WRP_Pages,  uint8_t OB_RDP2);
FLASH_STS FLASH_ConfigUserOB(uint8_t OB_IWDG,   uint8_t OB_STOP,   uint8_t OB_PD,
                             uint8_t OB_nBOOT0, uint8_t OB_nBOOT1, uint8_t OB_nSWBOOT0);
uint32_t FLASH_GetUserOB(void);
uint32_t FLASH_GetWriteProtectionOB(void);
FlagStatus FLASH_GetReadOutProtectionSTS(void);
FlagStatus FLASH_GetReadOutProtectionL2STS(void);
FlagStatus FLASH_GetPrefetchBufSTS(void);
FLASH_SMPSEL FLASH_GetSMPSELStatus(void);
void FLASH_INTConfig(uint32_t FLASH_INT, FunctionalState Cmd);
FlagStatus FLASH_GetFlagSTS(uint32_t FLASH_FLAG);
void FLASH_ClearFlag(uint32_t FLASH_FLAG);
FLASH_STS FLASH_GetSTS(void);
FLASH_STS FLASH_WaitForLastOpt(uint32_t Timeout);

#ifdef __cplusplus
}
#endif

#endif /* __N32G030_FLASH_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
