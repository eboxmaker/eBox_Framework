/**
  ******************************************************************************
  * @file    t_menu.h
  * @author  ºÃÐÄÇé
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
#include "..\t_menuDef.h"

extern fsm_rt_t top_menu_engine(menuManager_t *ptThis);

extern fsm_rt_t top_menu_item_a_handler();
extern fsm_rt_t top_menu_item_b_handler();
extern fsm_rt_t top_menu_item_c_handler();
extern fsm_rt_t sec_menu_item_a_handler();
extern fsm_rt_t sec_menu_item_b_handler();

extern void displayCurrentMenu(menuManager_t * s_tmenu);
extern fsm_rt_t menu_task(menuManager_t *ptThis);

extern const menu_t c_tTopMenu;
extern const menu_t c_tSecMenu;
extern menuManager_t s_tMenuDemo;

#endif
