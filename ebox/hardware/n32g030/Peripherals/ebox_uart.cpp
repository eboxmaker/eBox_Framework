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
 *@name     Uart::Uart(USART_TypeDef *USARTx,Gpio *tx_pin,Gpio *rx_pin)
 *@brief    串口的构造函数
 *@param    USARTx:  USART1,2,3和UART4,5
 *          tx_pin:  外设所对应的tx引脚
 *          rx_pin:  外设所对应的rx引脚
 *@retval   None
*/
Uart::Uart(USART_Module *USARTx, Gpio *tx_pin, Gpio *rx_pin, uint16_t tx_buffer_size, uint16_t rx_buffer_size)
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
    USART_InitType   USART_InitStructure;

    rcc_clock_cmd((uint32_t)_USARTx, ENABLE);
    this->mode = mode;


    switch((uint32_t)_USARTx)
    {
#if USE_UART1
    case (uint32_t)USART1_BASE:
        dma_rx = new Dma(DMA_CH5);//&Dma1Ch5;
        index = NUM_UART1;
        break;
#endif

#if USE_UART2
    case (uint32_t)USART2_BASE:
        dma_rx = new Dma(DMA_CH3);//&Dma1Ch6;
        index = NUM_UART2;
        break;
#endif

#if USE_UART3
    case (uint32_t)USART3_BASE:
        dma_rx = new Dma(DMA1_Channel3);//&Dma1Ch3;
        index = NUM_UART3;
        break;
#endif

#if defined (STM32F10X_HD)
#if USE_UART4
    case (uint32_t)UART4_BASE:
        index = NUM_UART4;
        this->mode = RxIt;
        break;
#endif

#if USE_UART5
    case (uint32_t)UART5_BASE:
        index = NUM_UART5;
        this->mode = RxIt;
        break;
#endif
#endif
    }

    _tx_buffer_size[index] = tx_buffer_size;
    _rx_buffer_size[index] = rx_buffer_size;

#ifdef UART_9_BIT
    _tx_ptr[index] = (uint16_t *)rt_malloc(_tx_buffer_size[index] * sizeof(uint16_t));
    _rx_ptr[index] = (uint16_t *)rt_malloc(_rx_buffer_size[index] * sizeof(uint16_t));
#else
    _tx_ptr[index] = (uint8_t *)ebox_malloc(_tx_buffer_size[index] * sizeof(uint8_t));
    _rx_ptr[index] = (uint8_t *)ebox_malloc(_rx_buffer_size[index] * sizeof(uint8_t));
#endif



    serial_irq_handler(index, Uart::_irq_handler, (uint32_t)this);


    USART_InitStructure.BaudRate = baud_rate;
    switch(data_bit)
    {
    case 8:
        USART_InitStructure.WordLength = USART_WL_8B;
        break;
    case 9:
        USART_InitStructure.WordLength = USART_WL_9B;
        break;
    default :
        USART_InitStructure.WordLength = USART_WL_8B;
        break;
    }
    switch(parity)
    {
    case 0:
        USART_InitStructure.Parity = USART_PE_NO;
        break;
    case 1:
        USART_InitStructure.Parity = USART_PE_EVEN;
        break;
    case 2:
        USART_InitStructure.Parity = USART_PE_ODD;
        break;
    default :
        USART_InitStructure.Parity = USART_PE_NO;
        break;
    }
    if(stop_bit == 0.5)
        USART_InitStructure.StopBits = USART_STPB_0_5;
    else if(stop_bit == 1)
        USART_InitStructure.StopBits = USART_STPB_1;
    else if(stop_bit == 1.5)
        USART_InitStructure.StopBits = USART_STPB_1_5;
    else if(stop_bit == 2)
        USART_InitStructure.StopBits = USART_STPB_2;

    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode = USART_MODE_RX | USART_MODE_TX;
    USART_Init(_USARTx, &USART_InitStructure);


    if((mode == RxDMA) && (dma_rx != NULL))
    {
        USART_EnableDMA(_USARTx, USART_DMAREQ_RX, ENABLE);
        dma_rx->rcc_enable();
        dma_rx->nvic(DISABLE, 0, 0);
        dma_rx->interrupt(DmaItTc, DISABLE);
        dma_rx->interrupt(DmaItTe, DISABLE);
        dma_rx->interrupt(DmaItHt, DISABLE);

        DMA_InitType DMA_InitStructure;

        dma_rx->deInit();

        DMA_InitStructure.PeriphAddr = (uint32_t)&_USARTx->DAT;
        DMA_InitStructure.MemAddr = (uint32_t) _rx_ptr[index];
        DMA_InitStructure.Direction = DMA_DIR_PERIPH_SRC;
        DMA_InitStructure.BufSize = _rx_buffer_size[index];
        DMA_InitStructure.PeriphInc = DMA_PERIPH_INC_DISABLE;
        DMA_InitStructure.DMA_MemoryInc = DMA_MEM_INC_ENABLE;
#ifdef UART_9_BIT
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
#else
        DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_BYTE;
        DMA_InitStructure.MemDataSize = DMA_MemoryDataSize_Byte;
#endif
        DMA_InitStructure.CircularMode = DMA_MODE_CIRCULAR;
        DMA_InitStructure.Priority = DMA_PRIORITY_HIGH;
        DMA_InitStructure.Mem2Mem = DMA_M2M_DISABLE;
        dma_rx->init(&DMA_InitStructure);
        DMA_RequestRemap(DMA_REMAP_USART1_RX, DMA, dma_rx->DMAy_Channelx, ENABLE);

        dma_rx->enable();
    }

    USART_Enable(_USARTx,ENABLE);
    _tx_pin->mode(AF_PP,GPIO_AF4_USART1);
    _rx_pin->mode(AF_PP,GPIO_AF4_USART1);

    nvic(ENABLE, 0, 0);
    interrupt(RxIrq, ENABLE);
    interrupt(TxIrq, DISABLE);

    _is_inited = true;
}

