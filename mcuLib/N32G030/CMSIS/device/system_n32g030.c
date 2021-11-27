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
 * @file system_n32g030.c
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g030.h"

/* Uncomment the line corresponding to the desired System clock (SYSCLK)
   frequency (after reset the HSI is used as SYSCLK source)

   IMPORTANT NOTE:
   ==============
   1. After each device reset the HSI is used as System clock source.

   2. Please make sure that the selected System clock doesn't exceed your
   device's maximum frequency.

   3. If none of the define below is enabled, the HSI is used as System clock
    source.

   4. The System clock configuration functions provided within this file assume
   that:
        - For Low and High density Value line devices an external 8MHz
          crystal is used to drive the System clock.
        - For Low and High density devices an external 8MHz crystal is
          used to drive the System clock.
        - For Connectivity line devices an external 25MHz crystal is used to
   drive the System clock. If you are using different crystal you have to adapt
   those functions accordingly.
    */

#define SYSCLK_USE_HSI     0
#define SYSCLK_USE_HSE     1
#define SYSCLK_USE_HSI_PLL 2
#define SYSCLK_USE_HSE_PLL 3
#define SYSCLK_USE_LSE     4
#define SYSCLK_USE_LSI     5

#ifndef SYSCLK_FREQ
#define SYSCLK_FREQ 48000000
#endif

/*
* SYSCLK_SRC *
** SYSCLK_USE_HSI     **
** SYSCLK_USE_HSE     **
** SYSCLK_USE_HSI_PLL **
** SYSCLK_USE_HSE_PLL **
** SYSCLK_USE_LSE     **
** SYSCLK_USE_LSI     **
*/
#ifndef SYSCLK_SRC
#define SYSCLK_SRC SYSCLK_USE_HSI_PLL
#endif

#if SYSCLK_SRC == SYSCLK_USE_HSI

    #ifndef HSI_VALUE
        #error HSI_VALUE must be defined!
    #endif

    #if SYSCLK_FREQ != HSI_VALUE
        #error SYSCL_FREQ must be set to HSI_VALUE
    #endif

#elif SYSCLK_SRC == SYSCLK_USE_HSE

    #ifndef HSE_VALUE
        #error HSE_VALUE must be defined!
    #endif

    #if SYSCLK_FREQ != HSE_VALUE
        #error SYSCL_FREQ must be set to HSE_VALUE
    #endif

#elif SYSCLK_SRC == SYSCLK_USE_HSI_PLL
    
    #ifndef HSI_VALUE
        #error HSI_VALUE must be defined!
    #endif
    
    #if ( (((SYSCLK_FREQ * 1) / HSI_VALUE) >= 3) && (((SYSCLK_FREQ * 1) / HSI_VALUE) <= 18) && (((SYSCLK_FREQ * 1) % HSI_VALUE) == 0) )
        #define PLL_MUL    (SYSCLK_FREQ / HSI_VALUE)
        #define PLL_PRE    0
        #define PLLOUT_DIV 0
    #else
        #if ( (((SYSCLK_FREQ * 2) / HSI_VALUE) >= 3) && (((SYSCLK_FREQ * 2) / HSI_VALUE) <= 18) && (((SYSCLK_FREQ * 2) % HSI_VALUE) == 0) )
            #define PLL_MUL    ((SYSCLK_FREQ * 2) / HSI_VALUE)
            #define PLL_PRE    1
            #define PLLOUT_DIV 0
        #else
            #if ( (((SYSCLK_FREQ * 3) / HSI_VALUE) >= 3) && (((SYSCLK_FREQ * 3) / HSI_VALUE) <= 18) && (((SYSCLK_FREQ * 3) % HSI_VALUE) == 0) )
                #define PLL_MUL    ((SYSCLK_FREQ * 3) / HSI_VALUE)
                #define PLL_PRE    0
                #define PLLOUT_DIV 2
            #else
                #if ( (((SYSCLK_FREQ * 4) / HSI_VALUE) >= 3) && (((SYSCLK_FREQ * 4) / HSI_VALUE) <= 18) && (((SYSCLK_FREQ * 4) % HSI_VALUE) == 0) )
                    #define PLL_MUL    ((SYSCLK_FREQ * 4) / HSI_VALUE)
                    #define PLL_PRE    1
                    #define PLLOUT_DIV 1
                #else
                    #if ( (((SYSCLK_FREQ * 6) / HSI_VALUE) >= 3) && (((SYSCLK_FREQ * 6) / HSI_VALUE) <= 18) && (((SYSCLK_FREQ * 6) % HSI_VALUE) == 0) )
                        #define PLL_MUL    ((SYSCLK_FREQ * 6) / HSI_VALUE)
                        #define PLL_PRE    1
                        #define PLLOUT_DIV 2
                    #else
                        #if ( (((SYSCLK_FREQ * 8) / HSI_VALUE) >= 3) && (((SYSCLK_FREQ * 8) / HSI_VALUE) <= 18) && (((SYSCLK_FREQ * 8) % HSI_VALUE) == 0) )
                            #define PLL_MUL    ((SYSCLK_FREQ * 8) / HSI_VALUE)
                            #define PLL_PRE    2
                            #define PLLOUT_DIV 2
                        #else
                            #error Cannot make a PLL multiply factor to SYSCLK_FREQ.
                        #endif
                    #endif
                #endif
            #endif
        #endif
    #endif

