/**
  ******************************************************************************
  * @file    t_menuDef.h
  * @author  ������
  * @version V1.0
  * @date    2019/3/14
  * @brief	 һ���򵥵��ı��˵�������Gorgon_Meducer��ɵ���ӣ�����Ĵ������������
	*		ԭ������ 
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

// Ϊ�˵��ṹ�崴������������ʹ��
typedef struct __menuItem      		menuItem_t;					// �˵���
typedef struct __menu        			menu_t;   					// �˵������˵�����,�˵�ҳ
typedef struct __menuEngineCb     menuEngineCb_t;		  // �˵���������״̬����������ǵ�ǰ�˵�״̬
// �ص�����
typedef fsm_rt_t menuItemHandler_t(void);	            // �˵���ܴ�����	
typedef fsm_rt_t menuEngine_t(menuEngineCb_t *);			// �˵���������



/*------------------------------------����Ϊ�˵��ṹ��--------------------------------------------*/

/**
	*@brief    �˵���ṹ�壬��������һ���˵�����Լ̳���չ
	*					*fnHandle �����Ϊnull��˵���ò˵��ж�Ӧ����
	*					*ptChild	�����Ϊnull��˵�����Ӳ˵���
	*					һ��������ָ��ȡֵ���⣬����ò˵����ж��������Ӳ˵�����Ҫ���⴦��
	*/
struct __menuItem {
    menuItemHandler_t    *fnHandle;                      //!< handler
    menu_t               *ptChild;                       //!< Child Menu
    //! depends on your application, you can add/remove/change following members
    char                 *pchTitle;                      //!< Menu Title
//    char                *pchDescription;                //!< Description for this menu item
//    char                chShortCutKey;                  //!< Shortcut Key value in current menu
};



/**
	*@brief   �˵���������������һϵ�о���˵���
	*					*ptItems��ָ��˵�������
 	*					chCount		�˵�������
	*					*ptParent ��ǰ�˵����˵���һ���˵�����ֻ����һ�����˵�
	*					*fnEngine �˵��������棬ÿ���˵����Ը�����Ҫ�в�ͬ�Ĵ�������
	*/
struct __menu{
    menuItem_t          *ptItems;                         //!< menu item list
    uint_fast8_t        chCount;                          //!< menu item count
    menu_t              *ptParent;                        //!< parent menu;	ָ�򸸲˵�
    menuEngine_t        *fnEngine;                        //!< engine for process current menu ��ǰ�˵���������
};

/**
	*@brief   �˵���������״̬����������ǵ�ǰ�˵�״̬
	*					tState����ǰ�˵�״̬
 	*					*ptCurrentMenu			��ǰ�˵�
	*					chCurrentItemIndex 	��ǰ�˵���ptItems�е��±�����
	*/
struct __menuEngineCb {
    uint_fast8_t    tState;
    const menu_t    *ptCurrentMenu;
    uint_fast8_t    chCurrentItemIndex;
};
#endif
