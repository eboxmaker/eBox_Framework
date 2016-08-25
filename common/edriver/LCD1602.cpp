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
/******************************д����*************************************************/
/*********************************************************************
*��    �ƣ�Writecmd()
*��    �ܣ�дָ�Lcd1602,ָ������ռһ���ֽ�
*��ڲ�����cmd:��д���ָ��
*���ڲ�������
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
    //���Ӳ������ʱ˳��Ľ���ʹ�����·�ʽ
    //	DATAOUT &= 0XFF00;
    //	DATAOUT |= cmd;


    lcd_delay(delay_times);
    en->set();
    lcd_delay(delay_times);
    en->reset();
    lcd_delay(delay_times);
}
/*********************************************************************
*��    �ƣ�Writedata()
*��    �ܣ�дһ�ֽ����ݵ�Lcd1602
*��ڲ�����dat���޷����ֽ����ͣ�0~255  ��������ASCII���ַ�
*���ڲ�������
*********************************************************************/
void Lcd1602::wdat(uint8_t dat)
{
    while(bz())//�ȴ�LCD1602����
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


    //���Ӳ������ʱ˳��Ľ���ʹ�����·�ʽ
    //	DATAOUT &= 0XFF00;
    //	DATAOUT |= dat;

    lcd_delay(delay_times);
    en->set();
    lcd_delay(delay_times);
    en->reset();
}

/*********************************************************************
*��    �ƣ�pos()
*��    �ܣ��趨��ʾλ��
*��ڲ�����pos:��ʾλ��,ֵ�ķ�Χ���£�
 *   0x00----------------0x0f 0x10-------0x27 ��һ�У�һ����ʾ16���ַ���
 *   0x40----------------0x4f 0x50-------0x67 �ڶ���
*���ڲ�������
*********************************************************************/
void Lcd1602::pos(uint8_t pos)
{
    wcmd(pos | 0x80);
}