#elif SYSCLK_SRC == SYSCLK_USE_HSE_PLL

    #ifndef HSE_VALUE
        #error HSE_VALUE must be defined!
    #endif

    #if ( (((SYSCLK_FREQ * 1) / HSE_VALUE) >= 3) && (((SYSCLK_FREQ * 1) / HSE_VALUE) <= 18) && (((SYSCLK_FREQ * 1) % HSE_VALUE) == 0) )
        #define PLL_MUL    (SYSCLK_FREQ / HSE_VALUE)
        #define PLL_PRE    0
        #define PLLOUT_DIV 0
    #else
        #if ( (((SYSCLK_FREQ * 2) / HSE_VALUE) >= 3) && (((SYSCLK_FREQ * 2) / HSE_VALUE) <= 18) && (((SYSCLK_FREQ * 2) % HSE_VALUE) == 0) )
            #define PLL_MUL    ((SYSCLK_FREQ * 2) / HSE_VALUE)
            #define PLL_PRE    1
            #define PLLOUT_DIV 0
        #else
            #if ( (((SYSCLK_FREQ * 3) / HSE_VALUE) >= 3) && (((SYSCLK_FREQ * 3) / HSE_VALUE) <= 18) && (((SYSCLK_FREQ * 3) % HSE_VALUE) == 0) )
                #define PLL_MUL    ((SYSCLK_FREQ * 3) / HSE_VALUE)
                #define PLL_PRE    0
                #define PLLOUT_DIV 2
            #else
                #if ( (((SYSCLK_FREQ * 4) / HSE_VALUE) >= 3) && (((SYSCLK_FREQ * 4) / HSE_VALUE) <= 18) && (((SYSCLK_FREQ * 4) % HSE_VALUE) == 0) )
                    #define PLL_MUL    ((SYSCLK_FREQ * 4) / HSE_VALUE)
                    #define PLL_PRE    1
                    #define PLLOUT_DIV 1
                #else
                    #if ( (((SYSCLK_FREQ * 6) / HSE_VALUE) >= 3) && (((SYSCLK_FREQ * 6) / HSE_VALUE) <= 18) && (((SYSCLK_FREQ * 6) % HSE_VALUE) == 0) )
                        #define PLL_MUL    ((SYSCLK_FREQ * 6) / HSE_VALUE)
                        #define PLL_PRE    1
                        #define PLLOUT_DIV 2
                    #else
                        #if ( (((SYSCLK_FREQ * 8) / HSE_VALUE) >= 3) && (((SYSCLK_FREQ * 8) / HSE_VALUE) <= 18) && (((SYSCLK_FREQ * 8) % HSE_VALUE) == 0) )
                            #define PLL_MUL    ((SYSCLK_FREQ * 8) / HSE_VALUE)
                            #define PLL_PRE    2
                            #define PLLOUT_DIV 2
                        #else
                            #error Cannot make a PLL multiply factor to SYSCLK_FREQ.
                        #endif
                    #endif
                #endif
            #endif
        #endif
    #endif

#elif SYSCLK_SRC == SYSCLK_USE_LSE

    #ifndef LSE_VALUE
        #error LSE_VALUE must be defined!
    #endif

    #if SYSCLK_FREQ != LSE_VALUE
        #error SYSCL_FREQ must be set to LSE_VALUE
    #endif

#elif SYSCLK_SRC == SYSCLK_USE_LSI

    #ifndef LSI_VALUE
        #error LSI_VALUE must be defined!
    #endif

    #if SYSCLK_FREQ != LSI_VALUE
        #error SYSCL_FREQ must be set to LSI_VALUE
    #endif

#else
#error wrong value for SYSCLK_SRC
#endif

/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET 0x0 /*!< Vector Table base offset field. This value must be a multiple of 0x200. */

/*******************************************************************************
 *  Clock Definitions
 *******************************************************************************/
