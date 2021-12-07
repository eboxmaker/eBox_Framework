/**
  ******************************************************************************
  * @file    bsp_ebox.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2018/07/31
  * @brief   Ӳ�������Ϣ����
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

    ConsoleUart.print("\n");
    ConsoleUart.print("*************************************************************\n");
    ConsoleUart.print("* \r\n");	                /* ��ӡһ�пո� */
    ConsoleUart.print("* ��������      : ");	    /* ��ӡ�������� */
    ConsoleUart.println(name);
    ConsoleUart.print("* ��������      : ");	    /* ��ӡ�������� */
    ConsoleUart.println(date);

    ConsoleUart.print("* Ӳ��ƽ̨      : ");       /* Ӳ��ƽ̨ */
    ConsoleUart.println(HARDWARE);
    ConsoleUart.print("* EBOX��汾    : ebox_V" );/* �汾��Ϣ */
    ConsoleUart.println(EBOX_VERSION);
    ConsoleUart.print("*                     CPU ��Ϣ\n");	/* CPU��Ϣ */
    ConsoleUart.println();
    ConsoleUart.print("* CPU COMPANY   : ");	    /* ��ӡCPU��˾ */
    ConsoleUart.println(cpu.company);


    ConsoleUart.print("* CPUID         : ");       /* ��ӡCPUΨһID */
    for(int i = 0 ; i <11; i++)
    {
        ConsoleUart.print("0X");
        ConsoleUart.print(cpu.chip_id[i],HEX);
        ConsoleUart.print("-");
    }
    ConsoleUart.print("0X");
    ConsoleUart.println(cpu.chip_id[11],HEX);




    ConsoleUart.print("* core          : ");
    ConsoleUart.print(cpu.clock.core / 1000000.0);	/* ��ӡʱ������Ϣ */
    ConsoleUart.println("Mhz");
    ConsoleUart.print("* hclk          : ");
    ConsoleUart.print(cpu.clock.hclk / 1000000.0);
    ConsoleUart.println("Mhz");
    ConsoleUart.print("* pclk1         : ");
    ConsoleUart.print(cpu.clock.pclk1 / 1000000.0);
    ConsoleUart.println("Mhz");
    ConsoleUart.print("* pclk2         : ");
    ConsoleUart.print(cpu.clock.pclk2 / 1000000.0);
    ConsoleUart.println("Mhz");


    ConsoleUart.print("* flash size    : ");       /* ��ӡflash��С */
    ConsoleUart.print(cpu.flash.size/1024);
    ConsoleUart.println("KB");
    
    ConsoleUart.print("* flash page size    : ");  /* ��ӡflash page��С */
    ConsoleUart.print(cpu.flash.page_size);
    ConsoleUart.println("B");


    ConsoleUart.print("* flash used    : ");	    /* ��ӡflashʹ���˶���KB */
    ConsoleUart.print(cpu.flash.used / 1024.0);
    ConsoleUart.println("KB");

    ConsoleUart.print("* flash remaind : ");	    /* ��ӡflashʣ���˶���KB  */
    ConsoleUart.print((cpu.flash.size - cpu.flash.used) / 1024.0);
    ConsoleUart.println("KB");
    
    
    ConsoleUart.print("* mem used      : ");       /* ��ӡSRAM��С */
    ConsoleUart.print(MCU_SRAM1_USED / 1024.0);
    ConsoleUart.println("KB");

    
    
    ConsoleUart.print("* heap size      : ");       /* ��ӡSRAM��С */
    ConsoleUart.print(MCU_SRAM1_REMAIND / 1024.0);
    ConsoleUart.println("KB");

    ConsoleUart.print("* heap used     : ");	    /* ��ӡheapʹ���� */
    ConsoleUart.print(ebox_mem_usage() / 1024.0);
    ConsoleUart.println("KB");

    ConsoleUart.print("* heap free     : ");	    /* ��ӡheapʹ���˶���KB */
    ConsoleUart.print(ebox_get_free() / 1024.0);
    ConsoleUart.println("KB");

    ConsoleUart.print("*\n");	                /* ��ӡһ�пո� */
    ConsoleUart.print("*************************************************************\n");
    ConsoleUart.flush();
}


