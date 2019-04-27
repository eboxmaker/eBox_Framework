
#include "t_menu.h"
#include "bsp_ebox.h"
#include "..\component\menu\menuDef.h"
#include "gt30l32s4w.h"
#include "ebox_mem.h"
// 1选项
const menuItem_t menu1_items[] = {
    {	NULL,               (menu_t *)&menu1_1, "用户菜单"},
    {	NULL,               NULL,               "厂家菜单"},
    {	menu1_3_handler,    NULL,               "设备信息"},
};

// 菜单1
const menu_t menu1 = {
    "主菜单",
    (menuItem_t *)menu1_items,                      //!< menu item list
    getArraySize(menu1_items),                            //!< menu item count
    NULL,                                               //!< top menu has no parent
    top_menu_engine,                                    
};

// 1.1选项
const menuItem_t menu1_1_items[] = {
    {   NULL,    NULL,                  "输出设置1.1"},
    {   NULL,    (menu_t *)&menu1_1_2,  "报警设置1.2"},
    {   NULL,    NULL,                  "设备标定1.3"},
    {   NULL,    NULL,                  "系统设置1.4"},
};

// 菜单1.1
const menu_t menu1_1 = {
     "用户菜单",
    (menuItem_t *)menu1_1_items,                      //!< menu item list
    getArraySize(menu1_1_items),                            //!< menu item count
    (menu_t*)&menu1,                                               //!< top menu has no parent
    top_menu_engine,                                    
};

const menuItem_t menu1_1_2_items[] = 
{
    {   NULL,    NULL,   "氧含量高高报"},
    {   NULL,    NULL,   "氧含量搞报"},
    {   NULL,    NULL,   "氧含量低报"},
    {   NULL,    NULL,   "氧含量低低报"},
    
    {   NULL,    NULL,   "O2报警滞后值"},
    {   NULL,    NULL,   "锆池温度报警滞后值"},
    {   NULL,    NULL,   "报警滞后时间"},
    {   NULL,    NULL,   "锆池温度高"},
    {   NULL,    NULL,   "锆池温度低"},
};
//菜单
const menu_t menu1_1_2 = {
     "报警设置",
    (menuItem_t *)menu1_1_2_items,                      //!< menu item list
    getArraySize(menu1_1_2_items),                            //!< menu item count
    (menu_t*)&menu1,                                               //!< top menu has no parent
    top_menu_engine,                                    

};

menuManager_t menu_demo = {0,&menu1,0};

fsm_rt_t menu1_3_handler()
{
    gui.clear();

    gui.set_font_hz_extern(Hz_15x16_ID);
    gui.set_font_ascii_extern(Ascii_8x16_ID);
    gui.printf(0,0,"企业、person who is in 店铺等的 经理,经营者,老a person who is in charge of running a business, a shop/store or a similar organization or part of one");
     gui.set_font_hz_extern(Hz_32x32_ID);
    gui.set_font_ascii_extern(Ascii_16x32_ID);
    gui.flush();
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
    UART.println("start");
  gui.clear();
  gui.printf(0,0,"请输入mv值：\r\n");
  oled.flush();

        LED1.toggle();
        delay_ms(1000);  
    LED1.toggle();
        delay_ms(1000);
    
//  do{
//    while (UART.available()==0);
//    tmp = UART.parseFloat();
//  }while (UART.read() != 0x0d);
  gui.printf("input is %.02fmv \r\n",tmp);
  UART.setTimeout(200);
  oled.flush();
  UART.println("OVER");
  
  displayCurrentMenu(&menu_demo);

  return fsm_rt_cpl;
}
fsm_rt_t sec_menu_item_b_handler(){
		UART.printf("save data! \r\n");
		return fsm_rt_cpl;
}

void displayCurrentMenu(menuManager_t * manager){
    
	menuItem_t *item = manager->current_menu->items;
    gui.fill_screen(0);
    for(uint8_t i= manager->item_index; i < manager->current_menu->count; i++){
        gui.printf(22,(i-manager->item_index)*gui.get_extern_char_info().YSize,item[i].title);
    }	
    gui.printf(5,0,">");
    side_bar.max = manager->current_menu->count;
    side_bar.prograss = manager->item_index;
    gui.draw_sidebar(side_bar);
    UART.println(ebox_get_free());
    gui.flush();
}

fsm_rt_t top_menu_item_c_handler()
{
    menu_demo.current_menu =  menu_demo.current_menu->parent;
    menu_demo.state = 0;
    menu_demo.item_index = 0;
    displayCurrentMenu(&menu_demo);
    return fsm_rt_cpl;
}



fsm_rt_t top_menu_engine(menuManager_t *manager)
{

    enum {
        START = 0,
        READ_KEY_EVENT,
        KEY_PROCESS,
        RUN_ITEM_HANDLER
    };

    String cmd;
    menuItem_t *item;

    switch (manager->state){
        case START:
            manager->state++;
        case READ_KEY_EVENT: 	// 读取按键
            cmd = UART.readString();
            if (cmd == "")
            {
                break;
            }
            else
            {
            }
        case KEY_PROCESS:			// 处理按键响应
            if(cmd == "s")
            {
                manager->item_index++;
                if(manager->item_index >= manager->current_menu->count)
                    manager->item_index = manager->current_menu->count - 1;
            }
            else if(cmd == "w")
            {
                if(manager->item_index != 0)
                    manager->item_index--;
            }
            else if(cmd == "a")
            {
                if(manager->current_menu->parent != NULL)
                {
                    manager->current_menu = manager->current_menu->parent;
                    manager->item_index = 0;
                    manager->state = START;
                }
            }
            else if(cmd == "q")
            {
                item = &(manager->current_menu->items)[manager->item_index];
                if (NULL != item->handle) {
                    manager->state = RUN_ITEM_HANDLER;
                } else if (NULL != item->child) {
                    manager->current_menu = item->child;
                    manager->item_index = 0;
                    manager->state = START;
                }                
            }
            displayCurrentMenu(manager);
            break;
        case RUN_ITEM_HANDLER:// 执行菜单句柄
            item = &(manager->current_menu->items)[manager->item_index];
            item->handle();
            manager->state = 0;
            return fsm_rt_cpl;
            break;
    }

}

fsm_rt_t menu_task(menuManager_t *manager)
{

    return manager->current_menu->engine(manager); 
    
}