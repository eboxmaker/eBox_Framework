/**
  ******************************************************************************
  * @file    uart.cpp
  * @author  shentq
  * @version V2.1
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

static uint32_t serial_irq_ids[UART_NUM] = {0};
static uart_irq_handler irq_handler;



uint16_t _tx_buffer_size[UART_NUM];   // 发送环形缓冲区size
uint16_t _tx_buffer_head[UART_NUM];   // 缓冲区头,每写入（写入缓冲区）一个字符，向后移动1
uint16_t _tx_buffer_tail[UART_NUM];   // 缓冲区尾,每写出（写入串口TX）一个字符，向后移动1

uint16_t _rx_buffer_size[UART_NUM];
uint16_t _rx_buffer_head[UART_NUM];
uint16_t _rx_buffer_tail[UART_NUM];

#ifdef UART_9_BIT
uint16_t *_tx_ptr[UART_NUM];          // 缓冲区指针
uint16_t *_rx_ptr[UART_NUM];
#else
uint8_t *_tx_ptr[UART_NUM];          // 缓冲区指针
uint8_t *_rx_ptr[UART_NUM];
#endif
/**
 *@brief    串口的构造函数
 *@param    USARTx:  USART1,2,3和UART4,5
 *          tx_pin:  外设所对应的tx引脚
 *          rx_pin:  外设所对应的rx引脚
 *@retval   None
*/
Uart::Uart(USART_TypeDef *USARTx, Gpio *tx_pin, Gpio *rx_pin, uint16_t tx_buffer_size, uint16_t rx_buffer_size)
{
    _USARTx = USARTx;
    _tx_pin = tx_pin;
    _rx_pin = rx_pin;
    this->rx_buffer_size = rx_buffer_size;
    this->tx_buffer_size = tx_buffer_size;
}
/**
 *@name     void Uart::begin(uint32_t baud_rate,uint8_t _use_dma)
 *@brief    串口初始化函数，除了波特率可控外，其他参数默认：8bit数据，1个停止位，无校验，无硬件控制流
 *@param    baud_rate:  波特率，例如9600，115200，38400等等
 *@param    _use_dma:   是否使用DMA，默认值1：使用DMA，0：不适用DMA;
 *@retval   None
*/
void    Uart::begin(uint32_t baud_rate, RxMode_t mode)
{
    Uart::begin(baud_rate, 8, 0, 1, mode);
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
void Uart::begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit, RxMode_t mode)
{
    uint8_t  index;
    uint32_t _DataWidth;
    uint32_t _Parity;
    uint32_t _StopBits = 0;
    uint32_t periphclk = LL_RCC_PERIPH_FREQUENCY_NO;
    LL_RCC_ClocksTypeDef RCC_Clocks;
    LL_RCC_GetSystemClocksFreq(&RCC_Clocks);

    rcc_clock_cmd((uint32_t)_USARTx, ENABLE);
    _mode = mode;

    switch((uint32_t)_USARTx)
    {

#if USE_UART1
    case (uint32_t)USART1_BASE:
        dma_rx = &Dma1Ch5;
        _index = NUM_UART1;
        periphclk = LL_RCC_GetUSARTClockFreq(LL_RCC_USART1_CLKSOURCE);
        break;
#endif

#if USE_UART2
    case (uint32_t)USART2_BASE:
        dma_rx = &Dma1Ch6;
        _index = NUM_UART2;
#if defined(RCC_CFGR3_USART2SW)
      periphclk = LL_RCC_GetUSARTClockFreq(LL_RCC_USART2_CLKSOURCE);
#else
      /* USART2 clock is PCLK */
      LL_RCC_GetSystemClocksFreq(&RCC_Clocks);
      periphclk = RCC_Clocks.PCLK1_Frequency;
#endif
    break;
#endif

#if USE_UART3
    case (uint32_t)USART3_BASE:
        dma_rx = &Dma1Ch3;
        _index = NUM_UART3;
#if defined(RCC_CFGR3_USART3SW)
      periphclk = LL_RCC_GetUSARTClockFreq(LL_RCC_USART3_CLKSOURCE);
#else
      /* USART3 clock is PCLK */
      LL_RCC_GetSystemClocksFreq(&RCC_Clocks);
      periphclk = RCC_Clocks.PCLK1_Frequency;
#endif
    break;
#if USE_UART4
    case (uint32_t)USART4_BASE:
        break;
#endif
    default :
        break;

#endif
    }

    _tx_buffer_size[_index] = tx_buffer_size;
    _rx_buffer_size[_index] = rx_buffer_size;

#ifdef UART_9_BIT
    _tx_ptr[_index] = (uint16_t *)ebox_malloc(_tx_buffer_size[_index] * sizeof(uint16_t));
    _rx_ptr[_index] = (uint16_t *)ebox_malloc(_rx_buffer_size[_index] * sizeof(uint16_t));
#else
    _tx_ptr[_index] = (uint8_t *)ebox_malloc(_tx_buffer_size[_index] * sizeof(uint8_t));
    _rx_ptr[_index] = (uint8_t *)ebox_malloc(_rx_buffer_size[_index] * sizeof(uint8_t));
#endif
    serial_irq_handler(_index, Uart::_irq_handler, (uint32_t)this);

    _DataWidth = data_bit == 9 ? LL_USART_DATAWIDTH_9B : LL_USART_DATAWIDTH_8B;
    switch(parity)
    {
    case 0:
        _Parity = LL_USART_PARITY_NONE;
        break;
    case 1:
        _Parity = LL_USART_PARITY_EVEN;
        break;
    case 2:
        _Parity = LL_USART_PARITY_ODD;
        break;
    default :
        _Parity = LL_USART_PARITY_NONE;
        break;
    }

    if(stop_bit == 0.5)
        _StopBits = LL_USART_STOPBITS_0_5;
    else if(stop_bit == 1)
        _StopBits = LL_USART_STOPBITS_1;
    else if(stop_bit == 1.5)
        _StopBits = LL_USART_STOPBITS_2;
    else if(stop_bit == 2)
        _StopBits = LL_USART_STOPBITS_1_5;

    nvic(ENABLE, 0, 0);
    // 初始化IO
//    index = getIndex(_rx_pin->id, UART_MAP);
//    _rx_pin->mode(UART_MAP[index]._pinMode, UART_MAP[index]._pinAf);
//    index = getIndex(_tx_pin->id, UART_MAP);
//    _tx_pin->mode(UART_MAP[index]._pinMode, UART_MAP[index]._pinAf);

    LL_USART_SetTransferDirection(_USARTx, LL_USART_DIRECTION_TX_RX);
    LL_USART_ConfigCharacter(_USARTx, _DataWidth, _Parity, _StopBits);
    LL_USART_SetBaudRate(_USARTx, periphclk, LL_USART_OVERSAMPLING_16, baud_rate);
    //	禁止溢出检测
    LL_USART_DisableOverrunDetect(_USARTx);
    LL_USART_Enable(_USARTx);
    /* Polling USART initialisation */
    while ((!(LL_USART_IsActiveFlag_TEACK(_USARTx))) || (!(LL_USART_IsActiveFlag_REACK(_USARTx))))
    {
    }

    if((mode == RxDMA) && (dma_rx != NULL))
    {
        LL_USART_EnableDMAReq_RX(_USARTx);
        dma_rx->rcc_enable();
        dma_rx->nvic(DISABLE, 0, 0);
        dma_rx->interrupt(DmaItTc, DISABLE);
        dma_rx->interrupt(DmaItTe, DISABLE);
        dma_rx->interrupt(DmaItHt, DISABLE);

        LL_DMA_InitTypeDef DMA_InitStructure;

        dma_rx->deInit();

        DMA_InitStructure.PeriphOrM2MSrcAddress  = (uint32_t)&_USARTx->RDR;             //外设地址
        DMA_InitStructure.MemoryOrM2MDstAddress  = (uint32_t) _rx_ptr[_index];           //mem地址
        DMA_InitStructure.Direction              = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;   //传输方向，外设到内存
        DMA_InitStructure.NbData                 = _rx_buffer_size[_index];              //传输长度
        DMA_InitStructure.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;           //外设地址增量模式(不增)
        DMA_InitStructure.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;             //存储器地址增量模式
#ifdef UART_9_BIT
        DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
        DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
#else
        DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
        DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
#endif
        DMA_InitStructure.Mode                   = LL_DMA_MODE_CIRCULAR;                //传输模式,连续循环
        DMA_InitStructure.Priority               = LL_DMA_PRIORITY_HIGH;                //通道优先级高

        dma_rx->init(&DMA_InitStructure);
        dma_rx->enable();
    }
    _tx_pin->mode(AF_PP,7);
    _rx_pin->mode(AF_OD_PU,7);
    nvic(ENABLE, 0, 0);
    interrupt(RxIrq, ENABLE);
    interrupt(TxIrq, DISABLE);

}

