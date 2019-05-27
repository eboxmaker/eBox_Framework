#include "gui_button.h"

void GuiButton::draw()
{
    int16_t x1 = x + len -1;
    int16_t y1 = y + hight - 1;
    _gpu->fill_rect(x,y,x1,y1,_gpu->color);
    _gpu->draw_h_line(x,y,x1,0xd);
    _gpu->draw_h_line(x,y1,x1,0xd);
    _gpu->draw_v_line(x,y,y1,0xd);
    _gpu->draw_v_line(x1,y,y1,0xd);
    
}
