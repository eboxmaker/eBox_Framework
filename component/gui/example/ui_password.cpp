#include "ui_password.h"
#include "ui_menu.h"
#include "gui_menu.h"
#include "gui_list.h"
#include "gui_sidebar.h"
#include "bsp_ebox.h"
#include "ui.h"
#include "ui_setvalue.h"
#include "gui_button.h"

//static GuiSideBar bar(1,10,100,5);
static GuiList      *par[4];
static char *itemTableFloat[] = {"0","1","2","3","4","5","6","7","8","9"};

PwdPage *pagePwd = new PwdPage("pwd");

//static void onBtn1Click()
//{
//    UART.println("set BTN2 CLICK");
//    ui.go_back();
//}
//static void onBtn2Click()
//{
//    UART.println("set BTN2 CLICK");
//    ui.go_back();
//}

PwdPage::PwdPage(String name):GuiPage(name){

    
    
};

PwdPage::~PwdPage()
{

    activityList.clear();
    componentList.clear();
    UART.print("Ö´ÐÐÎö¹¹º¯Êý\r\n");

    
}
void PwdPage::create()
{
    UART.print("UI:");
    UART.println(name);
    _gpu->clear();
//    btn1 = new GuiButton(30,10,60,40,"btn1");
//    btn2 = new GuiButton(30,60,60,40,"btn2");
//    bar = new GuiSideBar(1,30,100,5);
//    bar1 = new GuiSideBar(1,80,100,5);
    
    for(int i = 0; i < 4; i++)
    {
        par[i] = new GuiList(0 + i*20,100,20,30,itemTableFloat,getArraySize(itemTableFloat));
        Register(par[i]);
    }
//    regedit(btn1);
//    regedit(btn2);
//    regedit(bar);
//    regedit(bar1);
    GuiPage::create();
}

   
void PwdPage::loop()
{
    
}
void PwdPage::event(Object *sender,GuiMessage *msg)
{
    GuiList *ptr = (GuiList *)get_selected_object();
    if(msg->str == "d")
    {
        if(index_next() == false)
            index_set(0);
    }
        
    if(msg->str == "w")
    {

        if(ptr->index >= 9)
            ptr->index = 0;  
         else
            ptr->index++;
         ptr->update_item_index();
    }
    
        
    if(msg->str == "s")
    {
        if(ptr->index <= 0)
            ptr->index=9;
        else
            ptr->index--;
        ptr->update_item_index();

    }

    if(msg->str == " ")
    {
        for(int i = 0; i < 4; i++)
        {
            pwd += par[i]->get_value();

//            temp += par[i]->get_value()* pow(10.0,3-i);

        }
            UART.printf("value:%d\r\n", pwd.c_str());
        ui.go_back();

    }
    UART.printf("index:%d;select:%d/%d\r\n", ptr->index,index_get(),activityList.size());
    
}
