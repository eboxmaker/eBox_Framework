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
 * @file n32g030_rcc.c
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g030_rcc.h"

/** @addtogroup n32g030_StdPeriph_Driver
 * @{
 */

/** @addtogroup RCC
 * @brief RCC driver modules
 * @{
 */

/** @addtogroup RCC_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup RCC_Private_Defines
 * @{
 */

/* ---------------------- RCC registers bit mask ------------------------ */

/* CTRL register bit mask */
#define CTRL_PLLEN_RESET    ((uint32_t)0xFEFFFFFF)
#define CTRL_PLLEN_SET      ((uint32_t)0x01000000)
#define CTRL_PLLOUTEN_RESET ((uint32_t)0xFF7FFFFF)
#define CTRL_PLLOUTEN_SET   ((uint32_t)0x00800000)
#define CTRL_CLKSSEN_RESET  ((uint32_t)0xFFF7FFFF)
#define CTRL_CLKSSEN_SET    ((uint32_t)0x00080000)
#define CTRL_HSEEN_RESET    ((uint32_t)0xFFFEFFFF)
#define CTRL_HSEEN_SET      ((uint32_t)0x00010000)
#define CTRL_HSEBP_RESET    ((uint32_t)0xFFFBFFFF)
#define CTRL_HSEBP_SET      ((uint32_t)0x00040000)
#define CTRL_HSITRIM_MASK   ((uint32_t)0xFFFFFF07)
#define CTRL_HSIEN_RESET    ((uint32_t)0xFFFFFFFE)
#define CTRL_HSIEN_SET      ((uint32_t)0x00000001)

/* CFG register bit mask */
#define CFG_PLL_MASK ((uint32_t)0xFEC0FFFF)

#define CFG_PLLSRC_MASK         ((uint32_t)0x01000000)
#define CFG_PLLOUTDIV_MASK      ((uint32_t)0x00C00000)
#define CFG_PLLPRE_MASK         ((uint32_t)0x00300000)
#define CFG_PLLMULFCT_MASK      ((uint32_t)0x000F0000)
#define CFG_APB2PRES_RESET_MASK ((uint32_t)0xFFFFC7FF)
#define CFG_APB2PRES_SET_MASK   ((uint32_t)0x00003800)
#define CFG_APB1PRES_RESET_MASK ((uint32_t)0xFFFFF8FF)
#define CFG_APB1PRES_SET_MASK   ((uint32_t)0x00000700)
#define CFG_AHBPRES_RESET_MASK  ((uint32_t)0xFFFFFF0F)
#define CFG_AHBPRES_SET_MASK    ((uint32_t)0x000000F0)
#define CFG_SCLKSTS_MASK        ((uint32_t)0x0000C008)
#define CFG_SCLKSW_MASK         ((uint32_t)0xFFFFFFF8)

/* CLKINT register bit mask */
#define CLKINT_RAMCERRRST_SET_MASK   ((uint32_t)0x00004000)
#define CLKINT_RAMCERRRST_RESET_MASK ((uint32_t)0xFFFFBFFF)

/* LSCTRL register bit mask */
#define LSCTRL_LSIEN_SET_MASK         ((uint32_t)0x00000001)
#define LSCTRL_LSIEN_RESET_MASK       ((uint32_t)0xFFFFFFFE)
#define LSCTRL_LSEEN_SET_MASK         ((uint32_t)0x00000004)
#define LSCTRL_LSEEN_RESET_MASK       ((uint32_t)0xFFFFFFFB)
#define LSCTRL_LSEBP_SET_MASK         ((uint32_t)0x00000010)
#define LSCTRL_LSEBP_RESET_MASK       ((uint32_t)0xFFFFFFEF)
#define LSCTRL_RTCSEL_SET_MASK        ((uint32_t)0x00000060)
#define LSCTRL_RTCSEL_RESET_MASK      ((uint32_t)0xFFFFFF9F)
#define LSCTRL_RTCEN_SET_MASK         ((uint32_t)0x00000080)
#define LSCTRL_RTCEN_RESET_MASK       ((uint32_t)0xFFFFFF7F)
#define LSCTRL_RTCRST_SET_MASK        ((uint32_t)0x00000100)
#define LSCTRL_RTCRST_RESET_MASK      ((uint32_t)0xFFFFFEFF)
#define LSCTRL_LPRUNCLKSEL_SET_MASK   ((uint32_t)0x00000200)
#define LSCTRL_LPRUNCLKSEL_RESET_MASK ((uint32_t)0xFFFFFDFF)

/* CFG2 register bit mask */
#define CFG2_TIM18CLKSEL_SET_MASK   ((uint32_t)0x80000000)
#define CFG2_TIM18CLKSEL_RESET_MASK ((uint32_t)0x7FFFFFFF)

#define CFG2_LPUARTCLK_SET_MASK     ((uint32_t)0x0E000000)
#define CFG2_LPUARTCLK_RESET_MASK   ((uint32_t)0xF1FFFFFF)
#define CFG2_LPTIMCLK_SET_MASK      ((uint32_t)0x00E00000)
#define CFG2_LPTIMCLK_RESET_MASK    ((uint32_t)0xFF1FFFFF)
#define CFG2_ADC1MSEL_SET_MASK      ((uint32_t)0x00000400)
#define CFG2_ADC1MSEL_RESET_MASK    ((uint32_t)0xFFFFFBFF)
#define CFG2_ADC1MPRES_SET_MASK     ((uint32_t)0x0000F800)
#define CFG2_ADC1MPRES_RESET_MASK   ((uint32_t)0xFFFF07FF)
#define CFG2_ADCPLLPRES_SET_MASK    ((uint32_t)0x000001F0)
#define CFG2_ADCPLLPRES_RESET_MASK  ((uint32_t)0xFFFFFE0F)
#define CFG2_ADCHPRES_SET_MASK      ((uint32_t)0x0000000F)
#define CFG2_ADCHPRES_RESET_MASK    ((uint32_t)0xFFFFFFF0)

/* CTRLSTS register bit mask */
#define CSR_RMRSTF_SET ((uint32_t)0x00000001)
#define CSR_RMRSTF_Reset ((uint32_t)0xfeffffff)

/* RCC Flag Mask */
#define FLAG_MASK ((uint8_t)0x1F)

/* CLKINT register(Bits[31:0]) base address */
#define CLKINT_ADDR ((uint32_t)0x40021008)

/* LSCTRL register base address */
#define LSCTRL_ADDR ((uint32_t)0x40021020)

/**
 * @}
 */

/** @addtogroup RCC_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup RCC_Private_Variables
 * @{
 */

static const uint8_t s_ApbAhbPresTable[16]     = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
static const uint8_t s_AdcHclkPresTable[16]    = {1, 2, 4, 6, 8, 10, 12, 16, 32, 32, 32, 32, 32, 32, 32, 32};
static const uint16_t s_AdcPllClkPresTable[16] = {1, 2, 4, 6, 8, 10, 12, 16, 32, 64, 128, 256, 256, 256, 256, 256};

/**
 * @}
 */

/** @addtogroup RCC_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup RCC_Private_Functions
 * @{
 */

/**
 * @brief  Resets the RCC clock configuration to the default reset state.
 */
void RCC_DeInit(void)
{
    /* Set HSIEN bit */
    RCC->CTRL |= (uint32_t)0x00000001;

    /* Reset SCLKSW, AHBPRE, APB1PRES, APB2PRES, MCO and MCOPRES bits */
    RCC->CFG &= (uint32_t)0x21FF0000;

    /* Reset HSEEN, CLKSSEN and PLLEN bits */
    RCC->CTRL &= (uint32_t)0xFEF6FFFF;

    /* Reset PLLBPbit */
    RCC->CTRL &= (uint32_t)0xFFBFFFFF;
  
    /* Reset PLLMULFCT, PLLPRE, PLLOUTDIV and PLLSRC bits */
    RCC->CFG &= (uint32_t)0xFE00FFFF;

    /* Reset CFG2 register */
    RCC->CFG2 = 0x00003800;

    /* Disable all interrupts and clear pending bits  */
    RCC->CLKINT = 0x00BF0000;
}

