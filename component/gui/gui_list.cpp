#include "gui.h"
#include "bsp_ebox.h"
void GUI::drawList(GuiList &list)
{
    
    uint16_t x1,y1;
    if(list.orientation == 0)
    {
        x1 = list.x + list.x_size;
        y1 = list.y + list.y_size;
    }
    else
    {
        x1 = list.x + list.y_size;
        y1 = list.y + list.x_size;
    }
     fill_rect(list.x,list.y,x1,y1,0);
    draw_rect(list.x,list.y,x1,y1);
    uint16_t sx0,sx1,sy0,sy1;

    list.slider_len = list.x_size/list.max;
    if(list.orientation == 0)
    {
        sx0 =list.x + list.x_size*list.prograss/list.max;
        sx1 = sx0 + list.slider_len;
        sy0 = list.y;
        sy1 = list.y + list.y_size;  
    }
    else
    {
        sx0 =list.x;
        sx1 = sx0 + list.y_size;
        sy0 = list.y + list.x_size*list.prograss/list.max;
        sy1 = sy0 + list.slider_len;  
//        fill_rect(sx0,sy0,sx1,sy1);
    }
     fill_rect(sx0,sy0,sx1,sy1);

}