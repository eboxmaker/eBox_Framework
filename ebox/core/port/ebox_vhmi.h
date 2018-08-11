#ifndef __EBOX_VHMI_H
#define __EBOX_VHMI_H

#include "ebox_type.h"


class Vhmi
{
    public:
        virtual void    draw_pixel(uint16_t x, uint16_t y, uint16_t color) = 0;
        virtual void    draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) = 0 ;
        virtual void    draw_v_line(uint16_t x, uint16_t y, uint16_t h, uint16_t color) = 0;
        virtual void    draw_h_line(uint16_t x, uint16_t y, uint16_t w, uint16_t color) = 0 ;
        virtual void    fill_rect(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, uint16_t color) = 0 ;
//        virtual void    draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color) = 0;
        virtual void    fill_screen(uint16_t color) = 0 ;
//        virtual void    invertDisplay(bool i) = 0 ;
    protected:
};

#endif
