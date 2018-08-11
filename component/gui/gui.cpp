
#include "gui.h"


void GUI::begin(Vhmi *_lcd,uint16_t w, uint16_t h)
{
    draw_mode = LCD_DRAWMODE_NORMAL;

    width = w;
    height = h;
    set_rotation(0);
    lcd = _lcd;
};

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

void GUI::set_cursor(uint16_t x,uint16_t y)
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
  u16  r,g,b,rgb;   
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);		 
  return(rgb);

}


