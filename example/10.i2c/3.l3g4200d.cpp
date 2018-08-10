/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "bsp_ebox.h"

#include "l3g4200d.h"

/**
	*	1	��������Ҫ����eDriveĿ¼�µ�L3G4200D����
	*	2	��������ʾ��L3G4200D�Ļ�������
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"L3G4200D example"
#define EXAMPLE_DATE	"2018-08-11"

L3G4200D lg(&si2c2);


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    lg.begin(100000);



}
u32 count;
int main(void)
{
    setup();
    while(1)
    {
        lg.read();
        UART.printf("==========================\r\n");
        lg.test();
        UART.printf("x:%0.2f;\r\n", lg.g.x);
        UART.printf("y:%0.2f;\r\n", lg.g.y);
        UART.printf("z:%0.2f;\r\n", lg.g.z);
        delay_ms(1000);
    }


}