/**
 * @brief  Configures the External High Speed oscillator (HSE).
 * @note   HSE can not be stopped if it is used directly or through the PLL as system clock.
 * @param RCC_HSE specifies the new state of the HSE.
 *   This parameter can be one of the following values:
 *     @arg RCC_HSE_DISABLE    HSE oscillator OFF
 *     @arg RCC_HSE_ENABLE     HSE oscillator ON
 *     @arg RCC_HSE_BYPASS     HSE Bypass mode,PF0 as input
 */
void RCC_ConfigHse(uint32_t RCC_HSE)
{
    /* Check the parameters */
    assert_param(IS_RCC_HSE(RCC_HSE));
    /* Reset HSEON and HSEIOSEL bits before configuring the HSE ------------------*/
    /* Reset HSEON bit */
    RCC->CTRL &= CTRL_HSEEN_RESET;
    /* Reset HSEBP bit */
    RCC->CTRL &= CTRL_HSEBP_RESET;
    /* Configure HSE (RCC_HSE_DISABLE is already covered by the code section above) */
    switch (RCC_HSE)
    {
        case RCC_HSE_ENABLE:
            /* Set HSEEN bit */
            RCC->CTRL |= CTRL_HSEEN_SET;
            break;

        case RCC_HSE_BYPASS:
            /* Set HSEIOSEL and HSEEN bits */
            RCC->CTRL |= RCC_HSE_BYPASS | CTRL_HSEEN_SET;
            break;

        default:
            break;
    }
}

/**
 * @brief  Waits for HSE start-up.
 * @return An ErrorStatus enumuration value:
 * - SUCCESS: HSE oscillator is stable and ready to use
 * - ERROR: HSE oscillator not yet ready
 */
ErrorStatus RCC_WaitHseStable(void)
{
    __IO uint32_t StartUpCounter = 0;
    ErrorStatus status           = ERROR;
    FlagStatus HSEStatus         = RESET;

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC_GetFlagStatus(RCC_CTRL_FLAG_HSERDF);
        StartUpCounter++;
    } while ((StartUpCounter != HSE_STARTUP_TIMEOUT) && (HSEStatus == RESET));

    if (RCC_GetFlagStatus(RCC_CTRL_FLAG_HSERDF) != RESET)
    {
        status = SUCCESS;
    }
    else
    {
        status = ERROR;
    }
    return (status);
}

/**
 * @brief  Configures the Internal High Speed oscillator (HSI).
 * @note   HSI can not be stopped if it is used directly or through the PLL as system clock.
 * @param RCC_HSI specifies the new state of the HSI.
 *   This parameter can be one of the following values:
 *     @arg RCC_HSI_DISABLE HSI oscillator OFF
 *     @arg RCC_HSI_ENABLE HSI oscillator ON
 */
void RCC_ConfigHsi(uint32_t RCC_HSI)
{
    /* Check the parameters */
    assert_param(IS_RCC_HSI(RCC_HSI));
    /* Reset HSIEN bit */
    RCC->CTRL &= CTRL_HSIEN_RESET;
    /* Configure HSI */
    switch (RCC_HSI)
    {
        case RCC_HSI_ENABLE:
            /* Set HSIEN bit */
            RCC->CTRL |= CTRL_HSIEN_SET;
            break;

        default:
            break;
    }
}

/**
 * @brief  Waits for HSI start-up.
 * @return An ErrorStatus enumuration value:
 * - SUCCESS: HSI oscillator is stable and ready to use
 * - ERROR: HSI oscillator not yet ready
 */
ErrorStatus RCC_WaitHsiStable(void)
{
    __IO uint32_t StartUpCounter = 0;
    ErrorStatus status           = ERROR;
    FlagStatus HSIStatus         = RESET;

    /* Wait till HSI is ready and if Time out is reached exit */
    do
    {
        HSIStatus = RCC_GetFlagStatus(RCC_CTRL_FLAG_HSIRDF);
        StartUpCounter++;
    } while ((StartUpCounter != HSI_STARTUP_TIMEOUT) && (HSIStatus == RESET));

    if (RCC_GetFlagStatus(RCC_CTRL_FLAG_HSIRDF) != RESET)
    {
        status = SUCCESS;
    }
    else
    {
        status = ERROR;
    }
    return (status);
}

/**
 * @brief  Adjusts the Internal High Speed oscillator (HSI) calibration value.
 * @param HSICalibrationValue specifies the calibration trimming value.
 *   This parameter must be a number between 0 and 0x1F.
 */
void RCC_SetHsiCalibValue(uint8_t HSICalibrationValue)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_RCC_CALIB_VALUE(HSICalibrationValue));
    tmpregister = RCC->CTRL;
    /* Clear HSITRIM[4:0] bits */
    tmpregister &= CTRL_HSITRIM_MASK;
    /* Set the HSITRIM[4:0] bits according to HSICalibrationValue value */
    tmpregister |= (uint32_t)HSICalibrationValue << 3;
    /* Store the new value */
    RCC->CTRL = tmpregister;
}

/**
 * @brief  Enables or disables the Internal High Speed oscillator (HSI).
 * @note   HSI can not be stopped if it is used directly or through the PLL as system clock.
 * @param Cmd new state of the HSI. This parameter can be: ENABLE or DISABLE.
 */
void RCC_EnableHsi(FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

   if(Cmd == ENABLE)
   {
       /* Set HSIEN bit */
       RCC->CTRL |= CTRL_HSIEN_SET;
   }
   else
   {
       /* Reset HSIEN bit */
       RCC->CTRL &= CTRL_HSIEN_RESET;
   }
}

/**
 * @brief  Configures the PLL clock source and multiplication factor.
 * @note   This function must be used only when the PLL is disabled.
 * @param RCC_PLLSource specifies the PLL entry clock source.
 *   this parameter can be one of the following values:
 *     @arg RCC_PLL_SRC_HSI HSI oscillator clock selected as PLL clock entry
 *     @arg RCC_PLL_SRC_HSE HSE oscillator clock selected as PLL clock entry
 *     @arg RCC_PLL_SRC_BP  PLL Bypass clock selected as PLL clock entry
 * @param RCC_PLLMul specifies the PLL multiplication factor.
 *    this parameter can be RCC_PLLMul_x where x:[3,18]
 * @param RCC_PLLPRE specifies the PLL Pre-Divider.
 *    this parameter can be RCC_PLLPRE_x where x:[1,4]
 * @param RCC_PLLOUTDIV specifies the PLL Out divider clock.
 *   this parameter can be one of the following values:
 *     this parameter can be RCC_PLLOUT_DIV_x where x:[1,4]
 */
void RCC_ConfigPll(uint32_t RCC_PLLSource, uint32_t RCC_PLLMul, uint32_t RCC_PLLPRE, uint32_t RCC_PLLOUTDIV)
{
    uint32_t tmpregister = 0;
    uint32_t rccctrlregister = 0;
    /* Check the parameters */
    assert_param(IS_RCC_PLL_SRC(RCC_PLLSource));
    assert_param(IS_RCC_PLL_MUL(RCC_PLLMul));
    assert_param(IS_RCC_PLL_PRE(RCC_PLLPRE));
    assert_param(IS_RCC_PLLOUT_DIV(RCC_PLLOUTDIV));
    
    /* Clear PLLOUTDIV PLLPRE bits */
    RCC->CFG &= (~CFG_PLLOUTDIV_MASK);

    tmpregister = RCC->CFG;
    /* Clear PLLSRC and PLLMULFCT[3:0] bits */
    tmpregister &= CFG_PLL_MASK;

    /* Set the PLL configuration bits */
    if((RCC_PLLSource == RCC_PLL_SRC_HSI) || (RCC_PLLSource == RCC_PLL_SRC_HSE))
    {
        tmpregister |= (RCC_PLLMul | RCC_PLLPRE | RCC_PLLOUTDIV);
    }
    /* (RCC_PLLSource == RCC_PLL_SRC_BP) */
    else
    {
        rccctrlregister = RCC->CTRL;

        tmpregister     |= RCC_PLLOUTDIV;
        rccctrlregister |= RCC_PLL_SRC_BP;
        RCC->CTRL = rccctrlregister;
    }
    /* Store the new value */
    RCC->CFG  = tmpregister;
}

