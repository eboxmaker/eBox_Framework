#ifndef __GUI_MENU_H
#define __GUI_MENU_H


#include "graphic.h"
#include "gui_base.h"

typedef struct _menu menu_t;
typedef struct _menuItem menuItem_t;

struct _menuItem {
    void            (*handle)(void);        //!< handler
    const menu_t    *child;                 //!< Child Menu
    const char      *title;                 //!< Menu Title

};
 struct _menu{
    const char          *name;                         //!< menu item list
    menuItem_t          *items;                         //!< menu item list
    uint_fast8_t        count;                          //!< menu item count
    menu_t              *parent;                        //!< parent menu;	指向父菜单
    uint8_t             parent_index;                        //!< parent menu;	指向父菜单
};

class GuiMenu :public ActivityComponent
{

    public:
        GuiMenu(int16_t x,int16_t y,int16_t len,int16_t hight,
                const menu_t *menu,String name = "")
        {
            this->x = x;
            this->y = y;
            this->len = len;
            this->hight = hight;
            font = &GUI_Font16_ASCII;
            text_mode = GuiDrawMode::Trans;
            index = 0;
            max = 3;
            this->name = name;
            type = GuiType::Menu;
            current_menu = menu;
        }
        virtual ~GuiMenu(){};
        virtual void create();
        virtual void show();
        virtual void hide();
        uint8_t get_value();
        virtual void set_select(bool state);

            
        //菜单选择索引
        int16_t index;
        int16_t index_get();
        void index_set(int16_t value);
        bool index_next();
        bool index_previous();

    public:
        int16_t x,y;
        int16_t len,hight;
        const menu_t    *current_menu;
        uint8_t max;
        const GUI_FONT *font;
        GuiDrawMode text_mode;
    private:
        void update_item_index();
};

#endif

