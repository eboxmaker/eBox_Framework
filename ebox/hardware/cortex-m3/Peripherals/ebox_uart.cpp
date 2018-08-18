/**
  ******************************************************************************
  * @file    uart.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief   
  *         2018/8/6  将Tx pin 设置放在串口初始化之后，防止发送0x00
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

uint8_t busy[UART_NUM];

static uint32_t serial_irq_ids[UART_NUM] = {0};

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
    _tx_pin = tx_pin;
    _rx_pin = rx_pin;
}
/**
 *@name     void Uart::begin(uint32_t baud_rate,uint8_t _use_dma)
 *@brief    串口初始化函数，除了波特率可控外，其他参数默认：8bit数据，1个停止位，无校验，无硬件控制流
 *@param    baud_rate:  波特率，例如9600，115200，38400等等
 *@param    _use_dma:   是否使用DMA，默认值1：使用DMA，0：不适用DMA;
 *@retval   None
*/
void    Uart::begin(uint32_t baud_rate,uint8_t use_dma)
{
    Uart::begin(baud_rate,8,0,1,use_dma);
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
void Uart::begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit,uint8_t use_dma)
{
    uint8_t             index;
    USART_InitTypeDef   USART_InitStructure;
        
    rcc_clock_cmd((uint32_t)_USARTx,ENABLE);

    
#if USE_UART_DMA
    _use_dma = use_dma;
    switch((uint32_t)_USARTx)
    {
        
    #if USE_UART1
    case (uint32_t)USART1_BASE:
        dma_tx = &Dma1Ch4;
        index = NUM_UART1;
        break;
    #endif

    #if USE_UART2 
    case (uint32_t)USART2_BASE:
        dma_tx = &Dma1Ch7;
        index = NUM_UART2;
        break;
    #endif
    
    #if USE_UART3
    case (uint32_t)USART3_BASE:
        dma_tx = &Dma1Ch2;
        index = NUM_UART3;
        break;
    #endif

#if defined (STM32F10X_HD)
    #if USE_UART4
    case (uint32_t)UART4_BASE:
        index = NUM_UART4;
        break;
    #endif

    #if USE_UART5
    case (uint32_t)UART5_BASE:
        index = NUM_UART5;
        break;
    #endif
#endif
    }
#else
    switch((uint32_t)_USARTx)
    {
        
    #if USE_UART1
    case (uint32_t)USART1_BASE:
        index = NUM_UART1;
        break;
    #endif

    #if USE_UART2 
    case (uint32_t)USART2_BASE:
        index = NUM_UART2;
        break;
    #endif
    
    #if USE_UART3
    case (uint32_t)USART3_BASE:
        index = NUM_UART3;
        break;
    #endif

#if defined (STM32F10X_HD)
    #if USE_UART4
    case (uint32_t)UART4_BASE:
        index = NUM_UART4;
        break;
    #endif

    #if USE_UART5
    case (uint32_t)UART5_BASE:
        index = NUM_UART5;
        break;
    #endif
#endif
    }
#endif

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

#if USE_UART_DMA
    if((_USARTx == USART1 || _USARTx == USART2 || _USARTx == USART3) && (_use_dma == 1) )
    {
        USART_DMACmd(_USARTx, USART_DMAReq_Tx, ENABLE);
        dma_tx->rcc_enable();
        dma_tx->nvic(DISABLE,0,0);
        dma_tx->interrupt(DmaItTc,DISABLE);
        dma_tx->interrupt(DmaItTe,DISABLE);
        dma_tx->interrupt(DmaItHt,DISABLE);
    }
#endif
    USART_Cmd(_USARTx, ENABLE);
    

    nvic(ENABLE,0,0);
    interrupt(RxIrq,DISABLE);
    interrupt(TcIrq,DISABLE);

    
    _tx_pin->mode(AF_PP);
    _rx_pin->mode(INPUT);

}
void Uart::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
    nvic_dev_set_priority((uint32_t)_USARTx,0,0,0);
    if(enable != DISABLE)
        nvic_dev_enable((uint32_t)_USARTx,0);
    else
        nvic_dev_disable((uint32_t)_USARTx,0);
}
///////////////////////////////////////////////////////////////

