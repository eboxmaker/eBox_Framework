#include "graphic.h"

Graphic *_gpu;

Graphic::Graphic(HMI *_lcd, uint16_t w, uint16_t h)
{
    lcd = _lcd;
    lcd_width = w;
    lcd_height = h;
    _gpu = this;
}

void Graphic::begin()
{
    //设置显示器方向
    set_rotation(0);

    //设置前后景颜色
    set_back_color(0);
    set_color(0xffff);

    
    set_text_auto_reline(0);
    _gpu->set_font(&GUI_Font16_ASCII);
    set_text_mode(GuiDrawMode::Normal);
};


/*********************************************************************
*
*       Graphic settings
*
**********************************************************************
*/

void Graphic::set_color(uint32_t color)
{
    this->color = (color);
}
void Graphic::set_back_color(uint32_t back_color)
{
    this->back_color = (back_color);    
}

void Graphic::set_cursor(int16_t x, int16_t y)
{
    this->cursor_x = x;
    this->cursor_y = y;
}
void Graphic::set_draw_mode(uint8_t mode)
{
    draw_mode = mode;
}

/*********************************************************************
*
*       Graphic graphic
*
**********************************************************************
*/
void Graphic::draw_circle(int16_t x0, int16_t y0, int16_t r)
{

    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    draw_pixel(x0, y0 + r);
    draw_pixel(x0, y0 - r);
    draw_pixel(x0 + r, y0  );
    draw_pixel(x0 - r, y0  );

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        draw_pixel(x0 + x, y0 + y);
        draw_pixel(x0 - x, y0 + y);
        draw_pixel(x0 + x, y0 - y);
        draw_pixel(x0 - x, y0 - y);
        draw_pixel(x0 + y, y0 + x);
        draw_pixel(x0 - y, y0 + x);
        draw_pixel(x0 + y, y0 - x);
        draw_pixel(x0 - y, y0 - x);
    }
}

void Graphic::draw_circle_helper( int16_t x0, int16_t y0,
                              int16_t r, uint8_t cornername)
{
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4)
        {
            draw_pixel(x0 + x, y0 + y);
            draw_pixel(x0 + y, y0 + x);
        }
        if (cornername & 0x2)
        {
            draw_pixel(x0 + x, y0 - y);
            draw_pixel(x0 + y, y0 - x);
        }
        if (cornername & 0x8)
        {
            draw_pixel(x0 - y, y0 + x);
            draw_pixel(x0 - x, y0 + y);
        }
        if (cornername & 0x1)
        {
            draw_pixel(x0 - y, y0 - x);
            draw_pixel(x0 - x, y0 - y);
        }
    }
}
void Graphic::fill_circle(int16_t x0, int16_t y0, int16_t r)
{
    draw_v_line(x0, y0 - r, y0 + r + 1);
    fill_circle_helper(x0, y0, r, 3, -1);
}
// Used to do circles and roundrects
void Graphic::fill_circle_helper(int16_t x0, int16_t y0, int16_t r,
                             uint8_t cornername, int16_t delta)
{

    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;

        if (cornername & 0x1)
        {
            draw_v_line(x0 + x, y0 - y, y0 + y + 1 + delta);
            draw_v_line(x0 + y, y0 - x, y0 + x + 1 + delta);
        }
        if (cornername & 0x2)
        {
            draw_v_line(x0 - x, y0 - y, y0 + y + 1 + delta);
            draw_v_line(x0 - y, y0 - x, y0 + x + 1 + delta);
        }
    }
}





// Draw a rounded rectangle
void Graphic::draw_round_rect(int16_t x, int16_t y, int16_t w,
                          int16_t h, int16_t r)
{
    // smarter version
    draw_h_line(x + r, y, x + w - r + 1);   // Top
    draw_h_line(x + r, y + h - 1, x + w - r + 1); // Bottom
    draw_v_line(x, y + r, y + h - r + 1);   // Left
    draw_v_line(x + w - 1, y + r, y + h - r + 1); // Right
    // draw four corners
    draw_circle_helper(x + r, y + r, r, 1);
    draw_circle_helper(x + w - r - 1, y + r, r, 2);
    draw_circle_helper(x + w - r - 1, y + h - r - 1, r, 4);
    draw_circle_helper(x + r, y + h - r - 1, r, 8);
}

// Fill a rounded rectangle
void Graphic::fill_round_rect(int16_t x, int16_t y, int16_t w,
                          int16_t h, int16_t r)
{
    // smarter version
    fill_rect(x + r, y, x + w - r + 1, y + h);

    // draw four corners
    fill_circle_helper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1);
    fill_circle_helper(x + r, y + r, r, 2, h - 2 * r - 1);
}

