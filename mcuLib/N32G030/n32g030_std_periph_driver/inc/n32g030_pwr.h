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
 * @file n32g030_pwr.h
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __N32G030_PWR_H__
#define __N32G030_PWR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g030.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */



#define PWR_PVDLEVEL_1V8     0
#define PWR_PVDLEVEL_2V      1
#define PWR_PVDLEVEL_2V2     2
#define PWR_PVDLEVEL_2V4     3
#define PWR_PVDLEVEL_5V      15

#define IS_PWR_PVD_LEVEL(LEVEL)                                                                          \
       (((LEVEL) <= PWR_PVDLEVEL_5V) )

/**
 * @}
 */

/** @addtogroup Regulator_state_is_LPRUN_mode
 * @{
 */

#define PWR_LPRUN_EFLASH_ACTIVE          ((uint32_t)0x00020)
#define PWR_LPRUN_EFLASH_DEEEPSTANDBY      ((uint32_t)0x00000)
#define IS_PWR_LPRUNeFlash(eFlash) (((eFlash) == PWR_LPRUN_EFLASH_ACTIVE) || \
                                     ((eFlash) == PWR_LPRUN_EFLASH_DEEEPSTANDBY))
/**
  * @}
  */



/** @defgroup Regulator_state_is_STOP_mode 
  * @{
  */

#define PWR_STOPPLUSE_ENABLE          ((uint32_t)0x00000400)
#define PWR_STOPPLUSE_DISABLE         ((uint32_t)0x00000000)
#define IS_STOPPLUSE(pluse) (((pluse) == PWR_STOPPLUSE_ENABLE) || \
                                     ((pluse) == PWR_STOPPLUSE_DISABLE))
/**
 * @}
 */

/** @defgroup SLEEP_mode_entry 
  * @{
  */
#define SLEEP_ON_EXIT              (1)
#define SLEEP_OFF_EXIT             (0)
#define PWR_SLEEPENTRY_WFI         ((uint8_t)0x01)
#define PWR_SLEEPENTRY_WFE         ((uint8_t)0x02)
#define IS_PWR_SLEEP_ENTRY(ENTRY) (((ENTRY) == PWR_SLEEPENTRY_WFI) || ((ENTRY) == PWR_SLEEPENTRY_WFE))
 
 
/**
  * @}
  */


/** @defgroup STOP_mode_entry 
  * @{
  */

#define PWR_STOPENTRY_WFI        ((uint8_t)0x01)
#define PWR_STOPENTRY_WFE        ((uint8_t)0x02)
#define IS_PWR_STOP_ENTRY(ENTRY) (((ENTRY) == PWR_STOPENTRY_WFI) || ((ENTRY) == PWR_STOPENTRY_WFE))

/** @defgroup Powerdown_mode_entry 
  * @{
  */

#define PWR_PDEntry_WFI         ((uint8_t)0x01)
#define PWR_PDEntry_WFE         ((uint8_t)0x02)
#define IS_PWR_PD_ENTRY(ENTRY) (((ENTRY) == PWR_PDEntry_WFI) || ((ENTRY) == PWR_PDEntry_WFE))
 
/**
 * @}
 */

/** @addtogroup PWR_Flag
 * @{
 */

#define PWR_DBGPD_WKUP_FLAG    ((uint32_t)0x00000001)
#define PWR_DBGPD_FLAG         ((uint32_t)0x00000002)
#define PWR_PVDO_FLAG          ((uint32_t)0x00000004)

#define IS_PWR_GET_FLAG(FLAG) \
    (((FLAG) == PWR_DBGPD_WKUP_FLAG) || ((FLAG) == PWR_DBGPD_FLAG)|| ((FLAG) == PWR_PVDO_FLAG) )

#define PWR_CLR_WKUP_FLAG    ((uint32_t)0x00000004)
#define PWR_CLR_DBGPD_FLAG         ((uint32_t)0x00000008)

#define IS_PWR_CLEAR_FLAG(FLAG) \
    (((FLAG) == PWR_CLR_WKUP_FLAG) || ((FLAG) == PWR_CLR_DBGPD_FLAG))

/** @defgroup Clock_source select before entring lprun mode 
  * @{
  */

#define LSI                  (0x0)
#define LSE                  (0x1)
#define IS_PWR_LPRUNCLK(clk) (((clk) == LSI) || \
                                     ((clk) == LSE))

/** @addtogroup DBGMCU_Exported_Constants
 * @{
 */


#define DBG_STOP              ((uint32_t)0x00000002)
#define DBG_PD                ((uint32_t)0x00000004)
#define DBG_IWDG_STOP         ((uint32_t)0x00000100)
#define DBG_WWDG_STOP         ((uint32_t)0x00000200)
#define DBG_TIM1_STOP         ((uint32_t)0x00000400)

#define DBG_TIM3_STOP         ((uint32_t)0x00001000)
#define DBG_TIM4_STOP         ((uint32_t)0x00002000)
#define DBG_CAN_STOP          ((uint32_t)0x00004000)
#define DBG_I2C1SMBUS_TIMEOUT ((uint32_t)0x00008000)
#define DBG_I2C2SMBUS_TIMEOUT ((uint32_t)0x00010000)
#define DBG_TIM8_STOP         ((uint32_t)0x00020000)
#define DBG_TIM6_STOP         ((uint32_t)0x00080000)

/** @addtogroup wakeup pin polarity
 * @{
 */

#define WKUP_POL_RISE     1
#define WKUP_POL_DOWN     0


#define WKUP_PIN1         1
#define WKUP_PIN2         2
#define WKUP_PIN3         3
/** @addtogroup PWR_Exported_Functions
 * @{
 */

void PWR_DeInit(void);

void PWR_PvdEnable(FunctionalState Cmd);
void PWR_PVDLevelConfig(uint8_t PWR_PVDLevel);
void PWR_WakeUpPinEnable(uint8_t num,FunctionalState Cmd);
void PWR_WakeUpPinPolarity(uint8_t polarity);
void PWR_EnterSLEEPMode(uint8_t SLEEPONEXIT, uint8_t PWR_STOPEntry);
void PWR_EnterSTOPMode(uint32_t PWR_STOPeFlash, uint8_t PWR_STOPEntry);
void PWR_EnterLowPowerRunMode(uint32_t PWR_LPRUNeFlash,uint8_t clk);
void PWR_ExitLowPowerRunMode(void);
void PWR_EnterPDMode(uint8_t PWR_PDEntry);
FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG);
void PWR_ClearFlag(uint32_t PWR_FLAG);
void DBG_ConfigPeriph(uint32_t DBG_Periph, FunctionalState Cmd);
void SetSysClock_LSI(void);
#ifdef __cplusplus
}
#endif

#endif /* __N32G030_PWR_H__ */
       /**
        * @}
        */

/**
 * @}
 */

/**
 * @}
 */
