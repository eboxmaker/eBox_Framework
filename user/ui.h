#ifndef __UI_H
#define __UI_H
#include "ebox_core.h"
#include "gui_page.h"
class Ui
{
    public:
        Ui(){
//           father = NULL;
           cur_page = NULL;
        };
        virtual ~Ui(){};
        void begin(GuiPage  *page);
        void open(GuiPage  *page);
        void loop();
        void event(Object *sender,GuiMessage *msg);
            
        void go_back();
        void go_home();
            
    private:
        GuiPage *cur_page;
        GuiPage *main_page;
};
extern Ui ui;


#endif