uint32_t SystemCoreClock = SYSCLK_FREQ; /*!< System Clock Frequency (Core Clock) */

const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

static void SetSysClock(void);

#ifdef DATA_IN_ExtSRAM
static void SystemInit_ExtMemCtl(void);
#endif /* DATA_IN_ExtSRAM */

/**
 * @brief  Setup the microcontroller system
 *         Initialize the Embedded Flash Interface, the PLL and update the
 *         SystemCoreClock variable.
 * @note   This function should be used only after reset.
 */
void SystemInit(void)
{
    /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
    /* Set HSIEN bit */
    RCC->CTRL |= (uint32_t)0x00000001;

    /* Reset SCLKSW, AHBPRE, APB1PRES, APB2PRES, MCO and MCOPRES bits */
    RCC->CFG &= (uint32_t)0x21FF0000;

    /* Reset HSEON, CLKSSEN and PLLEN bits */
    RCC->CTRL &= (uint32_t)0xFEF6FFFF;
  
    /* Reset PLLBP bit */
    RCC->CTRL &= (uint32_t)0xFFBFFFFF;

    /* Reset PLLMULFCT, PLLPRE, PLLOUTDIV and PLLSRC bits */
    RCC->CFG &= (uint32_t)0xFE00FFFF;

    /* Reset CFG2 register */
    RCC->CFG2 = 0x00003800;

    /* Disable all interrupts and clear pending bits  */
    RCC->CLKINT = 0x00BF0000;

    /* Enable Prefetch Buffer */
    FLASH->AC |= (uint32_t)FLASH_AC_PRFTBFEN;
    
    /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
    /* Configure the Flash Latency cycles and enable prefetch buffer */
    SetSysClock();
}

/**
 * @brief  Update SystemCoreClock variable according to Clock Register Values.
 *         The SystemCoreClock variable contains the core clock (HCLK), it can
 *         be used by the user application to setup the SysTick timer or
 * configure other parameters.
 *
 * @note   Each time the core clock (HCLK) changes, this function must be called
 *         to update SystemCoreClock variable value. Otherwise, any
 * configuration based on this variable will be incorrect.
 *
 * @note   - The system frequency computed by this function is not the real
 *           frequency in the chip. It is calculated based on the predefined
 *           constant and the selected clock source:
 *
 *           - If SYSCLK source is HSI, SystemCoreClock will contain the
 * HSI_VALUE(*)
 *
 *           - If SYSCLK source is HSE, SystemCoreClock will contain the
 * HSE_VALUE(**)
 *
 *           - If SYSCLK source is PLL, SystemCoreClock will contain the
 * HSE_VALUE(***) or HSI_VALUE(**) multiplied by the PLL factors.
 *
 *           - If SYSCLK source is LSE, SystemCoreClock will contain the
 * LSE_VALUE(****)
 *
 *           - If SYSCLK source is LSI, SystemCoreClock will contain the
 * LSI_VALUE(*****)
 *
 *         (*) HSI_VALUE is a constant defined in n32g030.h file (default value
 *             8 MHz) but the real value may vary depending on the variations
 *             in voltage and temperature.
 *
 *         (**) HSE_VALUE is a constant defined in n32g030.h file (default value
 *              8 MHz or 25 MHz, depedning on the product used), user has to
 *              ensure that HSE_VALUE is same as the real frequency of the crystal used.
 *              Otherwise, this function may have wrong result.
 *
 *         (****) LSE_VALUE is a constant defined in n32g030.h file (default value
 *              32.768KHz, depedning on the product used), user has to
 *              ensure that LSE_VALUE is same as the real frequency of the crystal used.
 *              Otherwise, this function may have wrong result.
 *
 *         (*****) LSE_VALUE is a constant defined in n32g030.h file (default value
 *             30KHz) but the real value may vary depending on the variations
 *             in voltage and temperature.
 *
 *         - The result of this function could be not correct when using
 * fractional value for HSE crystal.
 */
