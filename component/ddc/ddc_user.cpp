
#include "ddc.h"
#include "ddc_user.h"
#include "ebox.h"


uint8_t bufr1[100];
uint8_t bufr2[100];
uint8_t bufr3[100];

uint8_t buft1[100];
uint8_t buft2[100];
uint8_t buft3[100];

extern DdcNode_t list_send;

uint8_t data[5] = {0x01,0X01,0x05,0X06,0X07};
uint16_t len;


void ddc_test()
{

        //������֡Ҫ���͵�����֡������ӵ����������С�����ddc_loop�е��������ͺ������ͳ�ȥ
        uart1.printf("----------������Ϣ----------------------\r\n");
        len = ddc_make_frame(buft1,data,100,DDC_Ack,1);
        ddc_add_to_list(buft1);
        len = ddc_make_frame(buft2,data,50,DDC_NoAck,1);
        ddc_add_to_list(buft2);
        len = ddc_make_frame(buft3,data,5,DDC_NoAck,1);
        ddc_add_to_list(buft3);
        uart1.printf("----------------------------------------\r\n");

        
        //����ʹ�õ�����£��û�����Ҫ���ô˹��̡��˴�����ģ����շ�������buft1�Ļظ�֡
        uart1.printf("----------ģ����յ��ظ�֡----------------------\r\n");
        len = ddc_make_ack_frame(bufr1,get_frame_id(buft1));
        
        no_interrupts();
        for(int i = 0; i < len; i++)
            ddc_get_char(bufr1[i]);
        interrupts();
        uart1.printf("----------------------------------------\r\n");


        //�û�������õ�һ��ѭ������������ѭ���е��á�Ҳ�����ڲ���ϵͳ�е�����һ�������е��á�
        ddc_loop();
        
        uart1.printf("----------�����Ҫ�ظ����б�------------\r\n");
        ddc_check_ack_list();
         uart1.printf("---------------------------------------\r\n");
}

