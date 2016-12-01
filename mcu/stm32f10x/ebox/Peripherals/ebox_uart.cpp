/**
  ******************************************************************************
  * @file    uart.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "ebox_uart.h"
#include "ebox_mem.h"


uint8_t busy[5];

static uint32_t serial_irq_ids[UART_NUM] = {0, 0, 0,0,0};

static uart_irq_handler irq_handler;




/**
 *@name     Uart::Uart(USART_TypeDef *USARTx,Gpio *tx_pin,Gpio *rx_pin)
 *@brief    串口的构造函数
 *@param    USARTx:  USART1,2,3和UART4,5
 *          tx_pin:  外设所对应的tx引脚
 *          rx_pin:  外设所对应的rx引脚
 *@retval   None
*/
Uart::Uart(USART_TypeDef *USARTx, Gpio *tx_pin, Gpio *rx_pin)
{
    _USARTx = USARTx;
    tx_pin->mode(AF_PP);
    rx_pin->mode(INPUT);
}
/**
 *@name     void Uart::begin(uint32_t baud_rate,uint8_t _use_dma)
 *@brief    串口初始化函数，除了波特率可控外，其他参数默认：8bit数据，1个停止位，无校验，无硬件控制流
 *@param    baud_rate:  波特率，例如9600，115200，38400等等
 *@param    _use_dma:   是否使用DMA，默认值1：使用DMA，0：不适用DMA;
 *@retval   None
*/
void    Uart::begin(uint32_t baud_rate,uint8_t _use_dma)
{
    Uart::begin(baud_rate,8,0,1,_use_dma);
}


/**
 *@name     void    begin(uint32_t baud_rate,uint8_t data_bit,uint8_t parity,float stop_bit,uint8_t _use_dma);
 *@brief    串口初始化函数，并带有更多配置参数
 *@param    baud_rate:  波特率，例如9600，115200，38400等等
 *          data_bit:   数据位数，只能输入8或者9
 *          parity:     检验位；0：无校验位，1奇校验，2偶校验
 *          stop_bit:   停止位；0.5,1,1.5,2四个可选参数
 *          _use_dma:   是否使用DMA，默认值1：使用DMA，0：不适用DMA;
 *@retval   None
*/
void Uart::begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit,uint8_t _use_dma)
{
    uint8_t             index;
    USART_InitTypeDef   USART_InitStructure;
    
    use_dma = _use_dma;
    if(use_dma == 1)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA时钟

    switch((uint32_t)_USARTx)
    {
    case (uint32_t)USART1_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        _DMA1_Channelx = DMA1_Channel4;
				index = NUM_UART1;
        break;

    case (uint32_t)USART2_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        _DMA1_Channelx = DMA1_Channel7;
				index = NUM_UART2;
        break;

    case (uint32_t)USART3_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        _DMA1_Channelx = DMA1_Channel2;
				index = NUM_UART3;
        break;

#if defined (STM32F10X_HD)
    case (uint32_t)UART4_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
				index = NUM_UART4;
        break;

    case (uint32_t)UART5_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
				index = NUM_UART5;
        break;
#endif
    }

    serial_irq_handler(index, Uart::_irq_handler, (uint32_t)this);
    
    
    USART_InitStructure.USART_BaudRate = baud_rate;
    switch(data_bit)
    {
    case 8:
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        break;
    case 9:
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        break;
    default :
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        break;
    }
    switch(parity)
    {
    case 0:
        USART_InitStructure.USART_Parity = USART_Parity_No;
        break;
    case 1:
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        break;
    case 2:
        USART_InitStructure.USART_Parity = USART_Parity_Odd;
        break;
    default :
        USART_InitStructure.USART_Parity = USART_Parity_No;
        break;
    }
    if(stop_bit == 0.5)
        USART_InitStructure.USART_StopBits = USART_StopBits_0_5;
    else if(stop_bit == 1)
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
    else if(stop_bit == 1.5)
        USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
    else if(stop_bit == 2)
        USART_InitStructure.USART_StopBits = USART_StopBits_2;
		
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(_USARTx, &USART_InitStructure);

    if((_USARTx == USART1 || _USARTx == USART2 || _USARTx == USART3) && (use_dma == 1) )
        USART_DMACmd(_USARTx, USART_DMAReq_Tx, ENABLE);
    USART_Cmd(_USARTx, ENABLE);
    interrupt(ENABLE);
USART_ClearITPendingBit(_USARTx, USART_IT_TC);
USART_ClearFlag(USART2,USART_FLAG_TC); 
}

/**
 *@name     size_t Uart::write(uint8_t c)
 *@brief    串口方式发送一个字节
 *@param    ch：    要发送的字符
 *@retval   已发送的数据
*/
size_t Uart::write(uint8_t c)
{
    while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//单字节等待，等待寄存器空
    USART_SendData(_USARTx, c);
    return 1;
}

