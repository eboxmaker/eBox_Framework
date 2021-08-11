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
#include "at24x.h"
At24x ee(&mcuI2c1);
/**
    *	1	此例程需要调用eDrive目录下的at24c02驱动
	*	2	此例程演示了at24c02的读写操作
    *   3   通过begin可以设置i2c速率为10k，100k，200k，400k
    *   4   通过config可以自由设置i2c速率，注意f0需要借助官方工具生成timing传入
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"AT24C02 example"
#define EXAMPLE_DATE	"2018-08-11"

uint8_t data;

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
   // Wire.begin();
    ee.begin();

}
int16_t x, i;
uint8_t wbuf[512];
uint8_t rbuf[512];
#define MAX_LEN 10
int ret = 0;
void test();


int main(void)
{
    setup();

    while(1)
    {
        test();
        
    }
}

void test()
{
    ret = 0;

    UART.printf("=================wbuf================\r\n");
    for(uint16_t i = 0; i < MAX_LEN; i++)
    {
        wbuf[i] = random()%255;
        rbuf[i] = 0;
    }
    for(uint16_t i = 0; i < MAX_LEN; i++)
    {
        UART.printf(" %02x ", wbuf[i ]);
        //ee.byteWrite(i*16+j,buf[i*16+j]);
    }
    UART.printf("\r\n ");

    ret = ee.write_byte(0, wbuf, MAX_LEN);
    if(ret)
        UART.printf("write:%d\r\n",ret);

    UART.printf("==================rbuf==============\r\n");
    UART.flush();
    data = ee.read_byte(0, rbuf, MAX_LEN);
    for(uint16_t i = 0; i < MAX_LEN; i++)
    {
        UART.printf(" %02x ", rbuf[i]);
    }
    UART.printf("\r\n ");
    for(int i = 0; i < MAX_LEN; i++)
    {
        if(wbuf[i] != rbuf[i])
        {
            ret = 1;
            break;
        }
    }
    if(ret == 1)
    {
        UART.printf("eeprom check ......[err]");
//        ee.begin();
    }
    else
        UART.printf("eeprom check ......[OK]");

    UART.printf("\r\n================================\r\n");
    delay_ms(1000);

}
