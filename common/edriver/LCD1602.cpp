/**
  ******************************************************************************
  * @file    lcd1602.cpp
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
#include "lcd1602.h"
#include "stdio.h"

#define DATAOUT  GPIOB->ODR             //PD[0..7]--(LCD)D0~D7

Lcd1602::Lcd1602(Gpio *led, Gpio *en, Gpio *rw, Gpio *rs, Gpio *d0, Gpio *d1, Gpio *d2, Gpio *d3, Gpio *d4, Gpio *d5, Gpio *d6, Gpio *d7)
{
    this->led   = led;
    this->en	= en;
    this->rw	= rw;
    this->rs	= rs;
    this->d0    = d0;
    this->d1	= d1;
    this->d2	= d2;
    this->d3	= d3;
    this->d4	= d4;
    this->d5	= d5;
    this->d6	= d6;
    this->d7	= d7;

    delay_times = 1;
}

inline void Lcd1602::lcd_delay(uint32_t nCount)
{
    for(; nCount != 0; nCount--);
}
inline uint8_t Lcd1602::bz(void)
{
    uint8_t result;


    d7->mode(INPUT);
    rs->reset();
    rw->set();
    en->set();
    lcd_delay(delay_times);
    result = d7->read();//
    en->reset();

    return result;

}
/******************************写命令*************************************************/
/*********************************************************************
*名    称：Writecmd()
*功    能：写指令到Lcd1602,指令数据占一个字节
*入口参数：cmd:待写入的指令
*出口参数：无
*********************************************************************/
void Lcd1602::wcmd(uint8_t cmd)
{
    while(bz())
    {
        ;
    }

    d7->mode(OUTPUT_PP);
    rs->reset();
    rw->reset();
    en->reset();

    d0->write(cmd & 0x01);
    d1->write(cmd & 0x02);
    d2->write(cmd & 0x04);
    d3->write(cmd & 0x08);
    d4->write(cmd & 0x10);
    d5->write(cmd & 0x20);
    d6->write(cmd & 0x40);
    d7->write(cmd & 0x80);
    //如果硬件连接时顺序的建议使用以下方式
    //	DATAOUT &= 0XFF00;
    //	DATAOUT |= cmd;


    lcd_delay(delay_times);
    en->set();
    lcd_delay(delay_times);
    en->reset();
    lcd_delay(delay_times);
}
/*********************************************************************
*名    称：Writedata()
*功    能：写一字节数据到Lcd1602
*入口参数：dat：无符号字节类型，0~255  包括各个ASCII码字符
*出口参数：无
*********************************************************************/
void Lcd1602::wdat(uint8_t dat)
{
    while(bz())//等待LCD1602空闲
    {
        ;
    }

    d7->mode(OUTPUT_PP);
    rs->set();
    rw->reset();
    en->reset();

    d0->write(dat & 0x01);
    d1->write(dat & 0x02);
    d2->write(dat & 0x04);
    d3->write(dat & 0x08);
    d4->write(dat & 0x10);
    d5->write(dat & 0x20);
    d6->write(dat & 0x40);
    d7->write(dat & 0x80);


    //如果硬件连接时顺序的建议使用以下方式
    //	DATAOUT &= 0XFF00;
    //	DATAOUT |= dat;

    lcd_delay(delay_times);
    en->set();
    lcd_delay(delay_times);
    en->reset();
}

/*********************************************************************
*名    称：pos()
*功    能：设定显示位置
*入口参数：pos:显示位置,值的范围如下：
 *   0x00----------------0x0f 0x10-------0x27 第一行（一次显示16个字符）
 *   0x40----------------0x4f 0x50-------0x67 第二行
*出口参数：无
*********************************************************************/
void Lcd1602::pos(uint8_t pos)
{
    wcmd(pos | 0x80);
}

