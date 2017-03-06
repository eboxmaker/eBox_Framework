#include "lcd_port.h"

#define LCD_ROTATION 0
#define GUI_Optimize 1

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; };
#endif

LcdPort::LcdPort(Lcd *_lcd,int16_t w, int16_t h)
{
    lcd = _lcd;
    #if LCD_ROTATION == 0 || LCD_ROTATION > 3 //正向
        _width = w;
        _height = h;
    #elif LCD_ROTATION == 1//顺时针旋转90度
        _width = h;
        _height = w;
    #elif LCD_ROTATION == 2//顺时针旋转180度
        _width = w;
        _height = h;
    #elif LCD_ROTATION == 3//顺时针旋转270度
        _width = h;
        _height = w;
    #endif
}
void LcdPort::begin()
{
    #if LCD_ROTATION == 0 || LCD_ROTATION > 3 //正向
       lcd->column_order(1);
       lcd->row_order(1);
    #elif LCD_ROTATION == 1//顺时针旋转90度.
       lcd->column_order(0);
       lcd->row_order(1);
    #elif LCD_ROTATION == 2//顺时针旋转180度
       lcd->column_order(0);
       lcd->row_order(0);
    #elif LCD_ROTATION == 3//顺时针旋转270度
       lcd->column_order(1);
       lcd->row_order(0);
    #endif
}
void LcdPort::dev_draw_pixel(int16_t x, int16_t y, uint16_t color)
{
    #if LCD_ROTATION == 0 || LCD_ROTATION > 3 //正向
        lcd->draw_pixel(x,y,color);
    #elif LCD_ROTATION == 1//顺时针旋转90度
        lcd->draw_pixel(y,x,color);
    #elif LCD_ROTATION == 2//顺时针旋转180度
        lcd->draw_pixel(x,y,color);
    #elif LCD_ROTATION == 3//顺时针旋转270度
        lcd->draw_pixel(y,x,color);
    #endif

}
void LcdPort::dev_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    #if GUI_Optimize
        lcd->draw_line(x0,y0,x1,y1,color);
    #else
        int16_t steep = abs(y1 - y0) > abs(x1 - x0);
        if (steep) {
            _swap_int16_t(x0, y0);
            _swap_int16_t(x1, y1);
        }

        if (x0 > x1) {
            _swap_int16_t(x0, x1);
            _swap_int16_t(y0, y1);
        }

        int16_t dx, dy;
        dx = x1 - x0;
        dy = abs(y1 - y0);

        int16_t err = dx / 2;
        int16_t ystep;

        if (y0 < y1) {
            ystep = 1;
        } else {
            ystep = -1;
        }

        for (; x0<=x1; x0++) {
            if (steep) {
                drawPixel(y0, x0, color);
            } else {
                drawPixel(x0, y0, color);
            }
            err -= dy;
            if (err < 0) {
                y0 += ystep;
                err += dx;
            }
        }
  #endif

} 
void LcdPort::dev_draw_v_line(int16_t x0, int16_t y0, int16_t y1, uint16_t color)
{
    #if emWin_Optimize
        lcd->draw_v_line(x0,y0,y1,color); 
    #else
	    for (; y0 <= y1; y0++) {
	      lcd->draw_pixel(x0, y0, color);
	    }
    #endif
} 
void LcdPort::dev_draw_h_line(int16_t x0, int16_t y0, int16_t x1, uint16_t color)
{
    #if GUI_Optimize
        lcd->draw_h_line(x0,y0,x1,color);
    #else
	    for (; x0 <= x1; x0++) {
	      lcd->draw_pixel(x0, y0, color);
	    }
    #endif
}
void LcdPort::dev_draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    #if GUI_Optimize
        lcd->draw_h_line(x0, y0, x1, color);
        lcd->draw_h_line(x0, y1, x1, color);
        lcd->draw_v_line(x0, y0, y1, color);
        lcd->draw_v_line(x1, y0, y1, color);
    #else
        drawHLine(x0, y0, x1, color);
        drawHLine(x0, y1, x1, color);
        drawVLine(x0, y0, y1, color);
        drawVLine(x1, y0, y1, color);
    #endif
}
void LcdPort::dev_fill_fect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    #if emWin_Optimize
        lcd->fill_rect(x0,y0,x1,y1,color);
    #else
        lcd->fill_rect(x0,y0,x1,y1,color);
    #endif
}
void LcdPort::dev_draw_circle(u16 x, u16 y, u16 r, uint16_t color)
{
    lcd->draw_circle(x,y,r,color);
}

void LcdPort::dev_fill_screen(uint16_t color) 
{
    lcd->clear(color);
}

void LcdPort::invertDisplay(bool i) 
{
//    lcd->drawPixel(x,y,color);
}

