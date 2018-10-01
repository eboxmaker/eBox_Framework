/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : cat_li
  * @version: V2.0
  * @date   : 2017/12/17
  * @brief   ebox application example .
  *	core API�й���systick�жϻص�Ӧ�ã���
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include "bsp_ebox.h"

/**
	*	1	��������ʾ��systemtick�жϻص���Ӧ��
    *   2   �ص�����SystickCallBackÿ1ms �� N ms ����һ��
	*/

#define N     100		//��������,systickΪ1ms,���Ե�������Ϊ10ms

/**
	*	1	ͨ�����ڴ�ӡ��Ϣ
	*/
/* ���������������̷������� */
#define EXAMPLE_NAME	"systemTick example"
#define EXAMPLE_DATE	"2018-10-01"

//    �ص���������������ȡ����systick�жϻص�ע�ắ�������systick�ж�ֻ����һ������ʹ��
void SystickCallBack(void)
{
  LED1.toggle();
}

void setup()
{
  ebox_init();
  UART.begin(115200);
  print_log(EXAMPLE_NAME,EXAMPLE_DATE);
  LED1.mode(OUTPUT_PP);

  // Ĭ�����ڻص���1ms
  attachSystickCallBack(SystickCallBack);
  // ָ���ص����ڣ��ص�������ϵͳĬ�����ڵ�N��,500ms
  UART.printf("��������,ÿ%dmsִ��һ��(led ��ת)",N);
//  attachSystickCallBack(SystickCallBack,N);
}

int main(void){
  setup();
  while (1)
  {
  }
}