/*********************************************************************
*名    称：setPos()
*功    能：根据习惯设定显示位置
*入口参数：row:行，row=1表示第一行，row=2表示第二行
 *         col:列，0~15，用于指定显示的列，范围可以是0~40
*出口参数：无
*********************************************************************/
void Lcd1602::set_pos(uint8_t row, uint8_t col)
{
    if(row == 1) wcmd(col | 0x80);
    else wcmd(col | 0xC0);
}
/*********************************************************************
*功  能：显示一个字符
*入  口：ch：待显示的字符
*********************************************************************/
void Lcd1602::disp_char(char ch)
{
    wdat(ch);
}
/*********************************************************************
*功  能：在指定位置显示一个字符
*入  口：row:行 1或2        col:列，0~15
 *       ch：待显示的字符
*********************************************************************/
void Lcd1602::disp_char(uint8_t row, uint8_t col, char ch)
{
    set_pos(row, col);
    wdat(ch);
}
/*********************************************************************
*名    称：dispString()
*功    能：使LCD1602显示一个字符串，显示位置需提前设定
*入口参数：str[]:待显示的字符串
*出口参数：无
*********************************************************************/
void Lcd1602::disp_string(char str[])
{
    uint8_t i = 0;
    while(str[i] != '\0')
    {
        wdat(str[i]);
        ++i;
    }
}
/*********************************************************************
*名    称：setPos_dispString()
*功    能：使Lcd1602从指定位置开始显示一个字符串
*入口参数：row:显示行，1或2
 *         col:显示列，0~15 （0~40）
 *         str[]:待显示的字符串
*出口参数：无
*说    明：指定位置是显示的初始位置，第一个字符显示的位置
*********************************************************************/
void Lcd1602::disp_string(uint8_t row, uint8_t col, char str[])
{
    set_pos(row, col);
    disp_string(str);
}
void Lcd1602::printf(const char *fmt, ...)
{
    char buf[16];
    uint8_t i = 0;
    va_list va_params;
    va_start(va_params, fmt);
    vsprintf(buf, fmt, va_params);
    va_end(va_params);

    while(buf[i] != '\0')
    {
        wdat(buf[i++]);
    }
}
void Lcd1602::printf(uint8_t row, uint8_t col, const char *fmt, ...)
{
    char buf[16];
    uint8_t i = 0;
    va_list va_params;
    va_start(va_params, fmt);
    vsprintf(buf, fmt, va_params);
    va_end(va_params);
    set_pos(row, col);
    while(buf[i] != '\0')
    {
        wdat(buf[i++]);
    }
}

/*********************************************************************
*名    称：DispDateTime()
*功    能：显示日历，显示日期与时间
*入口参数：year:年,可以是任意值
 *         month:月，1~12
 *         day:日，1~31
 *         hour:时，1~24 或  1~12
 *         min:分，0~59
 *         sec:秒，0~59
*出口参数：无
*说    明：使用前必须先宏定义USE_DISP_DATE，在包含文件之前定义有效
*显示格式：Date:yyyy-mm-dd  *
 *         Time:hh:mm:ss    *
*********************************************************************/

void Lcd1602::disp_date_time(uint32_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
    char s[8];
    set_pos(1, 0);
    disp_string("Date:");

    sprintf(s, "%04d-%02d-%02d", year, month, day);
    disp_string(s);


    set_pos(1, 15);
    disp_char('*'); //第一行结束符显示
    set_pos(2, 0);
    disp_string("Time:  ");

    sprintf(s, "%02d:%02d:%02d", hour, min, sec);
    disp_string(s);

    set_pos(2, 15);
    disp_char('*'); //第二行结束符显示
}

