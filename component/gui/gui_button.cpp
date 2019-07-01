#include "gui_button.h"
#include "ebox_mem.h"

GuiButton::~GuiButton()
{
    ebox_printf("btn %s free\r\n",name.c_str());
}

void GuiButton::create()
{
    int16_t x1 = x + len -1;
    int16_t y1 = y + hight - 1;
    _gpu->fill_rect(x+2,y+2,x1-2,y1-2,RGB565::Black);
    _gpu->draw_rect(x+2,y+2,x1-2,y1-2,RGB565::Dimgray);

    
    _gpu->set_font(font);
    _gpu->set_text_mode(text_mode);

    uint16_t slen = _gpu->get_string_xlength(text.c_str());
    int16_t startx = (len - slen)/2 + x;
    int16_t starty = (hight - _gpu->current_font->YSize)/2 + y;
    _gpu->printf(startx,starty,text.c_str());
    
}
void GuiButton::show()
{
    create();
}
void GuiButton::hide()
{
    _gpu->fill_rect(x,y,x+len,y+hight,_gpu->back_color);

}

void GuiButton::set_text(String str)
{
    _gpu->set_font(font);
    _gpu->set_text_mode(text_mode);
    uint16_t slen = _gpu->get_string_xlength(text.c_str());
    int16_t startx = (len - slen)/2 + x;
    int16_t starty = (hight - _gpu->current_font->YSize)/2 + y;
    _gpu->fill_rect(startx,starty,startx+slen,starty + font->YSize,_gpu->back_color);

    text = str;

    slen = _gpu->get_string_xlength(text.c_str());
    startx = (len - slen)/2 + x;
    starty = (hight - _gpu->current_font->YSize)/2 + y;
    _gpu->printf(startx,starty,text.c_str());
//        create();


}
void GuiButton::set_select(bool state)
{
    selected = state;
    int16_t x1 = x + len -1;
    int16_t y1 = y + hight - 1;
    if(selected)
    {
        _gpu->draw_h_line(x,y,x1,RGB565::LightGray);
        _gpu->draw_h_line(x,y1,x1,RGB565::LightGray);
        _gpu->draw_v_line(x,y,y1,RGB565::LightGray);
        _gpu->draw_v_line(x1,y,y1,RGB565::LightGray);
    }
    else
    {
        _gpu->draw_h_line(x,y,x1,_gpu->back_color);
        _gpu->draw_h_line(x,y1,x1,_gpu->back_color);
        _gpu->draw_v_line(x,y,y1,_gpu->back_color);
        _gpu->draw_v_line(x1,y,y1,_gpu->back_color);
    }
}


///**************************************************************************************
//功能描述: 在屏幕显示一凸起的按钮框
//输    入: int16_t x1,y1,x2,y2 按钮框左上角和右下角坐标
//输    出: 无
//**************************************************************************************/
//void GuiButton::btn_down(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
//{
//    
//    _gpu->draw_line(x1,  y1,  x2, y1, RGB565::Dimgray); //H
//    _gpu->draw_line(x1 + 1, y1 + 1, x2, y1 + 1, RGB565::LightGray); //H
//    _gpu->draw_line(x1,  y1,  x1, y2, RGB565::Dimgray); //V
//    _gpu->draw_line(x1 + 1, y1 + 1, x1 + 1, y2, RGB565::LightGray); //V
//    _gpu->draw_line(x1,  y2,  x2, y2, RGB565::White); //H
//    _gpu->draw_line(x2,  y1,  x2, y2, RGB565::White); //V
//    _gpu->set_font(&GUI_FontHZ16X16);
//    _gpu->set_text_mode(GuiDrawMode::Reverse);
//    
//    uint16_t len = _gpu->get_string_xlength("申同强");
//    int16_t startx = ((x2 - x1) - len)/2 + x1;
//    int16_t starty = ((y2 - y1) - _gpu->current_font->YSize)/2 + y1;
//    


//}

///**************************************************************************************
//功能描述: 在屏幕显示一凹下的按钮框
//输    入: int16_t x1,y1,x2,y2 按钮框左上角和右下角坐标
//输    出: 无
//**************************************************************************************/
//void GuiButton::btn_up(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
//{
//    _gpu->draw_line(x1,  y1,  x2, y1, RGB565::White); //H
//    _gpu->draw_line(x1,  y1,  x1, y2, RGB565::White); //V

//    _gpu->draw_line(x1 + 1, y2 - 1, x2, y2 - 1, RGB565::LightGray); //H
//    _gpu->draw_line(x1,  y2,  x2, y2, RGB565::Dimgray); //H
//    _gpu->draw_line(x2 - 1, y1 + 1, x2 - 1, y2, RGB565::LightGray); //V
//    _gpu->draw_line(x2, y1, x2, y2, RGB565::Dimgray);  //V
//}
