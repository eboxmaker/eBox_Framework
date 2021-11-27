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
 * @file n32g030_pwr.c
 * @author Nations 
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g030_pwr.h"

/** @addtogroup n32g030_StdPeriph_Driver
 * @{
 */


/* ------------------ PWR registers bit mask ------------------------ */

/* CLR register bit mask */
#define CLR_PLS_MASK                  ((uint32_t)0xFFFFFE1F)
#define CLR_LPRUN_MASK                ((uint32_t)0xFFFFFF8F)


#define CLR_PDSTOP_MASK               ((uint32_t)0xFFFFFFFC)
#define CLR_PLUS_MASK                 ((uint32_t)0xFFFFFBFF)
//#define CLR_FLPDS_MASK              ((uint32_t)0xFFFFEBFF)


#define CTRL4_WRITE_PROTECT_ENABLE    0x01753603
/**
 * @}
 */

/** @addtogroup PWR_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup PWR_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup PWR_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup PWR_Private_Functions
 * @{
 */
 
 

/**
 * @brief  Deinitializes the PWR peripheral registers to their default reset values.
 */
void PWR_DeInit(void)
{
    RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_PWR, ENABLE);
    RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_PWR, DISABLE);
}


/**
 * @brief  Enables or disables the Power Voltage Detector(PVD).
 * @param Cmd new state of the PVD.
 *   This parameter can be: ENABLE or DISABLE.
 */
void PWR_PvdEnable(FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if(Cmd == ENABLE)
    {
        PWR->CTRL |= PWR_CTRL_PVDEN;
    }else
    {
        PWR->CTRL &= ~PWR_CTRL_PVDEN;
    }
}

/**
  * @brief  Configures the voltage threshold detected by the Power Voltage Detector(PVD).
  * @param  PWR_PVDLevel: specifies the PVD detection level
  *   This parameter can be one of the following values:
  *     @arg the range is 0 to 15 (1.8V to 5V) ,one step is add 200mv
  * @retval None
  */
void PWR_PVDLevelConfig(uint8_t PWR_PVDLevel)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_PWR_PVD_LEVEL(PWR_PVDLevel));
    tmpregister = PWR->CTRL;
    /* Clear PLS[8:5] bits */
    tmpregister &= CLR_PLS_MASK;
    /* Set PLS[8:5] bits according to PWR_PVDLevel value */
    tmpregister |= (uint32_t)(PWR_PVDLevel << 5);
    /* Store the new value */
    PWR->CTRL = tmpregister;
}

/**
 * @brief  Enables or disables the WakeUp Pin functionality.
 * @param the wakeup pin number.
*   note: this program is 1 to 2 .so move start 7.
 * @param Cmd new state of the WakeUp Pin functionality.
 *   This parameter can be: ENABLE or DISABLE.
 */
void PWR_WakeUpPinEnable(uint8_t num,FunctionalState Cmd)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    tmpregister = PWR->CTRLSTS ;
    /* Clear WAKEUP bits */
    tmpregister &= (~( 1 << (7+num)));
    /* Set wakeup value */
    tmpregister |= (uint32_t)(Cmd << (7+num));
    /* Store the new value */
    PWR->CTRLSTS = tmpregister;
}
/**
 * @brief  set the WakeUp Pin polarity.
 * @param wakeup polarity .
 *   This parameter can be: WKUP_POL_RISE or WKUP_POL_DOWN.
 */
void PWR_WakeUpPinPolarity(uint8_t polarity)
{
    uint32_t tmpregister = 0;
    
    tmpregister = PWR->CTRLSTS ;
    /* Clear WAKEUP bits */
    tmpregister &= (~PWR_CTRLSTS_WKUPPOL);
    /* Set wakeup polarity value */
    tmpregister |= (uint32_t)(polarity << 11);
    /* Store the new value */
    PWR->CTRLSTS = tmpregister;
}
/**
  * @brief  Enters SLEEP mode.
  * @param  SLEEPONEXIT: specifies the SLEEPONEXIT state in SLEEP mode.
  *   This parameter can be one of the following values:
  *     @arg 0: SLEEP mode with SLEEPONEXIT disable
  *     @arg 1: SLEEP mode with SLEEPONEXIT enable
  * @param  PWR_STOPEntry: specifies if SLEEP mode in entered with WFI or WFE instruction.
  *   This parameter can be one of the following values:
  *     @arg PWR_SLEEPEntry_WFI: enter SLEEP mode with WFI instruction
  *     @arg PWR_SLEEPEntry_WFE: enter SLEEP mode with WFE instruction
  * @retval None
  */
