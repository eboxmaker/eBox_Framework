#ifndef __EBOX_VHMI_H
#define __EBOX_VHMI_H

#include "ebox_type.h"

class Vhmi
{
    public:
        virtual void    draw_pixel(int16_t x, int16_t y, uint32_t color) = 0;
        virtual void    draw_v_line(int16_t x, int16_t y, int16_t h, uint32_t color);
        virtual void    draw_h_line(int16_t x, int16_t y, int16_t w, uint32_t color);
        virtual void    draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color);
        virtual void    fill_rect(int16_t x, int16_t y, int16_t x1, int16_t y1, uint32_t color);
        virtual void    draw_circle(int16_t x, int16_t y, int16_t r, uint32_t color);
        virtual void    fill_screen(uint32_t color) = 0 ;
};
#endif
