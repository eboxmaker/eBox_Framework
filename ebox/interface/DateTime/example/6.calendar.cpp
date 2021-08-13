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
#include "BasicRtc.h"

/**
	*	1	��������Ҫ����eDriveĿ¼�µ�ds3231����
	*	2	��������ʾ��ds3231ʱ��оƬ�Ļ�������
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"ds3231 example"
#define EXAMPLE_DATE	"2021-08-11"

ChinaCalendar Cdt;
DateTime newdt("2021-8-21 0.0.0");

uint32_t last_time;

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
}

int main(void)
{
    setup();
    while(1)
    {

        if(millis() - last_time > 1000)
        {
            last_time = millis();            
            Cdt.set(newdt);
            Cdt.print(UART);
            newdt.addDays(1);
        }
    }


}
