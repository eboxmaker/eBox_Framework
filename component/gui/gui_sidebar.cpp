#include "gui.h"
#include "bsp_ebox.h"
void GUI::draw_sidebar(GuiSideBar &bar)
{
    
    int16_t x1,y1;//bar结束位置
    int16_t sx0,sx1,sy0,sy1;//滑块的位置
    
    bar.slider_len = bar.len/bar.max;//计算滑块的长度
    
    if(bar.prograss >= bar.max)bar.prograss = bar.max - 1;
    if(bar.orientation == 0)
    {
        x1 = bar.x + bar.len - 1;
        y1 = bar.y + bar.width - 1;
        
        sx0 =bar.x + bar.len*bar.prograss/bar.max + 1;
        sx1 = sx0 + bar.slider_len - 2;
        sy0 = bar.y + 1;
        sy1 = bar.y + bar.width - 2;  
    }
    else
    {
        x1 = bar.x + bar.width - 1;
        y1 = bar.y + bar.len - 1;
        
        sx0 =bar.x + 1;
        sx1 = sx0 + bar.width - 2;
        sy0 = bar.y + bar.len*bar.prograss/bar.max + 1;
        sy1 = sy0 + bar.slider_len - 2;  
    }
    fill_rect(bar.x,bar.y,x1,y1,0);//清空bar的整个区域
    draw_rect(bar.x,bar.y,x1,y1);//重新构建轮廓
    fill_rect(sx0,sy0,sx1,sy1);//更新滑块显示

}