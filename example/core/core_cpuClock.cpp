/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : cat_li
  * @version: V2.0
  * @date   : 2017/12/17
  * @brief   ebox application example .
  *	core API中关于修改CPU频率,以及延时部分。
	*	1  通过修改不同的CPU频率，可以看到启动速度，以及us延时的影响
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */
#include "ebox.h"
#include "bsp_ebox.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32 cpuClock example"
#define EXAMPLE_DATE	"2017-12-31"

#if (STM32_TYPE < STM32F098VC)

#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_rcc.h"

// 通过不同的组合可以设置不同的工作频率，默认工作在48M,无需SystemClock_Config函数
#define	CPUCLOCK96M		LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_1
#define CPUCLOCK72M		LL_RCC_PLL_MUL_9 , LL_RCC_PREDIV_DIV_1
#define CPUCLOCK48M		LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_2

#define CPUCLOCK			CPUCLOCK48M

void SystemClock_Config(void)
{
	/* Configuration will allow to reach a SYSCLK frequency set to 24MHz:
	 Syst freq = ((HSI_VALUE * PLLMUL)/ PLLDIV)
							 ((8MHz * 12)/ 2)                  = 48MHz             */
	LL_UTILS_PLLInitTypeDef sUTILS_PLLInitStruct = {CPUCLOCK};
	/* Variable to store AHB and APB buses clock configuration */
	/* Settings to have HCLK set to 48MHz and APB to 48 MHz */
	LL_UTILS_ClkInitTypeDef sUTILS_ClkInitStruct = {LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_1};

	/* Switch to PLL with HSI as clock source             */
	LL_PLL_ConfigSystemClock_HSI(&sUTILS_PLLInitStruct, &sUTILS_ClkInitStruct);
}
#else
//void SystemClock_Config(void)
//{
//  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
//  
//  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
//  /* Enable HSE */    
//  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
// 
//  /* Wait till HSE is ready and if Time out is reached exit */
//  do
//  {
//    HSEStatus = RCC->CR & RCC_CR_HSERDY;
//    StartUpCounter++;  
//  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

//  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
//  {
//    HSEStatus = (uint32_t)0x01;
//  }
//  else
//  {
//    HSEStatus = (uint32_t)0x00;
//  }  

//  if (HSEStatus == (uint32_t)0x01)
//  {
//#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL && !defined STM32F10X_HD_VL 
//    /* Enable Prefetch Buffer */
//    FLASH->ACR |= FLASH_ACR_PRFTBE;

//    /* Flash 0 wait state */
//    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
//    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;    
//#endif
// 
//    /* HCLK = SYSCLK */
//    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
//      
//    /* PCLK2 = HCLK */
//    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
//    
//    /* PCLK1 = HCLK */
//    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
//    
//#ifdef STM32F10X_CL
//    /* Configure PLLs ------------------------------------------------------*/
//    /* PLL configuration: PLLCLK = PREDIV1 * 6 = 24 MHz */ 
//    RCC->CFGR &= (uint32_t)~(RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
//    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLSRC_PREDIV1 | 
//                            RCC_CFGR_PLLMULL6); 

//    /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
//    /* PREDIV1 configuration: PREDIV1CLK = PLL2 / 10 = 4 MHz */       
//    RCC->CFGR2 &= (uint32_t)~(RCC_CFGR2_PREDIV2 | RCC_CFGR2_PLL2MUL |
//                              RCC_CFGR2_PREDIV1 | RCC_CFGR2_PREDIV1SRC);
//    RCC->CFGR2 |= (uint32_t)(RCC_CFGR2_PREDIV2_DIV5 | RCC_CFGR2_PLL2MUL8 |
//                             RCC_CFGR2_PREDIV1SRC_PLL2 | RCC_CFGR2_PREDIV1_DIV10);
//  
//    /* Enable PLL2 */
//    RCC->CR |= RCC_CR_PLL2ON;
//    /* Wait till PLL2 is ready */
//    while((RCC->CR & RCC_CR_PLL2RDY) == 0)
//    {
//    }   
//#elif defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
//    /*  PLL configuration:  = (HSE / 2) * 6 = 24 MHz */
//    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
//    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLXTPRE_PREDIV1_Div2 | RCC_CFGR_PLLMULL6);
//#else    
//    /*  PLL configuration:  = (HSE / 2) * 6 = 24 MHz */
//    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
//    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLXTPRE_HSE_Div2 | RCC_CFGR_PLLMULL6);
//#endif /* STM32F10X_CL */

//    /* Enable PLL */
//    RCC->CR |= RCC_CR_PLLON;

//    /* Wait till PLL is ready */
//    while((RCC->CR & RCC_CR_PLLRDY) == 0)
//    {
//    }

//    /* Select PLL as system clock source */
//    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
//    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

//    /* Wait till PLL is used as system clock source */
//    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
//    {
//    }
//  }
//  else
//  { /* If HSE fails to start-up, the application will have wrong clock 
//         configuration. User can add here some code to deal with this error */
//  }
//}
#endif

void setup()
{
    ebox_init();
	UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    LED1.mode(OUTPUT_PP);
}

void testUs()
{
	uint8_t i = 255;
	while(i--){
				LED1.toggle();
			delay_us(1);
	}
}

void testMs()
{
	uint8_t i = 20;
	while(i--){
				LED1.toggle();
			delay_ms(1);
	}
}

int main(void){
		uint32_t t;
    setup();
		UART.printf("启动耗时：%d us,  %d ms \r\n",micros(),millis());
    while (1)
    {
			 t = micros();
			 testUs();
			 t = micros() - t;
			 UART.printf("testUs 耗时：%dus \r\n",t);
			 // 使用millis_seconds,减少调用消耗
			 t = millis_seconds;
			 testMs();
			 t = millis_seconds - t;
			 UART.printf("testMs 耗时：%dms \r\n",t);
             delay_ms(5000);
    }
}
