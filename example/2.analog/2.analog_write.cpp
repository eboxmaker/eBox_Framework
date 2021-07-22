/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *					 2018-8-5	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行
	* Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */



#include "ebox.h"
#include "math.h"
#include "bsp_ebox.h"

/**
	*	1	此例程演示了模拟DAC操作
	*	2	实际是利用PWM输出频率1Khz，占空比可调的方波（0-1000）
	*	3	实际运行时LED渐变
	*   4	可用资源请参考ebox_pwm.h说明
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Analog_Write example"
#define EXAMPLE_DATE	"2018-08-05"



void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
}

float x;
uint16_t y;
int main(void)
{

    setup();

    while(1)
    {
        x = x + PI * 0.01;
        if(x >= PI)x = 0;
        y = 2000 - (sin(x) + 1) * 1000;

        analog_write(&PB8, y);
        delay_ms(20);
    }
}
