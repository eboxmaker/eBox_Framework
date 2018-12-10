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

/**
	*	1 此例程演示了如何修改mcu的时钟，以及在不同频率下cpu的运算能力变化
    *   2 通过SystemClock_Config可以根据需要修改mcu时钟源，该函数系统会自动调用，无需手动调用
    *   3 f0可以通过st官方的cubeMx软件生成，在project->setting->advanced setting,选择LL即可
    *   4 f1系列因为使用标准库，无法使用cubeMx软件生成，system_stm32f10x.c中有些默认配置，可以
    *     通过宏定义选择
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32 cpuClock example"
#define EXAMPLE_DATE	"2017-12-31"

#ifdef F0
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_rcc.h"

// 通过不同的组合可以设置不同的工作频率，默认工作在48M,无需SystemClock_Config函数
#define	CPUCLOCK96M		LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_1
#define CPUCLOCK72M		LL_RCC_PLL_MUL_9 ,  LL_RCC_PREDIV_DIV_1
#define CPUCLOCK48M		LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_2
#define CPUCLOCK24M		LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_4
#define CPUCLOCK16M		LL_RCC_PLL_MUL_2 , LL_RCC_PREDIV_DIV_1
#define CPUCLOCK08M		LL_RCC_PLL_MUL_2 , LL_RCC_PREDIV_DIV_2

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
void SystemClock_Config(void)
{
    // f1系列可以直接修改system_stm32f10x.c中的宏定义修改频率，也可以在这里定义代码
}
#endif

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    LED1.mode(OUTPUT_PP);
}

void testUs()
{
    uint8_t i = 255;
    while (i--)
    {
        LED1.toggle();
        delay_us(1);
    }
}

void testMs()
{
    uint8_t i = 20;
    while (i--)
    {
        LED1.toggle();
        delay_ms(1);
    }
}

int main(void)
{
    uint32_t t;
    setup();
    UART.printf("启动耗时：%d us,  %d ms \r\n", micros(), millis());
    while (1)
    {
        t = micros();
        testUs();
        t = micros() - t;
        UART.printf("testUs 耗时：%dus \r\n", t);
        // 使用millis_seconds,减少调用消耗
        t = millis_seconds;
        testMs();
        t = millis_seconds - t;
        UART.printf("testMs 耗时：%dms \r\n", t);
        delay_ms(5000);
    }
}
