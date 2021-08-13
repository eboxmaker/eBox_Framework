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
	*	1	此例程需要调用eDrive目录下的ds3231驱动
	*	2	此例程演示了ds3231时钟芯片的基本操作
	*/

/* 定义例程名和例程发布日期 */
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