/**
 * @brief  Enables or disables the PLL.
 * @note   The PLL can not be disabled if it is used as system clock.
 * @param Cmd new state of the PLL. This parameter can be: ENABLE or DISABLE.
 */
void RCC_EnablePll(FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if(Cmd == ENABLE)
   {
       /* Set PLLEN bit */
       RCC->CTRL |= CTRL_PLLEN_SET;
   }
   else
   {
       /* Reset PLLEN bit */
       RCC->CTRL &= CTRL_PLLEN_RESET;
   }
}

/**
 * @brief  Enables or disables the PLL Clock Output.
 * @note   The PLL can not be disabled if it is used as system clock.
 * @param Cmd new state of the PLL. This parameter can be: ENABLE or DISABLE.
 */
void RCC_EnablePllClockOut(FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if(Cmd == ENABLE)
   {
       /* Set PLLOUTEN bit */
       RCC->CTRL |= CTRL_PLLOUTEN_SET;
   }
   else
   {
       /* Reset PLLOUTEN bit */
       RCC->CTRL &= CTRL_PLLOUTEN_RESET;
   }
}

/**
 * @brief  Configures the system clock (SYSCLK).
 * @param RCC_SYSCLKSource specifies the clock source used as system clock.
 *   This parameter can be one of the following values:
 *     @arg RCC_SYSCLK_SRC_HSI    HSI selected as system clock
 *     @arg RCC_SYSCLK_SRC_HSE    HSE selected as system clock
 *     @arg RCC_SYSCLK_SRC_PLLCLK PLL selected as system clock
 *     @arg RCC_SYSCLK_SRC_LSE    LSE selected as system clock
 *     @arg RCC_SYSCLK_SRC_LSI    LSI selected as system clock
 */
void RCC_ConfigSysclk(uint32_t RCC_SYSCLKSource)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_RCC_SYSCLK_SRC(RCC_SYSCLKSource));
    tmpregister = RCC->CFG;
    /* Clear SW[2:0] bits */
    tmpregister &= CFG_SCLKSW_MASK;
    /* Set SW[2:0] bits according to RCC_SYSCLKSource value */
    tmpregister |= RCC_SYSCLKSource;
    /* Store the new value */
    RCC->CFG = tmpregister;
}

/**
 * @brief  Returns the clock source used as system clock.
 * @return The clock source used as system clock. The returned value can
 *   be one of the following:
 *     - RCC_CFG_SCLKSTS_HSI: HSI used as system clock
 *     - RCC_CFG_SCLKSTS_HSE: HSE used as system clock
 *     - RCC_CFG_SCLKSTS_PLL: PLL used as system clock
 *     - RCC_CFG_SCLKSTS_LSE: LSE used as system clock
 *     - RCC_CFG_SCLKSTS_LSI: LSI used as system clock
 */
uint32_t RCC_GetSysclkSrc(void)
{
    return ((uint32_t)(RCC->CFG & CFG_SCLKSTS_MASK));
}

/**
 * @brief  Configures the AHB clock (HCLK).
 * @param RCC_SYSCLK defines the AHB clock divider. This clock is derived from
 *   the system clock (SYSCLK).
 *   This parameter can be one of the following values:
 *     @arg RCC_SYSCLK_DIV1 AHB clock = SYSCLK
 *     @arg RCC_SYSCLK_DIV2 AHB clock = SYSCLK/2
 *     @arg RCC_SYSCLK_DIV4 AHB clock = SYSCLK/4
 *     @arg RCC_SYSCLK_DIV8 AHB clock = SYSCLK/8
 *     @arg RCC_SYSCLK_DIV16 AHB clock = SYSCLK/16
 *     @arg RCC_SYSCLK_DIV64 AHB clock = SYSCLK/64
 *     @arg RCC_SYSCLK_DIV128 AHB clock = SYSCLK/128
 *     @arg RCC_SYSCLK_DIV256 AHB clock = SYSCLK/256
 *     @arg RCC_SYSCLK_DIV512 AHB clock = SYSCLK/512
 */
void RCC_ConfigHclk(uint32_t RCC_SYSCLK)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_RCC_SYSCLK_DIV(RCC_SYSCLK));
    tmpregister = RCC->CFG;
    /* Clear HPRE[3:0] bits */
    tmpregister &= CFG_AHBPRES_RESET_MASK;
    /* Set HPRE[3:0] bits according to RCC_SYSCLK value */
    tmpregister |= RCC_SYSCLK;
    /* Store the new value */
    RCC->CFG = tmpregister;
}

/**
 * @brief  Configures the Low Speed APB clock (PCLK1).
 * @param RCC_HCLK defines the APB1 clock divider. This clock is derived from
 *   the AHB clock (HCLK).
 *   This parameter can be one of the following values:
 *     @arg RCC_HCLK_DIV1 APB1 clock = HCLK
 *     @arg RCC_HCLK_DIV2 APB1 clock = HCLK/2
 *     @arg RCC_HCLK_DIV4 APB1 clock = HCLK/4
 *     @arg RCC_HCLK_DIV8 APB1 clock = HCLK/8
 *     @arg RCC_HCLK_DIV16 APB1 clock = HCLK/16
 */
void RCC_ConfigPclk1(uint32_t RCC_HCLK)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_RCC_HCLK_DIV(RCC_HCLK));
    tmpregister = RCC->CFG;
    /* Clear PPRE1[2:0] bits */
    tmpregister &= CFG_APB1PRES_RESET_MASK;
    /* Set PPRE1[2:0] bits according to RCC_HCLK value */
    tmpregister |= RCC_HCLK;
    /* Store the new value */
    RCC->CFG = tmpregister;
}

/**
 * @brief  Configures the High Speed APB clock (PCLK2).
 * @param RCC_HCLK defines the APB2 clock divider. This clock is derived from
 *   the AHB clock (HCLK).
 *   This parameter can be one of the following values:
 *     @arg RCC_HCLK_DIV1 APB2 clock = HCLK
 *     @arg RCC_HCLK_DIV2 APB2 clock = HCLK/2
 *     @arg RCC_HCLK_DIV4 APB2 clock = HCLK/4
 *     @arg RCC_HCLK_DIV8 APB2 clock = HCLK/8
 *     @arg RCC_HCLK_DIV16 APB2 clock = HCLK/16
 */
void RCC_ConfigPclk2(uint32_t RCC_HCLK)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_RCC_HCLK_DIV(RCC_HCLK));
    tmpregister = RCC->CFG;
    /* Clear PPRE2[2:0] bits */
    tmpregister &= CFG_APB2PRES_RESET_MASK;
    /* Set PPRE2[2:0] bits according to RCC_HCLK value */
    tmpregister |= RCC_HCLK << 3;
    /* Store the new value */
    RCC->CFG = tmpregister;
}

/**
 * @brief  Enables or disables the specified RCC interrupts.
 * @param RccInt specifies the RCC interrupt sources to be enabled or disabled.
 *
 *   this parameter can be any combination of the following values
 *     @arg RCC_INT_LSIRDIF LSI ready interrupt
 *     @arg RCC_INT_LSERDIF LSE ready interrupt
 *     @arg RCC_INT_HSIRDIF HSI ready interrupt
 *     @arg RCC_INT_HSERDIF HSE ready interrupt
 *     @arg RCC_INT_PLLRDIF PLL ready interrupt
 *     @arg RCC_INT_RAMCPIF RAMC parity interrupt
 *
 * @param Cmd new state of the specified RCC interrupts.
 *   This parameter can be: ENABLE or DISABLE.
 */
void RCC_ConfigInt(uint8_t RccInt, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_RCC_INT(RccInt));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        /* Perform Byte access to RCC_CLKINT bits to enable the selected interrupts */
        *(__IO uint32_t*)CLKINT_ADDR |= (((uint32_t)RccInt) << 8);
    }
    else
    {
        /* Perform Byte access to RCC_CLKINT bits to disable the selected interrupts */
        *(__IO uint32_t*)CLKINT_ADDR &= (~(((uint32_t)RccInt) << 8));
    }
}

