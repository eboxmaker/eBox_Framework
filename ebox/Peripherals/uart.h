/*
file   : uartx.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#ifndef __UARTX_H
#define __UARTX_H

#include "common.h"
#include <stdio.h>
#include <stdarg.h>  


/*
	1.支持串口1,2,3,4,5；
	2.支持一个中断事件 rx_it
	3.发送模式采用DMA自动发送模式，大大节省cpu占用。发送缓存最大为UART_MAX_SEND_BUF，
	4.支持强大的printf
	5.暂时不支持引脚的remap
    注意：
        发送缓冲区溢出可能导致单片机出现异常，
        串口4，5没有使用DMA；
*/
//用户配置//////////////
#define USE_DMA 1//开启dma，只有串口1，2，3支持,4和5不支持
#define UART_MAX_SEND_BUF 128

class USART
{
    public:
        USART(USART_TypeDef *USARTx,GPIO *tx_pin,GPIO *rx_pin);

        void    begin(uint32_t baud_rate);
        void    begin(uint32_t baud_rate,uint8_t data_bit,uint8_t Parity,float stop_bit);
        void    attach_rx_interrupt(void (*callback_fun)(void));
        void    attach_tx_interrupt(void (*callback_fun)(void));

        int 	put_char(uint16_t ch);
        void 	put_string(const char *str);
        void    printf_length(const char *str,uint16_t length);
        void    printf(const char *fmt,...);//需要注意缓冲区溢出
        void    wait_busy();
    
        uint16_t    receive();

    private:
        USART_TypeDef       *_USARTx;
        DMA_Channel_TypeDef *_DMA1_Channelx;
        char                send_buf[UART_MAX_SEND_BUF];
        uint16_t            dma_send_string(const char *str,uint16_t length);
        void                put_string(const char *str,uint16_t length);
        void                set_busy();
        void                interrupt(FunctionalState enable);

};






#endif