void Lcd1602::disp_time_HMS(uint8_t hour, uint8_t min, uint8_t sec)
{
    char s[8];

    set_pos(2, 0);
    disp_string("Time:  ");

    sprintf(s, "%02d:%02d:%02d", hour, min, sec);
    disp_string(s);

    set_pos(2, 15);
    disp_char('*'); //第二行结束符显示
}
//屏幕整体左移一格，用于滚动显示
void Lcd1602::shift_left()
{
    wcmd(0x18);
}
//屏幕整体右移一格，用于滚动显示
void Lcd1602::shift_right(void)
{
    wcmd(0x1C);
}
//清屏，清除显示
void Lcd1602::clear(void)
{
    wcmd(0x01);
}
//使光标还回原点位置
void Lcd1602::Return(void)
{
    wcmd(0x02);
}
//关显示
void Lcd1602::close(void)
{
    wcmd(0x08);
}
//开显示
void Lcd1602::open(void)
{
    wcmd(0x0C);
}
/*********************************************************************
*名    称：FlickerChar()
*功    能：使指定位置字符闪烁，不显示光标
*入口参数：row:行 1,2
 *         col:列 0~15
*出口参数：无
*说    明：写命令0x0D不显示光标的闪烁，写命令0x0F是显示光标的闪烁
 *         一旦设定闪烁后，会根据位置变化闪烁，关闪烁写命令0x0C
*********************************************************************/
void Lcd1602::flicker(void)
{
    wcmd(0x0D);
}
void Lcd1602::flicker_char(uint8_t row, uint8_t col)
{
    wcmd(0x0D);
    set_pos(row, col);
}
//关闭字符闪烁
void Lcd1602::close_flicker(void)
{
    wcmd(0x0C);
}
//屏幕秒闪烁一次
void Lcd1602::flicker_screen(void)
{
    wcmd(0x08);//关显示
    lcd_delay(0x3fffff);
    wcmd(0x0C);//开显示
    lcd_delay(0x3fffff);
}
void Lcd1602::back_light(uint8_t i)
{
    if(i == 1)
        led->set();
    else
        led->reset();
}

/*********************************************************************
*名    称：Init()
*功    能：初始化Lcd1602
*入口参数：无
*出口参数：无
*常用命令：0x38:16*2显示，5*&点阵显示字符，8位数据;指令执行时间40us
 *         0x0C:开显示，关光标	  40us
 *         0x08:关显示，关光标	  40us
 *         0x0D:字符闪烁，关光标，接着设定位置，闪烁周期0.4ms左右
 *         0x0F:字符闪烁，开光标
 *         0x06:写完数据自动右移光标，普通情形，从左向右显示  40us
 *         0x04:写完数据自动左移光标，可以从右向左显示	 40us
 *         0x01:清除显示内容，即清屏  1.64ms
 *         0x02:使光标还回起始位置	  1.64ms
 *         0x18:屏幕上所有字符同时左移一格，适合滚动显示 40us
 *         0x1C:屏幕上所有字符同时右移一格，适合滚动显示 40us
*********************************************************************/
void Lcd1602::begin(void)
{
    led->mode(OUTPUT_PP);
    en->mode(OUTPUT_PP);
    rw->mode(OUTPUT_PP);
    rs->mode(OUTPUT_PP);
    d0->mode(OUTPUT_PP);
    d1->mode(OUTPUT_PP);
    d2->mode(OUTPUT_PP);
    d3->mode(OUTPUT_PP);
    d4->mode(OUTPUT_PP);
    d5->mode(OUTPUT_PP);
    d6->mode(OUTPUT_PP);
    d7->mode(OUTPUT_PP);

    wcmd(0x38);  //16*2显示，5*7点阵，8位数据
    lcd_delay(100);
    wcmd(0x38);  //16*2显示，5*7点阵，8位数据
    lcd_delay(100);
    wcmd(0x38);  //16*2显示，5*7点阵，8位数据
    lcd_delay(100);
    wcmd(0x08);  //先关显示，后开显示
    lcd_delay(100);
    wcmd(0x01);  //清除LCD的显示内容
    lcd_delay(100);
    wcmd(0x06);  //自动右移光标,0x04为左移光标
    lcd_delay(100);
    wcmd(0x0c);  //显示开，关光标;0x08为关显示
    lcd_delay(100);
    back_light(1);
}
void Lcd1602::test(void)
{
    char str[] = {"Hello World  !"};
    disp_string(1, 1, str);
    flicker_char(2, 3);
    flicker_screen();
    flicker_screen();
    flicker_screen();
}

