/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

/*
 *  硬件环境:
 *     	0.96寸oled，4线SPI，128*64
 * 	OLED 4接口演示例程(STM32系列)
 *             说明: SPI协议
 *             ----------------------------------------------------------------
 *              GND    接
 *              VCC  接5V或3.3v电源
 *              D0   接PD6(SCL)
 *              D1   接PD7(SDA)
 *              RES  接PD4
 *              DC   接PD5
 *              CS   接PD3
*/
#include "ebox.h"
#include "oled_ssd1306.h"
#include "font.h"
OLED_SSD1306 oled(&PD3, &PD4, &PD5, &PD6, &PD7);
void setup()
{
    ebox_init();
    oled.begin();
}

int main(void)
{
    uint8_t t;
    setup();
    oled.OLED_Clear();
    oled.OLED_ShowString(30, 0, (uint8_t *)"OLED TEST");
    oled.OLED_ShowString(30, 0, (uint8_t *)"OLED TEST");
    oled.OLED_ShowString(8, 2, (uint8_t *)"ZHONGJINGYUAN");
    oled.OLED_ShowString(20, 4, (uint8_t *)"2014/05/01");
    oled.OLED_ShowString(0, 6, (uint8_t *)"ASCII:");
    oled.OLED_ShowString(63, 6, (uint8_t *)"CODE:");
    t = ' ';
    oled.OLED_ShowCHinese(0, 0, 0); //中
    oled.OLED_ShowCHinese(18, 0, 1); //景
    oled.OLED_ShowCHinese(36, 0, 2); //园
    oled.OLED_ShowCHinese(54, 0, 3); //电
    oled.OLED_ShowCHinese(72, 0, 4); //子
    oled.OLED_ShowCHinese(90, 0, 5); //科
    oled.OLED_ShowCHinese(108, 0, 6); //技
    while(1)
    {
        oled.OLED_Clear();
        oled.OLED_ShowCHinese(0, 0, 0); //中
        oled.OLED_ShowCHinese(18, 0, 1); //景
        oled.OLED_ShowCHinese(36, 0, 2); //园
        oled.OLED_ShowCHinese(54, 0, 3); //电
        oled.OLED_ShowCHinese(72, 0, 4); //子
        oled.OLED_ShowCHinese(90, 0, 5); //科
        oled.OLED_ShowCHinese(108, 0, 6); //技
        oled.OLED_ShowString(0, 3, (uint8_t *)"1.3' OLED TEST");
        oled.OLED_ShowString(0, 6, (uint8_t *)"ASCII:");
        oled.OLED_ShowString(63, 6, (uint8_t *)"CODE:");
        oled.OLED_ShowChar(48, 6, t); //显示ASCII字符
        t++;
        if(t > '~')t = ' ';
        oled.OLED_ShowNum(103, 6, t, 3, 16); //显示ASCII字符的码值


        delay_ms(1000);
        oled.OLED_Clear();
        delay_ms(800);
        oled.OLED_DrawBMP(0, 0, 128, 8, BMP1); //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
        delay_ms(800);
        oled.OLED_DrawBMP(0, 0, 128, 8, BMP2);
        delay_ms(800);
    }


}








