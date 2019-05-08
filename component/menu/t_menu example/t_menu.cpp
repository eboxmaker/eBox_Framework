
#include "t_menu.h"
#include "bsp_ebox.h"

// 父菜单项
const menuItem_t c_tTopMenuItems[] = {
    {	top_menu_item_a_handler,NULL,"1 开灯"},
		{	top_menu_item_b_handler,NULL,"2 关灯"},
		{	NULL,(menu_t *)&c_tSecMenu,"3 进入子菜单"},
};

// 顶层菜单
const menu_t c_tTopMenu = {
    (menuItem_t *)c_tTopMenuItems,                      //!< menu item list
    getArraySize(c_tTopMenuItems),                            //!< menu item count
    NULL,                                               //!< top menu has no parent
    top_menu_engine,                                    
};

// 子菜单项
const menuItem_t c_tSecMenuItems[] = {
    {	sec_menu_item_a_handler,NULL,"1 输入小数"},
		{	sec_menu_item_b_handler,NULL,"2 保存数据"},
		{	top_menu_item_c_handler,NULL,"3 返回上一层"},
};

// 二级菜单
const menu_t c_tSecMenu = {
    (menuItem_t *)c_tSecMenuItems,                      //!< menu item list
    getArraySize(c_tSecMenuItems),                            //!< menu item count
    (menu_t*)&c_tTopMenu,                                               //!< top menu has no parent
    top_menu_engine,                                    
};

menuEngineCb_t s_tMenuDemo = {0,&c_tTopMenu,0};

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

fsm_rt_t sec_menu_item_a_handler(){
  float tmp;
  uint8_t t;
  UART.setTimeout(2000);
  UART.printf("请输入mv值：\r\n");
  do{
    while (UART.available()==0);
    tmp = UART.parseFloat();
  }while (UART.read() != 0x0d);
  UART.printf("input is %.02fmv \r\n",tmp);
  UART.setTimeout(200);
  return fsm_rt_cpl;
}
fsm_rt_t sec_menu_item_b_handler(){
		UART.printf("save data! \r\n");
		return fsm_rt_cpl;
}

void displayCurrentMenu(menuEngineCb_t * s_tmenu){
			menuItem_t *ptItem = s_tmenu->ptCurrentMenu->ptItems;
			UART.printf("\f");
			for(uint8_t i= 0;i<s_tmenu->ptCurrentMenu->chCount;i++){
      UART.printf(ptItem[i].pchTitle);
			UART.printf("\r\n");
		}	
}

fsm_rt_t top_menu_item_c_handler()
{
    s_tMenuDemo.ptCurrentMenu =  s_tMenuDemo.ptCurrentMenu->ptParent;
    s_tMenuDemo.tState = 0;
    s_tMenuDemo.chCurrentItemIndex = 0;
		displayCurrentMenu(&s_tMenuDemo);
    return fsm_rt_cpl;
}



fsm_rt_t top_menu_engine(menuEngineCb_t *ptThis)
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
  menuItem_t *ptItem;

  switch (ptThis->tState){
  case START:
    ptThis->tState++;
  case READ_KEY_EVENT: 	// 读取按键
    key = UART.parseInt();
    if (key == 0 || key > ptThis->ptCurrentMenu->chCount) break;
  case KEY_PROCESS:			// 处理按键响应
		ptThis->chCurrentItemIndex = key-1;
    ptItem = &(ptThis->ptCurrentMenu->ptItems)[ptThis->chCurrentItemIndex];
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
		ptItem = &(ptThis->ptCurrentMenu->ptItems)[ptThis->chCurrentItemIndex];
		ptItem->fnHandle();
		ptThis->tState = 0;
    return fsm_rt_cpl;
    break;
  }
}

fsm_rt_t menu_task(menuEngineCb_t *ptThis)
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