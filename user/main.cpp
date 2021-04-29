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

/**
    *	1	��������Ҫ����eDriveĿ¼�µ�at24c02����
	*	2	��������ʾ��at24c02�Ķ�д����
    *   3   ͨ��begin��������i2c����Ϊ10k��100k��200k��400k
    *   4   ͨ��config������������i2c���ʣ�ע��f0��Ҫ�����ٷ���������timing����
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"AT24C02 example"
#define EXAMPLE_DATE	"2018-08-11"

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
        
    }
}
