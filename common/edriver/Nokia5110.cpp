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
LCD_init: 5110LCD��ʼ��
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

    write_cmd(0x21);	// ʹ����չ��������LCDģʽ
    write_cmd(0xba);	// ����ƫ�õ�ѹ
    write_cmd(0x06);	// �¶�У��
    write_cmd(0x13);	// 1:48
    write_cmd(0x20);	// ʹ�û�������
    clear();	        // ����
    write_cmd(0x0c);	// �趨��ʾģʽ��������ʾ

    LCD_CE.reset();
    back_led(1);

}


/*-----------------------------------------------------------------------
LCD_clear         : LCD��������
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
LCD_set_XY        : ����LCD���꺯��

���������X       ��0��83
          Y       ��0��5
-----------------------------------------------------------------------*/
void NOKIA5110::set_xy(unsigned char X, unsigned char Y)
{
    write_cmd(0x40 | Y);		// column
    write_cmd(0x80 | X);          	// row
}

/*-----------------------------------------------------------------------
back_led        : ����LCD����

���������val��	1��on
								0��off
-----------------------------------------------------------------------*/
void NOKIA5110::back_led(uint8_t val)
{
    LCD_LED.write(val);
}

/*-----------------------------------------------------------------------
contrast        : ����LCD�Աȶ�

���������val��	0-255
-----------------------------------------------------------------------*/
void NOKIA5110::contrast(uint8_t val)
{
    write_cmd(0xba);	// ����ƫ�õ�ѹ
}

/*-----------------------------------------------------------------------
LCD_write_char    : ��ʾӢ���ַ�

���������c       ����ʾ���ַ���
-----------------------------------------------------------------------*/
void NOKIA5110::disp_char(unsigned char c)
{
    unsigned char line;

    c -= 32;

    for (line = 0; line < 6; line++)
        write_data(font6x8[c][line]);
}

/*-----------------------------------------------------------------------
LCD_write_english_String  : Ӣ���ַ�����ʾ����

���������*s      ��Ӣ���ַ���ָ�룻
          X��Y    : ��ʾ�ַ�����λ��,x 0-83 ,y 0-5
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
LCD_write_chinese_string: ��LCD����ʾ����

���������X��Y    ����ʾ���ֵ���ʼX��Y���ꣻ
          ch_with �����ֵ���Ŀ��
          num     ����ʾ���ֵĸ�����
          line    �����ֵ��������е���ʼ����
          row     ��������ʾ���м��
���ԣ�
	LCD_write_chi(0,0,12,7,0,0);
	LCD_write_chi(0,2,12,7,0,0);
	LCD_write_chi(0,4,12,7,0,0);
-----------------------------------------------------------------------*/
void NOKIA5110::write_chinese_string(unsigned char X, unsigned char Y,
                                     unsigned char ch_with, unsigned char num,
                                     unsigned char line, unsigned char row)
{
    unsigned char i, n;

    set_xy(X, Y);                            //���ó�ʼλ��

    for (i = 0; i < num;)
    {
        for (n = 0; n < ch_with * 2; n++)        //дһ������
        {
            if (n == ch_with)                    //д���ֵ��°벿��
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
LCD_draw_map      : λͼ���ƺ���

���������X��Y    ��λͼ���Ƶ���ʼX��Y���ꣻ
          *map    ��λͼ�������ݣ�
          Pix_x   ��λͼ���أ�����
          Pix_y   ��λͼ���أ���
-----------------------------------------------------------------------*/
void NOKIA5110::draw_bmp_pixel(unsigned char X, unsigned char Y, unsigned char *map,
                               unsigned char Pix_x, unsigned char Pix_y)
{
    unsigned int i, n;
    unsigned char row;

    if (Pix_y % 8 == 0) row = Pix_y / 8; //����λͼ��ռ����
    else
        row = Pix_y / 8 + 1;

    for (n = 0; n < row; n++)
    {
        set_xy(X, Y);
        for(i = 0; i < Pix_x; i++)
        {
            write_data(map[i + n * Pix_x]);
        }
        Y++;                         //����
    }
}

/*-----------------------------------------------------------------------
LCD_write_byte    : д���ݵ�LCD
���������data    �����ݣ�
-----------------------------------------------------------------------*/
void NOKIA5110::write_data(unsigned char dat)
{
    LCD_CE.reset(); // ʹ��LCD
    LCD_DC.set();// ��������
    shift_out(&SDIN, &SCLK, MSB_FIRST, dat);
    LCD_CE.set();

}

/*-----------------------------------------------------------------------
write_cmd 			: д���LCD
���������cmd 	������
-----------------------------------------------------------------------*/
void NOKIA5110::write_cmd(unsigned char cmd)
{
    LCD_CE.reset(); // ʹ��LCD
    LCD_DC.reset();// ��������
    shift_out(&SDIN, &SCLK, MSB_FIRST, cmd);
    LCD_CE.set();
}


