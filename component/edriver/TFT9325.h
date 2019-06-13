/**
  ******************************************************************************
  * @file    TFT9325.h
  * @author  cat_li
  * @version V1.0
  * @date    2019/06/09
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_ILI9325_H
#define __LCD_ILI9325_H

#include "ebox_core.h"
#include "font.h"
#include "parallel_gpio.h"

#define X_MAX_PIXEL	        170
#define Y_MAX_PIXEL	        220

#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	    //��ɫ0 3165 00110 001011 00101
#define GRAY1   0x8410      	//��ɫ1      00000 000000 00000
#define GRAY2   0x4208      	//��ɫ2  1111111111011111

class Lcd : public Vhmi
{
public:
    Lcd(Gpio *cs,Gpio *rst, Gpio *rd, Gpio *rs, Gpio *wr, ParallelGpio *db)
    {
				_cs = cs;
				_rst = rst;
				_rd = rd;
				_rs = rs;
				_wr = wr;
				_db = db;
    }
    void begin(void);
    void soft_reset(void);
    void clear(uint16_t Color);
    void on();
    void off();
    void column_order(uint8_t order);
    void row_order(uint8_t order);

    void set_xy(int16_t x, int16_t y);
		
		virtual void    draw_pixel(int16_t x, int16_t y, uint32_t color);
    virtual void    draw_v_line(int16_t x0, int16_t y0, int16_t y1, uint32_t color);
    virtual void    draw_h_line(int16_t x0, int16_t y0, int16_t x1, uint32_t color);
//    virtual void    draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color);
//    virtual void    fill_rect(int16_t x, int16_t y, int16_t x1, int16_t y1, uint32_t color);
//    virtual void    draw_circle(int16_t x, int16_t y, int16_t r, uint32_t color);
    virtual void    fill_screen(uint32_t color);
//    virtual void    flush();


    int16_t read_point(int16_t x, int16_t y);
    void h_disp_char8x16(int16_t x, int16_t y, uint8_t ch);
    void disp_char8x16(int16_t x, int16_t y, uint8_t ch);
    void printf(int16_t x, int16_t y, const char *fmt, ...);
    void draw_font_gbk16(int16_t x, int16_t y, uint8_t *s);

    void draw_bitmap(const unsigned char *p); //��ʾ40*40 QQͼƬ


private:
    Gpio *_cs;		// Ƭѡ
    Gpio *_rst;		// ��λ
    Gpio *_rd;		// ��
    Gpio *_rs;		// �Ĵ���
    Gpio *_wr;		// д

		uint16_t	_width;		//  ���
		uint16_t 	_height;	//	�߶�
		
		ParallelGpio *_db;	// ������

		void _init(void);
		void _write(uint16_t dat,uint8_t cmd);
		void _setRegion(int16_t x_start, int16_t y_start,int16_t x_end, int16_t y_end);

		void _setCommandMode();
		void _setDataMode();
		void _setWrite();
		void _writeCommand(uint16_t cmd,uint16_t dat);
		void _setIndexReg(uint16_t reg);
		void _writeDate(uint16_t dat);
		uint16_t _readReg(uint16_t reg);
};
#endif
