#include "ui_menu.h"
#include "gui_menu.h"
#include "gui_list.h"
#include "gui_sidebar.h"
#include "bsp_ebox.h"
#include "ui.h"
#include "ui_setvalue.h"
#include "ui_password.h"
extern const menu_t menu_1;
bool pwdIsOk = false;

GuiMenu *pMenu;
GuiList *pList;
GuiSideBar *bar;

MenuPage *pageMenu = new MenuPage("menu");



void check_pwd(void *p)
{
    String *str = (String *)p;
    if(*str == "4170")
        pwdIsOk = true;
}
void openPwd()
{
    ui.open(pagePwd);
}
/////////////////////////////���㼶�˵�/////////////////////////////////////////////////
const menuItem_t menu_items[] = {
    {	openPwd,               NULL,                "user"},
    {	openPwd,               NULL,               "factory"},
    {	openPwd,               NULL,               "dev info"},
};

// �˵�1
const menu_t menu = {
    "���˵�",
    (menuItem_t *)menu_items,                      //!< menu item list
    getArraySize(menu_items),                            //!< menu item count
    NULL,                                               //!< top menu has no parent
    0,
};


#if 1 // �û�-> xxx
// �û��˵�1
const menuItem_t menu_1_items[] = {
    {	NULL,               NULL,  "OUTPUT"},
    {	NULL,               NULL,  "warning"},
    {	NULL,               NULL,  "dev set"},
    {	NULL,               NULL,  "system set"},
};
const menu_t menu_1 = {
     "�û��˵�1",
    (menuItem_t *)menu_1_items,     //!< menu item list
    getArraySize(menu_1_items),     //!< menu item count
    (menu_t*)&menu,                 //!< top menu has no parent
    1 - 1,
};
#endif
//////////

 #if 1//����-> xxx

//2>x/////////////////////////////////////////////////////
const menuItem_t menu_2_items[] = {
    {   NULL,    NULL,                  "ZP temp set      2.1"},
    {   NULL,    NULL,                  "ZP temp rise set 2.2"},
    {   NULL,    NULL,                  "ZP Diagnosis       2.3"},
    {   NULL,    NULL,                  "dev info set       2.4"},
//    {   NULL,    &menu_2_2,   "ﯳ���������       2.2"},
//    {   NULL,    &menu_2_3,   "ﯳ��������       2.3"},
//    {   NULL,    &menu_2_4,   "�豸��Ϣ����       2.4"},
    {   NULL,    NULL,                  "1# 4mA  ratio     2.5"},
    {   NULL,    NULL,                  "1# 20mA ratio     2.6"},
    {   NULL,    NULL,                  "2# 4mA ratio      2.7"},
    {   NULL,    NULL,                  "2# 20mA ratio     2.8"},
    {   NULL,    NULL,                  "dev zero offset       2.9"},
    {   NULL,    NULL,                  "Least signal cutoff  2.10"},
    {   NULL,    NULL,                  "pwd set           2.11"},
    {   NULL,    NULL,                  "reset user pwd       2.12"},
    {   NULL,    NULL,                  "clear log           2.13"},
};

// �˵�2
const menu_t menu_2 = {
     "���Ҳ˵�2",
    (menuItem_t *)menu_2_items,                      //!< menu item list
    getArraySize(menu_2_items),                            //!< menu item count
    (menu_t*)&menu,                                               //!< top menu has no parent
    2 - 1,
};
#endif
//////////

#if 1//�豸��Ϣ-> xxx
//2>x/////////////////////////////////////////////////////
const menuItem_t menu_3_items[] = {
    {   NULL,    NULL,  "dev UID"},
    {   NULL,    NULL,  "Date of production"},
    {   NULL,    NULL,  "hard version"},
    {   NULL,    NULL,  "soft version"},
    {   NULL,    NULL,  "Communication mode"},
//    {   NULL,    (menu_t *)&menu_3_6,  "�鿴��־"},
    {   NULL,    NULL,  "read log"},
    {   NULL,    NULL,  "read warning"},
    {   NULL,    NULL,  "clear warning"},
};

// �˵�3
const menu_t menu_3 = {
     "�豸��Ϣ3",
    (menuItem_t *)menu_3_items,                      //!< menu item list
    getArraySize(menu_3_items),                            //!< menu item count
    (menu_t*)&menu,                                               //!< top menu has no parent
    3 - 1,
};
#endif
//////////

void MenuPage::create()
{
    _gpu->clear();
    pMenu = new GuiMenu(30,0,100,100,&menu,"menu");
    bar = new GuiSideBar(0,0,100,5,1,10);

    Register(pMenu);
    Register(bar);
    
    bar->set_max( pMenu->current_menu->count);
    bar->set_prograss( pMenu->index_get());
    GuiPage::create();
}
void MenuPage::show()
{
    UART.printf("Munu show pwd:%s",pagePwd->pwd.c_str());
    if(pagePwd->pwd != "")
    {
        if(pagePwd->pwd == "4170" && pMenu->index_get() == 0)
        {
            pMenu->current_menu = &menu_1;
        }
        else if(pagePwd->pwd == "4170" && pMenu->index_get() == 2)
        {
            pMenu->current_menu = &menu_3;
        }
        else if(pagePwd->pwd == "1819" && pMenu->index_get() == 1)
            pMenu->current_menu = &menu_2;
        else
        {
            goto skip_menu_set;
        }

        pMenu->index = 0;
        bar->max = ( pMenu->current_menu->count);
        bar->prograss = ( pMenu->index_get());
        skip_menu_set:
        pagePwd->pwd  = "";
    }
    GuiPage::show();
}
void MenuPage::loop()
{
}
void MenuPage::event(Object *sender,GuiMessage *msg)
{
     
    if(msg->str == "a")
    {
        if(pMenu->current_menu->parent != NULL)
        {
            pMenu->index = (pMenu->current_menu->parent_index);
            pMenu->current_menu = pMenu->current_menu->parent;
            pMenu->show();

        }
        else
        {
            ui.go_back();
            return ;
        }

    }   
    if(msg->str == "d")
    {
        menuItem_t *item = &pMenu->current_menu->items[pMenu->index_get()];
        if(item->handle != NULL)
        {
            item->handle();
            return ;
        }
        else if(item->child != NULL)
        {
            pMenu->current_menu = item->child;
            pMenu->index_set(0);
        }

    }
        
    if(msg->str == "w")
    {
        pMenu->index_previous();
    }
    
        
    if(msg->str == "s")
    {
        pMenu->index_next();
    }
    bar->set_max  ( pMenu->current_menu->count);
    bar->set_prograss( pMenu->index_get());
    
     UART.printf("cmd:%s;select:%d/%d\r\n",msg->str.c_str(),pMenu->index_get(),pMenu->current_menu->count);

}

