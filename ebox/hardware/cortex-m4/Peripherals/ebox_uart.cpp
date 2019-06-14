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
    uint8_t             gpio_af_usart;

    USART_InitTypeDef   USART_InitStructure;

    rcc_clock_cmd((uint32_t)_USARTx, ENABLE);
    this->mode = mode;
    switch((uint32_t)_USARTx)
    {
#if USE_UART1
    case (uint32_t)USART1_BASE:
        gpio_af_usart   = GPIO_AF_USART1;
        index           = NUM_UART1;
        dma_rx          = new Dma(DMA2_Stream2);//&Dma2Stream2;//Dma2Stream5
        dma_channel     = DMA_Channel_4;
        break;
#endif

#if USE_UART2
    case (uint32_t)USART2_BASE:
        gpio_af_usart   = GPIO_AF_USART2;
        index           = NUM_UART2;
        dma_rx          = new Dma(DMA1_Stream5);//&Dma1Stream5;
        dma_channel     = DMA_Channel_4;
        break;
#endif

#if USE_UART3
    case (uint32_t)USART3_BASE:
        gpio_af_usart   = GPIO_AF_USART3;
        index           = NUM_UART3;
        dma_rx          = new Dma(DMA1_Stream1);//&Dma1Stream1;
        dma_channel     = DMA_Channel_4;
        break;
#endif

#if USE_UART4
    case (uint32_t)UART4_BASE:
        gpio_af_usart   = GPIO_AF_UART4;
        index           = NUM_UART4;
        dma_rx          = new Dma(DMA1_Stream2);//&Dma1Stream2;
        dma_channel     = DMA_Channel_4;
        break;
#endif

#if USE_UART5
    case (uint32_t)UART5_BASE:
        gpio_af_usart   = GPIO_AF_UART5;
        index           = NUM_UART5;
        dma_rx          = new Dma(DMA1_Stream0);//&Dma1Stream0;
        dma_channel     = DMA_Channel_4;
        break;
#endif

#if USE_UART6
    case (uint32_t)USART6_BASE:
        gpio_af_usart   = GPIO_AF_USART6;
        index           = NUM_UART6;
        dma_rx          = new Dma(DMA2_Stream2);//&Dma2Stream1;//Dma2Stream2
        dma_channel     = DMA_Channel_5;
        break;

#endif
#if defined(STM32F427_437xx) || defined(STM32F429_439xx)
#if USE_UART7
    case (uint32_t)UART7_BASE:
        gpio_af_usart   = GPIO_AF_UART7;
        index           = NUM_UART7;
        dma_rx          = new Dma(DMA1_Stream3);//&Dma1Stream3;
        dma_channel     = DMA_Channel_5;
        break;
#endif

#if USE_UART8
    case (uint32_t)UART8_BASE:
        gpio_af_usart   = GPIO_AF_UART8;
        index           = NUM_UART8;
        dma_rx          = new Dma(DMA1_Stream6);//&Dma1Stream6;
        dma_channel     = DMA_Channel_5;
        break;
#endif
#endif
    }
    _tx_buffer_size[index] = tx_buffer_size;
    _rx_buffer_size[index] = rx_buffer_size;

#ifdef UART_9_BIT
    _tx_ptr[index] = (uint16_t *)ebox_malloc(_tx_buffer_size[index] * sizeof(uint16_t));
    _rx_ptr[index] = (uint16_t *)ebox_malloc(_rx_buffer_size[index] * sizeof(uint16_t));
