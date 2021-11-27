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
 * @file n32g030_beeper.c
 * @author Nations
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g030_beeper.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup BEEPER
 * @brief BEEPER driver modules
 * @{
 */

/** @addtogroup BEEPER_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup BEEPER_Private_Defines
 * @{
 */

/**
 * @}
 */

/** @addtogroup BEEPER_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup BEEPER_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup BEEPER_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup BEEPER_Private_Functions
 * @{
 */

/**
 * @brief  Initializes the BEEPER peripheral.
 * @param  inv_en The complementary output enables or disables options
 *         This parameter can be: BEEPER_INV_ENABLE or BEEPER_INV_DISABLE
 * @param  buzzer_clksel Tones to choose
 *         This parameter can be:BEEPER_BUZZER_CLKSEL_L1~BEEPER_BUZZER_CLKSEL_L7,
 *             BEEPER_BUZZER_CLKSEL_M1~BEEPER_BUZZER_CLKSEL_M7,
 *             BEEPER_BUZZER_CLKSEL_H1~BEEPER_BUZZER_CLKSEL_H7.
 * @param  freq_sel The apb clk frequency,It's range from 1 to 48,0 also remap to 48.
 */
void BEEPER_Init(uint32_t inv_en, uint32_t buzzer_clksel,uint32_t freq_sel)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_BEEPER_INV_EN(inv_en));
    assert_param(IS_BEEPER_BUZZER_CLKSEL(buzzer_clksel));
    assert_param(IS_BEEPER_FREQ_APBCLK(freq_sel));

    /* Set APB clk frequency */
    tmpregister |= freq_sel;
    /* Set Buzzer inverter */
    tmpregister |= inv_en;
    /* Buzzer output clock select */
    tmpregister |= buzzer_clksel;
    /* Store the new value */
    BEEPER->CTRL = tmpregister;
}

/**
 * @brief  Enables or disables the specified BEEPER peripheral,
 *         and Enter LowPowerRun or normal mode.
 * @param  Cmd new state of the BEEPER peripheral.
 *         This parameter can be: ENABLE or DISABLE.
 * @param  buzz_lprun:Controls enter or exit LowPowerRun mode.
 *         This parameter can be one of the following values:
 *         @arg BEEPER_LPRUN_MODE: buzz_lprun = 1,In LPRUN mode
 *         @arg BEEPER_NORMAL_MODE: buzz_lprun = 0,In normal mode
 */
void BEEPER_Config(FunctionalState Cmd,uint32_t buzz_lprun)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_BEEPER_RUN_MODE(buzz_lprun));
    tmpregister = BEEPER->CTRL;
    
    if((RCC->APB1PCLKEN & RCC_APB1PCLKEN_PWREN) == 0)
    {
        /* Enable PWR Clock */
        RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
    }
    
    if ((Cmd != DISABLE) && (buzz_lprun == BEEPER_LPRUN_MODE))
    {
        /*Enter LowPowerRun mode*/
        PWR_EnterLowPowerRunMode(PWR_LPRUN_EFLASH_ACTIVE,LSE);

        tmpregister |= buzz_lprun;
        /*Enable BEEPER*/
        tmpregister |= BEEPER_BUZZER_BUZZER_ENABLE;
        /* Store the new value*/
        BEEPER->CTRL = tmpregister;
    }
    else if((Cmd != DISABLE) && (buzz_lprun == BEEPER_NORMAL_MODE))
    {
        if((PWR->CTRL4 & PWR_CTRL4_LPRUNSTS) != 0)
        {
            PWR_ExitLowPowerRunMode();
        }
        /*Enter normal mode*/
        tmpregister |= buzz_lprun;
        /*Enable BEEPER*/
        tmpregister |= BEEPER_BUZZER_BUZZER_ENABLE;
        /* Store the new value*/
        BEEPER->CTRL = tmpregister;
    }
    else
    {
        /* Disable BEEPER */
        BEEPER->CTRL &= (~BEEPER_BUZZER_BUZZER_ENABLE);
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
