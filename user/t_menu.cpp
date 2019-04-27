
#include "t_menu.h"
#include "bsp_ebox.h"
#include "..\component\menu\menuDef.h"
#include "gt30l32s4w.h"
#include "ebox_mem.h"
// 1ѡ��
const menuItem_t menu1_items[] = {
    {	NULL,               (menu_t *)&menu1_1, "�û��˵�"},
    {	NULL,               NULL,               "���Ҳ˵�"},
    {	menu1_3_handler,    NULL,               "�豸��Ϣ"},
};

// �˵�1
const menu_t menu1 = {
    "���˵�",
    (menuItem_t *)menu1_items,                      //!< menu item list
    getArraySize(menu1_items),                            //!< menu item count
    NULL,                                               //!< top menu has no parent
    top_menu_engine,                                    
};

// 1.1ѡ��
const menuItem_t menu1_1_items[] = {
    {   NULL,    NULL,                  "�������1.1"},
    {   NULL,    (menu_t *)&menu1_1_2,  "��������1.2"},
    {   NULL,    NULL,                  "�豸�궨1.3"},
    {   NULL,    NULL,                  "ϵͳ����1.4"},
};

// �˵�1.1
const menu_t menu1_1 = {
     "�û��˵�",
    (menuItem_t *)menu1_1_items,                      //!< menu item list
    getArraySize(menu1_1_items),                            //!< menu item count
    (menu_t*)&menu1,                                               //!< top menu has no parent
    top_menu_engine,                                    
};

const menuItem_t menu1_1_2_items[] = 
{
    {   NULL,    NULL,   "�������߸߱�"},
    {   NULL,    NULL,   "�������㱨"},
    {   NULL,    NULL,   "�������ͱ�"},
    {   NULL,    NULL,   "�������͵ͱ�"},
    
    {   NULL,    NULL,   "O2�����ͺ�ֵ"},
    {   NULL,    NULL,   "ﯳ��¶ȱ����ͺ�ֵ"},
    {   NULL,    NULL,   "�����ͺ�ʱ��"},
    {   NULL,    NULL,   "ﯳ��¶ȸ�"},
    {   NULL,    NULL,   "ﯳ��¶ȵ�"},
};
//�˵�
const menu_t menu1_1_2 = {
     "��������",
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
    gui.printf(0,0,"��ҵ��person who is in ���̵ȵ� ����,��Ӫ��,��a person who is in charge of running a business, a shop/store or a similar organization or part of one");
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
  gui.printf(0,0,"������mvֵ��\r\n");
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
        case READ_KEY_EVENT: 	// ��ȡ����
            cmd = UART.readString();
            if (cmd == "")
            {
                break;
            }
            else
            {
            }
        case KEY_PROCESS:			// ��������Ӧ
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
        case RUN_ITEM_HANDLER:// ִ�в˵����
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