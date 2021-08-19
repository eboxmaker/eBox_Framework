#include "ui.h"

Ui ui;


void Ui::begin(GuiPage  *page)
{
    cur_page = page;
    main_page = page;
    
    cur_page->create();

}
void Ui::open(GuiPage  *page)
{
    if(page == main_page)
        cur_page->cancel();
    else
        page->father = cur_page;

    cur_page->hide();
    cur_page = page;
    
    page->create();
}
void Ui::go_back()
{
    if(cur_page->father != NULL)
    {
        cur_page->cancel();
        cur_page = cur_page->father;
        cur_page->show();
    }

}
void Ui::go_home()
{
    while(1)
    {
        cur_page->cancel();
        if(cur_page->father != NULL && cur_page->father != main_page)
        {
            cur_page = cur_page->father;
        }
        else
        {
            main_page->show();
            break;
        }
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