/**
 *@brief    复位串口，并清空RX缓冲区
 *@param    none
 *@retval   None
*/
void Uart::end()
{
    LL_USART_DeInit(_USARTx);
    // clear any received data
    _rx_buffer_head[_index] = _rx_buffer_tail[_index];
}
void Uart::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
    this->preemption_priority = preemption_priority;
    this->sub_priority = sub_priority;
    nvic_dev_set_priority((uint32_t)_USARTx, 0, preemption_priority, sub_priority);
    if(enable != DISABLE)
        nvic_dev_enable((uint32_t)_USARTx, 0);
    else
        nvic_dev_disable((uint32_t)_USARTx, 0);

}

/**
 *@brief    串口中断控制函数
 *@param    enable:  ENABLE使能串口的发送完成和接收中断；DISABLE：关闭这两个中断
 *@retval   None
*/
void Uart::interrupt(IrqType type, FunctionalState enable)
{
    if(type == RxIrq)
        LL_USART_EnableIT_RXNE(_USARTx);
    if(type == TxIrq)
    {
        (enable == ENABLE) ? (LL_USART_EnableIT_TXE(_USARTx)) : ( LL_USART_DisableIT_TXE(_USARTx)) ;
    }
}
/**
 *@brief    相当与read，但读取后不从缓冲区清除数据
 *@param    NONE
 *@retval   -1，空；或返回tail指向的数据
*/
int Uart::peek(void)
{
    if ((_rx_buffer_size[_index] - LL_DMA_GetDataLength(DMA1, dma_rx->DMAy_Channelx)) == _rx_buffer_tail[_index])
    {
        return -1;
    }
    else
    {
        return _rx_ptr[_index][_rx_buffer_tail[_index]];
    }
}
/**
 *@brief    判断接收缓冲区是否有数据
 *@param    NONE
 *@retval   0 无数据，非0，当前缓冲区数据长度
*/
int Uart::available()
{
    if(_mode == RxDMA)
        return ((unsigned int)(_rx_buffer_size[_index] + (_rx_buffer_size[_index] - LL_DMA_GetDataLength(DMA1, dma_rx->DMAy_Channelx)) - _rx_buffer_tail[_index])) % _rx_buffer_size[_index];
    else
        return ((unsigned int)(_rx_buffer_size[_index] + _rx_buffer_head[_index] - _rx_buffer_tail[_index])) % _rx_buffer_size[_index];
}

