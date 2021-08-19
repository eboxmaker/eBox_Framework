#include "gui_page.h"
#include "bsp_ebox.h"

void GuiPage::Register(ActivityComponent *object)
{
    activityList.add(object);
}
void GuiPage::Register(Component *object)
{
    componentList.add(object);
}
void GuiPage::create()
{
    UART.printf("创建:%s",name.c_str());
    
    _gpu->fill_rect(x,y, x + len - 1,  y+ hight - 1,_gpu->back_color);

    Component *p;
    for(int i = 0; i < componentList.size(); i++)
    {
        componentList[i]->create();
    }
    
    for(int i = 0; i < activityList.size(); i++)
    {
        activityList[i]->create();
    }  
    index_set(0);
    
    if(outline == GuiOutlineMode::Surround)
        _gpu->draw_rect(x,y, x + len - 1,  y+ hight - 1);


}
void GuiPage::show()
{
    UART.printf("显示:%s",name.c_str());

    Component *p;
    for(int i = 0; i < componentList.size(); i++)
    {
         componentList[i]->show();
    }
    for(int i = 0; i < activityList.size(); i++)
    {
        activityList[i]->show();        
    }  
    update_index();
}

void GuiPage::hide()
{
    _gpu->fill_rect(x,y, x + len - 1,  y+ hight - 1,_gpu->back_color);
    UART.printf("隐藏：%s\r\n",name.c_str());

}
void GuiPage::cancel()
{

    activityList.clear();
    componentList.clear();

    index = 0;
    _gpu->clear();
    UART.printf("注销：%s\r\n",name.c_str());

}
int16_t GuiPage::index_get()
{
    
    return index;
}

void GuiPage::index_set(int16_t value)
{
    index = value;
    update_index();
}

bool GuiPage::index_next()
{
    if(index < activityList.size() - 1)
    {
        index++;
        update_index();
        return true;
    }
    return false;
}
bool GuiPage::index_previous()
{
    if(index > 0)
    {
        index--;
        update_index();
        return true;
    }
    return false;
}

void GuiPage::loop()
{
    
}
ActivityComponent * GuiPage::get_selected_object()
{
    return activityList[index];
}
void GuiPage::update_index()
{
    if(activityList.size() == 0)
        return;
    activityList[last_index]->set_select(false);
    activityList[index]->set_select(true);
    last_index = index;
}
