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
#include "i2c.h"
/**
    *	1	������ΪIICɨ�����
	*	2	��������ʾ��ɨ�����������е�ַ����Ӧ���豸
    *   3   ͨ��begin��������i2c����Ϊ10k��100k��200k��400k��Ĭ��100k
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"IIC Scan example"
#define EXAMPLE_DATE	"2021-02-27"

uint8_t x = 0;  
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    Wire.begin();

}


int main(void)
{
    setup();

    while(1)
    {
        Wire.beginTransmission(0xA0); // transmit to device #8
//        Wire.write("x is ");        // sends five bytes
        Wire.write(x);              // sends one byte
        Wire.endTransmission();    // stop transmitting
        x++;
        delay_ms(500);
    }
}





