#include "gui.h"

void  GUI::set_rotation(uint8_t value)
{
    rotation = value;
    _width = width;
    _height = height;
    switch(rotation)
    {
    case 1:
    case 3:
        swap(&_width, &_height);
        break;
    }
}
int16_t GUI::ro_x(int16_t x, int16_t y)
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
int16_t GUI::ro_y(int16_t x, int16_t y)
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
void GUI::draw_pixel(int16_t x, int16_t y)
{
    int16_t tempx, tempy;
    tempx = ro_x(x, y);
    tempy = ro_y(x, y);
    lcd->draw_pixel(tempx, tempy, this->color);
}
void GUI::draw_pixel(int16_t x, int16_t y, uint32_t color)
{
    int16_t tempx, tempy;
    tempx = ro_x(x, y);
    tempy = ro_y(x, y);
    lcd->draw_pixel(tempx, tempy, color);
}

void GUI::draw_h_line(int16_t x0, int16_t y0, int16_t x1)
{
    int16_t tempx0, tempy0, tempx1, tempy1;
    tempx0 = ro_x(x0, y0);
    tempy0 = ro_y(x0, y0);
    tempx1 = ro_x(x1, y0);
    tempy1 = ro_y(x1, y0);
    switch(rotation)
    {
    case 0:
        lcd->draw_h_line(tempx0, tempy0, tempx1, this->color);
        break;
    case 1:
        lcd->draw_v_line(tempx0, tempy0, tempy1, this->color);
        break;
    case 2:
        lcd->draw_h_line(tempx1, tempy0, tempx0, this->color);
        break;
    case 3:
        lcd->draw_v_line(tempx0, tempy1, tempy0, this->color);
        break;

    }
}
void GUI::draw_v_line(int16_t x0, int16_t y0, int16_t y1)
{
    int16_t tempx0, tempy0, tempx1, tempy1;
    tempx0 = ro_x(x0, y0);
    tempy0 = ro_y(x0, y0);
    tempx1 = ro_x(x0, y1);
    tempy1 = ro_y(x0, y1);
    switch(rotation)
    {
    case 0:
        lcd->draw_v_line(tempx0, tempy0, tempy1, this->color);
        break;
    case 1:
        lcd->draw_h_line(tempx1, tempy0, tempx0, this->color);
        break;
    case 2:
        lcd->draw_v_line(tempx0, tempy1, tempy0, this->color);
        break;
    case 3:
        lcd->draw_h_line(tempx0, tempy0, tempx1, this->color);
        break;

    }
    //    lcd->draw_v_line(x0,y0,y1,this->color);
}
void GUI::draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    int16_t tempx0, tempy0, tempx1, tempy1;
    tempx0 = ro_x(x0, y0);
    tempy0 = ro_y(x0, y0);
    tempx1 = ro_x(x1, y1);
    tempy1 = ro_y(x1, y1);

    lcd->draw_line(tempx0, tempy0, tempx1, tempy1, this->color);
}
void GUI::draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color)
{
    int16_t tempx0, tempy0, tempx1, tempy1;
    tempx0 = ro_x(x0, y0);
    tempy0 = ro_y(x0, y0);
    tempx1 = ro_x(x1, y1);
    tempy1 = ro_y(x1, y1);

    lcd->draw_line(tempx0, tempy0, tempx1, tempy1, color);
}
#include "bsp_ebox.h"
void GUI::draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    draw_h_line(x0, y0, x1);
    draw_h_line(x0, y1, x1);
    draw_v_line(x0, y0, y1);
    draw_v_line(x1, y0, y1);
}
void GUI::fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    int16_t tempx0, tempy0, tempx1, tempy1;
    tempx0 = ro_x(x0, y0);
    tempy0 = ro_y(x0, y0);
    tempx1 = ro_x(x1, y1);
    tempy1 = ro_y(x1, y1);

    lcd->fill_rect(min(tempx0, tempx1), min(tempy0, tempy1), max(tempx0, tempx1), max(tempy0, tempy1), this->color);
}
void GUI::fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint32_t color)
{
    int16_t tempx0, tempy0, tempx1, tempy1;
    tempx0 = ro_x(x0, y0);
    tempy0 = ro_y(x0, y0);
    tempx1 = ro_x(x1, y1);
    tempy1 = ro_y(x1, y1);

    lcd->fill_rect(min(tempx0, tempx1), min(tempy0, tempy1), max(tempx0, tempx1), max(tempy0, tempy1), color);
}
void GUI::fill_screen(uint32_t color)
{
    lcd->fill_screen(color);
}

