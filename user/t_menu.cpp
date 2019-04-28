
#include "t_menu.h"
#include "bsp_ebox.h"
#include "..\component\menu\menuDef.h"
#include "gt30l32s4w.h"
#include "ebox_mem.h"
/////////////////////////////顶层级菜单/////////////////////////////////////////////////
const menuItem_t menu_items[] = {
    {	NULL,               (menu_t *)&menu_1, "用户菜单       1"},
    {	NULL,               (menu_t *)&menu_2, "厂家菜单       2"},
    {	menu1_3_handler,    NULL,               "设备信息       3"},
};

// 菜单1
const menu_t menu = {
    "主菜单",
    (menuItem_t *)menu_items,                      //!< menu item list
    getArraySize(menu_items),                            //!< menu item count
    NULL,                                               //!< top menu has no parent
    0,
    top_menu_engine,                                    
};

#if 1///////////////////一级菜单/////////////////////////////////////////////////

//用户-> xxx////////////////////////////////////////////////////////////////
//1>x/////////////////////////////////////////////////////
const menuItem_t menu_1_items[] = {
    {   NULL,    (menu_t *)&menu_1_1,   "输出设置   1.1"},
    {   NULL,    (menu_t *)&menu_1_2,   "报警设置   1.2"},
    {   NULL,    (menu_t *)&menu_1_3,                  "设备标定   1.3"},
    {   NULL,    (menu_t *)&menu_1_4,                  "系统设置   1.4"},
};

// 菜单1
const menu_t menu_1 = {
     "用户菜单1",
    (menuItem_t *)menu_1_items,                      //!< menu item list
    getArraySize(menu_1_items),                            //!< menu item count
    (menu_t*)&menu,                                               //!< top menu has no parent
    1 - 1,
    top_menu_engine,                                    
};


//厂家-> xxx////////////////////////////////////////////////////////////////
//2>x/////////////////////////////////////////////////////
const menuItem_t menu_2_items[] = {
    {   NULL,    NULL,                  "锆池设定温度       2.1"},
    {   NULL,    (menu_t *)&menu_2_2,   "锆池升温设置       2.2"},
    {   NULL,    NULL,                  "锆池性能诊断       2.3"},
    {   NULL,    NULL,                  "设备信息设置       2.4"},
    {   NULL,    NULL,                  "1#电流4mA系数      2.5"},
    {   NULL,    NULL,                  "1#电流20mA系数     2.6"},
    {   NULL,    NULL,                  "2#电流4mA系数      2.7"},
    {   NULL,    NULL,                  "2#电流20mA系数     2.8"},
    {   NULL,    NULL,                  "设备零点修正       2.9"},
    {   NULL,    NULL,                  "小信号切除         2.10"},
    {   NULL,    NULL,                  "密码设置           2.11"},
    {   NULL,    NULL,                  "重置用户密码       2.12"},
    {   NULL,    NULL,                  "清楚日志           2.13"},
};

// 菜单2
const menu_t menu_2 = {
     "厂家菜单2",
    (menuItem_t *)menu_2_items,                      //!< menu item list
    getArraySize(menu_2_items),                            //!< menu item count
    (menu_t*)&menu,                                               //!< top menu has no parent
    2 - 1,
    top_menu_engine,                                    
};
///二级菜单已结束//////////////////////////////////////////////////////////////
#endif
///////二级菜单结束

#if 1///////////////////用户二级菜单/////////////////////////////////////////////////

#if 1//用户->输出设置->xxx////////////////////////////////////////////////////////////////
///// 1.1选项/////////////////////////////////////////////////////
const menuItem_t menu_1_1_items[] = 
{
    {   NULL,    (menu_t *)&menu_1_1_1,     "电流环输出      1.1.1"},
    {   NULL,    NULL,                      "继电器输出      1.1.2"},
};
//菜单
const menu_t menu_1_1 = {
     "输出设置1.1",
    (menuItem_t *)menu_1_1_items,                      //!< menu item list
    getArraySize(menu_1_1_items),                            //!< menu item count
    (menu_t*)&menu_1,                                               //!< top menu has no parent
    1 - 1,
    top_menu_engine,                                    

};
#endif

#if 1//用户-.报警设置->xxx////////////////////////////////////////////////////////////////
///// 1.2选项/////////////////////////////////////////////////////
const menuItem_t menu_1_2_items[] = 
{
    {   NULL,    NULL,   "氧含量高高报      1.2.1"},
    {   NULL,    NULL,   "氧含量搞报         1.2.2"},
    {   NULL,    NULL,   "氧含量低报         1.2.3"},
    {   NULL,    NULL,   "氧含量低低报        1.2.4"},
    
    {   NULL,    NULL,   "O2报警滞后值       1.2.5"},
    {   NULL,    NULL,   "锆池温度报警滞后值 1.2.6"},
    {   NULL,    NULL,   "报警滞后时间        1.2.7"},
    {   NULL,    NULL,   "锆池温度高         1.2.8"},
    {   NULL,    NULL,   "锆池温度低         1.2.9"},
};
//菜单1.2
const menu_t menu_1_2 = {
     "报警设置",
    (menuItem_t *)menu_1_2_items,                      //!< menu item list
    getArraySize(menu_1_2_items),                            //!< menu item count
    (menu_t*)&menu_1,                                               //!< top menu has no parent
    2 - 1,
    top_menu_engine,                                    

};
#endif



