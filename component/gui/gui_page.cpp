#include "gui_page.h"
#include "bsp_ebox.h"

void GuiPage::Register(ActivityComponent *object)
{
    activityList.insert_tail(object);
}
void GuiPage::Register(Component *object)
{
    componentList.insert_tail(object);
}
void GuiPage::create()
{
    UART.printf("����:%s",name.c_str());
    
    _gpu->fill_rect(x,y, x + len - 1,  y+ hight - 1,_gpu->back_color);

    Component *p;
    for(int i = 0; i < componentList.size(); i++)
    {
         p = (Component *)componentList.data(i);
         p->create();
    }
    for(int i = 0; i < activityList.size(); i++)
    {
         p = (Component *)activityList.data(i);
         p->create();
    }  
    index_set(0);
    
    if(outline == GuiOutlineMode::Surround)
        _gpu->draw_rect(x,y, x + len - 1,  y+ hight - 1);


}
void GuiPage::show()
{
    UART.printf("��ʾ:%s",name.c_str());

    Component *p;
    for(int i = 0; i < componentList.size(); i++)
    {
         p = (Component *)componentList.data(i);
         p->show();
    }
    for(int i = 0; i < activityList.size(); i++)
    {
         p = (Component *)activityList.data(i);
         p->show();
    }  
    update_index();
}

void GuiPage::hide()
{
    _gpu->fill_rect(x,y, x + len - 1,  y+ hight - 1,_gpu->back_color);
    UART.printf("���أ�%s\r\n",name.c_str());

}
void GuiPage::cancel()
{
    for(int i = 0; i < activityList.size(); i++)
    {
        delete (ActivityComponent *)activityList.data(i);
    }
    for(int i = 0; i < componentList.size(); i++)
    {
        delete (Component *)componentList.data(i);
    }
    
    componentList.clear();
    activityList.clear();
    index = 0;
    _gpu->clear();
    UART.printf("ע����%s\r\n",name.c_str());

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
    return (ActivityComponent *)activityList.data(index);
}
void GuiPage::update_index()
{
    if(activityList.size() == 0)
        return;
    
    ActivityComponent * p1 = (ActivityComponent *)(activityList.data(last_index));
    p1->set_select(false);
    ActivityComponent * p2 = (ActivityComponent *)(activityList.data(index));
    p2->set_select(true);
    last_index = index;
}
