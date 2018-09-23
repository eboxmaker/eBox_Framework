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
static uint32_t serial_irq_ids[UART_NUM] = {0};

static uart_irq_handler irq_handler;



uint16_t _tx_buffer_size[UART_NUM];
uint16_t _tx_buffer_head[UART_NUM];
uint16_t _tx_buffer_tail[UART_NUM];
char     *_tx_ptr[UART_NUM];

uint16_t _rx_buffer_size[UART_NUM];
uint16_t _rx_buffer_head[UART_NUM];
uint16_t _rx_buffer_tail[UART_NUM];
uint16_t *_rx_ptr[UART_NUM];


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
void    Uart::begin(uint32_t baud_rate,RxMode_t mode)
{
    Uart::begin(baud_rate,8,0,1,mode);
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
void Uart::begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit,RxMode_t mode)
{
    USART_InitTypeDef   USART_InitStructure;
        
    rcc_clock_cmd((uint32_t)_USARTx,ENABLE);
    this->mode = mode;
    switch((uint32_t)_USARTx)
    {
    #if USE_UART1
    case (uint32_t)USART1_BASE:
        dma_rx = &Dma1Ch5;
        uart_index = NUM_UART1;
        _tx_buffer_size[uart_index] = TX_BUFFER_SIZE_UART1;
        _rx_buffer_size[uart_index] = RX_BUFFER_SIZE_UART1;
        break;
    #endif

    #if USE_UART2 
    case (uint32_t)USART2_BASE:
        dma_rx = &Dma1Ch6;
        uart_index = NUM_UART2;
        _tx_buffer_size[uart_index] = TX_BUFFER_SIZE_UART2;
        _rx_buffer_size[uart_index] = RX_BUFFER_SIZE_UART2;
        break;
    #endif

    #if USE_UART3 
    case (uint32_t)USART3_BASE:
        dma_rx = &Dma1Ch3;
        uart_index = NUM_UART3;
        _tx_buffer_size[uart_index] = TX_BUFFER_SIZE_UART3;
        _rx_buffer_size[uart_index] = RX_BUFFER_SIZE_UART3;
        break;
    #endif

#if defined (STM32F10X_HD)
    case (uint32_t)UART4_BASE:
        dma_rx = &Dma2Ch3;
        uart_index = NUM_UART4;
        _tx_buffer_size[uart_index] = TX_BUFFER_SIZE_UART4;
        _rx_buffer_size[uart_index] = RX_BUFFER_SIZE_UART4;
        break;

    case (uint32_t)UART5_BASE:
        uart_index = NUM_UART5;
        _tx_buffer_size[uart_index] = TX_BUFFER_SIZE_UART5;
        _rx_buffer_size[uart_index] = RX_BUFFER_SIZE_UART5;
        break;
#endif
    }
    _tx_ptr[uart_index] = (char *)ebox_malloc(_tx_buffer_size[uart_index]);
    _rx_ptr[uart_index] = (uint16_t *)ebox_malloc(_rx_buffer_size[uart_index]);

    serial_irq_handler(uart_index, Uart::_irq_handler, (uint32_t)this);
    
    
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


    if((mode == RxDMA) && (dma_rx != NULL))
    {
        USART_DMACmd(_USARTx, USART_DMAReq_Rx, ENABLE);
        dma_rx->rcc_enable();
        dma_rx->nvic(DISABLE,0,0);
        dma_rx->interrupt(DmaItTc,DISABLE);
        dma_rx->interrupt(DmaItTe,DISABLE);
        dma_rx->interrupt(DmaItHt,DISABLE);
        
        DMA_InitTypeDef DMA_InitStructure;

        dma_rx->deInit();
        
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&_USARTx->DR;
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) _rx_ptr[uart_index];
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize = _rx_buffer_size[uart_index];
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
        DMA_InitStructure.DMA_Priority = DMA_Priority_High;
        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
        dma_rx->init(&DMA_InitStructure);
        dma_rx->enable();
    }
    
    USART_Cmd(_USARTx, ENABLE);
    
    nvic(ENABLE,0,0);
    interrupt(RxIrq,ENABLE);
    interrupt(TxIrq,DISABLE);
    _tx_pin->mode(AF_PP);
    _rx_pin->mode(INPUT);

}
void Uart::end()
{
    USART_DeInit(_USARTx);
  // clear any received data
  _rx_buffer_head[uart_index] = _rx_buffer_tail[uart_index];
}
void Uart::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
    this->preemption_priority = preemption_priority;
    this->sub_priority = sub_priority;
    nvic_dev_set_priority((uint32_t)_USARTx,0,preemption_priority,sub_priority);
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
        USART_ITConfig(_USARTx, USART_IT_RXNE, ENABLE);//接收中断不能关闭，
    if(type == TxIrq)
        USART_ITConfig(_USARTx, USART_IT_TXE, enable);
}
int Uart::peek(void)
{
  if ((_rx_buffer_size[uart_index] - dma_rx->DMAy_Channelx->CNDTR) == _rx_buffer_tail[uart_index]) {
    return -1;
  } else {
    return _rx_ptr[uart_index][_rx_buffer_tail[uart_index]];
  }
}
/**
 *@name     uint16_t Uart::read()
 *@brief    串口接收数据，此函数只能在用户接收中断中调用
 *@param    NONE
 *@retval   串口所接收到的数据
*/
int Uart::available()
{
    if(mode == RxDMA)
        return ((unsigned int)(_rx_buffer_size[uart_index] + (_rx_buffer_size[uart_index] - dma_rx->DMAy_Channelx->CNDTR) - _rx_buffer_tail[uart_index])) % _rx_buffer_size[uart_index];
    else
        return ((unsigned int)(_rx_buffer_size[uart_index] + _rx_buffer_head[uart_index] - _rx_buffer_tail[uart_index])) % _rx_buffer_size[uart_index];
}

