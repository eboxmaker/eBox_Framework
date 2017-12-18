/**
  ******************************************************************************
  * @file    nokia5110.h
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

/* Define to prevent recursive inclusion -------------------------------------*/

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
    void back_led(uint8_t val);
    void contrast(uint8_t val);

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