void PWR_EnterSLEEPMode(uint8_t SLEEPONEXIT, uint8_t PWR_SLEEPEntry)
{
    /* Check the parameters */   
    assert_param(IS_PWR_SLEEP_ENTRY(PWR_SLEEPEntry));   

    /* CLEAR SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP); 

    /* Select SLEEPONEXIT mode entry --------------------------------------------------*/
    if(SLEEPONEXIT == 1)
    {   
        /* the MCU enters Sleep mode as soon as it exits the lowest priority ISR */
        SCB->SCR |= SCB_SCR_SLEEPONEXIT;
    }
    else if(SLEEPONEXIT == 0)
    {
        /* Sleep-now */
        SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPONEXIT);
    }

    /* Select SLEEP mode entry --------------------------------------------------*/
    if(PWR_SLEEPEntry == PWR_SLEEPENTRY_WFI)
    {   
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }
}

/**
  * @brief  Enters STOP mode.
  * @param  PWR_STOPeFlash: eFlash Low-power control for STOP mode.
  *   This parameter can be one of the following values:
  *     @arg PWR_STOPPLUSE_ENABLE: Enable keep a low power when system enters STOP mode
  *     @arg PWR_STOPPLUSE_DISABLE: Disable keep a low power when system enters STOP mode
  * @param  PWR_STOPEntry: specifies if STOP mode in entered with WFI or WFE instruction.
  *   This parameter can be one of the following values:
  *     @arg PWR_STOPEntry_WFI: enter STOP mode with WFI instruction
  *     @arg PWR_STOPEntry_WFE: enter STOP mode with WFE instruction
  * @retval None
  */
void PWR_EnterSTOPMode(uint32_t PWR_STOPplus, uint8_t PWR_STOPEntry)
{
    uint32_t tmpreg = 0;
    /* Check the parameters */
    assert_param(IS_STOPPLUSE(PWR_STOPplus));
    assert_param(IS_PWR_STOP_ENTRY(PWR_STOPEntry));

    /* Select the regulator state in STOP mode ---------------------------------*/
    tmpreg = PWR->CTRL;
    /* Clear PDDS and FLPDS bits */
    tmpreg &= CLR_PDSTOP_MASK;
    tmpreg &= CLR_PLUS_MASK;
    /* Set FLPDS bit according to PWR_Regulator value */
    tmpreg |= PWR_STOPplus;
    tmpreg |= PWR_CTRL_STOP;
    /* Store the new value */
    PWR->CTRL |= tmpreg;
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP;

    /* Select STOP mode entry --------------------------------------------------*/
    if(PWR_STOPEntry == PWR_STOPENTRY_WFI)
    {   
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }
  
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);  
}

/**
  * @brief  Enters LowPowerRun mode.
  * @param  PWR_LPRUNeFlash: eFlash Low-power control for LowPowerRun mode.
  *   This parameter can be one of the following values:
  *     @arg PWR_LPRUN_EFLASH_ACTIVE: keep eFlash awake when system enters LPRUN mode
  *     @arg PWR_LPRUN_EFLASH_DEEEPSTANDBY:Put eFlash to deep standby when system enters LPRUN mode
  * @retval None
  */
void PWR_EnterLowPowerRunMode(uint32_t PWR_LPRUNeFlash,uint8_t clk)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_PWR_LPRUNeFlash(PWR_LPRUNeFlash));
      /* Select Clock_source before entring lprun mode  --------------------------------------------------*/
    if(clk == LSI)
    {   
        /* open LSI clock */
        RCC_EnableLsi(ENABLE);

        while(RCC_GetFlagStatus(RCC_LSCTRL_FLAG_LSIRD) == RESET)
        {
        }
        RCC_ConfigLPRUNClk(RCC_LPRUNCLK_SRC_LSI);
    }
    else
    {
        RCC_ConfigLse(RCC_LSE_ENABLE);
        while(RCC_GetFlagStatus(RCC_LSCTRL_FLAG_LSERD) == RESET)
        {
        }
        RCC_ConfigLPRUNClk(RCC_LPRUNCLK_SRC_LSE);
    }   
    /* Open write protection */
    PWR->CTRL4 = CTRL4_WRITE_PROTECT_ENABLE;
    /* Select the regulator state in LPRUN mode ---------------------------------*/
    tmpreg = PWR->CTRL4;
    /* Clear LPRUN bits */
    tmpreg &= (~(PWR_CTRL4_LPRUNEN |PWR_CTRL4_LPRUNFLH));
    /* Set LPRUN bit according to PWR_Regulator value */

    tmpreg |= PWR_LPRUNeFlash;
    tmpreg |= PWR_CTRL4_LPRUNEN;
    /* Store the new value */
    PWR->CTRL4 = tmpreg;
}

