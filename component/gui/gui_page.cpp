#include "gui_page.h"
#include "bsp_ebox.h"

void GuiPage::regedit(ActivityComponent *object)
{
    activityList.insert_tail(object);
}
void GuiPage::regedit(Component *object)
{
    baseList.insert_tail(object);
}
void GuiPage::create()
{
    UART.printf("����:%s",name.c_str());

    Component *p;
    for(int i = 0; i < baseList.size(); i++)
    {
         p = (Component *)baseList.data(i);
         p->create();
    }
    for(int i = 0; i < activityList.size(); i++)
    {
         p = (Component *)activityList.data(i);
         p->create();
    }  
    update_select();


}
void GuiPage::show()
{
    UART.printf("��ʾ:%s",name.c_str());

    Component *p;
    for(int i = 0; i < baseList.size(); i++)
    {
         p = (Component *)baseList.data(i);
         p->show();
    }
    for(int i = 0; i < activityList.size(); i++)
    {
         p = (Component *)activityList.data(i);
         p->show();
    }  
    update_select();
}

void GuiPage::hide()
{
    _gpu->clear();
    UART.printf("���أ�%s\r\n",name.c_str());

}
void GuiPage::cancel()
{
    for(int i = 0; i < activityList.size(); i++)
    {
        delete (Component *)activityList.data(i);
    }
    for(int i = 0; i < baseList.size(); i++)
    {
        delete (Component *)baseList.data(i);
    }
    
    baseList.clear();
    activityList.clear();
    selection = 0;
    _gpu->clear();
    UART.printf("ע����%s\r\n",name.c_str());

}

void GuiPage::loop()
{
    
}
ActivityComponent * GuiPage::get_selected_object()
{
    return (ActivityComponent *)activityList.data(selection);
}
void GuiPage::update_select()
{
    
    ActivityComponent * p1 = (ActivityComponent *)(activityList.data(last_selection));
    p1->set_select(false);
    ActivityComponent * p2 = (ActivityComponent *)(activityList.data(selection));
    p2->set_select(true);
    last_selection = selection;
}