#if 1//用户->设备标定->xxx////////////////////////////////////////////////////////////////
///// 1.3选项/////////////////////////////////////////////////////
const menuItem_t menu_1_3_items[] = 
{
    {   NULL,    (menu_t *)&menu_1_3_1,   "标定参数      1.3.1"},
    {   NULL,    (menu_t *)&menu_1_3_2,   "标定模式      1.3.2"},
};
//菜单1.2
const menu_t menu_1_3 = {
     "设备标定",
    (menuItem_t *)menu_1_3_items,                      //!< menu item list
    getArraySize(menu_1_3_items),                            //!< menu item count
    (menu_t*)&menu_1,                                               //!< top menu has no parent
    3 - 1,
    top_menu_engine,                                    

};

///////////////////////////////////
#endif 

#if 1//用户->系统设置->xxx////////////////////////////////////////////////////////////////
///// 1.4选项/////////////////////////////////////////////////////

const menuItem_t menu_1_4_items[] = 
{
    {   NULL,    NULL,   "显示设置          1.4.1"},
    {   NULL,    NULL,   "系统参数设置      1.4.2"},
    {   NULL,    NULL,   "通讯设置          1.4.1"},
    {   NULL,    NULL,   "恢复出厂          1.4.2"},
};
//菜单1.2
const menu_t menu_1_4 = {
     "系统设置",
    (menuItem_t *)menu_1_4_items,                      //!< menu item list
    getArraySize(menu_1_4_items),                            //!< menu item count
    (menu_t*)&menu_1,                                               //!< top menu has no parent
    4 - 1,
    top_menu_engine,                                    

};


#endif 

///////////////////////////////////
#endif
//用户三级菜单结束

#if 1///////////////////厂家二级菜单/////////////////////////////////

#if 1//厂家->锆池升温设定->xxx////////////////////////////////////////////////////////////////
///// 2.2选项/////////////////////////////////////////////////////
const menuItem_t menu_2_2_items[] = {   
    {   NULL,    NULL,                  "升温节点1             2.2.1"},
    {   NULL,    NULL,                  "升温节点2             2.2.2"},
    {   NULL,    NULL,                  "区间1升温速率         2.2.3"},
    {   NULL,    NULL,                  "区间2升温速率         2.2.4"},
    {   NULL,    NULL,                  "区间3升温速率         2.2.5"},
};

// 菜单1.1
const menu_t menu_2_2 = {
     "锆池升温设定",
    (menuItem_t *)menu_2_2_items,                      //!< menu item list
    getArraySize(menu_2_2_items),                            //!< menu item count
    (menu_t*)&menu_2,                                               //!< top menu has no parent
    2 - 1,
    top_menu_engine,                                    
};
#endif 







#endif //end 厂家四级菜单











//厂家三级菜单结束


#if 1//三级用户菜单
////////////////////用户三级菜单//////////////////////////////////////////
#if 1//用户->输出设置->电流环输出->xxxx////////////////////////////////////////////////////////////////
///// 1.1.1选项/////////////////////////////////////////////////////
const menuItem_t menu_1_1_1_items[] = {
    {   NULL,    NULL,                      "电流环模式         1.1.1.1"},
    {   NULL,   (menu_t *)&menu_1_1_1_2,    "电流环1            1.1.1.2"},
    {   NULL,    NULL,                      "电流环2            1.1.1.3"},
    {   NULL,    NULL,                      "报警时电流输出     1.1.1.4"},
    {   NULL,    NULL,                      "报警时电流设置     1.1.1.5"},
    {   NULL,    NULL,                      "锆池加温时电流输出 1.1.1.6"},
    {   NULL,    NULL,                      "锆池加温时电流     1.1.1.7"},
    {   NULL,    NULL,                      "标定电流输出       1.1.1.8"},
    {   NULL,    NULL,                      "标定时电流         1.1.1.9"},
    {   NULL,    NULL,                      "故障时电流输出     1.1.1.10"},
    {   NULL,    NULL,                      "故障时电流         1.1.1.11"},
    {   NULL,    NULL,                      "锆池加温时电流     1.1.1.12"},
    {   NULL,    NULL,                      "恢复默认值         1.1.1.13"},
};

// 菜单1.1.1
const menu_t menu_1_1_1 = {
     "电流环输出",
    (menuItem_t *)menu_1_1_1_items,                      //!< menu item list
    getArraySize(menu_1_1_1_items),                            //!< menu item count
    (menu_t*)&menu_1_1,                                               //!< top menu has no parent
    1 - 1,
    top_menu_engine,                                    
};

#endif






