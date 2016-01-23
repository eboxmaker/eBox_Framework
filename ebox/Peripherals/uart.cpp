/*
file   : uart.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "uart.h"




callback_fun_type usart_callback_table[5];//֧�ִ��ڵ�rx�ж�


USART::USART(USART_TypeDef *USARTx,GPIO *tx_pin,GPIO *rx_pin)
{
	_USARTx = USARTx;
	tx_pin->mode(AF_PP);
	rx_pin->mode(INPUT);
}


void USART::begin(uint32_t baud_rate)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    #if (USE_DMA == 1)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMAʱ��
    #endif

    switch((uint32_t)_USARTx)
    {
        case (uint32_t)USART1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
            _DMA1_Channelx = DMA1_Channel4;
        
            NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
            break;

        case (uint32_t)USART2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
            _DMA1_Channelx = DMA1_Channel7;
        
            NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
            break;

        case (uint32_t)USART3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
            _DMA1_Channelx = DMA1_Channel2;
        
            NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
            break;
        #if defined (STM32F10X_HD)
        case (uint32_t)UART4:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
        
            NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
            break;
        case (uint32_t)UART5:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
        
            NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
            break;
        #endif
    }

    USART_InitStructure.USART_BaudRate = baud_rate;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(_USARTx, &USART_InitStructure);
            
    if(_USARTx == USART1 | _USARTx == USART2 | _USARTx == USART3 )
        USART_DMACmd(_USARTx,USART_DMAReq_Tx,ENABLE);    
    USART_Cmd(_USARTx, ENABLE);
}
void USART::begin(uint32_t baud_rate,uint8_t data_bit,uint8_t parity,float stop_bit)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    if (USE_DMA == 1)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMAʱ��

    switch((uint32_t)_USARTx)
    {
        case (uint32_t)USART1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
            _DMA1_Channelx = DMA1_Channel4;
        
            NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
            break;

        case (uint32_t)USART2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
            _DMA1_Channelx = DMA1_Channel7;
        
            NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
            break;

        case (uint32_t)USART3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
            _DMA1_Channelx = DMA1_Channel2;
        
            NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
            break;
        #if defined (STM32F10X_HD)
        case (uint32_t)UART4:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
        
            NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
            break;
        case (uint32_t)UART5:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
        
            NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
            break;
        #endif
    }

    USART_InitStructure.USART_BaudRate = baud_rate;
    switch(data_bit)
    {
        case 8:
            USART_InitStructure.USART_WordLength = USART_WordLength_8b;break;
        case 9:
            USART_InitStructure.USART_WordLength = USART_WordLength_9b;break;
        default :
            USART_InitStructure.USART_WordLength = USART_WordLength_8b;break;                
    }
    switch(parity)
    {
        case 0:
            USART_InitStructure.USART_Parity = USART_Parity_No;break;
        case 1:
            USART_InitStructure.USART_Parity = USART_Parity_Even;break;
        case 2:
            USART_InitStructure.USART_Parity = USART_Parity_Odd;break;
        default :
            USART_InitStructure.USART_Parity = USART_Parity_No;break;
    }
    if(parity == 0.5)
        USART_InitStructure.USART_StopBits = USART_StopBits_0_5;
    else if(parity == 1)
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
    else if(parity == 1.5)
        USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
    else if(parity == 2)
        USART_InitStructure.USART_StopBits = USART_StopBits_2;
   
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(_USARTx, &USART_InitStructure);
            
    if(_USARTx == USART1 | _USARTx == USART2 | _USARTx == USART3 )
        USART_DMACmd(_USARTx,USART_DMAReq_Tx,ENABLE);    
    USART_Cmd(_USARTx, ENABLE);
}

void USART::interrupt(FunctionalState enable)
{
	USART_ITConfig(_USARTx,USART_IT_RXNE,enable);
}
void USART::attach_interrupt(void (*callback_fun)(void))
{
	switch((uint32_t)_USARTx)
	{
		case (uint32_t)USART1:
			usart_callback_table[0] = callback_fun;break;
		case (uint32_t)USART2:
			usart_callback_table[1] = callback_fun;break;
		case (uint32_t)USART3:
			usart_callback_table[2] = callback_fun;break;
		case (uint32_t)UART4:
			usart_callback_table[3] = callback_fun;break;
		case (uint32_t)UART5:
			usart_callback_table[4] = callback_fun;break;
	}
}
uint16_t USART::receive()
{
	return (uint16_t)(_USARTx->DR & (uint16_t)0x01FF);
}
uint16_t USART::dma_send_string(const char *str,uint16_t length)
{
    wait_busy();
    DMA_DeInit(_DMA1_Channelx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    _DMA1_Channelx->CPAR = (u32)&_USARTx->DR; //�����ַ
    _DMA1_Channelx->CMAR = (u32) str; //mem��ַ
    _DMA1_Channelx->CNDTR = length ; //���䳤��
    _DMA1_Channelx->CCR = (0 << 14) | // �Ǵ洢�����洢��ģʽ
            (2 << 12) | // ͨ�����ȼ���
            (0 << 11) | // �洢�����ݿ��8bit
            (0 << 10) | // �洢�����ݿ��8bit
            (0 <<  9) | // �������ݿ��8bit
            (0 <<  8) | // �������ݿ��8bit
            (1 <<  7) | // �洢����ַ����ģʽ
            (0 <<  6) | // �����ַ����ģʽ(����)
            (0 <<  5) | // ��ѭ��ģʽ
            (1 <<  4) | // �Ӵ洢����
            (1 <<  3) | // �Ƿ�����������ж�
            (0 <<  2) | // �Ƿ�����봫���ж�
            (0 <<  1) | // �Ƿ�����������ж�
            (1);        // ͨ������
    return length;
}
int USART::put_char(uint16_t ch)
{
	USART_SendData(_USARTx,ch);
	wait_busy();
	return ch;
}
void USART::put_string(const char *str,uint16_t length)
{
    if((_USARTx == USART1 | _USARTx == USART2 | _USARTx == USART3 ) && (USE_DMA == 1))
    {
        dma_send_string(str,length);
    }
    else
    {
        while(length--)
        {
            USART_SendData(_USARTx,*str++);
            wait_busy();
        }
    }
}
void USART::put_string(const char *str)
{
    uint16_t i = 0;
    uint16_t length = 0;

    while(str[i++] != '\0')
    {
        length++;
    };
    put_string(str,length);
        
}	
	
void USART::printf_length(const char *str,uint16_t length)
{	
		put_string(str,length);
}
void USART::printf(const char *fmt,...)
{

	wait_busy();
	va_list va_params;   
	va_start(va_params,fmt);   
	vsprintf(send_buf,fmt,va_params);
	va_end(va_params); 

	put_string(send_buf);
	
}

void USART::wait_busy()
{
	while(USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
}


extern "C"{

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		usart_callback_table[0]();
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		usart_callback_table[1]();
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		usart_callback_table[2]();
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
}
void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
	{
		usart_callback_table[3]();
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);
	}
}
void UART5_IRQHandler(void)
{
	if(USART_GetITStatus(UART5,USART_IT_RXNE)==SET)
	{
		usart_callback_table[4]();
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
	}
}

//void DMA1_Channel4_IRQHandler(void)  
//	{  

//		DMA_Cmd(DMA1_Channel4,DISABLE);  
//		DMA_ClearFlag(DMA1_FLAG_TC4); 

//	}	 
//	void DMA1_Channel7_IRQHandler(void)  
//	{  
//		DMA_Cmd(DMA1_Channel7,DISABLE);  
//		DMA_ClearFlag(DMA1_FLAG_TC7); 
//	}	 
//	void DMA1_Channel2_IRQHandler(void)  
//	{  
//		DMA_Cmd(DMA1_Channel2,DISABLE);  
//		DMA_ClearFlag(DMA1_FLAG_TC2); 
//	}
}
