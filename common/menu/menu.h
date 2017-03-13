#ifndef __MENU_H
#define __MENU_H
#include "ebox.h"

 struct _MenuItem;
 struct _MenuPage;

typedef struct _MenuItem
{
    const char *name;
    _MenuPage *child;
    void (*callback)(void);
}MenuItem;

typedef struct _MenuPage
{
    const char *name;
    char count;
    char select;
    char style;
    _MenuItem *item;
    _MenuPage *parent;
    void (*callback)(void);
}MenuPage;
extern MenuPage main_page;
extern MenuPage setting_page;
extern MenuPage version_page;
extern MenuPage time_page;












#endif