void SystemCoreClockUpdate(void)
{
    uint32_t tmp = 0, pllmull = 0, pllpre = 0, plloutdiv = 0, pllsource = 0;

    /* Get SYSCLK source
     * -------------------------------------------------------*/
    tmp = RCC->CFG & RCC_CFG_SCLKSTS;

    switch (tmp)
    {
        case RCC_CFG_SCLKSTS_HSI: /* HSI used as system clock */
            SystemCoreClock = HSI_VALUE;
            break;
        case RCC_CFG_SCLKSTS_HSE: /* HSE used as system clock */
            SystemCoreClock = HSE_VALUE;
            break;
        case RCC_CFG_SCLKSTS_PLL: /* PLL used as system clock */

            /* Get PLL clock source, PLL Pre-Divider, PLL out div and multiplication factor ----------------------*/
            pllmull   = RCC->CFG & RCC_CFG_PLLMULFCT;
            pllpre    = RCC->CFG & RCC_CFG_PLLPRE;
            plloutdiv = RCC->CFG & RCC_CFG_PLLOUTDIV;
        
            pllmull = (pllmull >> 16) + 3;
        
            if ((pllsource >> 24) == 0x00)
            { 
                /* HSI selected as PLL clock entry */
                SystemCoreClock = HSI_VALUE * pllmull;
            }
            else
            {
                /* HSE selected as PLL clock entry */
                SystemCoreClock = HSE_VALUE * pllmull;
            }
            
            /* PLL Pre-Divider clock */
            pllpre = (pllpre >> 20) + 1;
            SystemCoreClock = SystemCoreClock / pllpre;

            /* PLL Out Div clock */
            plloutdiv = (plloutdiv >> 22) + 1;
            SystemCoreClock = SystemCoreClock / plloutdiv;
            break;
        case RCC_CFG_SCLKSTS_LSE: /* LSE used as system clock */
            SystemCoreClock = LSE_VALUE;
            break;
        case RCC_CFG_SCLKSTS_LSI: /* LSI used as system clock */
            SystemCoreClock = LSI_VALUE;
            break;

        default:
            SystemCoreClock = HSI_VALUE;
            break;
    }

    /* Compute HCLK clock frequency ----------------*/
    /* Get HCLK prescaler */
    tmp = AHBPrescTable[((RCC->CFG & RCC_CFG_AHBPRES) >> 4)];
    /* HCLK clock frequency */
    SystemCoreClock >>= tmp;
}

/**
 * @brief  Configures the System clock frequency, HCLK, PCLK2 and PCLK1
 * prescalers.
 */
static void SetSysClock(void)
{
    uint32_t rcc_cfg        = 0;
    uint32_t StartUpCounter = 0;

#if ((SYSCLK_SRC == SYSCLK_USE_HSI) || (SYSCLK_SRC == SYSCLK_USE_HSI_PLL))

    bool HSIStatus = 0;
    /* Enable HSI */
    RCC->CTRL |= ((uint32_t)RCC_CTRL_HSIEN);

    /* Wait till HSI is ready and if Time out is reached exit */
    do
    {
        HSIStatus = RCC->CTRL & RCC_CTRL_HSIRDF;
        StartUpCounter++;
    } while ((HSIStatus == 0) && (StartUpCounter != HSI_STARTUP_TIMEOUT));

    HSIStatus = ((RCC->CTRL & RCC_CTRL_HSIRDF) != RESET);
    if (!HSIStatus)
    {
        /* If HSI fails to start-up, the application will have wrong clock
         * configuration. User can add here some code to deal with this error */
        while(1);
    }

#elif ((SYSCLK_SRC == SYSCLK_USE_HSE) || (SYSCLK_SRC == SYSCLK_USE_HSE_PLL))

    bool HSEStatus = 0;
    /* Enable HSE */
    RCC->CTRL |= (uint32_t)RCC_CTRL_HSEEN;

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC->CTRL & RCC_CTRL_HSERDF;
        StartUpCounter++;
    } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    HSEStatus = ((RCC->CTRL & RCC_CTRL_HSERDF) != RESET);
    if (!HSEStatus)
    {
        /* If HSE fails to start-up, the application will have wrong clock
         * configuration. User can add here some code to deal with this error */
        SystemCoreClock = HSI_VALUE;
        return;
    }

#elif (SYSCLK_SRC == SYSCLK_USE_LSE)

    bool LSEStatus = 0;
    /* Enable LSE */
    RCC->LSCTRL |= ((uint32_t)RCC_LSCTRL_LSEEN);

    /* Wait till LSE is ready and if Time out is reached exit */
    do
    {
        LSEStatus = RCC->LSCTRL & RCC_LSCTRL_LSERD;
        StartUpCounter++;
    } while ((LSEStatus == 0) && (StartUpCounter != LSE_STARTUP_TIMEOUT));

    LSEStatus = ((RCC->LSCTRL & RCC_LSCTRL_LSERD) != RESET);
    if (!LSEStatus)
    {
        /* If LSE fails to start-up, the application will have wrong clock
         * configuration. User can add here some code to deal with this error */
        SystemCoreClock = HSI_VALUE;
        return;
    }