/**
 *@name     size_t Uart::write(const uint8_t *buffer, size_t size)
 *@brief    串口方式发送一个缓冲区
 *@param    buffer:   缓冲区指针
            size：    缓冲区大小
 *@retval   已发送的数据
*/
size_t Uart::write(const uint8_t *buffer, size_t size)
{
    wait_busy();
    if(uart_buf != NULL)
        ebox_free(uart_buf);
    set_busy();
    uart_buf = (char *)ebox_malloc(size);
    if(uart_buf == NULL)
        return 0;
    for(int i = 0; i < size; i++)
        uart_buf[i] = *buffer++;
    _write(uart_buf, size);
	return size;
}
/**
 *@name     uint16_t Uart::read()
 *@brief    串口接收数据，此函数只能在用户接收中断中调用
 *@param    NONE
 *@retval   串口所接收到的数据
*/
uint16_t Uart::read()
{
    return (uint16_t)(_USARTx->DR & (uint16_t)0x01FF);
}
/**
 *@name     int Uart::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   已发送的数据
*/

void Uart::printf(const char *fmt, ...)
{
    int     size1 = 0;
    size_t  size2 = 256;

    wait_busy();
    if(uart_buf != NULL)
        ebox_free(uart_buf);
    set_busy();
    va_list va_params;
    va_start(va_params, fmt);
    
    do{
        uart_buf = (char *)ebox_malloc(size2);
        if(uart_buf == NULL)
            return ;
        size1 = _vsnprintf(uart_buf, size2,fmt, va_params);
        if(size1 == -1  || size1 > size2)
        {
            size2+=128;
            size1 = -1;
            ebox_free(uart_buf);
        }
    }while(size1 == -1);

    vsprintf(uart_buf, fmt, va_params); 
    va_end(va_params);
    _write(uart_buf, size1);

}
///////////////////////////////////////////////////////////////

/**
 *@name     void Uart::interrupt(FunctionalState enable)
 *@brief    串口中断控制函数
 *@param    enable:  ENABLE使能串口的发送完成和接收中断；DISABLE：关闭这两个中断
 *@retval   None
*/
void Uart::interrupt(FunctionalState enable,uint8_t preemption_priority, uint8_t sub_priority)
{
    if(preemption_priority > 4)preemption_priority = 4;
    if(sub_priority > 4)sub_priority = 4;
    NVIC_InitTypeDef NVIC_InitStructure;

    USART_ClearITPendingBit(_USARTx, USART_IT_TC);
    USART_ClearFlag(USART2,USART_FLAG_TC); 
    switch((uint32_t)_USARTx)
    {
    case (uint32_t)USART1_BASE:
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemption_priority;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        break;

    case (uint32_t)USART2_BASE:
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemption_priority;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        break;

    case (uint32_t)USART3_BASE:
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemption_priority;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        break;
#if defined (STM32F10X_HD)
    case (uint32_t)UART4_BASE:
        NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemption_priority;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        break;
    case (uint32_t)UART5_BASE:
        NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemption_priority;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        break;
#endif
    }
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(_USARTx, USART_IT_RXNE, enable);
    USART_ITConfig(_USARTx, USART_IT_TC, enable);
}



