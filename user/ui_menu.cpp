#include "ui_menu.h"
#include "gui_menu.h"
#include "gui_list.h"
#include "gui_sidebar.h"
#include "bsp_ebox.h"
#include "ui.h"
#include "ui_setvalue.h"
extern const menu_t menu_1;
void setValue()
{
    ui.open(pageSetValue);
    
}
/////////////////////////////顶层级菜单/////////////////////////////////////////////////
const menuItem_t menu_items[] = {
    {	NULL,               (menu_t *)&menu_1,  "user"},
    {	NULL,               NULL,               "factory"},
    {	NULL,               NULL,               "dev info"},
};

// 菜单1
const menu_t menu = {
    "主菜单",
    (menuItem_t *)menu_items,                      //!< menu item list
    getArraySize(menu_items),                            //!< menu item count
    NULL,                                               //!< top menu has no parent
    0,
};

const menuItem_t menu_1_items[] = {
    {	NULL,               NULL,  "OUTPUT"},
    {	NULL,               NULL,  "warning"},
    {	setValue,           NULL,  "dev set"},
    {	NULL,               NULL,  "system set"},
};

// 菜单1
const menu_t menu_1 = {
     "用户菜单1",
    (menuItem_t *)menu_1_items,     //!< menu item list
    getArraySize(menu_1_items),     //!< menu item count
    (menu_t*)&menu,                 //!< top menu has no parent
    1 - 1,
};
 
GuiMenu *pMenu;
GuiList *pList;
GuiSideBar *bar;

MenuPage *pageMenu = new MenuPage("menu");

void MenuPage::create()
{
    _gpu->clear();
    pMenu = new GuiMenu(30,0,100,100,&menu);
    bar = new GuiSideBar(0,0,100,5,1,10);

    regedit(pMenu);
    regedit(bar);
    
    bar->set_max( pMenu->current_menu->count);
    bar->set_prograss( pMenu->index);
    GuiPage::create();
}
void MenuPage::loop()
{
}
void MenuPage::event(Object *sender,GuiMessage *msg)
{
     
    if(msg->str == "a")
    {
        pMenu->current_menu = pMenu->current_menu->parent;
        pMenu->index = 0;
        pMenu->update_item_index();
    }   
    if(msg->str == "d")
    {
        menuItem_t *item = &pMenu->current_menu->items[pMenu->index];
        if(item->handle != NULL)
        {
            item->handle();
        }
        else
        {
            pMenu->current_menu = item->child;
            pMenu->index = 0;
            pMenu->update_item_index();
        }

    }
        
    if(msg->str == "w")
    {
        if(pMenu->index > 0)
        {
            UART.printf("pMenu->index:--\r\n");
            pMenu->index--;
            pMenu->update_item_index();
        }
    }
    
        
    if(msg->str == "s")
    {
        if(pMenu->index < pMenu->current_menu->count - 1)
        {
            UART.printf("select:++\r\n");
            pMenu->index++;
            pMenu->update_item_index();
        }
        else
        {

        }
    }
    bar->set_max  ( pMenu->current_menu->count);
    bar->set_prograss( pMenu->index);
    
     UART.printf("cmd:%s;select:%d/%d\r\n",msg->str.c_str(),pMenu->index,pMenu->current_menu->count);

}