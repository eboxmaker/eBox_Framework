#ifndef __LCD_PORT_H
#define __LCD_PORT_H
#include "ebox.h"
#include "lcd_1.8.h"

class LcdPort
{
    public:
        LcdPort(Lcd *_lcd,int16_t w, int16_t h);
        void    begin();
        void    dev_draw_pixel(int16_t x, int16_t y, uint16_t color);
        void    dev_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) ;
        void    dev_draw_v_line(int16_t x, int16_t y, int16_t h, uint16_t color);
        void    dev_draw_h_line(int16_t x, int16_t y, int16_t w, uint16_t color) ;
        void    dev_draw_rect(int16_t x, int16_t y, int16_t x1, int16_t y1, uint16_t color) ;
        void    dev_fill_fect(int16_t x, int16_t y, int16_t x1, int16_t y1, uint16_t color) ;
        void    dev_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
        void    dev_fill_screen(uint16_t color) ;
        void    invertDisplay(bool i) ;
    protected:
        uint16_t _width, _height; // Display w/h as modified by current rotation 

    private:
         Lcd *lcd;
       

};

#endif
