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
	*	1	此例程需要调用eDrive目录下的L3G4200D驱动
	*	2	此例程演示了L3G4200D的基本操作
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"L3G4200D example"
#define EXAMPLE_DATE	"2018-08-11"

L3G4200D lg(&sI2c2);


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    lg.begin();



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


s