/**
 * @brief  Configures the TIM1/8 clock (TIM1/8CLK).
 * @param RCC_TIM18CLKSource specifies the TIM1/8 clock source.
 *   This parameter can be one of the following values:
 *     @arg RCC_TIM18CLK_SRC_TIM18CLK
 *     @arg RCC_TIM18CLK_SRC_SYSCLK
 */
void RCC_ConfigTim18Clk(uint32_t RCC_TIM18CLKSource)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_RCC_TIM18CLKSRC(RCC_TIM18CLKSource));

    tmpregister = RCC->CFG2;
    /* Clear TIMCLK_SEL bits */
    tmpregister &= CFG2_TIM18CLKSEL_RESET_MASK;
    /* Set TIMCLK_SEL bits according to RCC_TIM18CLKSource value */
    tmpregister |= RCC_TIM18CLKSource;

    /* Store the new value */
    RCC->CFG2 = tmpregister;
}

/**
 * @brief  Configures the ADCx 1M clock (ADC1MCLK).
 * @param RCC_ADC1MCLKSource specifies the ADC1M clock source.
 *   This parameter can be on of the following values:
 *     @arg RCC_ADC1MCLK_SRC_HSI
 *     @arg RCC_ADC1MCLK_SRC_HSE
 *
 * @param RCC_ADC1MPrescaler specifies the ADC1M clock prescaler.
 *   This parameter can be on of the following values:
 *     @arg RCC_ADC1MCLK_DIV1 ADC1M clock = RCC_ADC1MCLKSource_xxx/1
 *     @arg RCC_ADC1MCLK_DIV2 ADC1M clock = RCC_ADC1MCLKSource_xxx/2
 *     @arg RCC_ADC1MCLK_DIV3 ADC1M clock = RCC_ADC1MCLKSource_xxx/3
 *               ...
 *     @arg RCC_ADC1MCLK_DIV31 ADC1M clock = RCC_ADC1MCLKSource_xxx/31
 *     @arg RCC_ADC1MCLK_DIV32 ADC1M clock = RCC_ADC1MCLKSource_xxx/32
 */
void RCC_ConfigAdc1mClk(uint32_t RCC_ADC1MCLKSource, uint32_t RCC_ADC1MPrescaler)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_RCC_ADC1MCLKSRC(RCC_ADC1MCLKSource));
    assert_param(IS_RCC_ADC1MCLKPRE(RCC_ADC1MPrescaler));

    tmpregister = RCC->CFG2;
    /* Clear ADC1MSEL and ADC1MPRE[4:0] bits */
    tmpregister &= CFG2_ADC1MSEL_RESET_MASK;
    tmpregister &= CFG2_ADC1MPRES_RESET_MASK;
    /* Set ADC1MSEL bits according to RCC_ADC1MCLKSource value */
    tmpregister |= RCC_ADC1MCLKSource;
    /* Set ADC1MPRE[4:0] bits according to RCC_ADC1MPrescaler value */
    tmpregister |= RCC_ADC1MPrescaler;

    /* Store the new value */
    RCC->CFG2 = tmpregister;
}

/**
 * @brief  Configures the ADCPLLCLK prescaler, and enable/disable ADCPLLCLK.
 * @param RCC_ADCPLLCLKPrescaler specifies the ADCPLLCLK prescaler.
 *   This parameter can be on of the following values:
 *     @arg RCC_ADCPLLCLK_DIV1 ADCPLLCLKPRES[4:0] = 10000, Pll Clock Divided By 1
 *     @arg RCC_ADCPLLCLK_DIV2 ADCPLLCLKPRES[4:0] = 10001, Pll Clock Divided By 2
 *     @arg RCC_ADCPLLCLK_DIV3 ADCPLLCLKPRES[4:0] = 10010, Pll Clock Divided By 3
 *     @arg RCC_ADCPLLCLK_DIV4 ADCPLLCLKPRES[4:0] = 10011, Pll Clock Divided By 4
 *     @arg RCC_ADCPLLCLK_DIV6 ADCPLLCLKPRES[4:0] = 10100, Pll Clock Divided By 6
 *     @arg RCC_ADCPLLCLK_DIV8 ADCPLLCLKPRES[4:0] = 10101, Pll Clock Divided By 8
 *     @arg RCC_ADCPLLCLK_DIV10 ADCPLLCLKPRES[4:0] = 10110, Pll Clock Divided By 10
 *     @arg RCC_ADCPLLCLK_DIV12 ADCPLLCLKPRES[4:0] = 10111, Pll Clock Divided By 12
 *     @arg RCC_ADCPLLCLK_DIV16 ADCPLLCLKPRES[4:0] = 11000, Pll Clock Divided By 16
 *     @arg RCC_ADCPLLCLK_DIV32 ADCPLLCLKPRES[4:0] = 11001, Pll Clock Divided By 32
 *     @arg RCC_ADCPLLCLK_DIV64 ADCPLLCLKPRES[4:0] = 11010, Pll Clock Divided By 64
 *     @arg RCC_ADCPLLCLK_DIV128 ADCPLLCLKPRES[4:0] = 11011, Pll Clock Divided By 128
 *     @arg RCC_ADCPLLCLK_DIV256 ADCPLLCLKPRES[4:0] = 11100, Pll Clock Divided By 256
 *     @arg RCC_ADCPLLCLK_DIV_OTHERS ADCPLLCLKPRES[4:0] = others, Pll Clock Divided By 256
 *
 * @param Cmd specifies the ADCPLLCLK enable/disable selection.
 *   This parameter can be on of the following values:
 *     @arg ENABLE enable ADCPLLCLK
 *     @arg DISABLE disable ADCPLLCLK ADCPLLCLKPRES[4:0] = 0xxxx, ADC Pll Clock Disable
 */
void RCC_ConfigAdcPllClk(uint32_t RCC_ADCPLLCLKPrescaler, FunctionalState Cmd)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_RCC_ADCPLLCLKPRE(RCC_ADCPLLCLKPrescaler));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    tmpregister = RCC->CFG2;
    /* Clear ADCPLLPRES[4:0] bits */
    tmpregister &= CFG2_ADCPLLPRES_RESET_MASK;

    if (Cmd != DISABLE)
    {
        tmpregister |= RCC_ADCPLLCLKPrescaler;
    }
    else
    {
        tmpregister &= RCC_ADCPLLCLK_DISABLE;
    }

    /* Store the new value */
    RCC->CFG2 = tmpregister;
}

/**
 * @brief  Configures the ADCHCLK prescaler.
 * @param RCC_ADCHCLKPrescaler specifies the ADCHCLK prescaler.
 *   This parameter can be on of the following values:
 *     @arg RCC_ADCHCLK_DIV1 ADCHCLKPRE[3:0] = 0000, HCLK Clock Divided By 1
 *     @arg RCC_ADCHCLK_DIV2 ADCHCLKPRE[3:0] = 0001, HCLK Clock Divided By 2
 *     @arg RCC_ADCHCLK_DIV4 ADCHCLKPRE[3:0] = 0010, HCLK Clock Divided By 3
 *     @arg RCC_ADCHCLK_DIV4 ADCHCLKPRE[3:0] = 0011, HCLK Clock Divided By 4
 *     @arg RCC_ADCHCLK_DIV6 ADCHCLKPRE[3:0] = 0100, HCLK Clock Divided By 6
 *     @arg RCC_ADCHCLK_DIV8 ADCHCLKPRE[3:0] = 0101, HCLK Clock Divided By 8
 *     @arg RCC_ADCHCLK_DIV10 ADCHCLKPRE[3:0] = 0110, HCLK Clock Divided By 10
 *     @arg RCC_ADCHCLK_DIV12 ADCHCLKPRE[3:0] = 0111, HCLK Clock Divided By 12
 *     @arg RCC_ADCHCLK_DIV16 ADCHCLKPRE[3:0] = 1000, HCLK Clock Divided By 16
 *     @arg RCC_ADCHCLK_DIV32 ADCHCLKPRE[3:0] = 1001, HCLK Clock Divided By 32
 *     @arg RCC_ADCHCLK_DIV_OTHERS ADCHCLKPRE[3:0] = others, HCLK Clock Divided By 32
 */
