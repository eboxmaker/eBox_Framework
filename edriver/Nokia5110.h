/*
file   : nokia5110.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

#include "ebox.h"

#define SCLK 		PB13
#define SDIN 		PB12
#define LCD_DC 		PB11
#define LCD_CE 		PB10
#define LCD_RST 	PB9
#define LCD_LED 	PB8

class NOKIA5110
{
public:

    void begin();
    void clear(void);
    void set_xy(unsigned char X, unsigned char Y);
    void back_led(u8 val);
    void contrast(u8 val);

    void write_data(unsigned char dat);
    void write_cmd(unsigned char cmd);

    void disp_char(unsigned char c);
    void disp_string(unsigned char X, unsigned char Y, const char *s);
    void printf(uint8_t row, uint8_t col, const char *fmt, ...);


    void write_chinese_string(unsigned char X, unsigned char Y,
                              unsigned char ch_with, unsigned char num,
                              unsigned char line, unsigned char row);
    void draw_bmp_pixel(unsigned char X, unsigned char Y, unsigned char *map,
                        unsigned char Pix_x, unsigned char Pix_y);

};