/**
 *@name     void Uart::interrupt(FunctionalState enable)
 *@brief    串口中断控制函数
 *@param    enable:  ENABLE使能串口的发送完成和接收中断；DISABLE：关闭这两个中断
 *@retval   None
*/
void Uart::interrupt(IrqType type, FunctionalState enable)
{

    if(type == RxIrq)
        USART_ITConfig(_USARTx, USART_IT_RXNE, enable);
    if(type == TcIrq)
    {
        USART_ITConfig(_USARTx, USART_IT_TC, enable);//禁止关闭发送完成中断
    }
}
/**
 *@name     size_t Uart::write(uint8_t c)
 *@brief    串口方式发送一个字节
 *@param    ch：    要发送的字符
 *@retval   已发送的数据
*/
size_t Uart::write(uint8_t c)
{
//    while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//单字节等待，等待寄存器空
//    USART_SendData(_USARTx, c);
//    return 1;
    write(&c,1);
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
    if(size <= 0 ) return 0;
    wait_busy();
    #if USE_UART_DMA
    if((_USARTx == USART1 || _USARTx == USART2 || _USARTx == USART3 ) && (_use_dma == 1))
    {
        if(tx_buf_ptr != NULL)
            ebox_free(tx_buf_ptr);
        set_busy();
        tx_buf_ptr = (char *)ebox_malloc(size);
        if(tx_buf_ptr == NULL)
        {
            return 0;
        }
        for(int i = 0; i < size; i++)
            tx_buf_ptr[i] = *buffer++;
        dma_write(tx_buf_ptr, size);
    }
    else
    #endif
    {
        while(size--)
        {
            while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//单字节等待，等待寄存器空
            USART_SendData(_USARTx, *buffer++);
        }
    }
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
 *@name     uint16_t Uart::dma_send_string(const char *str,uint16_t length)
 *@brief    串口DMA方式发送字符串，缓冲区数据
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   发送数据的长度
*/
#if USE_UART_DMA
uint16_t Uart::dma_write(const char *str, uint16_t length)
{
//    DMA_DeInit(_DMA1_Channelx);   //将DMA的通道1寄存器重设为缺省值
//    _DMA1_Channelx->CPAR = (uint32_t)&_USARTx->DR; //外设地址
//    _DMA1_Channelx->CMAR = (uint32_t) str; //mem地址
//    _DMA1_Channelx->CNDTR = length ; //传输长度
//    _DMA1_Channelx->CCR = (0 << 14) | // 非存储器到存储器模式
//                          (2 << 12) | // 通道优先级高
//                          (0 << 11) | // 存储器数据宽度8bit
//                          (0 << 10) | // 存储器数据宽度8bit
//                          (0 <<  9) | // 外设数据宽度8bit
//                          (0 <<  8) | // 外设数据宽度8bit
//                          (1 <<  7) | // 存储器地址增量模式
//                          (0 <<  6) | // 外设地址增量模式(不增)
//                          (0 <<  5) | // 非循环模式
//                          (1 <<  4) | // 从存储器读
//                          (0 <<  3) | // 是否允许传输错误中断
//                          (0 <<  2) | // 是否允许半传输中断
//                          (0 <<  1) | // 是否允许传输完成中断
//                          (0);        // 通道开启
    
    DMA_InitTypeDef DMA_InitStructure;

    dma_tx->deInit();
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&_USARTx->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) str;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = length;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    dma_tx->init(&DMA_InitStructure);
    dma_tx->enable();
    return length;
}
#endif


/**
 *@name     int Uart::put_char(uint16_t ch)
 *@brief    串口方式发送一个字节
 *@param    str：       要发送的字符串，数据缓冲区
            length：    缓冲区的长度
 *@retval   已发送的数据
*/
void Uart::wait_busy()
{
    switch((uint32_t)_USARTx)
    {
    #if USE_UART1
        case (uint32_t)USART1_BASE:
            while(busy[0] == 1 ){
                while((USART1->SR & 0X80) == 0);
                if(USART1->SR & 0X40){
                    busy[0] = 0;
                    USART_ClearITPendingBit(USART1, USART_IT_TC);
                    break;
                }
            }
            break;
    #endif
            
    #if USE_UART2
        case (uint32_t)USART2_BASE:
            while(busy[1] == 1 ){
                while((USART2->SR & 0X80) == 0);
                if(USART2->SR & 0X40){
                    busy[1] = 0;
                    USART_ClearITPendingBit(USART2, USART_IT_TC);
                    break;
                }
            }
            break;
    #endif
            
    #if USE_UART3
        case (uint32_t)USART3_BASE:
            while(busy[2] == 1 ){
                if(USART3->SR & 0X40){
                    busy[2] = 0;
                    USART_ClearITPendingBit(USART3, USART_IT_TC);
                    break;
                }
            }
            break;
    #endif
    
    #if USE_UART4
        case (uint32_t)UART4_BASE:
            while(busy[3] == 1 ){
                if(UART4->SR & 0X40){
                    busy[3] = 0;
                    USART_ClearITPendingBit(UART4, USART_IT_TC);
                    break;
                }
            }
            break;
    #endif
    
    #if USE_UART5
        case (uint32_t)UART5_BASE:
            while(busy[4] == 1 ){
                if(UART5->SR & 0X40){
                    busy[4] = 0;
                    irq_handler(serial_irq_ids[NUM_UART5],TcIrq);
                    USART_ClearITPendingBit(UART5, USART_IT_TC);
                break;
                }
            }
        break;
    #endif
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
    #if USE_UART1
        case (uint32_t)USART1_BASE:
            busy[0] = 1;break;
    #endif
        
    #if USE_UART2
        case (uint32_t)USART2_BASE:
            busy[1] = 1;break;
    #endif
        
        
    #if USE_UART3
        case (uint32_t)USART3_BASE:
            busy[2] = 1;break;
    #endif
        
        
    #if USE_UART4
        case (uint32_t)UART4_BASE:
            busy[3] = 1;break;
    #endif
        
        
    #if USE_UART5
        case (uint32_t)UART5_BASE:
            busy[4] = 1;break;
    #endif
        
        
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

    #if USE_UART1
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
    #endif


    #if USE_UART2
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
    #endif


    #if USE_UART3
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
    #endif

#if defined (STM32F10X_HD)
    #if USE_UART4
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
    #endif

    #if USE_UART5
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
    #endif

#endif
		
void serial_irq_handler(uint8_t index, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[index] = id;
}
}