void RCC_ConfigAdcHclk(uint32_t RCC_ADCHCLKPrescaler)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_RCC_ADCHCLKPRE(RCC_ADCHCLKPrescaler));

    tmpregister = RCC->CFG2;
    /* Clear ADCHPRE[3:0] bits */
    tmpregister &= CFG2_ADCHPRES_RESET_MASK;
    /* Set ADCHPRE[3:0] bits according to RCC_ADCHCLKPrescaler value */
    tmpregister |= RCC_ADCHCLKPrescaler;

    /* Store the new value */
    RCC->CFG2 = tmpregister;
}

/**
 * @brief  Configures the LPTIM clock (LPTIMCLK).
 * @param RCC_LPTIMCLKSource specifies the LPTIM clock source.
 *   This parameter can be one of the following values:
 *     @arg RCC_LPTIMCLK_SRC_APB1  APB1 clock selected as LPTIM clock
 *     @arg RCC_LPTIMCLK_SRC_HSI   HSI selected as LPTIM clock
 *     @arg RCC_LPTIMCLK_SRC_HSE   HSE selected as LPTIM clock
 *     @arg RCC_LPTIMCLK_SRC_LSI   LSI selected as LPTIM clock
 *     @arg RCC_LPTIMCLK_SRC_LSE   LSE selected as LPTIM clock
 *     @arg RCC_LPTIMCLK_SRC_COMP1 COMP1 output selected as LPTIM clock
 * @note When switching from comparator1 to other clock sources,
 *       it is suggested to disable comparators first.
 */
void RCC_ConfigLPTIMClk(uint32_t RCC_LPTIMCLKSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM_CLK(RCC_LPTIMCLKSource));

    /* Clear the LPTIM clock source */
    RCC->CFG2 &= RCC_LPTIMCLK_SRC_MASK;

    /* Select the LPTIM clock source */
    RCC->CFG2 |= RCC_LPTIMCLKSource;
}

/**
 * @brief  Returns the clock source used as LPTIM clock.
 * @return The clock source used as system clock. The returned value can
 *   be one of the following:
 *     - RCC_LPTIMCLK_SRC_APB1:   APB1 used as LPTIM clock
 *     - RCC_LPTIMCLK_SRC_HSI:    HSI used as LPTIM clock
 *     - RCC_LPTIMCLK_SRC_HSE:    HSE used as LPTIM clock
 *     - RCC_LPTIMCLK_SRC_LSI:    LSI used as LPTIM clock
 *     - RCC_LPTIMCLK_SRC_LSE:    LSE used as LPTIM clock
 *     - RCC_LPTIMCLK_SRC_COMP1:  COMP1 used as LPTIM clock
 */
uint32_t RCC_GetLPTIMClkSrc(void)
{
    return ((uint32_t)(RCC->CFG2 & CFG2_LPTIMCLK_SET_MASK));
}

/**
 * @brief  Configures the LPUART clock (LPUARTCLK).
 * @param RCC_LPUARTCLKSource specifies the LPUART clock source.
 *   This parameter can be one of the following values:
 *     @arg RCC_LPUARTCLK_SRC_APB1   APB1 clock selected as LPUART clock
 *     @arg RCC_LPUARTCLK_SRC_SYSCLK SYSCLK selected as LPUART clock
 *     @arg RCC_LPUARTCLK_SRC_HSI    HSI selected as LPUART clock
 *     @arg RCC_LPUARTCLK_SRC_HSE    HSE selected as LPUART clock
 *     @arg RCC_LPUARTCLK_SRC_LSI    LSI selected as LPUART clock
 *     @arg RCC_LPUARTCLK_SRC_LSE    LSE selected as LPUART clock
 */
void RCC_ConfigLPUARTClk(uint32_t RCC_LPUARTCLKSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_LPUART_CLK(RCC_LPUARTCLKSource));

    /* LPUART */
    /* Clear the LPUART1 clock source */
    RCC->CFG2 &= RCC_LPUARTCLK_SRC_MASK;

    /* Select the LPTIM clock source */
    RCC->CFG2 |= (RCC_LPUARTCLKSource << 0x19);
}

/**
 * @brief  Returns the clock source used as LPUART clock.
 * @return The clock source used as system clock. The returned value can
 *   be one of the following:
 *     - RCC_LPUARTCLK_SRC_APB1:   APB1 used as LPUART clock
 *     - RCC_LPUARTCLK_SRC_SYSCLK: SYSCLK used as LPUART clock
 *     - RCC_LPUARTCLK_SRC_HSI:    HSI used as LPUART clock
 *     - RCC_LPUARTCLK_SRC_HSE:    HSE used as LPUART clock
 *     - RCC_LPUARTCLK_SRC_LSI:    LSI used as LPUART clock
 *     - RCC_LPUARTCLK_SRC_LSE:    LSE used as LPUART clock
 */
uint32_t RCC_GetLPUARTClkSrc(void)
{
    /* LPUART */
    return ((uint32_t)((RCC->CFG2 & CFG2_LPUARTCLK_SET_MASK) >> 0x19));
}

/**
 * @brief  Configures the External Low Speed oscillator (LSE).
 * @param RCC_LSE specifies the new state of the LSE.
 *   This parameter can be one of the following values:
 *     @arg RCC_LSE_DISABLE LSE oscillator OFF
 *     @arg RCC_LSE_ENABLE LSE oscillator ON
 *     @arg RCC_LSE_BYPASS LSE oscillator bypassed with external clock
 */
void RCC_ConfigLse(uint8_t RCC_LSE)
{
    /* Check the parameters */
    assert_param(IS_RCC_LSE(RCC_LSE));
    /* Reset LSEEN and LSEBYP bits before configuring the LSE ------------------*/
    *(__IO uint32_t*)LSCTRL_ADDR &= (~(RCC_LSCTRL_LSEEN | RCC_LSCTRL_LSEBP));
    /* Configure LSE (RCC_LSE_DISABLE is already covered by the code section above) */
    switch (RCC_LSE)
    {
        case RCC_LSE_ENABLE:
            /* Set LSEON bit */
            *(__IO uint32_t*)LSCTRL_ADDR |= RCC_LSE_ENABLE;
            break;

        case RCC_LSE_BYPASS:
            /* Set LSEBYP and LSEON bits */
            *(__IO uint32_t*)LSCTRL_ADDR |= (RCC_LSE_BYPASS | RCC_LSE_ENABLE);
            break;

        default:
            break;
    }
}

/**
 * @brief  Enables or disables the Internal Low Speed oscillator (LSI).
 * @note   LSI can not be disabled if the IWDG is running.
 * @param Cmd new state of the LSI. This parameter can be: ENABLE or DISABLE.
 */
void RCC_EnableLsi(FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
  
    if(Cmd == ENABLE)
    {
        /* Set LSIEN bit */
        *(__IO uint32_t*)LSCTRL_ADDR |= LSCTRL_LSIEN_SET_MASK;
    }
    else
    {
        /* Reset PLLEN bit */
        *(__IO uint32_t*)LSCTRL_ADDR &= LSCTRL_LSIEN_RESET_MASK;
    }
}

/**
 * @brief  Configures the RTC clock (RTCCLK).
 * @note   Once the RTC clock is selected it can't be changed unless the LowPower domain is reset.
 * @param RCC_RTCCLKSource specifies the RTC clock source.
 *   This parameter can be one of the following values:
 *     @arg RCC_RTCCLK_SRC_NONE:       No clock selected as RTC clock
 *     @arg RCC_RTCCLK_SRC_LSE:        LSE selected as RTC clock
 *     @arg RCC_RTCCLK_SRC_LSI:        LSI selected as RTC clock
 *     @arg RCC_RTCCLK_SRC_HSE_DIV128: HSE clock divided by 32 selected as RTC clock
 */
