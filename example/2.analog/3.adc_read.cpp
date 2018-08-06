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
	*	1	������ʵ����ģ��ת����
	* 
	*/

/* ���������������̷������� */
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
        UART.printf("========����IO��ѯ====================\r\n");
        UART.printf("adc0:0x%x\t(%0.1fmv)\r\n",adc.read(&PA0),adc.read_voltage(&PA0));
        UART.printf("adc1:0x%x\t(%0.1fmv)\r\n",adc.read(&PA1),adc.read_voltage(&PA1));
        UART.printf("adc2:0x%x\t(%0.1fmv)\r\n",adc.read(&PA2),adc.read_voltage(&PA2));
        UART.printf("adc temp:\t(%0.1f��)\r\n",adc.read_temp_senser());
        UART.printf("========��������˳��====================\r\n");
        UART.printf("adc0:0x%x\t(%0.1fmv)\r\n",adc.read(0),adc.read_voltage(0));
        UART.printf("adc1:0x%x\t(%0.1fmv)\r\n",adc.read(1),adc.read_voltage(1));
        UART.printf("adc2:0x%x\t(%0.1fmv)\r\n",adc.read(2),adc.read_voltage(2));
        UART.printf("adc temp:\t(%0.1f��)\r\n",adc.read_temp_senser());
        delay_ms(1000);
    }
}

/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	UART.printf("\n\r");
	UART.printf("*************************************************************\n\r");
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("* ��������   : %s\r\n",EXAMPLE_NAME);	/* ��ӡ�������� */
	UART.printf("* ��������   : %s\r\n", EXAMPLE_DATE);	/* ��ӡ�������� */

	UART.printf("* Ӳ��ƽ̨   : %s \r\n",HARDWARE);
	UART.printf("* EBOX��汾 : %s (ebox)\r\n", EBOX_VERSION);
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("*                     CPU ��Ϣ\r\n");	/* ��ӡһ�пո� */
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("* CPUID      : %08X %08X %08X\n\r"
			, cpu.chip_id[2], cpu.chip_id[1], cpu.chip_id[0]);
	UART.printf("* flash size : %d KB \r\n",cpu.flash_size);
	UART.printf("* core       : %d\r\n",cpu.clock.core);
    UART.printf("* hclk       : %d\r\n",cpu.clock.hclk);
    UART.printf("* pclk1      : %d\r\n",cpu.clock.pclk1);
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("*************************************************************\n\r");
}