#else
    _tx_ptr[index] = (uint8_t *)ebox_malloc(_tx_buffer_size[index] * sizeof(uint8_t));
    _rx_ptr[index] = (uint8_t *)ebox_malloc(_rx_buffer_size[index] * sizeof(uint8_t));
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


    if((mode == RxDMA) && (dma_rx != NULL))
    {
        USART_DMACmd(_USARTx, USART_DMAReq_Rx, ENABLE);
        dma_rx->rcc_enable();
        dma_rx->nvic(DISABLE, 0, 0);
        dma_rx->interrupt(DmaItTc, DISABLE);
        dma_rx->interrupt(DmaItTe, DISABLE);
        dma_rx->interrupt(DmaItHt, DISABLE);

        DMA_InitTypeDef DMA_InitStructure;

        dma_rx->deInit();


        DMA_InitStructure.DMA_Channel = dma_channel; /////////////////////
        //外设地址
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&_USARTx->DR);/////////////////
        //内存地址
        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)_rx_ptr[index];//////////////////////////
        //dma传输方向
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
        //设置DMA在传输时缓冲区的长度
        DMA_InitStructure.DMA_BufferSize = _rx_buffer_size[index];///////////////////////////////
        //设置DMA的外设递增模式，一个外设
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        //设置DMA的内存递增模式
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
#ifdef UART_9_BIT
        //外设数据字长
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        //内存数据字长
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
#else
        //外设数据字长
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        //内存数据字长
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
#endif
        //设置DMA的传输模式
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
        //设置DMA的优先级别
        DMA_InitStructure.DMA_Priority = DMA_Priority_High;

        //指定如果FIFO模式或直接模式将用于指定的流 ： 不使能FIFO模式
        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
        //指定了FIFO阈值水平
        DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
        //指定的Burst转移配置内存传输
        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
        //指定的Burst转移配置外围转移 */
        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;


        dma_rx->init(&DMA_InitStructure);
        dma_rx->enable();
    }

    USART_Cmd(_USARTx, ENABLE);

    nvic(ENABLE, 0, 0);
    interrupt(RxIrq, ENABLE);
    interrupt(TxIrq, DISABLE);

    _tx_pin->mode(AF_PP, gpio_af_usart);
    _rx_pin->mode(AF_PP, gpio_af_usart);
}
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
    nvic_dev_set_priority((uint32_t)_USARTx, 0, preemption_priority, sub_priority);
    if(enable != DISABLE)
        nvic_dev_enable((uint32_t)_USARTx, 0);
    else
        ;
    //        nvic_dev_disable((uint32_t)_USARTx,0);//禁止关闭
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
    if ((_rx_buffer_size[index] - dma_rx->DMAy_Streamx->NDTR) == _rx_buffer_tail[index])
    {
        return -1;
    }
    else
    {
        return _rx_ptr[index][_rx_buffer_tail[index]];
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
        return ((unsigned int)(_rx_buffer_size[index] + (_rx_buffer_size[index] - dma_rx->DMAy_Streamx->NDTR) - _rx_buffer_tail[index])) % _rx_buffer_size[index];
    else
        return ((unsigned int)(_rx_buffer_size[index] + _rx_buffer_head[index] - _rx_buffer_tail[index])) % _rx_buffer_size[index];
}

