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

#include "ds3231.h"

/**
	*	1	��������Ҫ����eDriveĿ¼�µ�ds3231����
	*	2	��������ʾ��ds3231ʱ��оƬ�Ļ�������
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"ds3231 example"
#define EXAMPLE_DATE	"2018-08-11"

DS3231 ds(&si2c);

DateTime_t t;
char time[9];
char date[9];


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    ds.begin(400000);

    t.year = 15;
    t.month = 7;
    t.date = 3;
    t.hour = 23;
    t.min = 59;
    t.sec = 55;


}
int main(void)
{
    setup();
    ds.set_time(&t);
    while(1)
    {
        ds.get_date_time(&t);
        ds.get_time(time);
        ds.get_date(date);
        UART.printf("=======\r\n");
        UART.printf("%02d-%02d-%02d %02d:%02d:%02d\r\n", t.year, t.month, t.date, t.hour, t.min, t.sec);

        UART.printf(date);
        UART.printf(" ");
        UART.printf(time);
        UART.printf("\r\n");
        delay_ms(1000);
    }


}




