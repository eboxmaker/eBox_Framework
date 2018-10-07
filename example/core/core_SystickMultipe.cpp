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
	*	1	��������ʾ��systemtick�жϻص��ĸ���Ӧ��
    *   2   �ص�����SystickCallBackNÿ N ms����һ��
    *   3   ����1������2ÿ N*Mms ����һ�Ρ�������100ms
    *   4   ����1������2֮���� Nms�� ������Ϊ10ms
    *   5   ����1������2���к����ΪLED2��10ms����90ms
    *   6   ����3Ϊÿ N*M1 ms����һ�Σ�������Ϊ1000ms
	*/

#define N       10		//��������,systickΪ1ms,���Ե�������Ϊ10ms
#define M		10		//��������,����������ÿ����һ�δ������ִ��һ�����񣬵�������ѭ������ΪN*M,���������ļ��ΪN
#define M1		100		//����������,����ΪN*M1



/* ���������������̷������� */
#define EXAMPLE_NAME	"systemTick example"
#define EXAMPLE_DATE	"2018-10-01"

// ����1
void led2On(){
  UART.printf("led2On,���� %dms,����������%dms\r\n",N*M,N);
  LED2.set();
}
// ����2
void led2Off(){
  UART.printf("led2off,���� %dms,����������%dms\r\n",N*M,N);
  LED2.reset();
}
// ����3
void led100(){
  UART.printf("����������LED2��ת,���ڣ�%d ms \r\n",N*M1);
  LED1.toggle();
}

// �����б�
// ������systick����ö����������Ҫ���õĺ��������������ÿ��ֻ����һ����2��
// ����ָ�����飬������Ҫ���õĺ���ָ��
fun_noPara_t funTable[M] = {
  led2Off,led2On,nullFun,nullFun,nullFun,
  nullFun,nullFun,nullFun,nullFun,nullFun
};



/**
*@brief    �ص�����
*@param    NONE
*@retval   NONE
*/
void SystickCallBackN(void){
  static uint8_t i;
  // iȡֵ0-199��0��100��Ӧ100�����ڡ�0,1,2,3,4
  i = (i>=199)?0:i+1;
  // ÿ10�����ڵ���һ��������Ԥ�ȱ�������������SystickCallBack1ÿ1ms������һ�Σ�
  // ������ĺ���ÿ10ms������һ�Σ��޸�10�����޸ı������ô�����ÿ10ms�����ʱ��ֻ
  // ����һ������������ÿ���ж���ִ�еĴ�������

  funTable[i%M]();		// 10Ϊ���鳤��,���ֻ��5��������Խ�10��Ϊ5��ÿ5����������ִ��һ��

  // ���������棬ÿ100�����ڵ���һ��
  if (i%M1 == 0)
  {
    led100();
  }

}

void setup()
{
  ebox_init();
  UART.begin(115200);
  print_log(EXAMPLE_NAME,EXAMPLE_DATE);
  LED1.mode(OUTPUT_PP);
  LED2.mode(OUTPUT_PP);

  // Ĭ�����ڻص���1ms
  //SystickCallBackRegister(SystickCallBack1);
  // ָ���ص����ڣ��ص�������ϵͳĬ�����ڵ�N��
  UART.printf("��������,ÿ10ms����һ�ζ����������,����ʵ�ʵ��ü��Ϊ10N,NΪ10 or 100");
  attachSystickCallBack(SystickCallBackN,N);
}

int main(void){
  setup();
  while (1)
  {
  }
}
