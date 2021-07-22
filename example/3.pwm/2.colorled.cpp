/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */

/**
	*	1	此例程演示了rgbled显示。依赖colorled，color_convert模块
	*   2   方法一演示了RGB模式，表现为跑马灯，蓝，绿，红
    *   3   方法二，方法三分别演示了HSV和HSL模式，
    *       c begin(),启动adc
    *       d 通过通道号(&PA0)或通道索引(1)读取转换结果
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"colorled example"
#define EXAMPLE_DATE	"2018-08-06"


#include "ebox.h"
#include "math.h"
#include "colorled.h"
#include "bsp_ebox.h"

COLOR_HSL hsl;  //HSL色彩模式
COLOR_RGB rgb;  //RGB色彩模式
COLOR_HSV hsv;  //HSV色彩模式

// 创建rgb led 对象
ColorLed led(&LED_R, &LED_G, &LED_B);

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    led.begin();
    hsl.h = 1;
    hsl.s = 1;
    hsl.l = 0.5;

    hsv.h = 2;
    hsv.s = 1;
    hsv.v = 0.5;
}

float x;
uint16_t y;

int main(void)
{
    setup();

    while(1)
    {
        //方法1：
        //    led.color_rgb(0,0,255);
        //    delay_ms(100);
        //    led.color_rgb(0,255,0);
        //    delay_ms(100);
        //    led.color_rgb(255,0,0);
        //    delay_ms(100);

        //方法2：
        //        hsv.h++;
        //        if(hsv.h>360)hsv.h = 0;
        //        led.color_hsv(hsv);
        //        delay_ms(10);

        //方法3：
        hsl.h++;
        if(hsl.h > 360)hsl.h = 0;
        led.color_hsl(hsl);
        delay_ms(10);




    }

}