/**
 *@brief    读取一个字节，并清除当前数据
 *@param    NONE
 *@retval   -1  无数据，其他，当前数据
*/
int Uart::read()
{
    if(_mode == RxDMA)
    {
        if (_rx_buffer_tail[_index] == (_rx_buffer_size[_index] - LL_DMA_GetDataLength(DMA1, dma_rx->DMAy_Channelx)))
        {
            return -1;
        }
    }
    else
    {
        if (_rx_buffer_tail[_index] == _rx_buffer_head[_index])
        {
            return -1;
        }
    }
    {
        int c = _rx_ptr[_index][_rx_buffer_tail[_index]];
        _rx_buffer_tail[_index] = (_rx_buffer_tail[_index] + 1) % _rx_buffer_size[_index];
        return c;
    }
}

/**
 *@brief    是否可以写入
 *@param    NONE
 *@retval   可以写入的数量
*/
int Uart::availableForWrite()
{
    uint16_t head = _tx_buffer_head[_index];
    uint16_t tail = _tx_buffer_tail[_index];

    if (head >= tail)
        return _tx_buffer_size[_index] - 1 - head + tail;

    return tail - head - 1;
}

/**
 *@brief    手动将tx缓冲区内容全部写出
 *@param    NONE
 *@retval   NONE
*/
void Uart::flush()
{
    uint8_t major, minor;
    while(_tx_buffer_head[_index] != _tx_buffer_tail[_index] )
    {
        //如果全局中断被关闭,发送使能中断被关闭、在其他中断函数中,则手动发送
        //        if(__get_PRIMASK() || ((_USARTx->CR1 & USART_FLAG_TXE) == 0))
        if(__get_PRIMASK() || ((_USARTx->CR1 & USART_ISR_TXE) == 0) || (__get_IPSR() != 0))
        {
            nvic_irq_get_priority((IRQn_Type)__get_IPSR(), &major, &minor);
            if(major <= preemption_priority )//串口优先级低于或等于所在中断服务函数
            {
                //                interrupt(TxIrq,DISABLE);//期间必须关闭串口中断
                while(LL_USART_IsActiveFlag_TXE(_USARTx) == RESET);//单字节等待，等待寄存器空
                tx_bufferx_one(_USARTx, _index);
            }
        }
        //或者等待串口中断发送完成所有数据的传输
    }
}


/**
 *@brief    发送一个字节
 *@param    c：要发送的字符
 *@retval   1
*/
size_t Uart::write(uint8_t c)
{
    uint16_t i = (_tx_buffer_head[_index] + 1) % _tx_buffer_size[_index];//计算头的位置
    // head = tail, 缓冲区过满，先发送
    while (i == _tx_buffer_tail[_index])
    {
        interrupt(TxIrq, DISABLE); //期间必须关闭串口中断
        while(LL_USART_IsActiveFlag_TXE(_USARTx) == RESET);//单字节等待，等待寄存器空
        tx_bufferx_one(_USARTx, _index);
        interrupt(TxIrq, ENABLE); //开启发送
    }
    // 加入新数据，移动head
    _tx_ptr[_index][_tx_buffer_head[_index]] = c;
    _tx_buffer_head[_index] = i;

    interrupt(TxIrq, ENABLE); //开启发送
    return 1;
}



