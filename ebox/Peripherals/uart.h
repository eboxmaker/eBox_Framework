/*
file   : uartx.h
author : shentq
version: V1.1
date   : 2016/4/6

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
	1.֧�ִ���1,2,3,4,5��
	2.֧��һ���ж��¼� rx_it
	3.����ģʽ����DMA�Զ�����ģʽ������ʡcpuռ�á����ͻ������ΪUART_MAX_SEND_BUF��
	4.֧��ǿ���printf
	5.��ʱ��֧�����ŵ�remap
    ע�⣺
        ���ͻ�����������ܵ��µ�Ƭ�������쳣��
        ����4��5û��ʹ��DMA��
        ���ڷ��ͣ�������no_interrupt������ִ�����Ρ�
*/

/**
 *@name     void    begin(uint32_t baud_rate,uint8_t data_bit,uint8_t parity,float stop_bit);
 *@brief    ���ڳ�ʼ�������������и������ò���
 *@param    baud_rate:  �����ʣ�����9600��115200��38400�ȵ�
 *          data_bit:   ����λ����ֻ������8����9
 *          parity:     ����λ��0����У��λ��1��У�飬2żУ��
 *          stop_bit:   ֹͣλ��0.5,1,1.5,2�ĸ���ѡ����
 *@retval   None
*/
//�û�����//////////////
#define USE_DMA 1//����dma��ֻ�д���1��2��3֧��,4��5��֧��
#define UART_MAX_SEND_BUF 128

class USART
{
public:
    USART(USART_TypeDef *USARTx, GPIO *tx_pin, GPIO *rx_pin);

    void    begin(uint32_t baud_rate);
    void    begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit);
    void    attach_rx_interrupt(void (*callback_fun)(void));
    void    attach_tx_interrupt(void (*callback_fun)(void));

    int 	put_char(uint16_t ch);
    void 	put_string(const char *str);
    void    printf_length(const char *str, uint16_t length);
    void    printf(const char *fmt, ...); //��Ҫע�⻺�������
    void    wait_busy();

    uint16_t    receive();

private:
    USART_TypeDef       *_USARTx;
    DMA_Channel_TypeDef *_DMA1_Channelx;
    char                send_buf[UART_MAX_SEND_BUF];
    uint16_t            dma_send_string(const char *str, uint16_t length);
    void                put_string(const char *str, uint16_t length);
    void                set_busy();
    void                interrupt(FunctionalState enable);
};






#endif
