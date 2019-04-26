
#include "gui.h"


void GUI::begin(Vhmi *_lcd, uint16_t w, uint16_t h)
{
    lcd = _lcd;
    width = w;
    height = h;

    //设置显示器方向
    set_rotation(0);

    //设置前后景颜色
    set_back_color(GUI_BLACK);
    set_color(GUI_WHITE);

    //设置字体和禁用自动换行
    set_text_auto_reline(0);
    set_text_mode(TEXT_MODE_NORMAL);
};
void GUI::clear()
{
    fill_screen(0);
}

/*********************************************************************
*
*       GUI settings
*
**********************************************************************
*/


void GUI::set_color(uint32_t color)
{
    this->color = color;
}
void GUI::set_back_color(uint32_t back_color)
{
    this->back_color = back_color;
}

void GUI::set_cursor(int16_t x, int16_t y)
{
    this->cursor_x = x;
    this->cursor_y = y;
}
void GUI::set_draw_mode(uint8_t mode)
{
    draw_mode = mode;
}

u16 GUI::bgr2rgb(u16 c)
{
    u16  r, g, b, rgb;
    b = (c >> 0) & 0x1f;
    g = (c >> 5) & 0x3f;
    r = (c >> 11) & 0x1f;
    rgb = (b << 11) + (g << 5) + (r << 0);
    return(rgb);

}