/*********************************************************************
*��    �ƣ�setPos()
*��    �ܣ�����ϰ���趨��ʾλ��
*��ڲ�����row:�У�row=1��ʾ��һ�У�row=2��ʾ�ڶ���
 *         col:�У�0~15������ָ����ʾ���У���Χ������0~40
*���ڲ�������
*********************************************************************/
void Lcd1602::set_pos(uint8_t row, uint8_t col)
{
    if(row == 1) wcmd(col | 0x80);
    else wcmd(col | 0xC0);
}
/*********************************************************************
*��  �ܣ���ʾһ���ַ�
*��  �ڣ�ch������ʾ���ַ�
*********************************************************************/
void Lcd1602::disp_char(char ch)
{
    wdat(ch);
}
/*********************************************************************
*��  �ܣ���ָ��λ����ʾһ���ַ�
*��  �ڣ�row:�� 1��2        col:�У�0~15
 *       ch������ʾ���ַ�
*********************************************************************/
void Lcd1602::disp_char(uint8_t row, uint8_t col, char ch)
{
    set_pos(row, col);
    wdat(ch);
}
/*********************************************************************
*��    �ƣ�dispString()
*��    �ܣ�ʹLCD1602��ʾһ���ַ�������ʾλ������ǰ�趨
*��ڲ�����str[]:����ʾ���ַ���
*���ڲ�������
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
*��    �ƣ�setPos_dispString()
*��    �ܣ�ʹLcd1602��ָ��λ�ÿ�ʼ��ʾһ���ַ���
*��ڲ�����row:��ʾ�У�1��2
 *         col:��ʾ�У�0~15 ��0~40��
 *         str[]:����ʾ���ַ���
*���ڲ�������
*˵    ����ָ��λ������ʾ�ĳ�ʼλ�ã���һ���ַ���ʾ��λ��
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
*��    �ƣ�DispDateTime()
*��    �ܣ���ʾ��������ʾ������ʱ��
*��ڲ�����year:��,����������ֵ
 *         month:�£�1~12
 *         day:�գ�1~31
 *         hour:ʱ��1~24 ��  1~12
 *         min:�֣�0~59
 *         sec:�룬0~59
*���ڲ�������
*˵    ����ʹ��ǰ�����Ⱥ궨��USE_DISP_DATE���ڰ����ļ�֮ǰ������Ч
*��ʾ��ʽ��Date:yyyy-mm-dd  *
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
    disp_char('*'); //��һ�н�������ʾ
    set_pos(2, 0);
    disp_string("Time:  ");

    sprintf(s, "%02d:%02d:%02d", hour, min, sec);
    disp_string(s);

    set_pos(2, 15);
    disp_char('*'); //�ڶ��н�������ʾ
}

void Lcd1602::disp_time_HMS(uint8_t hour, uint8_t min, uint8_t sec)
{
    char s[8];

    set_pos(2, 0);
    disp_string("Time:  ");

    sprintf(s, "%02d:%02d:%02d", hour, min, sec);
    disp_string(s);

    set_pos(2, 15);
    disp_char('*'); //�ڶ��н�������ʾ
}
//��Ļ��������һ�����ڹ�����ʾ
void Lcd1602::shift_left()
{
    wcmd(0x18);
}
//��Ļ��������һ�����ڹ�����ʾ
void Lcd1602::shift_right(void)
{
    wcmd(0x1C);
}
//�����������ʾ
void Lcd1602::clear(void)
{
    wcmd(0x01);
}
//ʹ��껹��ԭ��λ��
void Lcd1602::Return(void)
{
    wcmd(0x02);
}
//����ʾ
void Lcd1602::close(void)
{
    wcmd(0x08);
}
//����ʾ
void Lcd1602::open(void)
{
    wcmd(0x0C);
}
/*********************************************************************
*��    �ƣ�FlickerChar()
*��    �ܣ�ʹָ��λ���ַ���˸������ʾ���
*��ڲ�����row:�� 1,2
 *         col:�� 0~15
*���ڲ�������
*˵    ����д����0x0D����ʾ������˸��д����0x0F����ʾ������˸
 *         һ���趨��˸�󣬻����λ�ñ仯��˸������˸д����0x0C
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
//�ر��ַ���˸
void Lcd1602::close_flicker(void)
{
    wcmd(0x0C);
}
//��Ļ����˸һ��
void Lcd1602::flicker_screen(void)
{
    wcmd(0x08);//����ʾ
    lcd_delay(0x3fffff);
    wcmd(0x0C);//����ʾ
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
*��    �ƣ�Init()
*��    �ܣ���ʼ��Lcd1602
*��ڲ�������
*���ڲ�������
*�������0x38:16*2��ʾ��5*&������ʾ�ַ���8λ����;ָ��ִ��ʱ��40us
 *         0x0C:����ʾ���ع��	  40us
 *         0x08:����ʾ���ع��	  40us
 *         0x0D:�ַ���˸���ع�꣬�����趨λ�ã���˸����0.4ms����
 *         0x0F:�ַ���˸�������
 *         0x06:д�������Զ����ƹ�꣬��ͨ���Σ�����������ʾ  40us
 *         0x04:д�������Զ����ƹ�꣬���Դ���������ʾ	 40us
 *         0x01:�����ʾ���ݣ�������  1.64ms
 *         0x02:ʹ��껹����ʼλ��	  1.64ms
 *         0x18:��Ļ�������ַ�ͬʱ����һ���ʺϹ�����ʾ 40us
 *         0x1C:��Ļ�������ַ�ͬʱ����һ���ʺϹ�����ʾ 40us
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

    wcmd(0x38);  //16*2��ʾ��5*7����8λ����
    lcd_delay(100);
    wcmd(0x38);  //16*2��ʾ��5*7����8λ����
    lcd_delay(100);
    wcmd(0x38);  //16*2��ʾ��5*7����8λ����
    lcd_delay(100);
    wcmd(0x08);  //�ȹ���ʾ������ʾ
    lcd_delay(100);
    wcmd(0x01);  //���LCD����ʾ����
    lcd_delay(100);
    wcmd(0x06);  //�Զ����ƹ��,0x04Ϊ���ƹ��
    lcd_delay(100);
    wcmd(0x0c);  //��ʾ�����ع��;0x08Ϊ����ʾ
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

