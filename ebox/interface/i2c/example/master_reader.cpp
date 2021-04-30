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

uint8_t data;

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    Wire.begin();

}

void scan();



int main(void)
{
    setup();

    while(1)
    {
        Wire.requestFrom(8, 6);    // request 6 bytes from slave device #8

      while (Wire.available()) 
      { // slave may send less than requested
            char c = Wire.read(); // receive a byte as character
            Serial.print(c);         // print the character
      }

      delay_ms(500);
    }
}





