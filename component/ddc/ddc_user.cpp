
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

uint8_t data[5] = {0x01, 0X01, 0x05, 0X06, 0X07};
uint16_t len;


void ddc_test()
{

    //创建几帧要发送的数据帧。并添加到发送链表中。将被ddc_loop中的真正发送函数发送出去
    uart1.printf("----------创建消息----------------------\r\n");
    len = ddc_make_frame(buft1, data, 100, DDC_Ack, 1);
    ddc_add_to_list(buft1);
    len = ddc_make_frame(buft2, data, 50, DDC_NoAck, 1);
    ddc_add_to_list(buft2);
    len = ddc_make_frame(buft3, data, 5, DDC_NoAck, 1);
    ddc_add_to_list(buft3);
    uart1.printf("----------------------------------------\r\n");


    //正常使用的情况下，用户不需要调用此过程。此处就是模拟接收方发回了buft1的回复帧
    uart1.printf("----------模拟接收到回复帧----------------------\r\n");
    len = ddc_make_ack_frame(bufr1, get_frame_id(buft1));

    no_interrupts();
    for(int i = 0; i < len; i++)
        ddc_get_char(bufr1[i]);
    interrupts();
    uart1.printf("----------------------------------------\r\n");


    //用户必须调用的一个循环。可以在主循环中调用。也可以在操作系统中的任意一个任务中调用。
    ddc_loop();

    uart1.printf("----------检查需要回复的列表------------\r\n");
    ddc_check_ack_list();
    uart1.printf("---------------------------------------\r\n");
}

