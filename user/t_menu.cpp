
#include "t_menu.h"
#include "bsp_ebox.h"
#include "..\component\menu\menuDef.h"
#include "gt30l32s4w.h"
#include "ebox_mem.h"
/////////////////////////////���㼶�˵�/////////////////////////////////////////////////
const menuItem_t menu_items[] = {
    {	NULL,               (menu_t *)&menu_1, "�û��˵�       1"},
    {	NULL,               (menu_t *)&menu_2, "���Ҳ˵�       2"},
    {	menu1_3_handler,    NULL,               "�豸��Ϣ       3"},
};

// �˵�1
const menu_t menu = {
    "���˵�",
    (menuItem_t *)menu_items,                      //!< menu item list
    getArraySize(menu_items),                            //!< menu item count
    NULL,                                               //!< top menu has no parent
    0,
    top_menu_engine,                                    
};

#if 1///////////////////һ���˵�/////////////////////////////////////////////////

//�û�-> xxx////////////////////////////////////////////////////////////////
//1>x/////////////////////////////////////////////////////
const menuItem_t menu_1_items[] = {
    {   NULL,    (menu_t *)&menu_1_1,   "�������   1.1"},
    {   NULL,    (menu_t *)&menu_1_2,   "��������   1.2"},
    {   NULL,    (menu_t *)&menu_1_3,                  "�豸�궨   1.3"},
    {   NULL,    (menu_t *)&menu_1_4,                  "ϵͳ����   1.4"},
};

// �˵�1
const menu_t menu_1 = {
     "�û��˵�1",
    (menuItem_t *)menu_1_items,                      //!< menu item list
    getArraySize(menu_1_items),                            //!< menu item count
    (menu_t*)&menu,                                               //!< top menu has no parent
    1 - 1,
    top_menu_engine,                                    
};


//����-> xxx////////////////////////////////////////////////////////////////
//2>x/////////////////////////////////////////////////////
const menuItem_t menu_2_items[] = {
    {   NULL,    NULL,                  "ﯳ��趨�¶�       2.1"},
    {   NULL,    (menu_t *)&menu_2_2,   "ﯳ���������       2.2"},
    {   NULL,    NULL,                  "ﯳ��������       2.3"},
    {   NULL,    NULL,                  "�豸��Ϣ����       2.4"},
    {   NULL,    NULL,                  "1#����4mAϵ��      2.5"},
    {   NULL,    NULL,                  "1#����20mAϵ��     2.6"},
    {   NULL,    NULL,                  "2#����4mAϵ��      2.7"},
    {   NULL,    NULL,                  "2#����20mAϵ��     2.8"},
    {   NULL,    NULL,                  "�豸�������       2.9"},
    {   NULL,    NULL,                  "С�ź��г�         2.10"},
    {   NULL,    NULL,                  "��������           2.11"},
    {   NULL,    NULL,                  "�����û�����       2.12"},
    {   NULL,    NULL,                  "�����־           2.13"},
};

// �˵�2
const menu_t menu_2 = {
     "���Ҳ˵�2",
    (menuItem_t *)menu_2_items,                      //!< menu item list
    getArraySize(menu_2_items),                            //!< menu item count
    (menu_t*)&menu,                                               //!< top menu has no parent
    2 - 1,
    top_menu_engine,                                    
};
///�����˵��ѽ���//////////////////////////////////////////////////////////////
#endif
///////�����˵�����

#if 1///////////////////�û������˵�/////////////////////////////////////////////////

#if 1//�û�->�������->xxx////////////////////////////////////////////////////////////////
///// 1.1ѡ��/////////////////////////////////////////////////////
const menuItem_t menu_1_1_items[] = 
{
    {   NULL,    (menu_t *)&menu_1_1_1,     "���������      1.1.1"},
    {   NULL,    NULL,                      "�̵������      1.1.2"},
};
//�˵�
const menu_t menu_1_1 = {
     "�������1.1",
    (menuItem_t *)menu_1_1_items,                      //!< menu item list
    getArraySize(menu_1_1_items),                            //!< menu item count
    (menu_t*)&menu_1,                                               //!< top menu has no parent
    1 - 1,
    top_menu_engine,                                    

};
#endif

#if 1//�û�-.��������->xxx////////////////////////////////////////////////////////////////
///// 1.2ѡ��/////////////////////////////////////////////////////
const menuItem_t menu_1_2_items[] = 
{
    {   NULL,    NULL,   "�������߸߱�      1.2.1"},
    {   NULL,    NULL,   "�������㱨         1.2.2"},
    {   NULL,    NULL,   "�������ͱ�         1.2.3"},
    {   NULL,    NULL,   "�������͵ͱ�        1.2.4"},
    
    {   NULL,    NULL,   "O2�����ͺ�ֵ       1.2.5"},
    {   NULL,    NULL,   "ﯳ��¶ȱ����ͺ�ֵ 1.2.6"},
    {   NULL,    NULL,   "�����ͺ�ʱ��        1.2.7"},
    {   NULL,    NULL,   "ﯳ��¶ȸ�         1.2.8"},
    {   NULL,    NULL,   "ﯳ��¶ȵ�         1.2.9"},
};
//�˵�1.2
const menu_t menu_1_2 = {
     "��������",
    (menuItem_t *)menu_1_2_items,                      //!< menu item list
    getArraySize(menu_1_2_items),                            //!< menu item count
    (menu_t*)&menu_1,                                               //!< top menu has no parent
    2 - 1,
    top_menu_engine,                                    

};
#endif



