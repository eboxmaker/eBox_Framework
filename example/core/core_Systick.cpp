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

// �������ֵ��÷�ʽ
// use1 = 1, �жϻص�ֻ��������
// use1 = 0, �жϻص���������񣬵�ÿ�ν����ж�ֻ����һ������
#define use1	1

#define N     100		//��������,systickΪ1ms,���Ե�������Ϊ10ms
#define M			10		//��������,����������ÿ����һ�δ������ִ��һ�����񣬵�������ѭ������ΪN*M,���������ļ��ΪN
#define M1		100		//����������,����ΪN*M1



/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F0 core_systick example"
#define EXAMPLE_DATE	"2017-12-31"

#if		 use1  		//�жϻص�ֻ��������

//    �ص���������������ȡ����systick�жϻص�ע�ắ�������systick�ж�ֻ����һ������ʹ��
void SystickCallBack(void)
{
    LED1.toggle();
}

#else 					//�жϻص���������񣬵�ÿ�ν����ж�ֻ����һ������

// ����1
void led2On(){
    LED1.set();
}
// ����2
void led2Off(){
    LED1.reset();
}

void led100(){
		UART.printf("����������LED1��ת,���ڣ�%d ms \r\n",N*M1);
		LED1.toggle();
}

// �����б�
// ������systick����ö����������Ҫ���õĺ��������������ÿ��ֻ����һ����2��
// ����ָ�����飬������Ҫ���õĺ���ָ��
fun_noPara_t funTable[M] = {
    led2Off,nullFun,nullFun,nullFun,nullFun,
    led2On,nullFun,nullFun,nullFun,nullFun
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
		UART.printf("������,���� %dms,����������%dms\r\n",N*M,N);
	  funTable[i%M]();		// 10Ϊ���鳤��,���ֻ��5��������Խ�10��Ϊ5��ÿ5����������ִ��һ��
//    switch (i%10)
//    {
//    case 0:
//        funTable[0]();		// �ú���ÿ10x����һ��
//        break;
//    case 1:
//        funTable[1]();		// �ú���ÿ10x����һ�Σ��������ں���֮����1x
//        break;
//    case 2:
//        funTable[2]();
//        break;
//    case 3:
//        funTable[3]();
//        break;
//		case 4:
//				funTable[4]();
//				break;
//    default:
//        break;
//    }

    // ���������棬ÿ100�����ڵ���һ��
		if(i%M1 == 0)
		{
			led100();
		}

}
#endif



void setup()
{
    ebox_init();
		UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
        LED1.mode(OUTPUT_PP);
		LED1.mode(OUTPUT_PP);

	#if	use1
	  // Ĭ�����ڻص���1ms
    //SystickCallBackRegister(SystickCallBack);
    // ָ���ص����ڣ��ص�������ϵͳĬ�����ڵ�N��,500ms
		UART.printf("��������,ÿ500msִ��һ��(led ��ת)");
    attachSystickCallBack(SystickCallBack,500);
	#else
    // Ĭ�����ڻص���1ms
    //SystickCallBackRegister(SystickCallBack1);
    // ָ���ص����ڣ��ص�������ϵͳĬ�����ڵ�N��
		UART.printf("��������,ÿ10ms����һ�ζ����������,����ʵ�ʵ��ü��Ϊ10N,NΪ10 or 100");
        SystickCallBackMultipleReg(SystickCallBackN,N);
		#endif
}

int main(void){
    setup();
    while (1)
    {
    }
}
