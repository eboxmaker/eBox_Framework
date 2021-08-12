/**
  ******************************************************************************
  * @file    uart.h
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __UARTX_H
#define __UARTX_H

#include "stream.h"
#include "mcu.h"
#include "FunctionPointer.h"
#include "dma.h"

/**
 * Modification History:
 * -shentq                  -version 2.1(2018/9/23)
 *  新架构的串口特性
 *  1、接收方式：
 *     A、DMA+环形缓冲区(默认模式)，在关闭所有中断的情况下依然可以正常使用，但是此种模式每一个串口
 *        将独占一个DMA通道，用户需要注意任何时候不要使用串口使用的DMA通道，除非你使用完成之后完整
 *        的恢复DMA的所有设置和CNTR值。
 *     B、中断+环形缓冲区，用户不能关闭全局中断和接收中断否则将无法接收数据
 *  2、发送方式：中断+环形缓冲区，用户写入实际是向缓冲区中写入数据，然后开启
 *      TX中断，单片机将自动将缓冲区内容传输出去直到完成，并关闭TX中断。
 *      A、关闭全局中断：调用flush函数依然可以正确的将缓冲区传输完成，并关闭TX中断。
 *         如果不调用flush，数据则会卡在缓冲区中。但是并未丢失，当用户发送足够多
 *         的数据使缓冲区满之后，用户再发送新的数据，缓冲区旧的数据将会被以此发送
 *         出去，但是缓冲区一直会有N个数据卡在里面，直到摆脱A的前提条件或者调用flush
 *         则会发送完成所有数据
 *      B、在其他中断服务函数：当次服务函数优先级高于或者等于串口优先级情况下和A一样，
 *         当次服务函数；优先级低于串口时对串口无影响
 *      C、本身的TX中断被关闭：如果用户在数据没有发送完的情况下主动关闭了TX中断，则会
 *         导致环形缓冲区卡在里面，用户可以通过再次调用写入函数重新开始传输，或者调用
 *         flush来完成。
 *   3、缓冲区的内存采用动态分配方式。
 *   4、读取：用户需要在某个循环中通过available判断缓冲区有多少个字节需要读取，然后一次
 *            性读取（循环n次read函数 ）所有数据或者一个一个的读去（一次判断available
 *            调用一次read函数 ），
 *            风险：当用户读取数据的速度比接收的数据慢的情况发生，可能会发生数据丢失，新的
 *            数据则会覆盖老的数据
 *   5、写入：用户可调用Print提供的所有API接口进行写入，写入之前可以通过availableForWrite判断
 *            缓冲区还有多少字节可以写入，如果不够则可调用flush冲刷一次，如果不调用系统依然能够
 *            完整的写入所有数据
 *   6、RxIrq中断：RxIrq中断在DMA+环形缓冲区的接收方式下，无法使用，在中断+环形缓冲区的接收方式下可以
 *                 正常使用。
 *   6、TxIrq中断：每发送一个字节中断一次，用户可以正常使用，但建议不要在回调函数中填写过于占用时间
 *                 的函数。当用户调用flush后，由于发送是手动强制执行的，则会丢失此部分中断，如果用户
 *                 写入的数据量大于availableForWrite时，也会丢失部分中断。其他情况正常使用，所以建议
 *                 用户再需要使用TxIrq中断的情况下，不要调用flush，不要写入大于availableForWrite的数
 *                 据量，则可完全保证中断不会丢失。
 */


//用户配置//////////////

#define USE_UART1 1
#define USE_UART2 1
#define USE_UART3 1
#define USE_UART4 0
#define USE_UART5 0
#define USE_UART6 0
#define USE_UART7 0
#define USE_UART8 0
#define UART_NUM (USE_UART1 + USE_UART2 + USE_UART3 + USE_UART4 + USE_UART5 + USE_UART6 + USE_UART7 + USE_UART8)


#define UART_MAX_BITS   8

#if (UART_MAX_BITS > 8)
#define UART_9_BIT
#endif

enum IrqType
{
    RxIrq = 0,
    TxIrq = 1
};

typedef enum
{
    RxDMA = 0,
    RxIt = 1,
} RxMode_t;

enum Uart_It_Index
{
    NUM_UART1  = 0,
    NUM_UART2  = 1,
    NUM_UART3  = 2,
    NUM_UART4  = 3,
    NUM_UART5  = 4,
    NUM_UART6  = 5,
    NUM_UART7  = 6,
    NUM_UART8  = 7,
} ;

typedef void (*uart_irq_handler)(uint32_t id, IrqType type);

class Uart: public Stream
{
public:
    Uart(USART_TypeDef *USARTx, Gpio *tx_pin, Gpio *rx_pin, uint16_t tx_buffer_size = 128, uint16_t rx_buffer_size = 256);

    //initial uart
    void    begin(uint32_t baud_rate, RxMode_t mode = RxDMA);
    void    begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit, RxMode_t mode = RxDMA);
    void    end();
    void    nvic(FunctionalState enable, uint8_t preemption_priority = 3, uint8_t sub_priority = 3);


    virtual int     available();
    virtual int     peek(void);
    virtual int     read();
    virtual int     availableForWrite();
    virtual void    flush();
    virtual size_t  write(uint8_t c);
    inline  size_t  write(unsigned long n)
    {
        return write((uint8_t)n);
    }
    inline  size_t  write(long n)
    {
        return write((uint8_t)n);
    }
    inline  size_t  write(unsigned int n)
    {
        return write((uint8_t)n);
    }
    inline  size_t  write(int n)
    {
        return write((uint8_t)n);
    }
    //    virtual size_t  write(const uint8_t *buffer, size_t size);
    using       Print::write;



    /** Attach a function to call whenever a serial interrupt is generated
     *
     *  @param fptr A pointer to a void function, or 0 to set as none
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    //attach user event
    void attach(void (*fptr)(void), IrqType type);
    void interrupt(IrqType type, FunctionalState enable);

    //    void printf(const char *fmt, ...);

    /** Attach a member function to call whenever a serial interrupt is generated
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void), IrqType type)
    {
        if((mptr != NULL) && (tptr != NULL))
        {
            _irq[type].attach(tptr, mptr);
        }
    }

    static void _irq_handler(uint32_t id, IrqType irq_type);



private:
    USART_TypeDef       *_USARTx;
    Gpio                *_tx_pin;
    Gpio                *_rx_pin;
    uint8_t             gpio_af_usart;
    uint16_t            rx_buffer_size;
    uint16_t            tx_buffer_size;

    RxMode_t            _mode;
    Dma                 *dma_rx;
    uint32_t            dma_channel;
    uint8_t             _index;
    uint8_t             preemption_priority;
    uint8_t             sub_priority;

protected:
    FunctionPointer _irq[2];
};


#ifdef __cplusplus
extern "C" {
#endif
void tx_bufferx_one(USART_TypeDef *uart, uint8_t uart_num);

void serial_irq_handler(uint8_t index, uart_irq_handler handler, uint32_t id);
#ifdef __cplusplus
}
#endif

#endif
