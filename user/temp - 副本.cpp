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


// ������������
typedef struct __menu_item  		menu_item_t;					// �˵���
typedef struct __menu      			menu_t;								// �˵�����
typedef struct __menu_engine_cb menu_engine_cb_t;			// �˵���������״̬��������ǵ�ǰ�˵�״̬

typedef fsm_rt_t menu_item_handler_t(void);	          // �˵���ܴ�����	
typedef fsm_rt_t menu_engine_t(menu_engine_cb_t *);		// �˵���������
typedef struct __default_menu_item_t  default_menu_item_t;

/**
	*@brief    �˵���ṹ�壬��������һ���˵�����Լ̳���չ
	*					*fnHandle �����Ϊnull��˵���ò˵��ж�Ӧ����
	*					*ptChild	�����Ϊnull��˵�����Ӳ˵���
	*					һ��������ָ��ȡֵ���⣬����ò˵����ж��������Ӳ˵�����Ҫ���⴦��
	*/
struct __menu_item {
    menu_item_handler_t *fnHandle;                      //!< handler
    menu_t              *ptChild;                       //!< Child Menu
};

/**
	*@brief   �˵���������������һϵ�о���˵���
	*					*ptItems��ָ��˵�������
 	*					chCount		�˵�������
	*					*ptParent ��ǰ�˵����˵���һ���˵�����ֻ����һ�����˵�
	*					*fnEngine �˵��������棬ÿ���˵����Ը�����Ҫ�в�ͬ�Ĵ�������
	*/
struct __menu {
    menu_item_t        *ptItems;                        //!< menu item list
    uint_fast8_t        chCount;                        //!< menu item count
    menu_t             *ptParent;                       //!< parent menu;	ָ�򸸲˵�
    menu_engine_t      *fnEngine;                       //!< engine for process current menu ��ǰ�˵���������
};

/**
	*@brief   
	*					tState����ǰ�˵�״̬
 	*					*ptCurrentMenu			��ǰ�˵�
	*					chCurrentItemIndex 	��ǰ�˵���ptItems�е��±�����
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

// ����һϵ�в˵���
default_menu_item_t c_tTopMenuItems[] = {
    {	top_menu_item_a_handler,NULL,"1 У׼","This is Top Menu A",},
		{	{NULL,(menu_t *)&c_tSecMenu},"2 �鿴У׼ϵ��","This is Top Menu B",},
		{	top_menu_item_c_handler,NULL,"��������ѡ��������س�ȷ�ϣ�","This is Top Menu C",},
};

// ����һϵ�в˵���
default_menu_item_t c_tSecMenuItems[] = {
    {	top_menu_item_a_handler,NULL,"1 �����˵�1","This is Top Menu A",},
		{	{top_menu_item_b_handler,NULL},"2 �����˵�2","This is Top Menu B",},
		{	top_menu_item_c_handler,NULL,"��������ѡ��������س�ȷ�ϣ�","This is Top Menu C",},
};
// ����˵�����
const menu_t c_tTopMenu = {
    (menu_item_t*)c_tTopMenuItems,                      //!< menu item list
    UBOUND(c_tTopMenuItems),                            //!< menu item count
    NULL,                                               //!< top menu has no parent
    top_menu_engine,                                    
};

// ����˵�����
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
  case READ_KEY_EVENT: 	// ��ȡ����
    key = UART.parseInt();
    if (key == 0 || key > 2) break;
  case KEY_PROCESS:			// ��������Ӧ
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
  case RUN_ITEM_HANDLER:// ִ�в˵����
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