int Uart::read()
{
    if(mode == RxDMA)
    {
        if (_rx_buffer_tail[uart_index] == (_rx_buffer_size[uart_index] - dma_rx->DMAy_Channelx->CNDTR)) 
        {
            return -1;
        } 
    }
    else
    {
        if (_rx_buffer_tail[uart_index] == _rx_buffer_head[uart_index]) 
        {
            return -1;
        } 
    }
    {
        int c = _rx_ptr[uart_index][_rx_buffer_tail[uart_index]];
        _rx_buffer_tail[uart_index] = (_rx_buffer_tail[uart_index] + 1) % _rx_buffer_size[uart_index];
        return c;
    }
}


int Uart::availableForWrite()
{

  uint16_t head = _tx_buffer_head[uart_index];
  uint16_t tail = _tx_buffer_tail[uart_index];

  if (head >= tail) 
      return _tx_buffer_size[uart_index] - 1 - head + tail;
  
  return tail - head - 1;
}


void Uart::flush()
{
    uint8_t major,minor;
    while(_tx_buffer_head[uart_index] != _tx_buffer_tail[uart_index] )
    {
        //如果全局中断被关闭,发送使能中断被关闭、在其他中断函数中,则手动发送
//        if(__get_PRIMASK() || ((_USARTx->CR1 & USART_FLAG_TXE) == 0))
        if(__get_PRIMASK() || ((_USARTx->CR1 & USART_FLAG_TXE) == 0) || (__get_IPSR() != 0))
        {   
            nvic_irq_get_priority((IRQn_Type)__get_IPSR(),&major,&minor);
            if(major <= preemption_priority )//串口优先级低于或等于所在中断服务函数
            {
//                interrupt(TxIrq,DISABLE);//期间必须关闭串口中断
                while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//单字节等待，等待寄存器空
                tx_bufferx_one(_USARTx,uart_index);
            }
        }
        //或者等待串口中断发送完成所有数据的传输
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

    uint16_t i = (_tx_buffer_head[uart_index] + 1) % _tx_buffer_size[uart_index];//计算头的位置

    while (i == _tx_buffer_tail[uart_index]) //如果环形缓冲区满了就调用一次发送
    {
        interrupt(TxIrq,DISABLE);//期间必须关闭串口中断
        while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//单字节等待，等待寄存器空
        tx_bufferx_one(_USARTx,uart_index);
        interrupt(TxIrq,ENABLE);//开启发送
    }

    _tx_ptr[uart_index][_tx_buffer_head[uart_index]] = c;
    _tx_buffer_head[uart_index] = i;

    interrupt(TxIrq,ENABLE);//开启发送
  
  return 1;
}


///**
// *@name     size_t Uart::write(const uint8_t *buffer, size_t size)
// *@brief    串口方式发送一个缓冲区
// *@param    buffer:   缓冲区指针
//            size：    缓冲区大小
// *@retval   已发送的数据
//*/
//size_t Uart::write(const uint8_t *buffer, size_t size)
//{
//    if(size <= 0 ) return 0;

//    while(size--)
//    {
//        write(*buffer++);
//    }

//	return size;
//}



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
    void tx_bufferx_one(USART_TypeDef* uart,uint8_t uart_index)
    {
        if (_tx_buffer_head[uart_index] == _tx_buffer_tail[uart_index])//如果空则直接返回
            return;
        unsigned char c = _tx_ptr[uart_index][_tx_buffer_tail[uart_index]];
        _tx_buffer_tail[uart_index] = (_tx_buffer_tail[uart_index] + 1) % _tx_buffer_size[uart_index];
        uart->DR = (c & (uint16_t)0x01FF);
        if (_tx_buffer_head[uart_index] == _tx_buffer_tail[uart_index])//如果发送完所有数据
        {
            // Buffer empty, so disable interrupts
            USART_ITConfig(uart, USART_IT_TXE, DISABLE);
        }
    }
    void rx_buffer_one(USART_TypeDef* uart,uint8_t uart_index)
    {
        uint16_t i = (_rx_buffer_head[uart_index] + 1) % _rx_buffer_size[uart_index];//计算头的位置
        if(i == _rx_buffer_tail[uart_index]) //如果环形缓冲区满了就修改一次tail，将会舍弃最老的一个数据
        {
            _rx_buffer_tail[uart_index] = (_rx_buffer_tail[uart_index] + 1) % _rx_buffer_size[uart_index];

        }
        _rx_ptr[uart_index][_rx_buffer_head[uart_index]] = uart->DR;
        _rx_buffer_head[uart_index] = i;

    }

    #if USE_UART1

    void USART1_IRQHandler(void)
    {
        if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
        {
            rx_buffer_one(USART1,NUM_UART1);
            irq_handler(serial_irq_ids[NUM_UART1],RxIrq);
            USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        }      
        if(USART_GetITStatus(USART1, USART_IT_TXE) == SET)
        {
            tx_bufferx_one(USART1,NUM_UART1);
            irq_handler(serial_irq_ids[NUM_UART1],TxIrq);
        }        
    }
    #endif

    #if USE_UART2
    void USART2_IRQHandler(void)
    {
        if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
        {
            rx_buffer_one(USART2,NUM_UART2);
            irq_handler(serial_irq_ids[NUM_UART2],RxIrq);
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART2, USART_IT_TXE) == SET)
        {
            tx_bufferx_one(USART2,NUM_UART2);
            irq_handler(serial_irq_ids[NUM_UART2],TxIrq);
        }
    }
    #endif


    #if USE_UART3
    void USART3_IRQHandler(void)
    {
        if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
        {
            rx_buffer_one(USART3,NUM_UART3);
            irq_handler(serial_irq_ids[NUM_UART3],RxIrq);
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART3, USART_IT_TXE) == SET)
        {
            tx_bufferx_one(USART3,NUM_UART3);
            irq_handler(serial_irq_ids[NUM_UART3],TxIrq);
        }
    }
    #endif

    #if defined (STM32F10X_HD)
    void UART4_IRQHandler(void)
    {
        if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART4],RxIrq);
            USART_ClearITPendingBit(UART4, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART4, USART_IT_TXE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART4],TxIrq);
            USART_ClearITPendingBit(UART4, USART_IT_TXE);
        }
    }
    void UART5_IRQHandler(void)
    {
        if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART5],RxIrq);
            USART_ClearITPendingBit(UART5, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART5, USART_IT_TXE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART5],TxIrq);
            USART_ClearITPendingBit(UART5, USART_IT_TXE);
        }
    }
#endif
		
void serial_irq_handler(uint8_t uart_index, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[uart_index] = id;
}
}




