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

#include "w25xxx.h"



/**
	*	1	此例程需要调用eDrive目录下的w25x16驱动
	*	2	此例程演示了w25x16的读写功能
	*/



/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"w25x16 example"
#define EXAMPLE_DATE	"2018-08-11"


W25xxx flash(&PA15, &spi1);

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    flash.begin();


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
        for(int i = 0; i < 100; i++)
            wbuf[i] = random() % 256;

        id = flash.read_id();
        uart1.printf("\r\n==readid=======\r\n");
        uart1.printf("id = %x", id);

        uart1.printf("\r\n==write========\r\n");
        flash.write(wbuf, 0, 100);
        for(int i = 0; i < 100; i++)
            uart1.printf(" %x", wbuf[i]);

        uart1.printf("\r\n==read========\r\n");
        flash.read(rbuf, 0, 100);
        for(int i = 0; i < 100; i++)
            uart1.printf(" %x", rbuf[i]);

        uart1.printf("\r\n\r\n");

        delay_ms(1000);
    }
}