/**
 *@name     uint16_t Uart::dma_send_string(const char *str,uint16_t length)
 *@brief    串口DMA方式发送字符串，缓冲区数据
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   发送数据的长度
*/
uint16_t Uart::dma_send_string(const char *str, uint16_t length)
{
    DMA_DeInit(_DMA1_Channelx);   //将DMA的通道1寄存器重设为缺省值
    _DMA1_Channelx->CPAR = (uint32_t)&_USARTx->DR; //外设地址
    _DMA1_Channelx->CMAR = (uint32_t) str; //mem地址
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

///**
// *@name     int Uart::put_char(uint16_t ch)
// *@brief    串口方式发送一个字节
// *@param    ch：    要发送的字符
// *@retval   已发送的数据
//*/
//int Uart::put_char(uint16_t ch)
//{
//    while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//单字节等待，等待寄存器空
//    USART_SendData(_USARTx, ch);
//    return ch;
//}

/**
 *@name     void Uart::_write(const char *str,uint16_t length)
 *@brief    串口方式发送字符串，缓冲区数据
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   NONE
*/
void Uart::_write(const char *str, uint16_t length)
{
    if((_USARTx == USART1 | _USARTx == USART2 | _USARTx == USART3 ) && (use_dma == 1))
    {
        dma_send_string(str, length);
    }
    else
    {
        while(length--)
        {
            while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//单字节等待，等待寄存器空
            USART_SendData(_USARTx, *str++);
        }
    }
}






/**
 *@name     int Uart::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   已发送的数据
*/
void Uart::wait_busy()
{
    uint32_t last=millis();
    switch((uint32_t)_USARTx)
    {
    case (uint32_t)USART1_BASE:
        while(busy[0] == 1 ){
            if(USART_GetITStatus(USART1, USART_IT_TC) == SET){
                busy[0] = 0;
                irq_handler(serial_irq_ids[NUM_UART1],TcIrq);
                USART_ClearITPendingBit(USART1, USART_IT_TC);
                break;
            }
        }
        break;
    case (uint32_t)USART2_BASE:
        while(busy[1] == 1 ){
            if(USART_GetITStatus(USART2, USART_IT_TC) == SET){
                busy[1] = 0;
                irq_handler(serial_irq_ids[NUM_UART2],TcIrq);
                USART_ClearITPendingBit(USART2, USART_IT_TC);
                break;
            }
        }
        break;
    case (uint32_t)USART3_BASE:
        while(busy[2] == 1 ){
            if(USART_GetITStatus(USART3, USART_IT_TC) == SET){
                busy[2] = 0;
                irq_handler(serial_irq_ids[NUM_UART3],TcIrq);
                USART_ClearITPendingBit(USART3, USART_IT_TC);
                break;
            }
        }
        break;
    case (uint32_t)UART4_BASE:
        while(busy[3] == 1 ){
            if(USART_GetITStatus(UART4, USART_IT_TC) == SET){
                busy[3] = 0;
                irq_handler(serial_irq_ids[NUM_UART4],TcIrq);
                USART_ClearITPendingBit(UART4, USART_IT_TC);
                break;
            }
        }
        break;
    case (uint32_t)UART5_BASE:
        while(busy[4] == 1 ){
            if(USART_GetITStatus(UART5, USART_IT_TC) == SET){
                busy[4] = 0;
                irq_handler(serial_irq_ids[NUM_UART5],TcIrq);
                USART_ClearITPendingBit(UART5, USART_IT_TC);
                break;
            }
        }
        break;
    }
}

/**
 *@name     int Uart::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   已发送的数据
*/
void Uart::set_busy()
{
    switch((uint32_t)_USARTx)
    {
    case (uint32_t)USART1_BASE:
        busy[0] = 1;
        break;
    case (uint32_t)USART2_BASE:
        busy[1] = 1;
        break;
    case (uint32_t)USART3_BASE:
        busy[2] = 1;
        break;
    case (uint32_t)UART4_BASE:
        busy[3] = 1;
        break;
    case (uint32_t)UART5_BASE:
        busy[4] = 1;
        break;
    }
}

void Uart::attach(void (*fptr)(void), IrqType type) {
    if (fptr) {
        _irq[type].attach(fptr);
    }
}

void Uart::_irq_handler(uint32_t id, IrqType irq_type) {
    Uart *handler = (Uart*)id;
    handler->_irq[irq_type].call();
}


extern "C" {

    void USART1_IRQHandler(void)
    {
        if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART1],RxIrq);
            USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART1, USART_IT_TC) == SET)
        {
            busy[0] = 0;   

            irq_handler(serial_irq_ids[NUM_UART1],TcIrq);
            USART_ClearITPendingBit(USART1, USART_IT_TC);
        }        
    }
    void USART2_IRQHandler(void)
    {
        if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
        {
						irq_handler(serial_irq_ids[NUM_UART2],RxIrq);
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART2, USART_IT_TC) == SET)
        {
            busy[1] = 0;
						irq_handler(serial_irq_ids[NUM_UART2],TcIrq);
            USART_ClearITPendingBit(USART2, USART_IT_TC);
        }
    }
    void USART3_IRQHandler(void)
    {
        if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
        {
						irq_handler(serial_irq_ids[NUM_UART3],RxIrq);
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART3, USART_IT_TC) == SET)
        {
            busy[2] = 0;
						irq_handler(serial_irq_ids[NUM_UART3],TcIrq);
            USART_ClearITPendingBit(USART3, USART_IT_TC);
        }
    }
    void UART4_IRQHandler(void)
    {
        if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
        {
						irq_handler(serial_irq_ids[NUM_UART4],RxIrq);
            USART_ClearITPendingBit(UART4, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART4, USART_IT_TC) == SET)
        {
            busy[3] = 0;
						irq_handler(serial_irq_ids[NUM_UART4],TcIrq);
            USART_ClearITPendingBit(UART4, USART_IT_TC);
        }
    }
    void UART5_IRQHandler(void)
    {
        if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
        {
						irq_handler(serial_irq_ids[NUM_UART5],RxIrq);
            USART_ClearITPendingBit(UART5, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART5, USART_IT_TC) == SET)
        {
            busy[4] = 0;
						irq_handler(serial_irq_ids[NUM_UART5],TcIrq);
            USART_ClearITPendingBit(UART5, USART_IT_TC);
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
		
void serial_irq_handler(uint8_t index, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[index] = id;
}
}




