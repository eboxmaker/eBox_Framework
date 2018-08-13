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
	*	1	��������ʾ��rgbled��ʾ������colorled��color_convertģ��
	*   2   ����һ��ʾ��RGBģʽ������Ϊ����ƣ������̣���
    *   3   ���������������ֱ���ʾ��HSV��HSLģʽ��
    *       c begin(),����adc
    *       d ͨ��ͨ����(&PA0)��ͨ������(1)��ȡת�����
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"colorled example"
#define EXAMPLE_DATE	"2018-08-06"


#include "ebox.h"
#include "math.h"
#include "colorled.h"
#include "bsp_ebox.h"

COLOR_HSL hsl;  //HSLɫ��ģʽ
COLOR_RGB rgb;  //RGBɫ��ģʽ
COLOR_HSV hsv;  //HSVɫ��ģʽ

// ����rgb led ����
ColorLed led(&LED_R, &LED_G, &LED_B);

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
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
//����1��        
//    led.color_rgb(0,0,255);
//    delay_ms(100);
//    led.color_rgb(0,255,0);
//    delay_ms(100);
//    led.color_rgb(255,0,0);
//    delay_ms(100);
        
//����2��
//        hsv.h++;
//        if(hsv.h>360)hsv.h = 0;
//        led.color_hsv(hsv);
//        delay_ms(10);

//����3��
        hsl.h++;
        if(hsl.h > 360)hsl.h = 0;
        led.color_hsl(hsl);
        delay_ms(50);
        
        


    }

}




