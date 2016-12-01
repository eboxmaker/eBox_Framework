/**
  ******************************************************************************
  * @file    uart.h
  * @author  shentq
  * @version V2.0
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

#include "ebox_common.h"
#include <stdio.h>
#include <stdarg.h>
#include "print.h"
#include "FunctionPointer.h"

/*
	1.支持串口1,2,3,4,5；
	2.支持一个中断事件 rx_it
	3.发送模式采用DMA自动发送模式，大大节省cpu占用。
	4.支持强大的printf
	5.暂时不支持引脚的remap
    6.支持动态申请内存和释放内存。
    注意：
        串口4，5没有使用DMA；即使初始化时使用DMA，也不会产生任何影响
        串口发送，不能在no_interrupt下连续执行两次。
*/
/**
 * Modification History:
 * -shentq                  -version 2.0(2016/10/19)
 *  *串口使用动态内存
 * -shentq                  -version 2.0(2016/10/20)
 *  *修复一个动态内存申请错误
 *  *修改使用DMA配置方式，删除宏定义配置方式，改用初始化参数配置
 *  *增加初始化的一种接口，支持更复杂的初始化要求
 */



//用户配置//////////////
#define UART_NUM (5)

enum IrqType {
		RxIrq = 0,
		TcIrq
};

enum Uart_It_Index{
    NUM_UART1  = 0,
    NUM_UART2  = 1,
    NUM_UART3  = 2,
    NUM_UART4  = 3,
    NUM_UART5  = 4,	
} ; 

typedef void (*uart_irq_handler)(uint32_t id, IrqType type);

class Uart:public Print
{
public:
    Uart(USART_TypeDef *USARTx, Gpio *tx_pin, Gpio *rx_pin);

    //initial uart
    void    begin(uint32_t baud_rate,uint8_t _use_dma = 1);
    void    begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit,uint8_t _use_dma);

    //write method
    virtual size_t  write(uint8_t c);
    virtual size_t  write(const uint8_t *buffer, size_t size);
    using   Print::write;

    //read method
    uint16_t    read();

    //user addation method
    void    printf(const char *fmt, ...); 
    void    wait_busy();
    /** Attach a function to call whenever a serial interrupt is generated
     *
     *  @param fptr A pointer to a void function, or 0 to set as none
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    //attach user event
    void attach(void (*fptr)(void), IrqType type);
    

    /** Attach a member function to call whenever a serial interrupt is generated
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    template<typename T>
    void attach(T* tptr, void (T::*mptr)(void), IrqType type) {
        if((mptr != NULL) && (tptr != NULL)) {
            _irq[type].attach(tptr, mptr);
        }
    }
		
		static void _irq_handler(uint32_t id, IrqType irq_type);

private:
    USART_TypeDef       *_USARTx;
    DMA_Channel_TypeDef *_DMA1_Channelx;
    char                *uart_buf;
    uint8_t             use_dma;
    uint16_t            dma_send_string(const char *str, uint16_t length);
    void                _write(const char *str, uint16_t length);
    void                set_busy();
    void                interrupt(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);

protected:
    FunctionPointer _irq[2];
};


#ifdef __cplusplus
extern "C" {
#endif

void serial_irq_handler(uint8_t index, uart_irq_handler handler, uint32_t id);
#ifdef __cplusplus
}
#endif
	
#endif
