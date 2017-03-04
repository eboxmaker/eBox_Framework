#ifndef __LCD_1_8_H
#define __LCD_1_8_H
/**
  ******************************************************************************
  * @file    lcd_1.8.h
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
typedef enum
{
    ENABLE_BACK_COLOR = 0,
    DISABLE_BACK_COLOR = 1
} TEXT_MODE_TYPE;

class Lcd
{
public:
    uint8_t MADCTL;
    u16 front_color;
    u16 back_color;
    TEXT_MODE_TYPE text_mode;
public:
    Lcd(Gpio *cs, Gpio *led, Gpio *rs, Gpio *rst, Spi *spi)
    {
        this->cs = cs;
        this->led = led;
        this->rs = rs;
        this->rst = rst;
        this->spi = spi;
    }
    void begin(uint8_t dev_num);
    void soft_reset();
    void clear(u16 Color);
    void on();
    void off();
    void column_order(uint8_t order);
    void row_order(uint8_t order);

    void set_xy(u16 x, u16 y);
    void draw_pixel(u16 x, u16 y, u16 Data);
        
    void draw_h_line(int x0, int y0, int x1, uint16_t color);
    void draw_v_line(int x0, int y0, int y1, uint16_t color);
    void draw_line(u16 x0, u16 y0, u16 x1, u16 y1, uint16_t color);
    void fill_rect  (int x0, int y0,  int x1, int y1, uint16_t color);
    void fill_rect  (int x0, int y0,  int x1, int y1, u16 *bitmap);
    void draw_circle(u16 x, u16 y, u16 r, uint16_t color);

    
    
    inline void draw_point(u16 x, u16 y, u16 Data);
    u16 read_point(u16 x, u16 y);
    void h_disp_char8x16(u16 x, u16 y, uint8_t ch);
    void disp_char8x16(u16 x, u16 y, uint8_t ch);
    void printf(u16 x, u16 y, const char *fmt, ...);
    void draw_font_gbk16(u16 x, u16 y, uint8_t *s);

    void draw_bitmap(const unsigned char *p); //œ‘ æ40*40 QQÕº∆¨
    

private:
    Gpio *cs;
    Gpio *led;
    Gpio *rs;
    Gpio *rst;
    Spi  *spi;
    SPI_CONFIG_TYPE config;

    void reset(void);
    void init(void);
    void spi_write_data(uint8_t Data);
    void write_index(uint8_t Index);
    void write_data_8bit(uint8_t Data);
    void write_data_16bit(u16 Data);
    uint8_t read_8bit();
    u16 read_16bit();

    void write_reg(uint8_t Index, uint8_t Data);
    uint8_t   read_reg(uint8_t Index);

    void set_region(u16 x_start, u16 y_start, u16 x_end, u16 y_end);

};
#endif