void RCC_ConfigRtcClk(uint32_t RCC_RTCCLKSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_RTCCLK_SRC(RCC_RTCCLKSource));

    /* Clear the RTC clock source */
    RCC->LSCTRL &= (~RCC_LSCTRL_RTCSEL);

    /* Select the RTC clock source */
    RCC->LSCTRL |= RCC_RTCCLKSource;
}

/**
 * @brief  Returns the clock source used as RTC clock (RTCCLK).
 * @return The clock source used as system clock. The returned value can
 *   be one of the following:
 *     - RCC_RTCCLK_SRC_NONE:       No clock used as RTC clock (RTCCLK)
 *     - RCC_RTCCLK_SRC_LSE:        LSE used as RTC clock (RTCCLK)
 *     - RCC_RTCCLK_SRC_LSI:        LSI used as RTC clock (RTCCLK)
 *     - RCC_RTCCLK_SRC_HSE_DIV128: HSE clock divided by 128 used as RTC clock (RTCCLK)
 */
uint32_t RCC_GetRTCClkSrc(void)
{
    return ((uint32_t)(RCC->LSCTRL & RCC_LSCTRL_RTCSEL));
}

/**
 * @brief  Enables or disables the RTC clock.
 * @note   This function must be used only after the RTC clock was selected using the RCC_ConfigRtcClk function.
 * @param Cmd new state of the RTC clock. This parameter can be: ENABLE or DISABLE.
 */
void RCC_EnableRtcClk(FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if(Cmd == ENABLE)
    {
        /* Set LSIEN bit */
        *(__IO uint32_t*)LSCTRL_ADDR |= LSCTRL_RTCEN_SET_MASK;
    }
    else
    {
        /* Reset PLLEN bit */
        *(__IO uint32_t*)LSCTRL_ADDR &= LSCTRL_RTCEN_RESET_MASK;
    }
}



/**
 * @brief Forces or releases RTC reset.
 * @param Cmd new state of the specified peripheral reset. This parameter can be ENABLE or DISABLE.
 */
void RCC_EnableRTCReset(FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if (Cmd != DISABLE)
    {
        RCC->LSCTRL |= LSCTRL_RTCRST_SET_MASK;
    }
    else
    {
        RCC->LSCTRL &= LSCTRL_RTCRST_RESET_MASK;
    }
}

/**
 * @brief  Configures the LPRUN clock.
 * @param RCC_LPRUNCLKSource specifies the LPRUN clock source.
 *   This parameter can be one of the following values:
 *     @arg RCC_LPRUNCLK_SRC_LSI LSI selected as LPRUN clock
 *     @arg RCC_LPRUNCLK_SRC_LSE LSE selected as LPRUN clock
 */
void RCC_ConfigLPRUNClk(uint32_t RCC_LPRUNCLKSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_LPRUNCLK_SRC(RCC_LPRUNCLKSource));

    /* Clear the LPRUN clock source */
    RCC->LSCTRL &= (~RCC_LSCTRL_LPRUNCLKSEL);

    /* Select the LPRUN clock source */
    RCC->LSCTRL |= RCC_LPRUNCLKSource;
}

/**
 * @brief  Returns the clock source used as LPRUN clock.
 * @return The clock source used as system clock. The returned value can
 *   be one of the following:
 *     - RCC_LPRUNCLK_SRC_LSI: LSI used as LPRUN clock
 *     - RCC_LPRUNCLK_SRC_LSE: LSE used as LPRUN clock
 */
uint32_t RCC_GetLPRUNClkSrc(void)
{
    return ((uint32_t)(RCC->LSCTRL & RCC_LSCTRL_LPRUNCLKSEL));
}

/**
 * @brief  Returns the frequencies of different on chip clocks.
 * @param RCC_Clocks pointer to a RCC_ClocksType structure which will hold
 *         the clocks frequencies.
 * @note   The result of this function could be not correct when using
 *         fractional value for HSE crystal.
 */
void RCC_GetClocksFreqValue(RCC_ClocksType* RCC_Clocks)
{
    uint32_t tmp = 0, pllclk = 0, pllmull = 0, pllpre = 0, plloutdiv = 0, pllsource = 0, presc = 0;

    /* Get PLL clock source, PLL Pre-Divider, PLL out div and multiplication factor ----------------------*/
    pllmull   = RCC->CFG & CFG_PLLMULFCT_MASK;
    pllpre    = RCC->CFG & CFG_PLLPRE_MASK;
    plloutdiv = RCC->CFG & CFG_PLLOUTDIV_MASK;
    pllsource = RCC->CFG & CFG_PLLSRC_MASK;

    pllmull = (pllmull >> 16) + 3;

    if ((pllsource >> 24) == 0x00)
    { 
        /* HSI selected as PLL clock entry */
        pllclk = HSI_VALUE * pllmull;
    }
    else
    {
        /* HSE selected as PLL clock entry */
        pllclk = HSE_VALUE * pllmull;
    }
    
    /* PLL Pre-Divider clock */
    pllpre = (pllpre >> 20) + 1;
    pllclk = pllclk / pllpre;

    /* PLL Out Div clock */
    plloutdiv = (plloutdiv >> 22) + 1;
    pllclk = pllclk / plloutdiv;

    /* Get SYSCLK source -------------------------------------------------------*/
    tmp = RCC->CFG & CFG_SCLKSTS_MASK;

    switch (tmp)
    {
        case RCC_CFG_SCLKSTS_HSI: /* HSI used as system clock */
            RCC_Clocks->SysclkFreq = HSI_VALUE;
            break;
        case RCC_CFG_SCLKSTS_HSE: /* HSE used as system clock */
            RCC_Clocks->SysclkFreq = HSE_VALUE;
            break;
        case RCC_CFG_SCLKSTS_PLL: /* PLL used as system clock */
            RCC_Clocks->SysclkFreq = pllclk;
            break;
        case RCC_CFG_SCLKSTS_LSE: /* LSE used as system clock */
            RCC_Clocks->SysclkFreq = LSE_VALUE;
            break;
        case RCC_CFG_SCLKSTS_LSI: /* LSI used as system clock */
            RCC_Clocks->SysclkFreq = LSI_VALUE;
            break;

        default:
            RCC_Clocks->SysclkFreq = HSI_VALUE;
            break;
    }

    /* Compute HCLK, PCLK1, PCLK2 and ADCCLK clocks frequencies ----------------*/
    /* Get HCLK prescaler */
    tmp   = RCC->CFG & CFG_AHBPRES_SET_MASK;
    tmp   = tmp >> 4;
    if(tmp < 0x8)
    {
        tmp = 0;
    }
    presc = s_ApbAhbPresTable[tmp];
    /* HCLK clock frequency */
    RCC_Clocks->HclkFreq = RCC_Clocks->SysclkFreq >> presc;
    /* Get PCLK1 prescaler */
    tmp   = RCC->CFG & CFG_APB1PRES_SET_MASK;
    tmp   = tmp >> 8;
    presc = s_ApbAhbPresTable[tmp];
    /* PCLK1 clock frequency */
    RCC_Clocks->Pclk1Freq = RCC_Clocks->HclkFreq >> presc;
    /* Get PCLK2 prescaler */
    tmp   = RCC->CFG & CFG_APB2PRES_SET_MASK;
    tmp   = tmp >> 11;
    presc = s_ApbAhbPresTable[tmp];
    /* PCLK2 clock frequency */
    RCC_Clocks->Pclk2Freq = RCC_Clocks->HclkFreq >> presc;

    /* Get ADCHCLK prescaler */
    tmp   = RCC->CFG2 & CFG2_ADCHPRES_SET_MASK;
    presc = s_AdcHclkPresTable[tmp];
    /* ADCHCLK clock frequency */
    RCC_Clocks->AdcHclkFreq = RCC_Clocks->HclkFreq / presc;
    /* Get ADCPLLCLK prescaler */
    tmp   = RCC->CFG2 & CFG2_ADCPLLPRES_SET_MASK;
    tmp   = tmp >> 4;
    presc = s_AdcPllClkPresTable[(tmp & 0xF)]; // ignore BIT5
    /* ADCPLLCLK clock frequency */
    RCC_Clocks->AdcPllClkFreq = pllclk / presc;
}

