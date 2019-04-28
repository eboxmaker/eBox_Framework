/**
  ******************************************************************************
  * @file    t_menu.h
  * @author  好心情
  * @version V1.0
  * @date    2019/3/14
  * @brief	 t_menu example
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
#ifndef __TMENU_H
#define __TMENU_H
#include "..\component\menu\menuDef.h"

extern fsm_rt_t top_menu_engine(menuManager_t *ptThis);

extern fsm_rt_t top_menu_item_a_handler();
extern fsm_rt_t top_menu_item_b_handler();
extern fsm_rt_t top_menu_item_c_handler();
extern fsm_rt_t sec_menu_item_a_handler();
extern fsm_rt_t sec_menu_item_b_handler();


extern fsm_rt_t menu1_3_handler();

extern void displayCurrentMenu(menuManager_t * s_tmenu);
extern fsm_rt_t menu_task(menuManager_t *ptThis);

//一级菜单
extern const menu_t menu;//顶层菜单

//一级菜单
extern const menu_t menu_1;
extern const menu_t menu_2;//厂家菜单
extern const menu_t menu_3;//厂家菜单

//二级菜单
extern const menu_t menu_1_1;//用户菜单->输出设置
extern const menu_t menu_1_2;//用户菜单->报警设置
extern const menu_t menu_1_3;//用户菜单->设备标定
extern const menu_t menu_1_4;//用户菜单->系统设置


extern const menu_t menu_2_2;//厂家菜单->锆池升温设置
extern const menu_t menu_2_3;//厂家菜单->锆池性能诊断
extern const menu_t menu_2_4;//厂家菜单->设备信息设置



//三级菜单
extern const menu_t menu_1_1_1;//用户菜单->报警设置->电流环输出
extern const menu_t menu_1_1_2;//用户菜单->报警设置->继电器输出

extern const menu_t menu_1_3_1;
extern const menu_t menu_1_3_2;

extern const menu_t menu_1_4_1;
extern const menu_t menu_1_4_2;
extern const menu_t menu_1_4_3;
extern const menu_t menu_1_4_4;

//四级菜单
extern const menu_t menu_1_1_1_2;//用户菜单->报警设置->电流环输出->电流环1
extern const menu_t menu_1_1_1_3;//用户菜单->报警设置->继电器输出->电流环2



extern menuManager_t menu_demo;


#endif
