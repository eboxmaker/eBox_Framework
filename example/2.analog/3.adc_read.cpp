 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V2.2
  * @date    2018/08/6
  * @brief   ebox application example .
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

#include "ebox.h"
#include "bsp_ebox.h"

/**
	*	1	此例程实现了模数转换。
	* 
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Adc example"
#define EXAMPLE_DATE	"2018-08-06"


static void PrintfLogo(void);

Adc adc(ADC1);
void setup()
{
    ebox_init();
    UART.begin(115200);
    PrintfLogo();

    adc.add_ch(&PA0);
    adc.add_ch(&PA1);
    adc.add_ch(&PA2);
    adc.add_temp_senser();
    adc.begin();
}
int main(void)
{
    setup();
    while(1)
    {
        UART.printf("========按照IO查询====================\r\n");
        UART.printf("adc0:0x%x\t(%0.1fmv)\r\n",adc.read(&PA0),adc.read_voltage(&PA0));
        UART.printf("adc1:0x%x\t(%0.1fmv)\r\n",adc.read(&PA1),adc.read_voltage(&PA1));
        UART.printf("adc2:0x%x\t(%0.1fmv)\r\n",adc.read(&PA2),adc.read_voltage(&PA2));
        UART.printf("adc temp:\t(%0.1f℃)\r\n",adc.read_temp_senser());
        UART.printf("========按照索引顺序====================\r\n");
        UART.printf("adc0:0x%x\t(%0.1fmv)\r\n",adc.read(0),adc.read_voltage(0));
        UART.printf("adc1:0x%x\t(%0.1fmv)\r\n",adc.read(1),adc.read_voltage(1));
        UART.printf("adc2:0x%x\t(%0.1fmv)\r\n",adc.read(2),adc.read_voltage(2));
        UART.printf("adc temp:\t(%0.1f℃)\r\n",adc.read_temp_senser());
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
			, cpu.chip_id[2], cpu.chip_id[1], cpu.chip_id[0]);
	UART.printf("* flash size : %d KB \r\n",cpu.flash_size);
	UART.printf("* core       : %d\r\n",cpu.clock.core);
    UART.printf("* hclk       : %d\r\n",cpu.clock.hclk);
    UART.printf("* pclk1      : %d\r\n",cpu.clock.pclk1);
	UART.printf("* \r\n");	/* 打印一行空格 */
	UART.printf("*************************************************************\n\r");
}



