#ifndef __GRAPHIC_H
#define __GRAPHIC_H

#include "ebox_core.h"


#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

class Graphic
{
    
    public:
        enum{
            
        };
    uint32_t color, back_color;

private:
    Vhmi     *lcd;
    uint8_t  rotation;
    uint16_t width, height; //  LCD属性。初始化后不可更改
    uint16_t _width, _height; // Display w/h as modified by current rotation
    int16_t  cursor_x, cursor_y;

    uint8_t  draw_mode;


public:
    Graphic(Vhmi *_lcd, uint16_t w, uint16_t h);
    void begin();

    //fun
    void clear();
    void flush();
    //settings
    void        set_rotation(uint8_t value);
    int16_t     ro_x(int16_t x, int16_t y);
    int16_t     ro_y(int16_t x, int16_t y);
    void        set_color(uint32_t color);
    void        set_back_color(uint32_t back_color);
    void        set_cursor(int16_t x, int16_t y);
    void        set_draw_mode(uint8_t mode);
    uint16_t    bgr2rgb(uint16_t c);

    //port
    void draw_pixel(int16_t x, int16_t y);
    void draw_pixel(int16_t x, int16_t y, uint32_t color);

    void draw_h_line(int16_t x0, int16_t y0, int16_t x1);
    void draw_h_line(int16_t x0, int16_t y0, int16_t x1, uint32_t color);
    
    void draw_v_line(int16_t x0, int16_t y0, int16_t y1);
    void draw_v_line(int16_t x0, int16_t y0, int16_t y1, uint32_t color);
    
    void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color);
    
    void draw_rect(int16_t x, int16_t y, int16_t x1, int16_t y1);
    void draw_rect(int16_t x, int16_t y, int16_t x1, int16_t y1, uint32_t color);
    
    void fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    void fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint32_t color);
    void fill_screen(uint32_t color) ;

    //graphic
    void draw_circle(int16_t x0, int16_t y0, int16_t r);
    void fill_circle(int16_t x0, int16_t y0, int16_t r);
    void draw_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername);
    void fill_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta);
    void draw_round_rect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius);
    void fill_round_rect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius);
    void draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    void fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

//    void box(int16_t x, int16_t y, int16_t w, int16_t h, int16_t bc);
//    void box2(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t mode);
    void btn_down(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    void btn_up(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

};
extern Graphic *_gpu;
#endif
