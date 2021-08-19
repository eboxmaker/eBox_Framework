#include "bsp_ebox.h"
#include "gui_button.h"
#include "gui_sidebar.h"
#include "gui_list.h"
#include "math.h"
#include "ui_setvalue.h"
#include "ebox_mem.h"
#include "ui.h"
#include "ui_main.h"
//static GuiButton  btnTest(10,10,60,40);
//static GuiSideBar bar(1,10,100,5);
static GuiButton *btn1;
static GuiButton *btn2;
static GuiSideBar *bar;
static GuiSideBar *bar1;
static GuiList      *par[5];

SetValuePage *pageSetValue = new SetValuePage("set");
static char *itemTableFloat[] = {"0","1","2","3","4","5","6","7","8","9",".","-"};

static void onBtn1Click()
{
    UART.println("set BTN2 CLICK");
    ui.go_back();
}
static void onBtn2Click()
{
    UART.println("set BTN2 CLICK");
    ui.go_back();
}

SetValuePage::SetValuePage(String name):GuiPage(name){

    
    
};

SetValuePage::~SetValuePage()
{
    activityList.clear();
    componentList.clear();
}
void SetValuePage::create()
{
    UART.print("UI:");
    UART.println(name);
    _gpu->clear();
    btn1 = new GuiButton(30,10,60,40,"btn1");
    btn2 = new GuiButton(30,60,60,40,"btn2");
    bar = new GuiSideBar(1,30,100,5);
    bar1 = new GuiSideBar(1,80,100,5);
    
    for(int i = 0; i < 5; i++)
    {
        par[i] = new GuiList(80 + i*20,32,20,30,itemTableFloat,getArraySize(itemTableFloat));
        Register(par[i]);
    }
    btn1->click = onBtn1Click;
    Register(btn1);
    Register(btn2);
    Register(bar);
    Register(bar1);
    GuiPage::create();
}

    

void SetValuePage::loop()
{
    
}
void SetValuePage::event(Object *sender,GuiMessage *msg)
{
//    if(get_selected_object()->type == GuiType::List)
//    {
//        GuiList *ptr = (GuiList *)get_selected_object();
//        if(msg->str == "d")
//        {
//            if(selection < activityList.size() - 1)
//            {
//                UART.printf("select:++\r\n");
//                selection++;
//                update_select();
//            }
//            else
//            {
//                selection = 0;
//                update_select();

//            }
//        }
//            
//        if(msg->str == "w")
//        {

//            if(ptr->index >= 9)
//                ptr->index = 0;  
//             else
//                ptr->index++;
//             ptr->update_value();
//        }
//        
//            
//        if(msg->str == "s")
//        {
//            if(ptr->index <= 0)
//                ptr->index=9;
//            else
//                ptr->index--;
//            ptr->update_value();

//        }
//        
//        if(msg->str == " ")
//        {
//            uint32_t temp = 0;
//            for(int i = 0; i < 5; i++)
//            {
//                temp += par[i]->get_value()* pow(10.0,4-i);
//                UART.printf("value:%u;%d\r\n", temp,par[i]->get_value());

//            }
//            UART.printf("value:%u;\r\n", temp);

//        }
//        UART.printf("index:%d;select:%d/%d\r\n", ptr->index,selection,activityList.size());
//        
//    
//    }
//    else
//    {
//        if(msg->str == "d")
//        {
//            if(get_selected_object()->click != NULL)
//                get_selected_object()->click();
//        }
//            
//        if(msg->str == "w")
//        {
//            if(selection > 0)
//            {
//                UART.printf("select:--\r\n");
//                selection--;
//                update_select();
//            }
//        }
//        
//            
//        if(msg->str == "s")
//        {
//            if(selection < activityList.size() - 1)
//            {
//                UART.printf("select:++\r\n");
//                selection++;
//                update_select();
//            }
//            else
//            {
//                selection = 0;
//                            update_select();

//            }
//        }
//     UART.printf("set value cmd:%s;select:%d/%d\r\n",msg->str.c_str(),selection,activityList.size());
//   }
//    


    
}
