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

#include "Max7456.h"



/**
	*	1	��������Ҫ����eDriveĿ¼�µ�w5500������network���
	*	2	��������ʾ��DNS��������
	*/



/* ���������������̷������� */
#define EXAMPLE_NAME	"w25x16 example"
#define EXAMPLE_DATE	"2018-08-11"


OSD osd(&PB0, &spi1);

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    osd.begin(1);


}
int16_t tmp[7];
uint16_t id;
uint8_t rbuf[100];
uint8_t wbuf[100];
int main(void)
{
    setup();
    while(1)
    {
        uart1.printf("ebox max7456 test\n\r");

        osd.set_panel(6, 6);
        osd.open_panel();
        osd.printf("ebox Max7456 test");
        osd.close_panel();
        delay_ms(1000);


    }
}




