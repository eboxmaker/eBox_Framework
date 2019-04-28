/**
  ******************************************************************************
  * @file    t_menu.h
  * @author  ������
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

//һ���˵�
extern const menu_t menu;//����˵�

//һ���˵�
extern const menu_t menu_1;
extern const menu_t menu_2;//���Ҳ˵�
extern const menu_t menu_3;//���Ҳ˵�

//�����˵�
extern const menu_t menu_1_1;//�û��˵�->�������
extern const menu_t menu_1_2;//�û��˵�->��������
extern const menu_t menu_1_3;//�û��˵�->�豸�궨
extern const menu_t menu_1_4;//�û��˵�->ϵͳ����


extern const menu_t menu_2_2;//���Ҳ˵�->ﯳ���������
extern const menu_t menu_2_3;//���Ҳ˵�->ﯳ��������
extern const menu_t menu_2_4;//���Ҳ˵�->�豸��Ϣ����



//�����˵�
extern const menu_t menu_1_1_1;//�û��˵�->��������->���������
extern const menu_t menu_1_1_2;//�û��˵�->��������->�̵������

extern const menu_t menu_1_3_1;
extern const menu_t menu_1_3_2;

extern const menu_t menu_1_4_1;
extern const menu_t menu_1_4_2;
extern const menu_t menu_1_4_3;
extern const menu_t menu_1_4_4;

//�ļ��˵�
extern const menu_t menu_1_1_1_2;//�û��˵�->��������->���������->������1
extern const menu_t menu_1_1_1_3;//�û��˵�->��������->�̵������->������2



extern menuManager_t menu_demo;


#endif
