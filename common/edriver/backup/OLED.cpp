/**
  ******************************************************************************
  * @file    oled.cpp
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
#include "oled.h"
#include "font.h"


OLED::OLED(Gpio *p_res_pin, Gpio *p_dc_pin, Gpio *p_scl_pin, Gpio *p_sda_pin)
{
    res_pin 	= p_res_pin;
    dc_pin 	= p_dc_pin;
    scl_pin 	= p_scl_pin;
    sda_pin 	= p_sda_pin;

}
void OLED::begin()
{
    res_pin->mode(OUTPUT_PP);
    dc_pin->mode(OUTPUT_PP);
    scl_pin->mode(OUTPUT_PP);
    sda_pin->mode(OUTPUT_PP);
}


/***********************************************************
*   函数名称：
*   功能描述：   OLED写数据
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED::
OLED_WrDat(unsigned char dat)
{
    unsigned char i;
    dc_pin->write(HIGH);//OLED_DC_H;
    for(i = 0; i < 8; i++) //发送一个八位数据
    {
        if((dat << i) & 0x80)
        {
            sda_pin->write(HIGH);//OLED_SDA_H;
        }
        else
        {
            sda_pin->write(LOW);//OLED_SDA_L;
        }
        //		delay_us(1);
        scl_pin->write(LOW);//OLED_SCL_L;
        //		delay_us(1);
        scl_pin->write(HIGH);//OLED_SCL_H;
    }
}
/***********************************************************
*   函数名称：
*   功能描述：   OLED写命令
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED::
OLED_WrCmd(unsigned char cmd)
{
    unsigned char i;
    dc_pin->write(LOW);//OLED_DC_L;
    for(i = 0; i < 8; i++) //发送一个八位数据
    {
        if((cmd << i) & 0x80)
        {
            sda_pin->write(HIGH);//OLED_SDA_H;
        }
        else
        {
            sda_pin->write(LOW);//OLED_SDA_L;
        }
        //		delay_us(1);
        scl_pin->write(LOW);//OLED_SCL_L;
        //		delay_us(1);
        scl_pin->write(HIGH);//OLED_SCL_H;

    }

}
/***********************************************************
*   函数名称：
*   功能描述：  OLED 设置坐标
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED::
OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WrCmd(0xb0 + y);
    OLED_WrCmd(((x & 0xf0) >> 4) | 0x10);
    OLED_WrCmd((x & 0x0f) | 0x01);
}
/***********************************************************
*   函数名称：
*   功能描述：  OLED全屏
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED::
OLED_Fill(unsigned char bmp_dat)
{
    unsigned char y, x;
    for(y = 0; y < 8; y++)
    {
        OLED_WrCmd(0xb0 + y);
        OLED_WrCmd(0x02);
        OLED_WrCmd(0x10);
        for(x = 0; x < X_WIDTH; x++)
            OLED_WrDat(bmp_dat);
    }
}
/***********************************************************
*   函数名称：
*   功能描述：   OLED复位
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED::
OLED_CLS(void)
{
    unsigned char y, x;
    for(y = 0; y < 8; y++)
    {
        OLED_WrCmd(0xb0 + y);
        OLED_WrCmd(0x02);
        OLED_WrCmd(0x10);
        for(x = 0; x < X_WIDTH; x++)
            OLED_WrDat(0);
    }
}
/***********************************************************
*   函数名称：
*   功能描述：   OLED初始化
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED::
OLED_init(void)
{
    //	OLED_IO_INIT;
    //	OLED(PA0,PA1,PA2,PA3);
    begin();
    uart1.printf("\r\n1!");
    //	delay_ms(50);
    scl_pin->write(HIGH);//OLED_SCL_H;
    res_pin->write(LOW);//OLED_RST_L;
    //	delay_ms(50);
    //	OLED_DLY_ms(50);
    res_pin->write(HIGH);//OLED_RST_H;       //从上电到下面开始初始化要有足够的时间，即等待RC复位完毕
    uart1.printf("\r\n2!");
    OLED_WrCmd(0xae);//--turn off oled panel
    OLED_WrCmd(0x00);//---set low column address
    OLED_WrCmd(0x10);//---set high column address
    OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WrCmd(0x81);//--set contrast control register
    OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
    OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WrCmd(0xa6);//--set normal display
    OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
    OLED_WrCmd(0x3f);//--1/64 duty
    OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WrCmd(0x00);//-not offset
    OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
    OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WrCmd(0xd9);//--set pre-charge period
    OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WrCmd(0xda);//--set com pins hardware configuration
    OLED_WrCmd(0x12);
    OLED_WrCmd(0xdb);//--set vcomh
    OLED_WrCmd(0x40);//Set VCOM Deselect Level
    OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WrCmd(0x02);//
    OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
    OLED_WrCmd(0x14);//--set(0x10) disable
    OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
    OLED_WrCmd(0xaf);//--turn on oled panel
    OLED_Fill(0x00);  //初始清屏
    OLED_Set_Pos(0, 0);
}
/***********************************************************
*   函数名称：
*   功能描述： 功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED::
OLED_P6x8Str(unsigned char x, unsigned char y, unsigned char *ch)
{
    unsigned char c = 0, i = 0, j = 0;
    while ( ch[j] != '\0' )
    {
        c = ch[j] - 32;
        if( x > 126 )
        {
            x = 0;
            y++;
        }
        OLED_Set_Pos(x, y);
        for(i = 0; i < 6; i++)
        {
            OLED_WrDat(font6x8[c][i]);
        }
        x += 6;
        j++;
    }
}
/***********************************************************
*   函数名称：
*   功能描述： 用于显示一个数组中的内容
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED::
OLED_P6x8p(unsigned char x, unsigned char y, unsigned char *p, unsigned int len)
{
    unsigned char c = 0, i = 0, j = 0;
    for (j = 0; j < len; j++)
    {
        c = p[j] + 16;
        if( x > 126 )
        {
            x = 0;
            y++;
        }
        OLED_Set_Pos(x, y);
        for(i = 0; i < 6; i++)
        {
            OLED_WrDat(font6x8[c][i]);
        }
        x += 6;
    }
}
/***********************************************************
*   函数名称：
*   功能描述：  数值显示函数，
*   参数列表：
*   返回结果：
*	备    注：
***********************************************************/
void OLED::
OLED_f6x8int(unsigned char x, unsigned char y, unsigned int data)
{
    unsigned char c = 0, i = 0, j;
    //	unsigned char s_i;
    unsigned char d_buff[3];

    d_buff[2] = data / 100;
    d_buff[1] = (data % 100) / 10;
    d_buff[0] = data % 10;

    for ( j = 0; j < 3; j++)
    {
        c = d_buff[2 - j] + 16;
        if( x > 126 )
        {
            x = 0;
            y++;
        }
        OLED_Set_Pos(x, y);
        for(i = 0; i < 6; i++)
        {
            OLED_WrDat(font6x8[c][i]);
        }
        x += 6;
    }
}
/***********************************************************
*   函数名称：
*   功能描述：功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED::
OLED_P8x16Str(unsigned char x, unsigned char y, unsigned char *ch)
{
    unsigned char c = 0, i = 0, j = 0;
    while (ch[j] != '\0')
    {
        c = ch[j] - 32;
        if( x > 120 )
        {
            x = 0;
            y++;
        }
        OLED_Set_Pos(x, y);
        for(i = 0; i < 8; i++)
            OLED_WrDat(font8x16[c * 16 + i]);
        OLED_Set_Pos(x, y + 1);
        for(i = 0; i < 8; i++)
            OLED_WrDat(font8x16[c * 16 + i + 8]);
        x += 8;
        j++;
    }
}
/***********************************************************
*   函数名称：
*   功能描述：功能描述：显示8*16一?标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED::
OLED_P8x16ASCII(unsigned char x, unsigned char y, unsigned char ch)
{
    unsigned char c = 0, i = 0, j = 0;

    c = ch + 16;
    if( x > 120 )
    {
        x = 0;
        y++;
    }
    OLED_Set_Pos(x, y);
    for(i = 0; i < 8; i++)
        OLED_WrDat(font8x16[c * 16 + i]);
    OLED_Set_Pos(x, y + 1);
    for(i = 0; i < 8; i++)
        OLED_WrDat(font8x16[c * 16 + i + 8]);
    x += 8;
    j++;

}
/***********************************************************
*   函数名称：
*   功能描述： 显示16*16点阵汉字
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED::
OLED_P16x16Ch(unsigned char x, unsigned char y, unsigned char N)
{
    unsigned char wm = 0;
    unsigned int adder = 32 * N; //
    OLED_Set_Pos(x , y);
    for(wm = 0; wm < 16; wm++)  //
    {
        OLED_WrDat(font16x16[adder]);
        adder += 1;
    }
    OLED_Set_Pos(x, y + 1);
    for(wm = 0; wm < 16; wm++) //
    {
        OLED_WrDat(font16x16[adder]);
        adder += 1;
    }
}
/***********************************************************
*   函数名称：
*   功能描述： 功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED::
OLED_P16x32num(unsigned char x, unsigned char y, unsigned char *bnum)
{
    unsigned char i, j;
    unsigned int adder = 0;              //
    for (j = 0; j < 4; j++)
    {
        OLED_Set_Pos(x , y + j);
        for(i = 0; i < 16; i++)           //
        {
            OLED_WrDat(bnum[adder]);
            adder += 1;
        }
    }
}
/***********************************************************
*   函数名称：
*   功能描述： 功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED::
Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char *bmp)
{
    unsigned int j = 0;
    unsigned char x, y;

    if(y1 % 8 == 0) y = y1 / 8;
    else y = y1 / 8 + 1;
    for(y = y0; y < y1; y++)
    {
        OLED_Set_Pos(x0, y);
        for(x = x0; x < x1; x++)
        {
            OLED_WrDat(bmp[j++]);
        }
    }
}


