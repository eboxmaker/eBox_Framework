/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
	*					 2018-8-5	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */



#include "ebox.h"
#include "bsp_ebox.h"

/**
	*	1	此例程演示了ADC读取原始hex值和电压值
	*	2	ebox ADC通过DMA操作，默认为启动状态
	*	3	系统72M时钟频率下ADC时钟12M，采样周期为54.4us
	* 4	analog_read 可以连续读取，返回值更新最快为54.4us
	* 5 精度为12位，所以analog_read返回值范围为0-4095
	* 6	支持ADC1的16个通道，暂不支持内部通道
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Analog_Read example"
#define EXAMPLE_DATE	"2018-08-05"

int16_t x;
static void PrintfLogo(void);
void setup()
{
    ebox_init();
    UART.begin(115200);
		PrintfLogo();
    ADC_0.mode(AIN);
		ADC_1.mode(AIN);
}

int main(void)
{
    setup();
    while(1)
    {
        x = analog_read(&ADC_0);
        UART.printf("ADC0 hex = %05d\r\n", x);
        x = analog_read_voltage(&ADC_0);
        UART.printf("ADC0 val = %04dmv\r\n", x);
			
			  x = analog_read(&ADC_1);
        UART.printf("ADC1 hex = %05d\r\n", x);
        x = analog_read_voltage(&ADC_1);
        UART.printf("ADC1 val = %04dmv\r\n", x);
        UART.printf("==============\r\n", x);
        delay_ms(1000);
    }
}


/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	UART.printf("\n\r");
	UART.printf("*************************************************************\n\r");
	UART.printf("* \r\n");	/* 打印一行空格 */
	UART.printf("* 例程名称   : %s\r\n",EXAMPLE_NAME);	/* 打印例程名称 */
	UART.printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	UART.printf("* 硬件平台   : %s \r\n",HARDWARE);
	UART.printf("* EBOX库版本 : %s (ebox)\r\n", EBOX_VERSION);
	UART.printf("* \r\n");	/* 打印一行空格 */
	UART.printf("*                     CPU 信息\r\n");	/* 打印一行空格 */
	UART.printf("* \r\n");	/* 打印一行空格 */
	UART.printf("* CPUID      : %08X %08X %08X\n\r"
			, cpu.chip_id[2], cpu.chip_id[1]);
	UART.printf("* flash size : %d KB \r\n",cpu.flash_size);
	UART.printf("* core       : %d\r\n",cpu.clock.core);
  UART.printf("* hclk       : %d\r\n",cpu.clock.hclk);
  UART.printf("* pclk1      : %d\r\n",cpu.clock.pclk1);
	UART.printf("* \r\n");	/* 打印一行空格 */
	UART.printf("*************************************************************\n\r");
}

