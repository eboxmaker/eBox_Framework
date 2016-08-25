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
 *                    �� �� �� �� ��                      *
 **********************************************************/
OLED_SSD1306::OLED_SSD1306(Gpio *cs_pin, Gpio *res_pin, Gpio *dc_pin, Gpio *scl_pin, Gpio *sda_pin)
{
    this->cs_pin	= cs_pin;
    this->res_pin	= res_pin;
    this->dc_pin	= dc_pin;
    this->scl_pin	= scl_pin;
    this->sda_pin	= sda_pin;
}
void OLED_SSD1306::begin(void)	//��ʼ��SSD1306
{
    res_pin->mode(OUTPUT_PP);
    dc_pin->mode(OUTPUT_PP);
    scl_pin->mode(OUTPUT_PP);
    sda_pin->mode(OUTPUT_PP);
    cs_pin->mode(OUTPUT_PP);
    init();
}

/***********************************************************
*   �������ƣ�
*   ����������   OLEDд����
*   �����б�
*   ���ؽ����
*   ��    ע��
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
*   �������ƣ�
*   ����������   OLEDд����
*   �����б�
*   ���ؽ����
*   ��    ע��
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
*   �������ƣ�
*   ����������   OLED��������
*   �����б�
*   ���ؽ����
*   ��    ע��
***********************************************************/
void OLED_SSD1306::
set_xy(uint16_t x, uint16_t y)
{
    write_cmd(0xb0 + y);
    write_cmd(((x & 0xf0) >> 4) | 0x10);
    write_cmd((x & 0x0f) | 0x01);
}

/***********************************************************
*   �������ƣ�
*   ����������   OLED����
*   �����б�
*   ���ؽ����
*   ��    ע����������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
***********************************************************/
void OLED_SSD1306::clear(void)
{
    uint8_t i, n;
    for(i = 0; i < 8; i++)
    {
        write_cmd(0xb0 + i);  //����Ҳ��ַ(0~7)
        write_cmd (0x00);      //������ʾλ�á��е͵�ַ
        write_cmd (0x10);      //������ʾλ�á��иߵ�ַ
        for(n = 0; n < 128; n++)write_data(0);
    } //������ʾ
}

/***********************************************************
*   �������ƣ�
*   ����������   OLED_SSD1306������ʾ
*   �����б�
*   ���ؽ����
*   ��    ע��
***********************************************************/
void OLED_SSD1306::display_on(void)
{
    write_cmd(0x8d);	//SET DCDC����
    write_cmd(0X14);  //DCDC ON
    write_cmd(0XAF);  //DISPLAY ON
}

/***********************************************************
*   �������ƣ�
*   ����������   OLED_SSD1306�ر���ʾ
*   �����б�
*   ���ؽ����
*   ��    ע��
***********************************************************/
void OLED_SSD1306::display_off(void)
{
    write_cmd(0x8d);	//SET DCDC����
    write_cmd(0X10);  //DCDC ON
    write_cmd(0XAE);  //DISPLAY ON
}

/***********************************************************
*   �������ƣ�
*   ����������   OLED��ʾ�����ַ�
*   �����б�
*   ���ؽ����
*   ��    ע����ָ��λ����ʾһ���ַ�,���������ַ�
***********************************************************/
void OLED_SSD1306::show_char(uint8_t x, uint8_t y, uint8_t chr)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; //�õ�ƫ�ƺ��ֵ
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
*   �������ƣ�
*   ����������   OLED��ʾһ���ַ���
*   �����б�
*   ���ؽ����
*   ��    ע����ָ��λ����ʾһ���ַ���
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
*   �������ƣ�
*   ����������   OLED��ʾһ���ַ���
*   �����б�len :���ֵ�λ����size:�����С��mode:ģʽ	0,���ģʽ;1,����ģʽ
*			  num:��ֵ(0~4294967295);
*   ���ؽ����
*   ��    ע����ָ��λ����ʾһ���ַ���
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
*   �������ƣ�
*   ����������   OLED��ʾһ������
*   �����б�
*   ���ؽ����
*   ��    ע����ָ��λ����ʾһ������
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
*   �������ƣ�
*   ������������ʾBMPͼƬ128��64��ʼ������(x,y),
*   �����б�x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
*   ���ؽ����
*   ��    ע��
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
*   �������ƣ�
*   ����������   OLED_SSD1306��ʼ��
*   �����б�
*   ���ؽ����
*   ��    ע��
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
    write_cmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    write_cmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
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

