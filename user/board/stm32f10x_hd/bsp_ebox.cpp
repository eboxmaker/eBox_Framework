/**
  ******************************************************************************
  * @file    bsp_ebox.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2018/07/31
  * @brief   硬件相关信息声明
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "bsp_ebox.h"
#include "ebox_mem.h"

// 如果不使用object.cpp, 使用下列方式创建IO对象
#if USE_OBJECT == 0
mcuGpio PA0(PA0_ID);
mcuGpio PA1(PA1_ID);
mcuGpio PA2(PA2_ID);
mcuGpio PA3(PA3_ID);
mcuGpio PA4(PA4_ID);
mcuGpio PA5(PA5_ID);
mcuGpio PA6(PA6_ID);
mcuGpio PA7(PA7_ID);
mcuGpio PA8(PA8_ID);
mcuGpio PA9(PA9_ID);
mcuGpio PA10(PA10_ID);
mcuGpio PA11(PA11_ID);
mcuGpio PA12(PA12_ID);
mcuGpio PA13(PA13_ID);
mcuGpio PA14(PA14_ID);
mcuGpio PA15(PA15_ID);

mcuGpio PB0(PB0_ID);
mcuGpio PB1(PB1_ID);
mcuGpio PB2(PB2_ID);
mcuGpio PB3(PB3_ID);
mcuGpio PB4(PB4_ID);
mcuGpio PB5(PB5_ID);
mcuGpio PB6(PB6_ID);
mcuGpio PB7(PB7_ID);
mcuGpio PB8(PB8_ID);
mcuGpio PB9(PB9_ID);
mcuGpio PB10(PB10_ID);
mcuGpio PB11(PB11_ID);
mcuGpio PB12(PB12_ID);
mcuGpio PB13(PB13_ID);
mcuGpio PB14(PB14_ID);
mcuGpio PB15(PB15_ID);


#if (MCU_PINS >= 64)
mcuGpio PC0(PC0_ID);
mcuGpio PC1(PC1_ID);
mcuGpio PC2(PC2_ID);
mcuGpio PC3(PC3_ID);
mcuGpio PC4(PC4_ID);
mcuGpio PC5(PC5_ID);
mcuGpio PC6(PC6_ID);
mcuGpio PC7(PC7_ID);
mcuGpio PC8(PC8_ID);
mcuGpio PC9(PC9_ID);
mcuGpio PC10(PC10_ID);
mcuGpio PC11(PC11_ID);
mcuGpio PC12(PC12_ID);
#endif
mcuGpio PC13(PC13_ID);
mcuGpio PC14(PC14_ID);
mcuGpio PC15(PC15_ID);

#if (MCU_PINS >= 64)
mcuGpio PD0(PD0_ID);
mcuGpio PD1(PD1_ID);
#endif

Uart uart1(USART1, &PA9, &PA10);
Uart uart2(USART2, &PA2, &PA3);
Uart uart3(USART3, &PB10, &PB11);

//spi
mcuSpi  spi1(SPI1, &PA5, &PA6, &PA7);
mcuSpi  spi2(SPI2, &PB13, &PB14, &PB15);
mcuSpi  spi3(SPI3, &PB3, &PB4, &PB5);
#endif
//Button  btn(&BtnPin,1);
//// 依赖edrive目录下 led模块
//Led     led1(&LED1,0);
//Led     led2(&LED2,0);
//Led     led3(&LED3,0);
//Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
W5500Class w5500(&PC13, &PC14, &PC15, &spi2);
EventManager manager;




/*
*********************************************************************************************************
*	函 数 名: PrintfLog
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void print_log(const char *name, const char *date)
{
    UART.print("\n");
    UART.print("*************************************************************\n");
    UART.print("* \r\n");	                /* 打印一行空格 */
    UART.print("* 例程名称      : ");	    /* 打印例程名称 */
    UART.println(name);
    UART.print("* 发布日期      : ");	    /* 打印例程日期 */
    UART.println(date);

    UART.print("* 硬件平台      : ");       /* 硬件平台 */
    UART.println(HARDWARE);
    UART.print("* EBOX库版本    : ebox_V" );/* 版本信息 */
    UART.println(EBOX_VERSION);
    UART.print("*                     CPU 信息\n");	/* CPU信息 */
    UART.println();
    UART.print("* CPU TYPE      : ");	    /* 打印CPU类型 */
    UART.println(MCU_TYPE);
    UART.print("* CPU PINS      : ");	    /* 打印CPU引脚数量 */
    UART.println(STM32_PINS);
    UART.print("* CPU COMPANY   : ");	    /* 打印CPU公司 */
    UART.println(MCU_COMPANY);


    UART.print("* CPUID         : ");       /* 打印CPU唯一ID */
    for(int i = 0 ; i <11; i++)
    {
        UART.printf("%02X-",cpu.chip_id[i]);
    }
    UART.printf("%02X",cpu.chip_id[11]);
    UART.printf("\n");



    UART.print("* core          : ");
    UART.print(cpu.clock.core / 1000000.0);	/* 打印时钟树信息 */
    UART.println("Mhz");
    UART.print("* hclk          : ");
    UART.print(cpu.clock.hclk / 1000000.0);
    UART.println("Mhz");
    UART.print("* pclk1         : ");
    UART.print(cpu.clock.pclk1 / 1000000.0);
    UART.println("Mhz");
    UART.print("* pclk2         : ");
    UART.print(cpu.clock.pclk2 / 1000000.0);
    UART.println("Mhz");

    UART.print("* ability/s     : ");
    UART.print(cpu.ability / 1000000.0);
    UART.println("Mhz");


    UART.print("* flash size    : ");       /* 打印flash大小 */
    UART.print(cpu.flash.size/1024);
    UART.println("KB");
    
    UART.print("* flash page size    : ");  /* 打印flash page大小 */
    UART.print(cpu.flash.page_size);
    UART.println("B");


    UART.print("* flash used    : ");	    /* 打印flash使用了多少KB */
    UART.print(cpu.flash.used / 1024.0);
    UART.println("KB");

    UART.print("* flash remaind : ");	    /* 打印flash剩余了多少KB  */
    UART.print((cpu.flash.size - cpu.flash.used) / 1024.0);
    UART.println("KB");

    UART.print("* mem size      : ");       /* 打印SRAM大小 */
    UART.print(MCU_SRAM1_SIZE / 1024.0);
    UART.println("KB");

    UART.print("* mem used      : ");	    /* 打印SRAM使用了多少KB */
    UART.print(MCU_SRAM1_USED / 1024.0);
    UART.println("KB");

    UART.print("* mem remaind   : ");	    /* 打印SRAM剩余了多少KB */
    UART.print(MCU_SRAM1_REMAIND / 1024.0);
    UART.println("KB");

    UART.print("* heap used     : ");	    /* 打印heap使用率 */
    UART.print(ebox_mem_usage());
    UART.println("%");

    UART.print("* heap used     : ");	    /* 打印heap使用率 */
    UART.print(ebox_mem_used() / 1024.0);
    UART.println("KB");

    UART.print("* heap free     : ");	    /* 打印heap使用了多少KB */
    UART.print(ebox_get_free() / 1024.0);
    UART.println("KB");

    UART.print("*\n");	                /* 打印一行空格 */
    UART.print("*************************************************************\n");
    UART.flush();
}


