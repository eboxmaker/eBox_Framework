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




callback_fun_type usart_callback_table[5][2];//支持串口的rx中断

uint8_t busy[5];

USART::USART(USART_TypeDef *USARTx,GPIO *tx_pin,GPIO *rx_pin)
{
	_USARTx = USARTx;
	tx_pin->mode(AF_PP);
	rx_pin->mode(INPUT);
}


void USART::begin(uint32_t baud_rate)
{
    USART_InitTypeDef USART_InitStructure;

    if(USE_DMA == 1)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA时钟

    switch((uint32_t)_USARTx)
    {
        case (uint32_t)USART1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
            _DMA1_Channelx = DMA1_Channel4;
            break;

        case (uint32_t)USART2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
            _DMA1_Channelx = DMA1_Channel7;
            break;

        case (uint32_t)USART3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
            _DMA1_Channelx = DMA1_Channel2;
            break;
        
        #if defined (STM32F10X_HD)
        case (uint32_t)UART4:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
            break;
        
        case (uint32_t)UART5:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
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
            
    if((_USARTx == USART1 | _USARTx == USART2 | _USARTx == USART3) && (USE_DMA == 1) )
        USART_DMACmd(_USARTx,USART_DMAReq_Tx,ENABLE);    
    USART_Cmd(_USARTx, ENABLE);
    interrupt(ENABLE);
}
void USART::begin(uint32_t baud_rate,uint8_t data_bit,uint8_t parity,float stop_bit)
{
    USART_InitTypeDef USART_InitStructure;
    if (USE_DMA == 1)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA时钟

    switch((uint32_t)_USARTx)
    {
        case (uint32_t)USART1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
            _DMA1_Channelx = DMA1_Channel4;
            break;

        case (uint32_t)USART2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
            _DMA1_Channelx = DMA1_Channel7;
            break;

        case (uint32_t)USART3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
            _DMA1_Channelx = DMA1_Channel2;
            break;
        
        #if defined (STM32F10X_HD)
        case (uint32_t)UART4:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
            break;
        
        case (uint32_t)UART5:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
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
            
    if((_USARTx == USART1 | _USARTx == USART2 | _USARTx == USART3) && (USE_DMA == 1) )
        USART_DMACmd(_USARTx,USART_DMAReq_Tx,ENABLE);    
    USART_Cmd(_USARTx, ENABLE);
}

void USART::interrupt(FunctionalState enable)
{
    NVIC_InitTypeDef NVIC_InitStructure;

	USART_ITConfig(_USARTx,USART_IT_RXNE,enable);
	USART_ITConfig(_USARTx,USART_IT_TC,enable);
	USART_ClearITPendingBit(USART1,USART_IT_TC);
    
    switch((uint32_t)_USARTx)
    {
        case (uint32_t)USART1:        
            NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            break;

        case (uint32_t)USART2:
            NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            break;

        case (uint32_t)USART3:
            NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            break;
        #if defined (STM32F10X_HD)
        case (uint32_t)UART4:        
            NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            break;
        case (uint32_t)UART5:
            NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            break;
        #endif
    }
    NVIC_Init(&NVIC_InitStructure);
}
void USART::attach_rx_interrupt(void (*callback_fun)(void))
{
	switch((uint32_t)_USARTx)
	{
		case (uint32_t)USART1:
			usart_callback_table[0][0] = callback_fun;break;
		case (uint32_t)USART2:
			usart_callback_table[1][0] = callback_fun;break;
		case (uint32_t)USART3:
			usart_callback_table[2][0] = callback_fun;break;
		case (uint32_t)UART4:
			usart_callback_table[3][0] = callback_fun;break;
		case (uint32_t)UART5:
			usart_callback_table[4][0] = callback_fun;break;
	}
}
void USART::attach_tx_interrupt(void (*callback_fun)(void))
{
	switch((uint32_t)_USARTx)
	{
		case (uint32_t)USART1:
			usart_callback_table[0][1] = callback_fun;break;
		case (uint32_t)USART2:
			usart_callback_table[1][1] = callback_fun;break;
		case (uint32_t)USART3:
			usart_callback_table[2][1] = callback_fun;break;
		case (uint32_t)UART4:
			usart_callback_table[3][1] = callback_fun;break;
		case (uint32_t)UART5:
			usart_callback_table[4][1] = callback_fun;break;
	}
}
uint16_t USART::receive()
{
	return (uint16_t)(_USARTx->DR & (uint16_t)0x01FF);
}
uint16_t USART::dma_send_string(const char *str,uint16_t length)
{
    DMA_DeInit(_DMA1_Channelx);   //将DMA的通道1寄存器重设为缺省值
    _DMA1_Channelx->CPAR = (u32)&_USARTx->DR; //外设地址
    _DMA1_Channelx->CMAR = (u32) str; //mem地址
    _DMA1_Channelx->CNDTR = length ; //传输长度
    _DMA1_Channelx->CCR = (0 << 14) | // 非存储器到存储器模式
            (2 << 12) | // 通道优先级高
            (0 << 11) | // 存储器数据宽度8bit
            (0 << 10) | // 存储器数据宽度8bit
            (0 <<  9) | // 外设数据宽度8bit
            (0 <<  8) | // 外设数据宽度8bit
            (1 <<  7) | // 存储器地址增量模式
            (0 <<  6) | // 外设地址增量模式(不增)
            (0 <<  5) | // 非循环模式
            (1 <<  4) | // 从存储器读
            (1 <<  3) | // 是否允许传输错误中断
            (0 <<  2) | // 是否允许半传输中断
            (0 <<  1) | // 是否允许传输完成中断
            (1);        // 通道开启
    return length;
}
int USART::put_char(uint16_t ch)
{
	while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//单字节等待，等待寄存器空
	USART_SendData(_USARTx,ch);
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
           while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//单字节等待，等待寄存器空
           USART_SendData(_USARTx,*str++);
        }
    }
}
void USART::put_string(const char *str)
{
    uint16_t i = 0;
    uint16_t length = 0;
    
	wait_busy();
    set_busy();
    while(str[i++] != '\0')
    {
        length++;
    };
    put_string(str,length);
        
}	
	
