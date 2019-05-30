#include "gui_page.h"
#include "bsp_ebox.h"

void GuiPage::regedit(ActivityComponent *object)
{
    activityList.insert_tail(object);
}
void GuiPage::regedit(GuiBase *object)
{
    baseList.insert_tail(object);
}
void GuiPage::create()
{
    UART.print("Page:");
    UART.println(name);

    GuiBase *p;
    for(int i = 0; i < baseList.size(); i++)
    {
         p = (GuiBase *)baseList.data(i);
         p->create();
    }
    for(int i = 0; i < activityList.size(); i++)
    {
         p = (GuiBase *)activityList.data(i);
         p->create();
    }  
    update_select();
    UART.printf("%s   bsize:%d,asize:%d\r\n",name.c_str(),baseList.size(),activityList.size());


}
void GuiPage::cancel()
{
    for(int i = 0; i < activityList.size(); i++)
    {
        delete (ActivityComponent *)activityList.data(i);
    }
    for(int i = 0; i < baseList.size(); i++)
    {
        delete (GuiBase *)baseList.data(i);
    }
    
    baseList.clear();
    activityList.clear();
    selection = 0;

}

void GuiPage::loop()
{
    
}
GuiBase * GuiPage::get_selected_object()
{
    return (GuiBase *)activityList.data(selection);
}
void GuiPage::update_select()
{
    
    ActivityComponent * p1 = (ActivityComponent *)(activityList.data(last_selection));
    p1->set_select(false);
    ActivityComponent * p2 = (ActivityComponent *)(activityList.data(selection));
    p2->set_select(true);
    last_selection = selection;
}