/**
 *@brief    复位串口，并清空RX缓冲区
 *@param    none
 *@retval   None
*/
void Uart::end()
{
    USART_DeInit(_USARTx);
    // clear any received data
    _rx_buffer_head[index] = _rx_buffer_tail[index];
}
void Uart::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
    this->preemption_priority = preemption_priority;
    this->sub_priority = sub_priority;
//    nvic_dev_set_priority((uint32_t)_USARTx, 0, preemption_priority, sub_priority);
//    if(enable != DISABLE)
//        nvic_dev_enable((uint32_t)_USARTx, 0);
//    else
//        nvic_dev_disable((uint32_t)_USARTx, 0);
    NVIC_InitType NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel          = dev_to_irqn((uint32_t)_USARTx,0);
    NVIC_InitStructure.NVIC_IRQChannelPriority  = preemption_priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd       = enable;
    NVIC_Init(&NVIC_InitStructure);
}
///////////////////////////////////////////////////////////////

/**
 *@brief    串口中断控制函数
 *@param    enable:  ENABLE使能串口的发送完成和接收中断；DISABLE：关闭这两个中断
 *@retval   None
*/
void Uart::interrupt(IrqType type, FunctionalState enable)
{

    if(type == RxIrq)
        USART_ConfigInt(_USARTx, USART_INT_RXDNE, ENABLE);//接收中断不能关闭，
    if(type == TxIrq)
        USART_ConfigInt(_USARTx, USART_INT_TXDE, enable);
}

/**
 *@brief    相当与read，但读取后不从缓冲区清除数据
 *@param    NONE
 *@retval   返回tail指向的数据
*/
int Uart::peek(void)
{
    if ((_rx_buffer_size[index] - dma_rx->DMAy_Channelx->TXNUM) == _rx_buffer_tail[index])
    {
        return -1;
    }
    else
    {
        return _rx_ptr[index][_rx_buffer_tail[index]];
    }
}
/**
 *@brief    判断接收缓冲区是否有数据
 *@param    NONE
 *@retval   0 无数据，非0，当前缓冲区数据长度
*/
int Uart::available()
{
    if(mode == RxDMA)
        return ((unsigned int)(_rx_buffer_size[index] + (_rx_buffer_size[index] - dma_rx->DMAy_Channelx->TXNUM) - _rx_buffer_tail[index])) % _rx_buffer_size[index];
    else
        return ((unsigned int)(_rx_buffer_size[index] + _rx_buffer_head[index] - _rx_buffer_tail[index])) % _rx_buffer_size[index];
}

/**
 *@brief    读取一个字节，并清除当前数据
 *@param    NONE
 *@retval   -1  无数据，其他，当前数据
*/
int Uart::read()
{
    if(mode == RxDMA)
    {
        if (_rx_buffer_tail[index] == (_rx_buffer_size[index] - dma_rx->DMAy_Channelx->TXNUM))
        {
            return -1;
        }
    }
    else
    {
        if (_rx_buffer_tail[index] == _rx_buffer_head[index])
        {
            return -1;
        }
    }
    int c = _rx_ptr[index][_rx_buffer_tail[index]];
    _rx_buffer_tail[index] = (_rx_buffer_tail[index] + 1) % _rx_buffer_size[index];
    return c;
}