int Uart::read()
{
    if(mode == RxDMA)
    {
        if (_rx_buffer_tail[index] == (_rx_buffer_size[index] - dma_rx->DMAy_Streamx->NDTR))
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
    {
        int c = _rx_ptr[index][_rx_buffer_tail[index]];
        _rx_buffer_tail[index] = (_rx_buffer_tail[index] + 1) % _rx_buffer_size[index];
        return c;
    }
}


int Uart::availableForWrite()
{

    uint16_t head = _tx_buffer_head[index];
    uint16_t tail = _tx_buffer_tail[index];

    if (head >= tail)
        return _tx_buffer_size[index] - 1 - head + tail;

    return tail - head - 1;
}


void Uart::flush()
{
    uint8_t major, minor;
    while(_tx_buffer_head[index] != _tx_buffer_tail[index] )
    {
        //如果全局中断被关闭,发送使能中断被关闭、在其他中断函数中,则手动发送
        //        if(__get_PRIMASK() || ((_USARTx->CR1 & USART_FLAG_TXE) == 0))
        if(__get_PRIMASK() || ((_USARTx->CR1 & USART_FLAG_TXE) == 0) || (__get_IPSR() != 0))
        {
            nvic_irq_get_priority((IRQn_Type)__get_IPSR(), &major, &minor);
            if(major <= preemption_priority )//串口优先级低于或等于所在中断服务函数
            {
                //                interrupt(TxIrq,DISABLE);//期间必须关闭串口中断
                while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//单字节等待，等待寄存器空
                tx_bufferx_one(_USARTx, index);
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

    uint16_t i = (_tx_buffer_head[index] + 1) % _tx_buffer_size[index];//计算头的位置

    while (i == _tx_buffer_tail[index]) //如果环形缓冲区满了就调用一次发送
    {
        interrupt(TxIrq, DISABLE); //期间必须关闭串口中断
        while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//单字节等待，等待寄存器空
        tx_bufferx_one(_USARTx, index);
        interrupt(TxIrq, ENABLE); //开启发送
    }

    _tx_ptr[index][_tx_buffer_head[index]] = c;
    _tx_buffer_head[index] = i;

    interrupt(TxIrq, ENABLE); //开启发送

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
    void tx_bufferx_one(USART_TypeDef *uart, uint8_t index)
    {
        if (_tx_buffer_head[index] == _tx_buffer_tail[index])//如果发送完所有数据
        {
            // Buffer empty, so disable interrupts
            USART_ITConfig(uart, USART_IT_TXE, DISABLE);
            return;
        }
        unsigned char c = _tx_ptr[index][_tx_buffer_tail[index]];
        _tx_buffer_tail[index] = (_tx_buffer_tail[index] + 1) % _tx_buffer_size[index];
        uart->DR = (c & (uint16_t)0x01FF);

    }
    void rx_buffer_one(USART_TypeDef *uart, uint8_t index)
    {
        uint16_t i = (_rx_buffer_head[index] + 1) % _rx_buffer_size[index];//计算头的位置
        if(i == _rx_buffer_tail[index]) //如果环形缓冲区满了就修改一次tail，将会舍弃最老的一个数据
        {
            _rx_buffer_tail[index] = (_rx_buffer_tail[index] + 1) % _rx_buffer_size[index];

        }
        _rx_ptr[index][_rx_buffer_head[index]] = uart->DR;
        _rx_buffer_head[index] = i;

    }

#if USE_UART1

    void USART1_IRQHandler(void)
    {
        if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
        {
            rx_buffer_one(USART1, NUM_UART1);
            irq_handler(serial_irq_ids[NUM_UART1], RxIrq);
            USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART1, USART_IT_TXE) == SET)
        {
            tx_bufferx_one(USART1, NUM_UART1);
            irq_handler(serial_irq_ids[NUM_UART1], TxIrq);
        }
    }
#endif

#if USE_UART2
    void USART2_IRQHandler(void)
    {
        if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
        {
            rx_buffer_one(USART2, NUM_UART2);
            irq_handler(serial_irq_ids[NUM_UART2], RxIrq);
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART2, USART_IT_TXE) == SET)
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

    void USART6_IRQHandler(void)
    {
        if(USART_GetITStatus(USART6, USART_IT_RXNE) == SET)
        {
            rx_buffer_one(USART6, NUM_UART6);
            irq_handler(serial_irq_ids[NUM_UART6], RxIrq);
            USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART6, USART_IT_TXE) == SET)
        {
            tx_bufferx_one(USART6, NUM_UART3);
            irq_handler(serial_irq_ids[NUM_UART6], TxIrq);
        }
    }

#if defined(STM32F427_437xx) || defined(STM32F429_439xx)
#if USE_UART7
    void UART7_IRQHandler(void)
    {
        if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
        {
            rx_buffer_one(UART7, NUM_UART7);
            irq_handler(serial_irq_ids[NUM_UART7], RxIrq);
            USART_ClearITPendingBit(UART7, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART7, USART_IT_TXE) == SET)
        {
            tx_bufferx_one(UART7, NUM_UART7);
            irq_handler(serial_irq_ids[NUM_UART7], TxIrq);
        }
    }
#endif
#if USE_UART8
    void UART8_IRQHandler(void)
    {
        if(USART_GetITStatus(UART8, USART_IT_RXNE) == SET)
        {
            rx_buffer_one(UART8, NUM_UART8);
            irq_handler(serial_irq_ids[NUM_UART8], RxIrq);
            USART_ClearITPendingBit(UART8, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART8, USART_IT_TXE) == SET)
        {
            tx_bufferx_one(UART8, NUM_UART8);
            irq_handler(serial_irq_ids[NUM_UART8], TxIrq);
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




