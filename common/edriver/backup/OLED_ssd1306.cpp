/**
  ******************************************************************************
  * @file    oled_ssd1306.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "oled_ssd1306.h"
#include "font.h"


/**********************************************************
 *                    函 数 声 明 区                      *
 **********************************************************/
OLED_SSD1306::OLED_SSD1306(Gpio *cs_pin, Gpio *res_pin, Gpio *dc_pin, Gpio *scl_pin, Gpio *sda_pin)
{
    this->cs_pin	= cs_pin;
    this->res_pin	= res_pin;
    this->dc_pin	= dc_pin;
    this->scl_pin	= scl_pin;
    this->sda_pin	= sda_pin;
}
void OLED_SSD1306::begin(void)	//初始化SSD1306
{
    res_pin->mode(OUTPUT_PP);
    dc_pin->mode(OUTPUT_PP);
    scl_pin->mode(OUTPUT_PP);
    sda_pin->mode(OUTPUT_PP);
    cs_pin->mode(OUTPUT_PP);
    init();
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED写数据
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::write_data(uint8_t dat)
{
    uint8_t i;
    dc_pin->set();
    cs_pin->reset();
    for(i = 0; i < 8; i++)
    {
        scl_pin->reset();
        if(dat & 0x80)
            sda_pin->set();
        else
            sda_pin->reset();
        scl_pin->set();
        dat <<= 1;
    }
    cs_pin->set();
    dc_pin->set();
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED写数据
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::write_cmd(uint8_t cmd)
{
    uint8_t i;
    dc_pin->reset();
    cs_pin->reset();
    for(i = 0; i < 8; i++)
    {
        scl_pin->reset();
        if(cmd & 0x80)
            sda_pin->set();
        else
            sda_pin->reset();
        scl_pin->set();
        cmd <<= 1;
    }
    cs_pin->set();
    dc_pin->set();
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED设置坐标
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::
set_xy(uint16_t x, uint16_t y)
{
    write_cmd(0xb0 + y);
    write_cmd(((x & 0xf0) >> 4) | 0x10);
    write_cmd((x & 0x0f) | 0x01);
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED清屏
*   参数列表：
*   返回结果：
*   备    注：清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
***********************************************************/
void OLED_SSD1306::clear(void)
{
    uint8_t i, n;
    for(i = 0; i < 8; i++)
    {
        write_cmd(0xb0 + i);  //设置也地址(0~7)
        write_cmd (0x00);      //设置显示位置—列低地址
        write_cmd (0x10);      //设置显示位置—列高地址
        for(n = 0; n < 128; n++)write_data(0);
    } //更新显示
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED_SSD1306开启显示
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::display_on(void)
{
    write_cmd(0x8d);	//SET DCDC命令
    write_cmd(0X14);  //DCDC ON
    write_cmd(0XAF);  //DISPLAY ON
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED_SSD1306关闭显示
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::display_off(void)
{
    write_cmd(0x8d);	//SET DCDC命令
    write_cmd(0X10);  //DCDC ON
    write_cmd(0XAE);  //DISPLAY ON
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED显示单个字符
*   参数列表：
*   返回结果：
*   备    注：在指定位置显示一个字符,包括部分字符
***********************************************************/
void OLED_SSD1306::show_char(uint8_t x, uint8_t y, uint8_t chr)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; //得到偏移后的值
    if(x > Max_Column - 1)
    {
        x = 0;
        y = y + 2;
    }
    if(SIZE == 16)
    {
        set_xy(x, y);
        for(i = 0; i < 8; i++)
            write_data(font8x16[c * 16 + i]);
        set_xy(x, y + 1);
        for(i = 0; i < 8; i++)
            write_data(font8x16[c * 16 + i + 8]);
    }
    else
    {
        set_xy(x, y + 1);
        for(i = 0; i < 6; i++)
            write_data(font6x8[c][i]);
    }
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED显示一个字符串
*   参数列表：
*   返回结果：
*   备    注：在指定位置显示一个字符串
***********************************************************/
void OLED_SSD1306::show_string(uint8_t x, uint8_t y, uint8_t *chr)
{
    unsigned char j = 0;
    while (chr[j] != '\0')
    {
        show_char(x, y, chr[j]);
        x += 8;
        if(x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED显示一个字符串
*   参数列表：len :数字的位数，size:字体大小，mode:模式	0,填充模式;1,叠加模式
*			  num:数值(0~4294967295);
*   返回结果：
*   备    注：在指定位置显示一个字符串
***********************************************************/
void OLED_SSD1306::show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    for(t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                show_char(x + (size / 2)*t, y, ' ');
                continue;
            }
            else enshow = 1;

        }
        show_char(x + (size / 2)*t, y, temp + '0');
    }
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED显示一个汉字
*   参数列表：
*   返回结果：
*   备    注：在指定位置显示一个汉字
***********************************************************/
void OLED_SSD1306::show_chinese(uint8_t x, uint8_t y, uint8_t no)
{
    uint8_t t, adder = 0;
    set_xy(x, y);
    for(t = 0; t < 16; t++)
    {
        write_data(Hzk[2 * no][t]);
        adder += 1;
    }
    set_xy(x, y + 1);
    for(t = 0; t < 16; t++)
    {
        write_data(Hzk[2 * no + 1][t]);
        adder += 1;
    }
}


/***********************************************************
*   函数名称：
*   功能描述：显示BMP图片128×64起始点坐标(x,y),
*   参数列表：x的范围0～127，y为页的范围0～7
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::draw_bmp(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, const unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if(y1 % 8 == 0) y = y1 / 8;
    else y = y1 / 8 + 1;
    for(y = y0; y < y1; y++)
    {
        set_xy(x0, y);
        for(x = x0; x < x1; x++)
        {
            write_data(BMP[j++]);
        }
    }
}

/*****************  m^n  ********************/
uint32_t OLED_SSD1306::oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while(n--)result *= m;
    return result;
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED_SSD1306初始化
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::init(void)
{
    res_pin->set();
    delay_ms(100);
    res_pin->reset();
    delay_ms(100);
    res_pin->set();

    write_cmd(0xae);//--turn off oled panel
    write_cmd(0x00);//---set low column address
    write_cmd(0x10);//---set high column address
    write_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    write_cmd(0x81);//--set contrast control register
    write_cmd(0xcf); // Set SEG Output Current Brightness
    write_cmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    write_cmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    write_cmd(0xa6);//--set normal display
    write_cmd(0xa8);//--set multiplex ratio(1 to 64)
    write_cmd(0x3f);//--1/64 duty
    write_cmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    write_cmd(0x00);//-not offset
    write_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
    write_cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    write_cmd(0xd9);//--set pre-charge period
    write_cmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    write_cmd(0xda);//--set com pins hardware configuration
    write_cmd(0x12);
    write_cmd(0xdb);//--set vcomh
    write_cmd(0x40);//Set VCOM Deselect Level
    write_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    write_cmd(0x02);//
    write_cmd(0x8d);//--set Charge Pump enable/disable
    write_cmd(0x14);//--set(0x10) disable
    write_cmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    write_cmd(0xa6);// Disable Inverse Display On (0xa6/a7)
    write_cmd(0xaf);//--turn on oled panel

    write_cmd(0xaf);	/*display ON*/
    clear();
    set_xy(0, 0);
}

