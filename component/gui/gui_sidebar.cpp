#include "bsp_ebox.h"
#include "graphic.h"
#include "gui_sidebar.h"

void GuiSideBar::create()
{
    int16_t x1,y1;//bar结束位置
    int16_t sx0,sx1,sy0,sy1;//滑块的位置
    
    limit(max,(int16_t)1,(int16_t)(len - 1));
    limit(prograss,(int16_t)0,(int16_t)(max - 1));
    slider_len = len/max;//计算滑块的长度
    

    if(orientation == 0)
    {
        x1 = x + len - 1;
        y1 = y + width - 1;
        
        sx0 = (x + 1) + len*prograss/max;
        sx1 = sx0 + slider_len - 1;
        sy0 = y + 1;
        sy1 = y + width - 2;  
        limit(sx0,(int16_t)(x + 1),(int16_t)(x1 - 1));
        limit(sx1,(int16_t)(x + 1),(int16_t)(x1 - 1));
    }
    else
    {
        x1 = x + width - 1;
        y1 = y + len - 1;
        
        sx0 = x + 1;
        sx1 = x + width - 2;
        sy0 = y + len*prograss/max + 1;
        sy1 = sy0 + slider_len - 1; 
        limit(sy0,(int16_t)(y + 1),(int16_t)(y1 - 1));
        limit(sy1,(int16_t)(y + 1),(int16_t)(y1 - 1));
    }
    _gpu->fill_rect(x,y,x1,y1,_gpu->back_color);//清空bar的整个区域
    _gpu->draw_rect(x,y,x1,y1);//重新构建轮廓
    _gpu->fill_rect(sx0,sy0,sx1,sy1);//更新滑块显示
}
void GuiSideBar::show()
{
    
    int16_t x1,y1;//bar结束位置
    int16_t sx0,sx1,sy0,sy1;//滑块的位置
    
    limit(max,(int16_t)1,(int16_t)(len - 1));
    limit(prograss,(int16_t)0,(int16_t)(max - 1));
    slider_len = len/max;//计算滑块的长度
    

    if(orientation == 0)
    {
        x1 = x + len - 1;
        y1 = y + width - 1;
        
        sx0 = (x + 1) + len*prograss/max;
        sx1 = sx0 + slider_len - 1;
        sy0 = y + 1;
        sy1 = y + width - 2;  
        limit(sx0,(int16_t)(x + 1),(int16_t)(x1 - 1));
        limit(sx1,(int16_t)(x + 1),(int16_t)(x1 - 1));
    }
    else
    {
        x1 = x + width - 1;
        y1 = y + len - 1;
        
        sx0 = x + 1;
        sx1 = x + width - 2;
        sy0 = y + len*prograss/max + 1;
        sy1 = sy0 + slider_len - 1; 
        limit(sy0,(int16_t)(y + 1),(int16_t)(y1 - 1));
        limit(sy1,(int16_t)(y + 1),(int16_t)(y1 - 1));
    }
    _gpu->fill_rect(x,y,x1,y1,_gpu->back_color);//清空bar的整个区域
    _gpu->draw_rect(x,y,x1,y1);//重新构建轮廓
    _gpu->fill_rect(sx0,sy0,sx1,sy1);//更新滑块显示

}
void GuiSideBar::hide()
{
     int16_t x1,y1;//bar结束位置
  
    if(orientation == 0)
    {
        x1 = x + len - 1;
        y1 = y + width - 1;
    }
    else
    {
        x1 = x + width - 1;
        y1 = y + len - 1;
    }
    _gpu->fill_rect(x,y,x1,y1,_gpu->back_color);//清空bar的整个区域
}

void GuiSideBar::draw_slider(uint32_t color)
{
    
    int16_t x1,y1;//bar结束位置
    int16_t sx0,sx1,sy0,sy1;//滑块的位置
    
    limit(max,(int16_t)1,(int16_t)(len - 1));
    limit(prograss,(int16_t)0,(int16_t)(max - 1));
    slider_len = len/max;//计算滑块的长度


    if(orientation == 0)
    {
        x1 = x + len - 1;
        y1 = y + width - 1;
        
        sx0 = (x + 1) + len*prograss/max;
        sx1 = sx0 + slider_len - 1;
        sy0 = y + 1;
        sy1 = y + width - 2;  
        limit(sx0,(int16_t)(x + 1),(int16_t)(x1 - 1));
        limit(sx1,(int16_t)(x + 1),(int16_t)(x1 - 1));
    }
    else
    {
        x1 = x + width - 1;
        y1 = y + len - 1;
        
        sx0 = x + 1;
        sx1 = x + width - 2;
        sy0 = y + len*prograss/max + 1;
        sy1 = sy0 + slider_len - 1; 
        limit(sy0,(int16_t)(y + 1),(int16_t)(y1 - 1));
        limit(sy1,(int16_t)(y + 1),(int16_t)(y1 - 1));
    }
    _gpu->fill_rect(sx0,sy0,sx1,sy1,color);//更新滑块显示
}

void GuiSideBar::set_max(int16_t value)
{
    limit(value,(int16_t)1,(int16_t)(len - 1));
    max = value;
    create();
}
void GuiSideBar::set_prograss(int16_t value)
{
    draw_slider(_gpu->back_color);
    prograss = value;
    draw_slider(_gpu->color);

//prograss = value;
//    draw();

}
void GuiSideBar::set_orientation(int16_t value)
{
    limit(value,(int16_t)0,(int16_t)1);
    orientation = value;
    create();

}