#if 1//�û�->�豸�궨->xxx////////////////////////////////////////////////////////////////
///// 1.3ѡ��/////////////////////////////////////////////////////
const menuItem_t menu_1_3_items[] = 
{
    {   NULL,    (menu_t *)&menu_1_3_1,   "�궨����      1.3.1"},
    {   NULL,    (menu_t *)&menu_1_3_2,   "�궨ģʽ      1.3.2"},
};
//�˵�1.2
const menu_t menu_1_3 = {
     "�豸�궨",
    (menuItem_t *)menu_1_3_items,                      //!< menu item list
    getArraySize(menu_1_3_items),                            //!< menu item count
    (menu_t*)&menu_1,                                               //!< top menu has no parent
    3 - 1,
    top_menu_engine,                                    

};

///////////////////////////////////
#endif 

#if 1//�û�->ϵͳ����->xxx////////////////////////////////////////////////////////////////
///// 1.4ѡ��/////////////////////////////////////////////////////

const menuItem_t menu_1_4_items[] = 
{
    {   NULL,    NULL,   "��ʾ����          1.4.1"},
    {   NULL,    NULL,   "ϵͳ��������      1.4.2"},
    {   NULL,    NULL,   "ͨѶ����          1.4.1"},
    {   NULL,    NULL,   "�ָ�����          1.4.2"},
};
//�˵�1.2
const menu_t menu_1_4 = {
     "ϵͳ����",
    (menuItem_t *)menu_1_4_items,                      //!< menu item list
    getArraySize(menu_1_4_items),                            //!< menu item count
    (menu_t*)&menu_1,                                               //!< top menu has no parent
    4 - 1,
    top_menu_engine,                                    

};


#endif 

///////////////////////////////////
#endif
//�û������˵�����

#if 1///////////////////���Ҷ����˵�/////////////////////////////////

#if 1//����->ﯳ������趨->xxx////////////////////////////////////////////////////////////////
///// 2.2ѡ��/////////////////////////////////////////////////////
const menuItem_t menu_2_2_items[] = {   
    {   NULL,    NULL,                  "���½ڵ�1             2.2.1"},
    {   NULL,    NULL,                  "���½ڵ�2             2.2.2"},
    {   NULL,    NULL,                  "����1��������         2.2.3"},
    {   NULL,    NULL,                  "����2��������         2.2.4"},
    {   NULL,    NULL,                  "����3��������         2.2.5"},
};

// �˵�1.1
const menu_t menu_2_2 = {
     "ﯳ������趨",
    (menuItem_t *)menu_2_2_items,                      //!< menu item list
    getArraySize(menu_2_2_items),                            //!< menu item count
    (menu_t*)&menu_2,                                               //!< top menu has no parent
    2 - 1,
    top_menu_engine,                                    
};
#endif 







#endif //end �����ļ��˵�











//���������˵�����


#if 1//�����û��˵�
////////////////////�û������˵�//////////////////////////////////////////
#if 1//�û�->�������->���������->xxxx////////////////////////////////////////////////////////////////
///// 1.1.1ѡ��/////////////////////////////////////////////////////
const menuItem_t menu_1_1_1_items[] = {
    {   NULL,    NULL,                      "������ģʽ         1.1.1.1"},
    {   NULL,   (menu_t *)&menu_1_1_1_2,    "������1            1.1.1.2"},
    {   NULL,    NULL,                      "������2            1.1.1.3"},
    {   NULL,    NULL,                      "����ʱ�������     1.1.1.4"},
    {   NULL,    NULL,                      "����ʱ��������     1.1.1.5"},
    {   NULL,    NULL,                      "ﯳؼ���ʱ������� 1.1.1.6"},
    {   NULL,    NULL,                      "ﯳؼ���ʱ����     1.1.1.7"},
    {   NULL,    NULL,                      "�궨�������       1.1.1.8"},
    {   NULL,    NULL,                      "�궨ʱ����         1.1.1.9"},
    {   NULL,    NULL,                      "����ʱ�������     1.1.1.10"},
    {   NULL,    NULL,                      "����ʱ����         1.1.1.11"},
    {   NULL,    NULL,                      "ﯳؼ���ʱ����     1.1.1.12"},
    {   NULL,    NULL,                      "�ָ�Ĭ��ֵ         1.1.1.13"},
};

