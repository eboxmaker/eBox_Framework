/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
	*					 2018-7-31	ͨ������bsp������Ӳ���˿ڣ��������̿����ڲ�ͬƽ̨������
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "bsp_ebox.h"
#include "ebox_mem.h"


//SoftI2c Wire(&PB8, &PB9);

/*
*********************************************************************************************************
*	�� �� ��: PrintfLog
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void print_log(const char *name, const char *date)
{
    UART.print("\n\r");
    UART.print("*************************************************************\n\r");
    UART.print("* \r\n");	                /* ��ӡһ�пո� */
    UART.print("* ��������      : ");	    /* ��ӡ�������� */
    UART.println(name);
    UART.print("* ��������      : ");	    /* ��ӡ�������� */
    UART.println(date);

    UART.print("* Ӳ��ƽ̨      : ");       /* Ӳ��ƽ̨ */
    UART.println(HARDWARE);
    UART.print("* EBOX��汾    : ebox_" );/* �汾��Ϣ */
    UART.println(EBOX_VERSION);
    UART.print("*                     CPU ��Ϣ\r\n");	/* CPU��Ϣ */
    UART.println();
    UART.print("* CPU TYPE      : ");	    /* ��ӡCPU���� */
    UART.println(MCU_TYPE);
    UART.print("* CPU PINS      : ");	    /* ��ӡCPU�������� */
    UART.println(STM32_PINS);
    UART.print("* CPU COMPANY   : ");	    /* ��ӡCPU��˾ */
    UART.println(MCU_COMPANY);


    UART.print("* CPUID         : ");       /* ��ӡCPUΨһID */
    UART.print(cpu.chip_id[2], HEX);
    UART.print(cpu.chip_id[1], HEX);
    UART.println(cpu.chip_id[0], HEX);


    UART.print("* core          : ");
    UART.print(cpu.clock.core / 1000000.0);	/* ��ӡʱ������Ϣ */
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


    UART.print("* flash size    : ");       /* ��ӡflash��С */
    UART.print(cpu.flash_size);
    UART.println("KB");

    UART.print("* flash used    : ");	    /* ��ӡflashʹ���˶���KB */
    UART.print(MCU_FLASH_USED / 1024.0);
    UART.println("KB");

    UART.print("* flash remaind : ");	    /* ��ӡflashʣ���˶���KB  */
    UART.print(MCU_FLASH_REMAIND / 1024.0);
    UART.println("KB");

    UART.print("* mem size      : ");       /* ��ӡSRAM��С */
    UART.print(MCU_SRAM1_SIZE / 1024.0);
    UART.println("KB");

    UART.print("* mem used      : ");	    /* ��ӡSRAMʹ���˶���KB */
    UART.print(MCU_SRAM1_USED / 1024.0);
    UART.println("KB");

    UART.print("* mem remaind   : ");	    /* ��ӡSRAMʣ���˶���KB */
    UART.print(MCU_SRAM1_REMAIND / 1024.0);
    UART.println("KB");

    UART.print("* heap used     : ");	    /* ��ӡheapʹ���� */
    UART.print(ebox_mem_usage());
    UART.println("%");

    UART.print("* heap used     : ");	    /* ��ӡheapʹ���� */
    UART.print(ebox_mem_used() / 1024.0);
    UART.println("KB");

    UART.print("* heap free     : ");	    /* ��ӡheapʹ���˶���KB */
    UART.print(ebox_get_free() / 1024.0);
    UART.println("KB");

    UART.print("* \r\n");	                /* ��ӡһ�пո� */
    UART.print("*************************************************************\n\r");
    UART.flush();
}