// Draw a triangle
void Graphic::draw_triangle(int16_t x0, int16_t y0,
                        int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    draw_line(x0, y0, x1, y1);
    draw_line(x1, y1, x2, y2);
    draw_line(x2, y2, x0, y0);
}

//// Fill a triangle
void Graphic::fill_triangle(int16_t x0, int16_t y0,
                        int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{

    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1)
    {
        _swap_int16_t(y0, y1);
        _swap_int16_t(x0, x1);
    }
    if (y1 > y2)
    {
        _swap_int16_t(y2, y1);
        _swap_int16_t(x2, x1);
    }
    if (y0 > y1)
    {
        _swap_int16_t(y0, y1);
        _swap_int16_t(x0, x1);
    }

    if(y0 == y2)   // Handle awkward all-on-same-line case as its own thing
    {
        a = b = x0;
        if(x1 < a)      a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a)      a = x2;
        else if(x2 > b) b = x2;
        draw_h_line(a, y0, b + 1);
        return;
    }

    int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
    int32_t
    sa   = 0,
    sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if(y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1 - 1; // Skip it

    for(y = y0; y <= last; y++)
    {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a, b);
        draw_h_line(a, y, b + 1);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y <= y2; y++)
    {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a, b);
        draw_h_line(a, y, b + 1);
    }
}


//void Graphic::box(int16_t x, int16_t y, int16_t w, int16_t h, int16_t bc)
//{
//    draw_line(x, y, x + w, y, 0xEF7D);
//    draw_line(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0x2965);
//    draw_line(x, y + h, x + w, y + h, 0x2965);
//    draw_line(x, y, x, y + h, 0xEF7D);
//    draw_line(x + 1, y + 1, x + 1 + w - 2, y + 1 + h - 2, bc);
//}
//void Graphic::box2(int16_t x, int16_t y, int16_t w, int16_t h, u8 mode)
//{
//    if (mode == 0)
//    {
//        draw_line(x, y, x + w, y, 0xEF7D);
//        draw_line(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0x2965);
//        draw_line(x, y + h, x + w, y + h, 0x2965);
//        draw_line(x, y, x, y + h, 0xEF7D);
//    }
//    if (mode == 1)
//    {
//        draw_line(x, y, x + w, y, 0x2965);
//        draw_line(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0xEF7D);
//        draw_line(x, y + h, x + w, y + h, 0xEF7D);
//        draw_line(x, y, x, y + h, 0x2965);
//    }
//    if (mode == 2)
//    {
//        draw_line(x, y, x + w, y, 0xffff);
//        draw_line(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0xffff);
//        draw_line(x, y + h, x + w, y + h, 0xffff);
//        draw_line(x, y, x, y + h, 0xffff);
//    }
//}




void  Graphic::set_rotation(uint8_t value)
{
    limit(value,(uint8_t)0,(uint8_t)3);
    rotation = value;
    _width = lcd_width;
    _height = lcd_height;
    switch(rotation)
    {
    case 1:
    case 3:
        swap(&_width, &_height);
        break;
    default :
            break;

    }
}
int16_t Graphic::ro_x(int16_t x, int16_t y)
{

    switch(rotation)
    {
    case 0:
        return x;
    case 1:
        return _height - 1 - y;
    case 2:
        return _width - 1 - x;
    case 3:
        return  y;

    }
    return 0;
}
int16_t Graphic::ro_y(int16_t x, int16_t y)
{
    switch(rotation)
    {
    case 0:
        return y;
    case 1:
        return x;
    case 2:
        return _height - 1 - y ;
    case 3:
        return _width - 1 - x;

    }
    return 0;

}

void Graphic::draw_pixel()
{
    int16_t tempx, tempy;
    tempx = ro_x(this->cursor_x, this->cursor_y);
    tempy = ro_y(this->cursor_x, this->cursor_y);
    lcd->draw_pixel(tempx, tempy, this->color);
}
void Graphic::draw_pixel(uint32_t color)
{
    int16_t tempx, tempy;
    tempx = ro_x(this->cursor_x, this->cursor_y);
    tempy = ro_y(this->cursor_x, this->cursor_y);
    lcd->draw_pixel(tempx, tempy, color);
}
void Graphic::draw_pixel(int16_t x, int16_t y)
{
    int16_t tempx, tempy;
    tempx = ro_x(x, y);
    tempy = ro_y(x, y);
    lcd->draw_pixel(tempx, tempy, this->color);
}
void Graphic::draw_pixel(int16_t x, int16_t y, uint32_t color)
{
    int16_t tempx, tempy;
    tempx = ro_x(x, y);
    tempy = ro_y(x, y);
    lcd->draw_pixel(tempx, tempy, color);
}

