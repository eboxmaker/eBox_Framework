/**
  ******************************************************************************
  * @file    nokia5110.cpp
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
#include "nokia5110.h"
#include "font.h"


/*-----------------------------------------------------------------------
LCD_init: 5110LCD初始化
-----------------------------------------------------------------------*/
void NOKIA5110::begin()
{
    SCLK.mode(OUTPUT_PP);
    SDIN.mode(OUTPUT_PP);
    LCD_DC.mode(OUTPUT_PP);
    LCD_CE.mode(OUTPUT_PP);
    LCD_RST.mode(OUTPUT_PP);
    LCD_LED.mode(OUTPUT_PP);

    LCD_RST.reset();
    LCD_RST.set();
    LCD_CE.reset();
    LCD_CE.set();

    write_cmd(0x21);	// 使用扩展命令设置LCD模式
    write_cmd(0xba);	// 设置偏置电压
    write_cmd(0x06);	// 温度校正
    write_cmd(0x13);	// 1:48
    write_cmd(0x20);	// 使用基本命令
    clear();	        // 清屏
    write_cmd(0x0c);	// 设定显示模式，正常显示

    LCD_CE.reset();
    back_led(1);

}


/*-----------------------------------------------------------------------
LCD_clear         : LCD清屏函数
-----------------------------------------------------------------------*/
void NOKIA5110::clear(void)
{
    unsigned int i = 0;

    write_cmd(0x0c);
    write_cmd(0x80);

    for (i = 0; i < 504; i++)
        write_data(0);
}

/*-----------------------------------------------------------------------
LCD_set_XY        : 设置LCD坐标函数

输入参数：X       ：0－83
          Y       ：0－5
-----------------------------------------------------------------------*/
void NOKIA5110::set_xy(unsigned char X, unsigned char Y)
{
    write_cmd(0x40 | Y);		// column
    write_cmd(0x80 | X);          	// row
}

/*-----------------------------------------------------------------------
back_led        : 设置LCD背光

输入参数：val：	1：on
								0：off
-----------------------------------------------------------------------*/
void NOKIA5110::back_led(uint8_t val)
{
    LCD_LED.write(val);
}

/*-----------------------------------------------------------------------
contrast        : 设置LCD对比度

输入参数：val：	0-255
-----------------------------------------------------------------------*/
void NOKIA5110::contrast(uint8_t val)
{
    write_cmd(0xba);	// 设置偏置电压
}

/*-----------------------------------------------------------------------
LCD_write_char    : 显示英文字符

输入参数：c       ：显示的字符；
-----------------------------------------------------------------------*/
void NOKIA5110::disp_char(unsigned char c)
{
    unsigned char line;

    c -= 32;

    for (line = 0; line < 6; line++)
        write_data(font6x8[c][line]);
}

/*-----------------------------------------------------------------------
LCD_write_english_String  : 英文字符串显示函数

输入参数：*s      ：英文字符串指针；
          X、Y    : 显示字符串的位置,x 0-83 ,y 0-5
-----------------------------------------------------------------------*/
void NOKIA5110::disp_string(unsigned char X, unsigned char Y, const char *s)
{
    set_xy(X, Y);
    while (*s)
    {
        disp_char(*s);
        s++;
    }
}
void NOKIA5110::printf(uint8_t row, uint8_t col, const char *fmt, ...)
{
    char buf[16];
    uint8_t i = 0;
    va_list va_params;
    va_start(va_params, fmt);
    vsprintf(buf, fmt, va_params);
    va_end(va_params);
    set_xy(row, col);
    while(buf[i] != '\0')
    {
        disp_char(buf[i++]);
    }
}
/*-----------------------------------------------------------------------
LCD_write_chinese_string: 在LCD上显示汉字

输入参数：X、Y    ：显示汉字的起始X、Y坐标；
          ch_with ：汉字点阵的宽度
          num     ：显示汉字的个数；
          line    ：汉字点阵数组中的起始行数
          row     ：汉字显示的行间距
测试：
	LCD_write_chi(0,0,12,7,0,0);
	LCD_write_chi(0,2,12,7,0,0);
	LCD_write_chi(0,4,12,7,0,0);
-----------------------------------------------------------------------*/
void NOKIA5110::write_chinese_string(unsigned char X, unsigned char Y,
                                     unsigned char ch_with, unsigned char num,
                                     unsigned char line, unsigned char row)
{
    unsigned char i, n;

    set_xy(X, Y);                            //设置初始位置

    for (i = 0; i < num;)
    {
        for (n = 0; n < ch_with * 2; n++)        //写一个汉字
        {
            if (n == ch_with)                    //写汉字的下半部分
            {
                if (i == 0) set_xy(X, Y + 1);
                else
                    set_xy((X + (ch_with + row)*i), Y + 1);
            }
            write_data(write_chinese[line + i][n]);
        }
        i++;
        set_xy((X + (ch_with + row)*i), Y);
    }
}



/*-----------------------------------------------------------------------
LCD_draw_map      : 位图绘制函数

输入参数：X、Y    ：位图绘制的起始X、Y坐标；
          *map    ：位图点阵数据；
          Pix_x   ：位图像素（长）
          Pix_y   ：位图像素（宽）
-----------------------------------------------------------------------*/
void NOKIA5110::draw_bmp_pixel(unsigned char X, unsigned char Y, unsigned char *map,
                               unsigned char Pix_x, unsigned char Pix_y)
{
    unsigned int i, n;
    unsigned char row;

    if (Pix_y % 8 == 0) row = Pix_y / 8; //计算位图所占行数
    else
        row = Pix_y / 8 + 1;

    for (n = 0; n < row; n++)
    {
        set_xy(X, Y);
        for(i = 0; i < Pix_x; i++)
        {
            write_data(map[i + n * Pix_x]);
        }
        Y++;                         //换行
    }
}

/*-----------------------------------------------------------------------
LCD_write_byte    : 写数据到LCD
输入参数：data    ：数据；
-----------------------------------------------------------------------*/
void NOKIA5110::write_data(unsigned char dat)
{
    LCD_CE.reset(); // 使能LCD
    LCD_DC.set();// 传送数据
    shift_out(&SDIN, &SCLK, MSB_FIRST, dat);
    LCD_CE.set();

}

/*-----------------------------------------------------------------------
write_cmd 			: 写命令到LCD
输入参数：cmd 	：命令
-----------------------------------------------------------------------*/
void NOKIA5110::write_cmd(unsigned char cmd)
{
    LCD_CE.reset(); // 使能LCD
    LCD_DC.reset();// 传送命令
    shift_out(&SDIN, &SCLK, MSB_FIRST, cmd);
    LCD_CE.set();
}


