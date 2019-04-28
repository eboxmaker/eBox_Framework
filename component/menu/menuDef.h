/**
  ******************************************************************************
  * @file    t_menuDef.h
  * @author  好心情
  * @version V1.0
  * @date    2019/3/14
  * @brief	 一个简单的文本菜单，根据Gorgon_Meducer（傻孩子）大神的代码整理而来，
	*		原文连接 
	* https://www.amobbs.com/forum.php?mod=viewthread&tid=5688720&highlight=%E8%8F%9C%E5%8D%95
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMENU_DEF_H
#define __TMENU_DEF_H
#include "ebox.h"

extern "C"
{
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

// 为菜单结构体创建别名，方便使用
typedef struct __menuItem       menuItem_t;					// 菜单项
typedef struct __menu           menu_t;   					// 菜单，即菜单容器,菜单页
typedef struct __menuManager    menuManager_t;		  // 菜单处理引擎状态机，用来标记当前菜单状态
// 回调函数
typedef fsm_rt_t menuItemHandler_t(void);	            // 菜单项功能处理函数	
typedef fsm_rt_t menuEngineCallBack_t(menuManager_t *);			// 菜单处理引擎



/*------------------------------------以下为菜单结构体--------------------------------------------*/

/**
	*@brief    菜单项结构体，用来描述一个菜单项，可以继承扩展
	*					*fnHandle 如果不为null，说明该菜单有对应动作
	*					*ptChild	如果不为null，说明有子菜单。
	*					一般这两个指针取值互斥，如果该菜单既有动作又有子菜单，需要特殊处理
	*/
struct __menuItem {
    menuItemHandler_t    *handle;                      //!< handler
    menu_t               *child;                       //!< Child Menu
    //! depends on your application, you can add/remove/change following members
    const char            *title;                      //!< Menu Title
//    char                *pchDescription;                //!< Description for this menu item
//    char                chShortCutKey;                  //!< Shortcut Key value in current menu
};



/**
	*@brief   菜单容器，用来保存一系列具体菜单项
	*					*ptItems，指向菜单项数组
 	*					chCount		菜单项数量
	*					*ptParent 当前菜单父菜单，一个菜单容器只能有一个父菜单
	*					*fnEngine 菜单处理引擎，每个菜单可以根据需要有不同的处理引擎
	*/
struct __menu{
    const char          *name;                         //!< menu item list
    menuItem_t          *items;                         //!< menu item list
    uint_fast8_t        count;                          //!< menu item count
    menu_t              *parent;                        //!< parent menu;	指向父菜单
    uint8_t             parent_index;                        //!< parent menu;	指向父菜单
    menuEngineCallBack_t        *engine;                        //!< engine for process current menu 当前菜单处理引擎
};

/**
	*@brief   菜单处理引擎状态机，用来标记当前菜单状态
	*					tState，当前菜单状态
 	*					*ptCurrentMenu			当前菜单
	*					chCurrentItemIndex 	当前菜单在ptItems中的下标索引
	*/
struct __menuManager {
    uint_fast8_t    state;
    const menu_t    *current_menu;
    uint_fast8_t    item_index;
};
}
#endif