void Uart::attach(void (*fptr)(void), IrqType type)
{
    if (fptr)
    {
        _irq[type].attach(fptr);
    }
}

void Uart::_irq_handler(uint32_t id, IrqType irq_type)
{
    Uart *handler = (Uart *)id;
    handler->_irq[irq_type].call();
}


extern "C" {
    /**
      *@brief    从缓冲区写出一个字符
      *@param    uart：串口； index 串口 IT 索引
      *@retval   1
      */
    void tx_bufferx_one(USART_TypeDef *uart, uint8_t index)
    {
        if (_tx_buffer_head[index] == _tx_buffer_tail[index])//如果空则直接返回
        {
            //LL_USART_DisableIT_TXE(uart);
            return;
        }
        unsigned char c = _tx_ptr[index][_tx_buffer_tail[index]];   // 取出字符
        _tx_buffer_tail[index] = (_tx_buffer_tail[index] + 1) % _tx_buffer_size[index];
        uart->TDR = (c & (uint16_t)0x01FF);
        if (_tx_buffer_head[index] == _tx_buffer_tail[index])//如果发送完所有数据
        {
            // Buffer empty, so disable interrupts
            LL_USART_DisableIT_TXE(uart);
        }
    }
    /**
      *@brief    读入一个字符放入缓冲区
      *@param    uart：串口； index 串口 IT 索引
      *@retval   1
      */
    void rx_buffer_one(USART_TypeDef *uart, uint8_t index)
    {
        uint16_t i = (_rx_buffer_head[index] + 1) % _rx_buffer_size[index];//计算头的位置
        if(i == _rx_buffer_tail[index]) //如果环形缓冲区满了就修改一次tail，将会舍弃最老的一个数据
        {
            _rx_buffer_tail[index] = (_rx_buffer_tail[index] + 1) % _rx_buffer_size[index];
        }
        _rx_ptr[index][_rx_buffer_head[index]] = uart->RDR;
        _rx_buffer_head[index] = i;
    }

#if USE_UART1
    void USART1_IRQHandler(void)
    {
        if(LL_USART_IsActiveFlag_RXNE(USART1) == SET)
        {
            rx_buffer_one(USART1, NUM_UART1);
            irq_handler(serial_irq_ids[NUM_UART1], RxIrq);
            CLEAR_BIT(USART1->ISR, B10000); //强制清除
        }
        if(LL_USART_IsActiveFlag_TXE(USART1) == SET)
        {
            tx_bufferx_one(USART1, NUM_UART1);
            irq_handler(serial_irq_ids[NUM_UART1], TxIrq);
            LL_USART_IsActiveFlag_TXE(USART1);
        }
    }
#endif


#if USE_UART2
    void USART2_IRQHandler(void)
    {
        if(LL_USART_IsActiveFlag_RXNE(USART2) == SET)
        {
            rx_buffer_one(USART2, NUM_UART2);
            irq_handler(serial_irq_ids[NUM_UART2], RxIrq);
            // 如果回调函数中没有读取数据，则将当前数据抛弃，准备下一次接收
            LL_USART_RequestRxDataFlush(USART2);
        }

        if(LL_USART_IsActiveFlag_TXE(USART2) == SET)
        {
            tx_bufferx_one(USART2, NUM_UART2);
            irq_handler(serial_irq_ids[NUM_UART2], TxIrq);
            // 清除发送结束中断标志
            //            LL_USART_IsActiveFlag_TXE(USART2);
        }
    }
#endif
    
#if USE_UART3
    void USART3_IRQHandler(void)
    {
        if(LL_USART_IsActiveFlag_RXNE(USART3) == SET)
        {
            rx_buffer_one(USART3, NUM_UART3);
            irq_handler(serial_irq_ids[NUM_UART3], RxIrq);
            // 如果回调函数中没有读取数据，则将当前数据抛弃，准备下一次接收
            LL_USART_RequestRxDataFlush(USART3);
        }

        if(LL_USART_IsActiveFlag_TXE(USART3) == SET)
        {
            tx_bufferx_one(USART3, NUM_UART3);
            irq_handler(serial_irq_ids[NUM_UART3], TxIrq);
            // 清除发送结束中断标志
            //            LL_USART_IsActiveFlag_TXE(USART2);
        }
    }
#endif

    void serial_irq_handler(uint8_t index, uart_irq_handler handler, uint32_t id)
    {
        irq_handler = handler;
        serial_irq_ids[index] = id;
    }
}