// �˵�1.1.1
const menu_t menu_1_1_1 = {
     "���������",
    (menuItem_t *)menu_1_1_1_items,                      //!< menu item list
    getArraySize(menu_1_1_1_items),                            //!< menu item count
    (menu_t*)&menu_1_1,                                               //!< top menu has no parent
    1 - 1,
    top_menu_engine,                                    
};

#endif






#if 1//�û�->�������->�̵�������->xxxx////////////////////////////////////////////////////////////////
///// 1.1.2ѡ��/////////////////////////////////////////////////////
const menuItem_t menu_1_1_2_items[] = {
    {   NULL,   NULL,   "�̵���1           1.1.2.1"},
    {   NULL,   NULL,   "�̵���2           1.1.2.2"},
    {   NULL,   NULL,   "�̵���3           1.1.2.3"},
    {   NULL,   NULL,   "�̵���4           1.1.2.4"},
    {   NULL,   NULL,   "�̵���1�������   1.1.2.5"},
    {   NULL,   NULL,   "�̵���2�������   1.1.2.6"},
    {   NULL,   NULL,   "�̵���3�������   1.1.2.7"},
    {   NULL,   NULL,   "�̵���4�������   1.1.2.8"},
};

// �˵�1.1.1
const menu_t menu_1_1_2 = {
     "�̵�������",
    (menuItem_t *)menu_1_1_2_items,                      //!< menu item list
    getArraySize(menu_1_1_2_items),                            //!< menu item count
    (menu_t*)&menu_1_1,                                               //!< top menu has no parent
    2 - 1,
    top_menu_engine,                                    
};
#endif




#if 1//�û�->�������->�궨����->xxxx////////////////////////////////////////////////////////////////

///// 1.1.2ѡ��/////////////////////////////////////////////////////
const menuItem_t menu_1_3_1_items[] = {
    {   NULL,   NULL,   "�궨���         1.3.1.1"},
    {   NULL,   NULL,   "�궨�м��       1.3.1.2"},
    {   NULL,   NULL,   "�궨����         1.3.1.3"},
    {   NULL,   NULL,   "�ȶ�ʱ��         1.3.1.4"},
    {   NULL,   NULL,   "�Զ��궨����     1.3.1.4"},

};

// �˵�1.1.1
const menu_t menu_1_3_1 = {
     "�궨����",
    (menuItem_t *)menu_1_3_1_items,                      //!< menu item list
    getArraySize(menu_1_3_1_items),                            //!< menu item count
    (menu_t*)&menu_1_3,                                               //!< top menu has no parent
    1 - 1,
    top_menu_engine,                                    
};



#endif

#if 1//�û�->�������->�궨ģʽ->xxxx////////////////////////////////////////////////////////////////

///// 1.1.2ѡ��/////////////////////////////////////////////////////
const menuItem_t menu_1_3_2_items[] = {
    {   NULL,   NULL,   "������Զ��궨       1.3.2.1"},
    {   NULL,   NULL,   "������Զ��궨       1.3.2.2"},
    {   NULL,   NULL,   "�����ֶ��궨         1.3.2.3"},
    {   NULL,   NULL,   "�����ֶ��궨         1.3.2.4"},
    {   NULL,   NULL,   "�����ֶ��궨         1.3.2.4"},
    {   NULL,   NULL,   "�Զ��궨ģʽ         1.3.2.4"},
    {   NULL,   NULL,   "�Զ��궨ģʽѡ��     1.3.2.4"},

};

// �˵�1.1.1
const menu_t menu_1_3_2 = {
     "�궨ģʽ",
    (menuItem_t *)menu_1_3_2_items,                      //!< menu item list
    getArraySize(menu_1_3_2_items),                            //!< menu item count
    (menu_t*)&menu_1_3,                                               //!< top menu has no parent
    2 - 1,
    top_menu_engine,                                    
};


#endif





#endif



#if 1//�ļ��û��˵�

#if 1//�û�->�������->���������->������2����->xxxx////////////////////////////////////////////////////////////////
////////////////// 1.1.1.2ѡ��/////////////////////////////////////////////////////
const menuItem_t menu_1_1_1_2_items[] = { 
    {   NULL,    NULL,                  "�������           1.1.1.2.1"},
    {   NULL,    NULL,                  "������           1.1.1.2.2"},
    {   NULL,    NULL,                  "�������           1.1.1.2.3"},
    {   NULL,    NULL,                  "�������           1.1.1.2.4"},
    {   NULL,    NULL,                  "�����Ӧ��ϵ       1.1.1.2.5"},
    {   NULL,    NULL,                  "���ģʽ           1.1.1.2.6"},
    {   NULL,    NULL,                  "У׼�ź����       1.1.1.2.7"},
};

// �˵�1.1.2.2
const menu_t menu_1_1_1_2 = {
     "������1",
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
    gui.printf(0,0,"��ҵ��person who is in ���̵ȵľ���,��Ӫ��,��a person who is in charge of running a business, a shop/store or a similar organization or part of one");
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