/**
 * @brief  Enables or disables the AHB peripheral clock.
 * @param RCC_AHBPeriph specifies the AHB peripheral to gates its clock.
 *
 *   this parameter can be any combination of the following values:
 *     @arg RCC_AHB_PERIPH_DMA
 *     @arg RCC_AHB_PERIPH_SRAM
 *     @arg RCC_AHB_PERIPH_FLITF
 *     @arg RCC_AHB_PERIPH_HSQRT
 *     @arg RCC_AHB_PERIPH_CRC
 *     @arg RCC_AHB_PERIPH_HDIV
 *     @arg RCC_AHB_PERIPH_ADC
 *
 * @param Cmd new state of the specified peripheral clock.
 *   This parameter can be: ENABLE or DISABLE.
 */
void RCC_EnableAHBPeriphClk(uint32_t RCC_AHBPeriph, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB_PERIPH(RCC_AHBPeriph));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if (Cmd != DISABLE)
    {
        RCC->AHBPCLKEN |= RCC_AHBPeriph;
    }
    else
    {
        RCC->AHBPCLKEN &= ~RCC_AHBPeriph;
    }
}

/**
 * @brief  Enables or disables the High Speed APB (APB2) peripheral clock.
 * @param RCC_APB2Periph specifies the APB2 peripheral to gates its clock.
 *   This parameter can be any combination of the following values:
 *     @arg RCC_APB2_PERIPH_AFIO, RCC_APB2_PERIPH_GPIOA, RCC_APB2_PERIPH_GPIOB,
 *          RCC_APB2_PERIPH_GPIOC, RCC_APB2_PERIPH_GPIOF, RCC_APB2_PERIPH_SPI1,
 *          RCC_APB2_PERIPH_SPI2, RCC_APB2_PERIPH_TIM1, RCC_APB2_PERIPH_TIM8,
 *          RCC_APB2_PERIPH_USART1
 * @param Cmd new state of the specified peripheral clock.
 *   This parameter can be: ENABLE or DISABLE.
 */
void RCC_EnableAPB2PeriphClk(uint32_t RCC_APB2Periph, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        RCC->APB2PCLKEN |= RCC_APB2Periph;
    }
    else
    {
        RCC->APB2PCLKEN &= ~RCC_APB2Periph;
    }
}

/**
 * @brief  Enables or disables the Low Speed APB (APB1) peripheral clock.
 * @param RCC_APB1Periph specifies the APB1 peripheral to gates its clock.
 *   This parameter can be any combination of the following values:
 *     @arg RCC_APB1_PERIPH_TIM3, RCC_APB1_PERIPH_LPTIMPCLK, RCC_APB1_PERIPH_LPTIM,
 *          RCC_APB1_PERIPH_TIM6, RCC_APB1_PERIPH_BEEP, RCC_APB1_PERIPH_COMP,
 *          RCC_APB1_PERIPH_COMPFILT, RCC_APB1_PERIPH_WWDG, RCC_APB1_PERIPH_USART2,
 *          RCC_APB1_PERIPH_LPUART, RCC_APB1_PERIPH_I2C1, RCC_APB1_PERIPH_I2C2,
 *          RCC_APB1_PERIPH_PWR, RCC_APB1_PERIPH_OPAMP
 *
 * @param Cmd new state of the specified peripheral clock.
 *   This parameter can be: ENABLE or DISABLE.
 */
void RCC_EnableAPB1PeriphClk(uint32_t RCC_APB1Periph, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        RCC->APB1PCLKEN |= RCC_APB1Periph;
    }
    else
    {
        RCC->APB1PCLKEN &= ~RCC_APB1Periph;
    }
}

/**
 * @brief Forces or releases AHB peripheral reset.
 * @param RCC_AHBPeriph specifies the AHB peripheral to reset.
 *   This parameter can be any combination of the following values:
 *     @arg   RCC_AHB_PERIPH_HSQRT.
 *            RCC_AHB_PERIPH_HDIV.
 *            RCC_AHB_PERIPH_ADC.
 * @param Cmd new state of the specified peripheral reset. This parameter can be ENABLE or DISABLE.
 */
void RCC_EnableAHBPeriphReset(uint32_t RCC_AHBPeriph, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB_PERIPH_RESET(RCC_AHBPeriph));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        RCC->AHBPRST |= RCC_AHBPeriph;
    }
    else
    {
        RCC->AHBPRST &= ~RCC_AHBPeriph;
    }
}

/**
 * @brief  Forces or releases High Speed APB (APB2) peripheral reset.
 * @param RCC_APB2Periph specifies the APB2 peripheral to reset.
 *   This parameter can be any combination of the following values:
 *     @arg RCC_APB2_PERIPH_AFIO, RCC_APB2_PERIPH_GPIOA, RCC_APB2_PERIPH_GPIOB,
 *          RCC_APB2_PERIPH_GPIOC, RCC_APB2_PERIPH_GPIOF, RCC_APB2_PERIPH_SPI1,
 *          RCC_APB2_PERIPH_SPI2, RCC_APB2_PERIPH_TIM1, RCC_APB2_PERIPH_TIM8,
 *          RCC_APB2_PERIPH_USART1
 * @param Cmd new state of the specified peripheral reset.
 *   This parameter can be: ENABLE or DISABLE.
 */
void RCC_EnableAPB2PeriphReset(uint32_t RCC_APB2Periph, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        RCC->APB2PRST |= RCC_APB2Periph;
    }
    else
    {
        RCC->APB2PRST &= ~RCC_APB2Periph;
    }
}

/**
 * @brief  Forces or releases Low Speed APB (APB1) peripheral reset.
 * @param RCC_APB1Periph specifies the APB1 peripheral to reset.
 *   This parameter can be any combination of the following values:
 *     @arg RCC_APB1_PERIPH_TIM3, RCC_APB1_PERIPH_LPTIM, RCC_APB1_PERIPH_TIM6,
 *          RCC_APB1_PERIPH_BEEP, RCC_APB1_PERIPH_WWDG, RCC_APB1_PERIPH_USART2,
 *          RCC_APB1_PERIPH_LPUART, RCC_APB1_PERIPH_I2C1, RCC_APB1_PERIPH_I2C2,
 *          RCC_APB1_PERIPH_PWR
 * @param Cmd new state of the specified peripheral clock.
 *   This parameter can be: ENABLE or DISABLE.
 */
void RCC_EnableAPB1PeriphReset(uint32_t RCC_APB1Periph, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB1_PERIPH_RESET(RCC_APB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        RCC->APB1PRST |= RCC_APB1Periph;
    }
    else
    {
        RCC->APB1PRST &= ~RCC_APB1Periph;
    }
}

/**
 * @brief  Enables or disables the Clock Security System.
 * @param Cmd new state of the Clock Security System..
 *   This parameter can be: ENABLE or DISABLE.
 */
void RCC_EnableClockSecuritySystem(FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if (Cmd != DISABLE)
    {
        RCC->CTRL |= CTRL_CLKSSEN_SET;
    }
    else
    {
        RCC->CTRL &= ~CTRL_CLKSSEN_RESET;
    }
}

/**
 * @brief  Configures the MCO PLL clock prescaler.
 * @param RCC_MCOPLLCLKPrescaler specifies the MCO PLL clock prescaler.
 *   This parameter can be on of the following values:
 *     @arg RCC_MCO_CLK_DIV2 MCOPRE[3:0] = 0010, PLL Clock Divided By 2
 *     @arg RCC_MCO_CLK_DIV3 MCOPRE[3:0] = 0011, PLL Clock Divided By 3
 *     @arg RCC_MCO_CLK_DIV4 MCOPRE[3:0] = 0100, PLL Clock Divided By 4
 *     @arg RCC_MCO_CLK_DIV5 MCOPRE[3:0] = 0101, PLL Clock Divided By 5
 *                 ...
 *     @arg RCC_MCO_CLK_DIV13 MCOPRE[3:0] = 1101, PLL Clock Divided By 13
 *     @arg RCC_MCO_CLK_DIV14 MCOPRE[3:0] = 1110, PLL Clock Divided By 14
 *     @arg RCC_MCO_CLK_DIV15 MCOPRE[3:0] = 1111, PLL Clock Divided By 15
 */
