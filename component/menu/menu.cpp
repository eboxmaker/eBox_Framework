#include "menu.h"

MenuItem main_item[]={

"setting",&setting_page,0,
"version",&version_page,0,
"time",0,0,
};
MenuItem setting_item[]={

"set1",0,0,
"set2",0,0,
"set3",0,0,
};

MenuPage main_page = {
"Main System",3,0,0,main_item,0,0
};
MenuPage setting_page = {
"Settings",3,0,0,setting_item,&main_page,0
};
MenuPage version_page = {
"version",0,0,0,0,&main_page,0
};