/**
 *@brief    是否可以写入
 *@param    NONE
 *@retval   可以写入的数量
*/
int Uart::availableForWrite()
{
    uint16_t head = _tx_buffer_head[index];
    uint16_t tail = _tx_buffer_tail[index];

    if (head >= tail)
        return _tx_buffer_size[index] - 1 - head + tail;

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
    while(_tx_buffer_head[index] != _tx_buffer_tail[index] )
    {
        //如果全局中断被关闭,发送使能中断被关闭、在其他中断函数中,则手动发送
        //        if(__get_PRIMASK() || ((_USARTx->CR1 & USART_FLAG_TXE) == 0))
        if(__get_PRIMASK() || ((_USARTx->CTRL1 & USART_FLAG_TXDE) == 0) || (__get_IPSR() != 0))
        {
            while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXDE) == RESET);//单字节等待，等待寄存器空
            tx_bufferx_one(_USARTx, index);
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
    if(!_is_inited) return 0;
    uint16_t i = (_tx_buffer_head[index] + 1) % _tx_buffer_size[index];//计算头的位置
    // head = tail, 缓冲区过满，先发送
    while (i == _tx_buffer_tail[index])
    {
        interrupt(TxIrq, DISABLE); //期间必须关闭串口中断
        while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXDE) == RESET);//单字节等待，等待寄存器空
        tx_bufferx_one(_USARTx, index);
        interrupt(TxIrq, ENABLE); //开启发送
    }
    // 加入新数据，移动head
    _tx_ptr[index][_tx_buffer_head[index]] = c;
    _tx_buffer_head[index] = i;

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
    void tx_bufferx_one(USART_Module *uart, uint8_t index)
    {
        if (_tx_buffer_head[index] == _tx_buffer_tail[index])//如果空则直接返回
        {
            USART_ConfigInt(uart, USART_INT_TXDE, DISABLE);// Buffer empty, so disable interrupts
            return;
        }
        unsigned char c = _tx_ptr[index][_tx_buffer_tail[index]];   // 取出字符
        _tx_buffer_tail[index] = (_tx_buffer_tail[index] + 1) % _tx_buffer_size[index];
        uart->DAT = (c & (uint16_t)0x01FF);

    }
    /**
      *@brief    读入一个字符放入缓冲区
      *@param    uart：串口； index 串口 IT 索引
      *@retval   1
      */
    void rx_buffer_one(USART_Module *uart, uint8_t index)
    {
        uint16_t i = (_rx_buffer_head[index] + 1) % _rx_buffer_size[index];//计算头的位置
        if(i == _rx_buffer_tail[index]) //如果环形缓冲区满了就修改一次tail，将会舍弃最老的一个数据
        {
            _rx_buffer_tail[index] = (_rx_buffer_tail[index] + 1) % _rx_buffer_size[index];
        }
        _rx_ptr[index][_rx_buffer_head[index]] = uart->DAT;
        _rx_buffer_head[index] = i;
    }

#if USE_UART1

    void USART1_IRQHandler(void)
    {
        
        if(USART_GetIntStatus(USART1, USART_INT_RXDNE) == SET)
        {
            rx_buffer_one(USART1, NUM_UART1);
            irq_handler(serial_irq_ids[NUM_UART1], RxIrq);
            USART_ClrIntPendingBit(USART1, USART_INT_RXDNE);
        }
        if(USART_GetIntStatus(USART1, USART_INT_TXDE) == SET)
        {
            tx_bufferx_one(USART1, NUM_UART1);
            irq_handler(serial_irq_ids[NUM_UART1], TxIrq);
        }
    }
#endif

#if USE_UART2
    void USART2_IRQHandler(void)
    {
        if(USART_GetIntStatus(USART2, USART_INT_RXDNE) == SET)
        {
            rx_buffer_one(USART2, NUM_UART2);
            irq_handler(serial_irq_ids[NUM_UART2], RxIrq);
            USART_ClrIntPendingBit(USART2, USART_INT_RXDNE);
        }
        if(USART_GetIntStatus(USART2, USART_INT_TXDE) == SET)
        {
            tx_bufferx_one(USART2, NUM_UART2);
            irq_handler(serial_irq_ids[NUM_UART2], TxIrq);
        }
    }
#endif


#if USE_UART3
    void USART3_IRQHandler(void)
    {
        if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
        {
            rx_buffer_one(USART3, NUM_UART3);
            irq_handler(serial_irq_ids[NUM_UART3], RxIrq);
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART3, USART_IT_TXE) == SET)
        {
            tx_bufferx_one(USART3, NUM_UART3);
            irq_handler(serial_irq_ids[NUM_UART3], TxIrq);
        }
    }
#endif


#if defined (STM32F10X_HD)
    #if USE_UART4
    void UART4_IRQHandler(void)
    {
        if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
        {
            rx_buffer_one(UART4, NUM_UART4);
            irq_handler(serial_irq_ids[NUM_UART4], RxIrq);
            USART_ClearITPendingBit(UART4, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART4, USART_IT_TXE) == SET)
        {
            tx_bufferx_one(UART4, NUM_UART4);
            irq_handler(serial_irq_ids[NUM_UART4], TxIrq);
        }
    }
    #endif

    #if USE_UART5

    void UART5_IRQHandler(void)
    {
        if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
        {
            rx_buffer_one(UART5, NUM_UART5);
            irq_handler(serial_irq_ids[NUM_UART5], RxIrq);
            USART_ClearITPendingBit(UART5, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART5, USART_IT_TXE) == SET)
        {
            tx_bufferx_one(UART5, NUM_UART5);
            irq_handler(serial_irq_ids[NUM_UART5], TxIrq);
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