#if 1//用户->输出设置->继电器设置->xxxx////////////////////////////////////////////////////////////////
///// 1.1.2选项/////////////////////////////////////////////////////
const menuItem_t menu_1_1_2_items[] = {
    {   NULL,   NULL,   "继电器1           1.1.2.1"},
    {   NULL,   NULL,   "继电器2           1.1.2.2"},
    {   NULL,   NULL,   "继电器3           1.1.2.3"},
    {   NULL,   NULL,   "继电器4           1.1.2.4"},
    {   NULL,   NULL,   "继电器1触电测试   1.1.2.5"},
    {   NULL,   NULL,   "继电器2触电测试   1.1.2.6"},
    {   NULL,   NULL,   "继电器3触电测试   1.1.2.7"},
    {   NULL,   NULL,   "继电器4触电测试   1.1.2.8"},
};

// 菜单1.1.1
const menu_t menu_1_1_2 = {
     "继电器设置",
    (menuItem_t *)menu_1_1_2_items,                      //!< menu item list
    getArraySize(menu_1_1_2_items),                            //!< menu item count
    (menu_t*)&menu_1_1,                                               //!< top menu has no parent
    2 - 1,
    top_menu_engine,                                    
};
#endif




#if 1//用户->输出设置->标定参数->xxxx////////////////////////////////////////////////////////////////

///// 1.1.2选项/////////////////////////////////////////////////////
const menuItem_t menu_1_3_1_items[] = {
    {   NULL,   NULL,   "标定零点         1.3.1.1"},
    {   NULL,   NULL,   "标定中简点       1.3.1.2"},
    {   NULL,   NULL,   "标定量程         1.3.1.3"},
    {   NULL,   NULL,   "稳定时间         1.3.1.4"},
    {   NULL,   NULL,   "自动标定参数     1.3.1.4"},

};

// 菜单1.1.1
const menu_t menu_1_3_1 = {
     "标定参数",
    (menuItem_t *)menu_1_3_1_items,                      //!< menu item list
    getArraySize(menu_1_3_1_items),                            //!< menu item count
    (menu_t*)&menu_1_3,                                               //!< top menu has no parent
    1 - 1,
    top_menu_engine,                                    
};



#endif

#if 1//用户->输出设置->标定模式->xxxx////////////////////////////////////////////////////////////////

///// 1.1.2选项/////////////////////////////////////////////////////
const menuItem_t menu_1_3_2_items[] = {
    {   NULL,   NULL,   "两点半自动标定       1.3.2.1"},
    {   NULL,   NULL,   "三点半自动标定       1.3.2.2"},
    {   NULL,   NULL,   "单点手动标定         1.3.2.3"},
    {   NULL,   NULL,   "两点手动标定         1.3.2.4"},
    {   NULL,   NULL,   "三点手动标定         1.3.2.4"},
    {   NULL,   NULL,   "自动标定模式         1.3.2.4"},
    {   NULL,   NULL,   "自动标定模式选择     1.3.2.4"},

};

// 菜单1.1.1
const menu_t menu_1_3_2 = {
     "标定模式",
    (menuItem_t *)menu_1_3_2_items,                      //!< menu item list
    getArraySize(menu_1_3_2_items),                            //!< menu item count
    (menu_t*)&menu_1_3,                                               //!< top menu has no parent
    2 - 1,
    top_menu_engine,                                    
};


#endif





#endif



#if 1//四级用户菜单

#if 1//用户->输出设置->电流环输出->电流环2设置->xxxx////////////////////////////////////////////////////////////////
////////////////// 1.1.1.2选项/////////////////////////////////////////////////////
const menuItem_t menu_1_1_1_2_items[] = { 
    {   NULL,    NULL,                  "输出基数           1.1.1.2.1"},
    {   NULL,    NULL,                  "输出零点           1.1.1.2.2"},
    {   NULL,    NULL,                  "输出量程           1.1.1.2.3"},
    {   NULL,    NULL,                  "输出阻尼           1.1.1.2.4"},
    {   NULL,    NULL,                  "输出对应关系       1.1.1.2.5"},
    {   NULL,    NULL,                  "输出模式           1.1.1.2.6"},
    {   NULL,    NULL,                  "校准信号输出       1.1.1.2.7"},
};

// 菜单1.1.2.2
const menu_t menu_1_1_1_2 = {
     "电流环1",
    (menuItem_t *)menu_1_1_1_2_items,                      //!< menu item list
    getArraySize(menu_1_1_1_2_items),                            //!< menu item count
    (menu_t*)&menu_1_1_1,                                               //!< top menu has no parent
    2 - 1,
    top_menu_engine,                                    
};
#endif

#endif

menuManager_t menu_demo = {0,&menu,0};

fsm_rt_t menu1_3_handler()
{
    gui.clear();

    gui.set_font_hz_extern(Hz_15x16_ID);
    gui.set_font_ascii_extern(Ascii_8x16_ID);
    gui.printf(0,0,"企业、person who is in 店铺等的经理,经营者,老a person who is in charge of running a business, a shop/store or a similar organization or part of one");
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
    gui.set_font_hz_extern(Hz_15x16_ID);
    gui.set_font_ascii_extern(Ascii_8x16_ID);
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
                    manager->item_index = manager->current_menu->parent_index;
                    manager->current_menu = manager->current_menu->parent;
                    manager->state = START;
                }
            }
            else if(cmd == "d")
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