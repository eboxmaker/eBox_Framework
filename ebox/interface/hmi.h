#ifndef __EBOX_HMI_H
#define __EBOX_HMI_H

#include "ebox_type.h"
#include "enum.h"
#include "color_table.h"


uint16_t bgr2rgb(uint16_t c);
uint16_t rgb24_2_rgb565(uint32_t color);   //应该会损失数据内容

class HMI
{
public:
    virtual void    draw_pixel(int16_t x, int16_t y, uint32_t color) = 0;
    virtual void    draw_v_line(int16_t x0, int16_t y0, int16_t y1, uint32_t color);
    virtual void    draw_h_line(int16_t x0, int16_t y0, int16_t x1, uint32_t color);
    virtual void    draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color);
    virtual void    fill_rect(int16_t x, int16_t y, int16_t x1, int16_t y1, uint32_t color);
    virtual void    draw_circle(int16_t x, int16_t y, int16_t r, uint32_t color);
    virtual void    fill_screen(uint32_t color) = 0 ;
    virtual void    flush();
};
#endif