void Graphic::draw_h_line(int16_t x0, int16_t y0, int16_t x1)
{
    draw_h_line(x0,y0,x1,this->color);
}
void Graphic::draw_h_line(int16_t x0, int16_t y0, int16_t x1,uint32_t color)
{
    int16_t tempx0, tempy0, tempx1, tempy1;
    tempx0 = ro_x(x0, y0);
    tempy0 = ro_y(x0, y0);
    tempx1 = ro_x(x1, y0);
    tempy1 = ro_y(x1, y0);
    switch(rotation)
    {
    case 0:
        lcd->draw_h_line(tempx0, tempy0, tempx1, color);
        break;
    case 1:
        lcd->draw_v_line(tempx0, tempy0, tempy1, color);
        break;
    case 2:
        lcd->draw_h_line(tempx1, tempy0, tempx0, color);
        break;
    case 3:
        lcd->draw_v_line(tempx0, tempy1, tempy0, color);
        break;

    }
}

void Graphic::draw_v_line(int16_t x0, int16_t y0, int16_t y1)
{
   draw_v_line(x0,y0,y1,this->color);
}
void Graphic::draw_v_line(int16_t x0, int16_t y0, int16_t y1,uint32_t color)
{
    int16_t tempx0, tempy0, tempx1, tempy1;
    tempx0 = ro_x(x0, y0);
    tempy0 = ro_y(x0, y0);
    tempx1 = ro_x(x0, y1);
    tempy1 = ro_y(x0, y1);
    switch(rotation)
    {
    case 0:
        lcd->draw_v_line(tempx0, tempy0, tempy1, color);
        break;
    case 1:
        lcd->draw_h_line(tempx1, tempy0, tempx0, color);
        break;
    case 2:
        lcd->draw_v_line(tempx0, tempy1, tempy0, color);
        break;
    case 3:
        lcd->draw_h_line(tempx0, tempy0, tempx1, color);
        break;

    }
    //    lcd->draw_v_line(x0,y0,y1,this->color);
}

void Graphic::draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    int16_t tempx0, tempy0, tempx1, tempy1;
    tempx0 = ro_x(x0, y0);
    tempy0 = ro_y(x0, y0);
    tempx1 = ro_x(x1, y1);
    tempy1 = ro_y(x1, y1);

    lcd->draw_line(tempx0, tempy0, tempx1, tempy1, this->color);
}
void Graphic::draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color)
{
    int16_t tempx0, tempy0, tempx1, tempy1;
    tempx0 = ro_x(x0, y0);
    tempy0 = ro_y(x0, y0);
    tempx1 = ro_x(x1, y1);
    tempy1 = ro_y(x1, y1);

    lcd->draw_line(tempx0, tempy0, tempx1, tempy1, color);
}
void Graphic::draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    draw_h_line(x0, y0, x1);
    draw_h_line(x0, y1, x1);
    draw_v_line(x0, y0, y1);
    draw_v_line(x1, y0, y1);
    
}
void Graphic::draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint32_t color)
{
    draw_h_line(x0, y0, x1, color);
    draw_h_line(x0, y1, x1, color);
    draw_v_line(x0, y0, y1, color);
    draw_v_line(x1, y0, y1, color);
    
}
void Graphic::fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    int16_t tempx0, tempy0, tempx1, tempy1;
    tempx0 = ro_x(x0, y0);
    tempy0 = ro_y(x0, y0);
    tempx1 = ro_x(x1, y1);
    tempy1 = ro_y(x1, y1);

    lcd->fill_rect(min(tempx0, tempx1), min(tempy0, tempy1), max(tempx0, tempx1), max(tempy0, tempy1), this->color);
}
void Graphic::fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint32_t color)
{
    int16_t tempx0, tempy0, tempx1, tempy1;
    tempx0 = ro_x(x0, y0);
    tempy0 = ro_y(x0, y0);
    tempx1 = ro_x(x1, y1);
    tempy1 = ro_y(x1, y1);

    lcd->fill_rect(min(tempx0, tempx1), min(tempy0, tempy1), max(tempx0, tempx1), max(tempy0, tempy1), color);
}
void Graphic::fill_screen(uint32_t color)
{
    lcd->fill_screen(color);
}
void Graphic::clear()
{
    fill_screen(0);
}
void Graphic::flush()
{
    lcd->flush();
}