/**
  * @brief  Exit Low power run mode.    
  * @param  
  *     @arg 
  *     @arg 
  * @retval None
  */
void PWR_ExitLowPowerRunMode(void)
{
  PWR->CTRL4 = CTRL4_WRITE_PROTECT_ENABLE;
  PWR->CTRL4 &= ~PWR_CTRL4_LPRUNEN;
}

 /**
  * @brief  Enters PD mode.
  * @param  PWR_PDEntry: specifies if PD mode in entered with WFI or WFE instruction.
  *   This parameter can be one of the following values:
  *     @arg PWR_PDEntry_WFI: enter PD mode with WFI instruction
  *     @arg PWR_PDEntry_WFE: enter PD mode with WFE instruction
  * @retval None
  */
void PWR_EnterPDMode(uint8_t PWR_PDEntry)
{
    uint32_t tmpreg = 0; 
    /* Select the regulator state in STOP mode ---------------------------------*/
    tmpreg = PWR->CTRL;
    /* Clear PDDS  bits */
    tmpreg &= CLR_PDSTOP_MASK;
    /* Set FLPDS bit according to PWR_Regulator value */
    tmpreg |= PWR_CTRL_PD;
    /* Store the new value */
    PWR->CTRL |= tmpreg;    

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP;
/* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM   )
    __force_stores();
#endif
    /* Select STOP mode entry --------------------------------------------------*/
    if(PWR_PDEntry == PWR_PDEntry_WFI)
    {   
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }
}

/**
  * @brief  Checks whether the specified PWR flag is set or not.
  * @param  PWR_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg PWR_DBGPD_WKUP_FLAG: Wake Up flag
  *     @arg PWR_DBGPD_FLAG: DBGPD flag
  *     @arg PWR_PVDO_FLAG: PVD Output
  * @retval The new state of PWR_FLAG (SET or RESET).
  */
FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_PWR_GET_FLAG(PWR_FLAG));

    if ((PWR->CTRLSTS & PWR_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    /* Return the flag status */
    return bitstatus;
}

/**
 * @brief  Clears the PWR's pending flags.
 * @param PWR_FLAG specifies the flag to clear.
 *   This parameter can be one of the following values:
 *     @arg PWR_CLR_WKUP_FLAG 
 *     @arg PWR_CLR_DBGPD_FLAG 
 */
void PWR_ClearFlag(uint32_t PWR_FLAG)
{
    /* Check the parameters */
    assert_param(IS_PWR_CLEAR_FLAG(PWR_FLAG));
         
    PWR->CTRL |= PWR_FLAG ;
}
/**
 * @brief  Configures the specified peripheral and low power mode behavior
 *   when the MCU under Debug mode.
 * @param DBG_Periph specifies the peripheral and low power mode.
 *   This parameter can be any combination of the following values:
 *     @arg DBG_STOP Keep debugger connection during STOP mode
 *     @arg DBG_PD Keep debugger connection during PD mode
 *     @arg DBG_IWDG_STOP Debug IWDG stopped when Core is halted
 *     @arg DBG_WWDG_STOP Debug WWDG stopped when Core is halted
 *     @arg DBG_TIM1_STOP TIM1 counter stopped when Core is halted
 *     @arg DBG_TIM2_STOP TIM2 counter stopped when Core is halted
 *     @arg DBG_TIM3_STOP TIM3 counter stopped when Core is halted
 *     @arg DBG_TIM4_STOP TIM4 counter stopped when Core is halted
 *     @arg DBG_CAN_STOP Debug CAN stopped when Core is halted
 *     @arg DBG_I2C1SMBUS_TIMEOUT I2C1 SMBUS timeout mode stopped when Core is halted
 *     @arg DBG_I2C2SMBUS_TIMEOUT I2C2 SMBUS timeout mode stopped when Core is halted
 *     @arg DBG_TIM8_STOP TIM8 counter stopped when Core is halted
 *     @arg DBG_TIM6_STOP TIM6 counter stopped when Core is halted
 * @param Cmd new state of the specified peripheral in Debug mode.
 *   This parameter can be: ENABLE or DISABLE.
 */
void DBG_ConfigPeriph(uint32_t DBG_Periph, FunctionalState Cmd)
{

    if (Cmd != DISABLE)
    {
        PWR->DBGCTRL |= DBG_Periph;
    }
    else
    {
        PWR->DBGCTRL &= ~DBG_Periph;
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