#elif (SYSCLK_SRC == SYSCLK_USE_LSI)

    bool LSIStatus = 0;
    /* Enable LSI */
    RCC->LSCTRL |= ((uint32_t)RCC_LSCTRL_LSIEN);

    /* Wait till LSI is ready and if Time out is reached exit */
    do
    {
        LSIStatus = RCC->LSCTRL & RCC_LSCTRL_LSERD;
        StartUpCounter++;
    } while ((LSIStatus == 0) && (StartUpCounter != LSI_STARTUP_TIMEOUT));

    LSIStatus = ((RCC->LSCTRL & RCC_LSCTRL_LSIRD) != RESET);
    if (!LSIStatus)
    {
        /* If LSI fails to start-up, the application will have wrong clock
         * configuration. User can add here some code to deal with this error */
        SystemCoreClock = HSI_VALUE;
        return;
    }

#endif

    /* Flash wait state
        0: HCLK <= 18M
        1: HCLK <= 36M
        2: HCLK <= 48M
     */
    FLASH->AC &= (uint32_t)((uint32_t)~FLASH_AC_LATENCY);
    FLASH->AC |= (uint32_t)((SYSCLK_FREQ - 1) / 18000000);

    /* HCLK = SYSCLK */
    RCC->CFG |= (uint32_t)RCC_CFG_AHBPRES_DIV1;

    /* PCLK2 max 48M */
    /* PCLK1 max 48M */
    
#if SYSCLK_SRC == SYSCLK_USE_HSI
    /* Select HSI as system clock source */
    RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_SCLKSW));
    RCC->CFG |= (uint32_t)RCC_CFG_SCLKSW_HSI;

    /* Wait till HSI is used as system clock source */
    while ((RCC->CFG & (uint32_t)RCC_CFG_SCLKSTS) != RCC_CFG_SCLKSTS_HSI)
    {
    }
#elif SYSCLK_SRC == SYSCLK_USE_HSE
    /* Select HSE as system clock source */
    RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_SCLKSW));
    RCC->CFG |= (uint32_t)RCC_CFG_SCLKSW_HSE;

    /* Wait till HSE is used as system clock source */
    while ((RCC->CFG & (uint32_t)RCC_CFG_SCLKSTS) != RCC_CFG_SCLKSTS_HSE)
    {
    }
#elif SYSCLK_SRC == SYSCLK_USE_HSI_PLL || SYSCLK_SRC == SYSCLK_USE_HSE_PLL

    /* clear bits */
    /* Clear PLLOUTDIV PLLPRE bits */
    RCC->CFG &= (~RCC_CFG_PLLOUTDIV);
    
    RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_PLLSRC | RCC_CFG_PLLPRE | RCC_CFG_PLLMULFCT));

    /* set PLL source */
    rcc_cfg = RCC->CFG;
    rcc_cfg |= (SYSCLK_SRC == SYSCLK_USE_HSI_PLL ? RCC_CFG_PLLSRC_HSI : RCC_CFG_PLLSRC_HSE);

    /* set PLL multiply factor */
    rcc_cfg |= (((uint32_t)PLL_MUL - 3) << 16);
    /* set PLL Pre-Divider factor */
    rcc_cfg |= ((uint32_t)PLL_PRE << 20);
    /* set PLL output clock divider factor */
    rcc_cfg |= ((uint32_t)PLLOUT_DIV << 22);

    RCC->CFG = rcc_cfg;

    /* Enable PLL */
    RCC->CTRL |= RCC_CTRL_PLLEN;

    /* Wait till PLL is ready */
    while ((RCC->CTRL & RCC_CTRL_PLLRDF) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_SCLKSW));
    RCC->CFG |= (uint32_t)RCC_CFG_SCLKSW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFG & (uint32_t)RCC_CFG_SCLKSTS) != (uint32_t)RCC_CFG_SCLKSTS_PLL)
    {
    }
#elif SYSCLK_SRC == SYSCLK_USE_LSE
    /* Select LSE as system clock source */
    RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_SCLKSW));
    RCC->CFG |= (uint32_t)RCC_CFG_SCLKSW_LSE;

    /* Wait till LSE is used as system clock source */
    while ((RCC->CFG & (uint32_t)RCC_CFG_SCLKSTS) != RCC_CFG_SCLKSTS_LSE)
    {
    }
#elif SYSCLK_SRC == SYSCLK_USE_LSI
    /* Select LSI as system clock source */
    RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_SCLKSW));
    RCC->CFG |= (uint32_t)RCC_CFG_SCLKSW_LSI;

    /* Wait till LSI is used as system clock source */
    while ((RCC->CFG & (uint32_t)RCC_CFG_SCLKSTS) != RCC_CFG_SCLKSTS_LSI)
    {
    }
#endif
}
