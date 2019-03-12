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


// 声明数据类型
typedef struct __menu_item  		menu_item_t;					// 菜单项
typedef struct __menu      			menu_t;								// 菜单容器
typedef struct __menu_engine_cb menu_engine_cb_t;			// 菜单处理引擎状态，用来标记当前菜单状态

typedef fsm_rt_t menu_item_handler_t(void);	          // 菜单项功能处理函数	
typedef fsm_rt_t menu_engine_t(menu_engine_cb_t *);		// 菜单处理引擎
typedef struct __default_menu_item_t  default_menu_item_t;

/**
	*@brief    菜单项结构体，用来描述一个菜单项，可以继承扩展
	*					*fnHandle 如果不为null，说明该菜单有对应动作
	*					*ptChild	如果不为null，说明有子菜单。
	*					一般这两个指针取值互斥，如果该菜单既有动作又有子菜单，需要特殊处理
	*/
struct __menu_item {
    menu_item_handler_t *fnHandle;                      //!< handler
    menu_t              *ptChild;                       //!< Child Menu
};

/**
	*@brief   菜单容器，用来保存一系列具体菜单项
	*					*ptItems，指向菜单项数组
 	*					chCount		菜单项数量
	*					*ptParent 当前菜单父菜单，一个菜单容器只能有一个父菜单
	*					*fnEngine 菜单处理引擎，每个菜单可以根据需要有不同的处理引擎
	*/
struct __menu {
    menu_item_t        *ptItems;                        //!< menu item list
    uint_fast8_t        chCount;                        //!< menu item count
    menu_t             *ptParent;                       //!< parent menu;	指向父菜单
    menu_engine_t      *fnEngine;                       //!< engine for process current menu 当前菜单处理引擎
};

/**
	*@brief   
	*					tState，当前菜单状态
 	*					*ptCurrentMenu			当前菜单
	*					chCurrentItemIndex 	当前菜单在ptItems中的下标索引
	*/
struct __menu_engine_cb {
    uint_fast8_t    tState;
    const menu_t    *ptCurrentMenu;
    uint_fast8_t    chCurrentItemIndex;
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

extern fsm_rt_t top_menu_item_a_handler();
extern fsm_rt_t top_menu_item_b_handler();
extern fsm_rt_t top_menu_item_c_handler();

extern const menu_t c_tTopMenu;
extern const menu_t c_tSecMenu;

// 定义一系列菜单项
default_menu_item_t c_tTopMenuItems[] = {
    {	top_menu_item_a_handler,NULL,"1 校准","This is Top Menu A",},
		{	{NULL,(menu_t *)&c_tSecMenu},"2 查看校准系数","This is Top Menu B",},
		{	top_menu_item_c_handler,NULL,"输入数字选择操作，回车确认！","This is Top Menu C",},
};

// 定义一系列菜单项
default_menu_item_t c_tSecMenuItems[] = {
    {	top_menu_item_a_handler,NULL,"1 二级菜单1","This is Top Menu A",},
		{	{top_menu_item_b_handler,NULL},"2 二级菜单2","This is Top Menu B",},
		{	top_menu_item_c_handler,NULL,"输入数字选择操作，回车确认！","This is Top Menu C",},
};
// 定义菜单容器
const menu_t c_tTopMenu = {
    (menu_item_t*)c_tTopMenuItems,                      //!< menu item list
    UBOUND(c_tTopMenuItems),                            //!< menu item count
    NULL,                                               //!< top menu has no parent
    top_menu_engine,                                    
};

// 定义菜单容器
const menu_t c_tSecMenu = {
    (menu_item_t*)c_tSecMenuItems,                      //!< menu item list
    UBOUND(c_tSecMenuItems),                            //!< menu item count
    NULL,                                               //!< top menu has no parent
    top_menu_engine,                                    
};


fsm_rt_t top_menu_item_a_handler()
{
    LED1.set();
    return fsm_rt_cpl;
}

fsm_rt_t top_menu_item_b_handler()
{
		LED1.reset();
    return fsm_rt_cpl;
}

fsm_rt_t top_menu_item_c_handler()
{
    return fsm_rt_cpl;
}

void displayCurrentMenu(menu_engine_cb_t * s_tmenu){
		default_menu_item_t *menu = (default_menu_item_t *)s_tmenu->ptCurrentMenu->ptItems;
			for(uint8_t i= 0;i<3;i++){
      UART.printf(menu[i].pchTitle);
			UART.printf("\r\n");
		}	
}

fsm_rt_t top_menu_engine(menu_engine_cb_t *ptThis)
{
  #define DEFAULT_MENU_ENGINE_RESET_FSM() \
    do { ptThis->tState = START; } while(0)
  enum {
    START = 0,
    READ_KEY_EVENT,
    KEY_PROCESS,
    RUN_ITEM_HANDLER
  };
  uint8_t key;
  menu_item_t *ptItem;
//	default_menu_item_t *ptItem;
  switch (ptThis->tState){
  case START:
    ptThis->tState++;
  case READ_KEY_EVENT: 	// 读取按键
    key = UART.parseInt();
    if (key == 0 || key > 2) break;
  case KEY_PROCESS:			// 处理按键响应
		ptThis->chCurrentItemIndex = key-1;
    ptItem = (menu_item_t *)&((default_menu_item_t*)ptThis->ptCurrentMenu->ptItems)[ptThis->chCurrentItemIndex];
    if (NULL != ptItem->fnHandle) {
      ptThis->tState = RUN_ITEM_HANDLER;
    } else if (NULL != ptItem->ptChild) {
      ptThis->ptCurrentMenu = ptItem->ptChild;
      ptThis->chCurrentItemIndex = 0;
			displayCurrentMenu(ptThis);
      DEFAULT_MENU_ENGINE_RESET_FSM();
      return fsm_rt_cpl;
    }
    break;
  case RUN_ITEM_HANDLER:// 执行菜单句柄
		ptItem = (menu_item_t *)&((default_menu_item_t*)ptThis->ptCurrentMenu->ptItems)[ptThis->chCurrentItemIndex];
		ptItem->fnHandle();
		ptThis->tState = 0;
    return fsm_rt_cpl;
    break;
  }

}

fsm_rt_t menu_task(menu_engine_cb_t *ptThis)
{
    do {
        /* this validation code could be removed for release version */
        if (NULL == ptThis) {
            break;
        } else if (NULL == ptThis->ptCurrentMenu) {
            break;
        } else if (NULL == ptThis->ptCurrentMenu->fnEngine) {
            break;
        } else if (NULL == ptThis->ptCurrentMenu->ptItems) {
            break;
        } else if (0 == ptThis->ptCurrentMenu->chCount) {
            break;
        }        
        return ptThis->ptCurrentMenu->fnEngine(ptThis);        
    } while(false);
    
    return fsm_rt_err;
}

static menu_engine_cb_t s_tMenuDemo = {0,&c_tTopMenu,0};



void setup()
{
    ebox_init();
    UART.begin(115200);
		UART.setTimeout(200);
		LED1.mode(OUTPUT_PP);
		UART.printf("\r\n");
	displayCurrentMenu(&s_tMenuDemo);

}

int main(void)
{
    setup();
		while(1){
			menu_task(&s_tMenuDemo);
		}
}