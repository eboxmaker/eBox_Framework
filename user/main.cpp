/*----------------------------------------------------------------------------
 * Name:    demo.c
 * Purpose: USB HID Demo
 * Version: V1.20
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/


#include <stdio.h>

#include "ebox.h"

#include "ddc.h"
#include "ddc_user.h"
Timer timer(TIM2);
extern callback_fun_type EP_evnent[3];




uint8_t test_recv_data[5] = {0x01,0X01,0x05,0X06,0X07};

void timer_event()
{
    static unsigned char n;
    uint8_t buf[100];
   
    int len =0;
    test_recv_data[1] = n++;
//    PB9.toggle();

    
    len = ddc_make_frame(buf,test_recv_data,5,DDC_NoAck,0);
    for(int i = 0; i < len; i++)
        ddc_get_char(buf[i]);
    len = ddc_make_frame(buf,test_recv_data,5,DDC_NoAck,1);
    for(int i = 0; i < len; i++)
        ddc_get_char(buf[i]);

}




void test(uint8_t *ptr,uint16_t len)
{
    PB8.toggle();
    uart1.printf("【value = %d】\r\n",ptr[1]);
}
void test1(uint8_t *ptr,uint16_t len)
{
    PB9.toggle();
    uart1.printf("【value = %d】\r\n",ptr[1]);
}
int main (void) 
{
    ebox_init();

    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP);
//    
	timer.begin(1);
    timer.attach(timer_event);
    timer.interrupt(ENABLE);
    timer.start();
    
    uart1.begin(115200);

    //初始化通讯协议
     ddc_init();
    //绑定使用通道的回调函数
    //用户不得使用255通道，因为255通道被内部的回复帧占用。
     ddc_attach_chx(0,test);
     ddc_attach_chx(1,test1);

    while (1)                                         /* Loop forever */
	{
        ddc_test();

	}
}
