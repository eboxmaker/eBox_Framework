/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

/*
 *  Ӳ������:
 *     	0.96��oled��4��SPI��128*64
 * 	OLED 4�ӿ���ʾ����(STM32ϵ��)
 *             ˵��: SPIЭ��
 *             ----------------------------------------------------------------
 *              GND    ��
 *              VCC  ��5V��3.3v��Դ
 *              D0   ��PD6(SCL)
 *              D1   ��PD7(SDA)
 *              RES  ��PD4
 *              DC   ��PD5
 *              CS   ��PD3
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
    oled.OLED_ShowCHinese(0, 0, 0); //��
    oled.OLED_ShowCHinese(18, 0, 1); //��
    oled.OLED_ShowCHinese(36, 0, 2); //԰
    oled.OLED_ShowCHinese(54, 0, 3); //��
    oled.OLED_ShowCHinese(72, 0, 4); //��
    oled.OLED_ShowCHinese(90, 0, 5); //��
    oled.OLED_ShowCHinese(108, 0, 6); //��
    while(1)
    {
        oled.OLED_Clear();
        oled.OLED_ShowCHinese(0, 0, 0); //��
        oled.OLED_ShowCHinese(18, 0, 1); //��
        oled.OLED_ShowCHinese(36, 0, 2); //԰
        oled.OLED_ShowCHinese(54, 0, 3); //��
        oled.OLED_ShowCHinese(72, 0, 4); //��
        oled.OLED_ShowCHinese(90, 0, 5); //��
        oled.OLED_ShowCHinese(108, 0, 6); //��
        oled.OLED_ShowString(0, 3, (uint8_t *)"1.3' OLED TEST");
        oled.OLED_ShowString(0, 6, (uint8_t *)"ASCII:");
        oled.OLED_ShowString(63, 6, (uint8_t *)"CODE:");
        oled.OLED_ShowChar(48, 6, t); //��ʾASCII�ַ�
        t++;
        if(t > '~')t = ' ';
        oled.OLED_ShowNum(103, 6, t, 3, 16); //��ʾASCII�ַ�����ֵ


        delay_ms(1000);
        oled.OLED_Clear();
        delay_ms(800);
        oled.OLED_DrawBMP(0, 0, 128, 8, BMP1); //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
        delay_ms(800);
        oled.OLED_DrawBMP(0, 0, 128, 8, BMP2);
        delay_ms(800);
    }


}