void USART::printf_length(const char *str,uint16_t length)
{	
	wait_busy();
    set_busy();
    put_string(str,length);
}
void USART::printf(const char *fmt,...)
{
    uint16_t i = 0;
    uint16_t length = 0;
    
	wait_busy();
    set_busy();
	va_list va_params;   
	va_start(va_params,fmt);   
	vsprintf(send_buf,fmt,va_params);
	va_end(va_params); 

    while(send_buf[i++] != '\0')
    {
        length++;
    };
    if(length > UART_MAX_SEND_BUF) length = UART_MAX_SEND_BUF;
    put_string(send_buf,length);	
}

void USART::wait_busy()
{
    switch((uint32_t)_USARTx)
    {
        case (uint32_t)USART1:
            while(busy[0] == 1);
            break;
        case (uint32_t)USART2:
            while(busy[1] == 1);
            break;
        case (uint32_t)USART3:
            while(busy[2] == 1);
            break;
        case (uint32_t)UART4:
            while(busy[3] == 1);
            break;
        case (uint32_t)UART5:
            while(busy[4] == 1);
            break;
    }
}
void USART::set_busy()
{
    switch((uint32_t)_USARTx)
    {
        case (uint32_t)USART1:
            busy[0] = 1;
            break;
        case (uint32_t)USART2:
            busy[1] = 1;
            break;
        case (uint32_t)USART3:
            busy[2] = 1;
            break;
        case (uint32_t)UART4:
            busy[3] = 1;
            break;
        case (uint32_t)UART5:
            busy[4] = 1;
            break;    
    }
}


extern "C"{

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
        if(usart_callback_table[0][0] != 0)
            usart_callback_table[0][0]();
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
    if(USART_GetITStatus(USART1,USART_IT_TC)==SET)
	{
        busy[0] = 0;
        if(usart_callback_table[0][1] != 0)
            usart_callback_table[0][1]();		
		USART_ClearITPendingBit(USART1,USART_IT_TC);
	}
}
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
        if(usart_callback_table[1][0] != 0)
            usart_callback_table[1][0]();
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
    if(USART_GetITStatus(USART2,USART_IT_TC)==SET)
	{
        busy[1] = 0;
        if(usart_callback_table[1][1] != 0)
            usart_callback_table[1][1]();		
		USART_ClearITPendingBit(USART2,USART_IT_TC);
	}
}
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
        if(usart_callback_table[2][0] != 0)
            usart_callback_table[2][0]();
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
    if(USART_GetITStatus(USART3,USART_IT_TC)==SET)
	{
        busy[2] = 0;
        if(usart_callback_table[2][1] != 0)
            usart_callback_table[2][1]();		
		USART_ClearITPendingBit(USART3,USART_IT_TC);
	}
}
void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
	{
        if(usart_callback_table[3][0] != 0)
            usart_callback_table[3][0]();
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);
	}
    if(USART_GetITStatus(UART4,USART_IT_TC)==SET)
	{
        busy[3] = 0;
        if(usart_callback_table[3][1] != 0)
            usart_callback_table[3][1]();		
		USART_ClearITPendingBit(UART4,USART_IT_TC);
	}
}
void UART5_IRQHandler(void)
{
	if(USART_GetITStatus(UART5,USART_IT_RXNE)==SET)
	{
        if(usart_callback_table[4][0] != 0)
            usart_callback_table[4][0]();
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
	}
    if(USART_GetITStatus(UART5,USART_IT_TC)==SET)
	{
        busy[4] = 0;
        if(usart_callback_table[4][1] != 0)
            usart_callback_table[4][1]();		
		USART_ClearITPendingBit(UART5,USART_IT_TC);
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
