/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "bsp_ebox.h"

#ifndef __FSM_RT_TYPE__
#define __FSM_RT_TYPE__
//! \name finit state machine state
//! @{
typedef enum {
    fsm_rt_err          = -1,    //!< fsm error, error code can be get from other interface
    fsm_rt_cpl          = 0,     //!< fsm complete
    fsm_rt_on_going     = 1,     //!< fsm on-going
    fsm_rt_wait_for_obj = 2,     //!< fsm wait for object
    fsm_rt_asyn         = 3,     //!< fsm asynchronose complete, you can check it later.
} fsm_rt_t;
//! @}

#endif

#define UBOUND(__ARR)    (sizeof(__ARR)/sizeof(__ARR[0]))
typedef struct __menu_item  menu_item_t;		// 菜单项
typedef struct __menu      menu_t;					// 菜单容器

typedef fsm_rt_t menu_item_handler_t(menu_item_t *);

struct __menu_item {
    menu_item_handler_t *fnHandle;                      //!< handler
    menu_t              *ptChild;                       //!< Child Menu
};

typedef struct __menu_engine_cb menu_engine_cb_t;
typedef fsm_rt_t menu_engine_t(menu_engine_cb_t *);

struct __menu {
    menu_item_t        *ptItems;                        //!< menu item list
    uint_fast8_t        chCount;                        //!< menu item count
    menu_t             *ptParent;                       //!< parent menu;	指向父菜单
    menu_engine_t      *fnEngine;                       //!< engine for process current menu 当前菜单处理引擎
};

typedef struct __default_menu_item_t  default_menu_item_t;

struct __default_menu_item_t   {

    //! inherit from base class menu_item_t
    menu_item_t					menu; 

    //! depends on your application, you can add/remove/change following members
    char                *pchTitle;                      //!< Menu Title
    char                *pchDescription;                //!< Description for this menu item
    char                chShortCutKey;                  //!< Shortcut Key value in current menu
};

extern fsm_rt_t top_menu_engine(menu_engine_cb_t*ptThis);

extern fsm_rt_t top_menu_item_a_handler(menu_item_t *ptItem);
extern fsm_rt_t top_menu_item_b_handler(menu_item_t *ptItem);
extern fsm_rt_t top_menu_item_c_handler(menu_item_t *ptItem);

extern const menu_t c_tTopMenu;

default_menu_item_t c_tTopMenuItems[] = {
    {
        top_menu_item_a_handler,
        NULL,                                           //!< child menu
        "Top Menu A",
        "This is Top Menu A",
    },
    {
        top_menu_item_b_handler,
        NULL,                                           //!< child menu
        "Top Menu B",
        "This is Top Menu B"
    },
    {
        top_menu_item_c_handler,
        NULL,                                           //!< child menu
        "Top Menu C",
        "This is Top Menu C"
    }
};

const menu_t c_tTopMenu = {
    (menu_item_t *)c_tTopMenuItems,                                    //!< menu item list
    UBOUND(c_tTopMenuItems),                            //!< menu item count
    NULL,                                               //!< top menu has no parent
    top_menu_engine,                                    
};


fsm_rt_t top_menu_item_a_handler(menu_item_t *ptItem)
{
    return fsm_rt_cpl;
}

fsm_rt_t top_menu_item_b_handler(menu_item_t *ptItem)
{
    return fsm_rt_cpl;
}

fsm_rt_t top_menu_item_c_handler(menu_item_t *ptItem)
{
    return fsm_rt_cpl;
}

fsm_rt_t top_menu_engine(menu_engine_cb_t*ptThis)
{
    return fsm_rt_cpl;
}

void setup()
{
    ebox_init();
    UART.begin(115200);
		UART.printf("test");
		for(uint8_t i= 0;i<3;i++){
			UART.printf(c_tTopMenuItems[i].pchTitle );
		}
}

int main(void)
{
    setup();
		while(1){
		}
}