void RCC_ConfigMcoClkPre(uint32_t RCC_MCOCLKPrescaler)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_RCC_MCOCLKPRE(RCC_MCOCLKPrescaler));

    tmpregister = RCC->CFG;
    /* Clear MCOPRE[3:0] bits */
    tmpregister &= ((uint32_t)0x0FFFFFFF);
    /* Set MCOPRE[3:0] bits according to RCC_ADCHCLKPrescaler value */
    tmpregister |= RCC_MCOCLKPrescaler;

    /* Store the new value */
    RCC->CFG = tmpregister;
}

/**
 * @brief  Selects the clock source to output on MCO pin.
 * @param RCC_MCO specifies the clock source to output.
 *
 *   this parameter can be one of the following values:
 *     @arg RCC_MCO_NOCLK       No clock selected
 *     @arg RCC_MCO_SYSCLK      System clock selected
 *     @arg RCC_MCO_HSI         HSI oscillator clock selected
 *     @arg RCC_MCO_HSE         HSE oscillator clock selected
 *     @arg RCC_MCO_PLLCLK_PRES PLL clock prescaler
 *
 */
void RCC_ConfigMco(uint8_t RCC_MCO)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_RCC_MCO(RCC_MCO));

    tmpregister = RCC->CFG;
    /* Clear MCO[2:0] bits */
    tmpregister &= (~RCC_CFG_MCO);
    /* Set MCO[2:0] bits according to RCC_MCO value */
    tmpregister |= ((uint32_t)(RCC_MCO << 25));

    /* Store the new value */
    RCC->CFG = tmpregister;
}

/**
 * @brief  Checks whether the specified RCC flag is set or not.
 * @param RCC_FLAG specifies the flag to check.
 *
 *   this parameter can be one of the following values:
 *     @arg RCC_CTRL_FLAG_HSIRDF        HSI oscillator clock ready
 *     @arg RCC_CTRL_FLAG_HSERDF        HSE oscillator clock ready
 *     @arg RCC_CTRL_FLAG_PLLRDF        PLL clock ready
 *     @arg RCC_LSCTRL_FLAG_LSIRD       LSI oscillator clock ready
 *     @arg RCC_LSCTRL_FLAG_LSERD       LSE oscillator clock ready
 *     @arg RCC_CTRLSTS_FLAG_RAMRSTF    RAM reset flag
 *     @arg RCC_CTRLSTS_FLAG_MMURSTF    MMU reset flag
 *     @arg RCC_CTRLSTS_FLAG_PINRSTF    Pin reset
 *     @arg RCC_CTRLSTS_FLAG_PORRSTF    POR reset
 *     @arg RCC_CTRLSTS_FLAG_SFTRSTF    Software reset
 *     @arg RCC_CTRLSTS_FLAG_IWDGRSTF   Independent Watchdog reset
 *     @arg RCC_CTRLSTS_FLAG_WWDGRSTF   Window Watchdog reset
 *     @arg RCC_CTRLSTS_FLAG_LPWRRSTF   Low Power reset
 *     @arg RCC_CTRLSTS_FLAG_EMCGBNRSTF EMCGBN reset
 *     @arg RCC_CTRLSTS_FLAG_EMCGBRSTF  EMCGB reset
 *     @arg RCC_CTRLSTS_FLAG_EMCCLPRSTF EMCCLP reset
 *
 * @return The new state of RCC_FLAG (SET or RESET).
 */
FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG)
{
    uint32_t tmp         = 0;
    uint32_t statusreg   = 0;
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_RCC_FLAG(RCC_FLAG));

    /* Get the RCC register index */
    tmp = RCC_FLAG >> 5;
    if (tmp == 1) /* The flag to check is in CTRL register */
    {
        statusreg = RCC->CTRL;
    }
    else if (tmp == 2) /* The flag to check is in BDCTRL register */
    {
        statusreg = RCC->LSCTRL;
    }
    else /* The flag to check is in CTRLSTS register */
    {
        statusreg = RCC->CTRLSTS;
    }

    /* Get the flag position */
    tmp = RCC_FLAG & FLAG_MASK;
    if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET)
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
 * @brief  Clears the RCC reset flags.
 * @note   The reset flags are: RCC_CTRLSTS_FLAG_RAMRSTF, RCC_CTRLSTS_FLAG_MMURSTF, RCC_CTRLSTS_FLAG_PINRSTF,
 *                              RCC_CTRLSTS_FLAG_PORRSTF, RCC_CTRLSTS_FLAG_SFTRSTF, RCC_CTRLSTS_FLAG_IWDGRSTF,
 *                              RCC_CTRLSTS_FLAG_WWDGRSTF, RCC_CTRLSTS_FLAG_LPWRRSTF, RCC_CTRLSTS_FLAG_EMCGBNRSTF,
 *                              RCC_CTRLSTS_FLAG_EMCGBRSTF, RCC_CTRLSTS_FLAG_EMCCLPRSTF
 */
void RCC_ClrFlag(void)
{
    /* Set RMVF bit to clear the reset flags */
    RCC->CTRLSTS |= CSR_RMRSTF_SET;
    /* RMVF bit should be reset */
    RCC->CTRLSTS &= CSR_RMRSTF_Reset;
}

/**
 * @brief  Checks whether the specified RCC interrupt has occurred or not.
 * @param RccInt specifies the RCC interrupt source to check.
 *
 *   this parameter can be one of the following values:
 *     @arg RCC_INT_LSIRDIF LSI ready interrupt
 *     @arg RCC_INT_LSERDIF LSE ready interrupt
 *     @arg RCC_INT_HSIRDIF HSI ready interrupt
 *     @arg RCC_INT_HSERDIF HSE ready interrupt
 *     @arg RCC_INT_PLLRDIF PLL ready interrupt
 *     @arg RCC_INT_RAMCPIF RAM parity error interrupt
 *     @arg RCC_INT_CLKSSIF Clock Security System interrupt
 *
 * @return The new state of RccInt (SET or RESET).
 */
INTStatus RCC_GetIntStatus(uint8_t RccInt)
{
    INTStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_RCC_GET_INT(RccInt));

    /* Check the status of the specified RCC interrupt */
    if ((RCC->CLKINT & RccInt) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    /* Return the RccInt status */
    return bitstatus;
}

/**
 * @brief  Clears the RCC's interrupt pending bits.
 * @param RccInt specifies the interrupt pending bit to clear.
 *
 *   this parameter can be any combination of the
 *   following values:
 *     @arg RCC_CLR_LSIRDIF Clear LSI ready interrupt flag
 *     @arg RCC_CLR_LSERDIF Clear LSE ready interrupt flag
 *     @arg RCC_CLR_HSIRDIF Clear HSI ready interrupt flag
 *     @arg RCC_CLR_HSERDIF Clear HSE ready interrupt flag
 *     @arg RCC_CLR_PLLRDIF Clear PLL ready interrupt flag
 *     @arg RCC_CLR_RAMCPIF Clear RAM parity error interrupt flag
 *     @arg RCC_CLR_CLKSSIF Clear Clock Security System interrupt flag
 */
void RCC_ClrIntPendingBit(uint32_t RccClrInt)
{
    /* Check the parameters */
    assert_param(IS_RCC_CLR_INTF(RccClrInt));
   /* Software set this bit to clear INT flag. */
    RCC->CLKINT |= RccClrInt;
}


/**
 * @brief  Enables or disables Reset when RAMC detects a parity error.
 * @note   The PLL can not be disabled if it is used as system clock.
 * @param Cmd new state of the PLL. This parameter can be: ENABLE or DISABLE.
 */
void RCC_EnableRAMCParityErrorReset(FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if(Cmd == ENABLE)
   {
       /* Set RAMCERRRST bit */
       RCC->CLKINT |= CLKINT_RAMCERRRST_SET_MASK;
   }
   else
   {
       /* Reset RAMCERRRST bit */
       RCC->CLKINT &= CLKINT_RAMCERRRST_RESET_MASK;
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
