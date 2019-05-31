#ifndef __GUI_MENU_H
#define __GUI_MENU_H


#include "graphic.h"
#include "gui_base.h"

typedef struct _menu menu_t;
typedef struct _menuItem menuItem_t;

struct _menuItem {
    void            (*handle)(void);        //!< handler
    menu_t          *child;                 //!< Child Menu
    const char      *title;                 //!< Menu Title

};
 struct _menu{
    const char          *name;                         //!< menu item list
    menuItem_t          *items;                         //!< menu item list
    uint_fast8_t        count;                          //!< menu item count
    menu_t              *parent;                        //!< parent menu;	ָ�򸸲˵�
    uint8_t             parent_index;                        //!< parent menu;	ָ�򸸲˵�
};

class GuiMenu :public ActivityComponent
{

    public:
        GuiMenu(int16_t x,int16_t y,int16_t len,int16_t hight,
                const menu_t *menu)
        {
            this->x = x;
            this->y = y;
            this->len = len;
            this->hight = hight;
            index = 0;
            max = 3;
            name = "";
            type = GuiType::Menu;
            current_menu = menu;
        }
        virtual ~GuiMenu(){};
        void create();
        virtual void show();
        virtual void hide();
        void update_item_index();
        uint8_t get_value();
        virtual void set_select(bool state);
        void update_value();
        
    public:
        int16_t x,y;
        int16_t len,hight;
        int16_t index;
        uint8_t max;
        const menu_t    *current_menu;
};

#endif

