#include "ui.h"

Ui ui;


void Ui::begin(GuiPage  *page)
{
    cur_page = page;
    
    cur_page->create();

}
void Ui::open(GuiPage  *page)
{
    if(page->name == "main")
        page->father = NULL;
    else
        page->father = cur_page;
//    cur_page->cancel();

    cur_page = page;
    
    cur_page->create();
}
void Ui::go_back()
{
    if(cur_page->father != NULL)
    {
        cur_page->cancel();
        cur_page = cur_page->father;
        cur_page->create();
    }

}

void Ui::loop()
{
    cur_page->loop();
}
void Ui::event(Object *sender,GuiMessage *msg)
{
    cur_page->event(sender,msg);

}