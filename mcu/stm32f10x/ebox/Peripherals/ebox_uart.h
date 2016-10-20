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
#include "FunctionPointer.h"

/*
	1.֧�ִ���1,2,3,4,5��
	2.֧��һ���ж��¼� rx_it
	3.����ģʽ����DMA�Զ�����ģʽ������ʡcpuռ�á�
	4.֧��ǿ���printf
	5.��ʱ��֧�����ŵ�remap
    6.֧�ֶ�̬�����ڴ���ͷ��ڴ档
    ע�⣺
        ����4��5û��ʹ��DMA����ʹ��ʼ��ʱʹ��DMA��Ҳ��������κ�Ӱ��
        ���ڷ��ͣ�������no_interrupt������ִ�����Ρ�
*/
/**
 * Modification History:
 * -shentq                  -version 2.0(2016/10/19)
 *  *����ʹ�ö�̬�ڴ�
 * -shentq                  -version 2.0(2016/10/20)
 *  *�޸�һ����̬�ڴ��������
 *  *�޸�ʹ��DMA���÷�ʽ��ɾ���궨�����÷�ʽ�����ó�ʼ����������
 *  *���ӳ�ʼ����һ�ֽӿڣ�֧�ָ����ӵĳ�ʼ��Ҫ��
 */



//�û�����//////////////
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

class Uart
{
public:
    Uart(USART_TypeDef *USARTx, Gpio *tx_pin, Gpio *rx_pin);

    void    begin(uint32_t baud_rate,uint8_t _use_dma = 1);
    void    begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit,uint8_t _use_dma);

    int 	put_char(uint16_t ch);
    void    printf_length(const char *str, uint16_t length);
    void    printf(const char *fmt, ...); //��Ҫע�⻺�������
    void    wait_busy();

    uint16_t    receive();

    /** Attach a function to call whenever a serial interrupt is generated
     *
     *  @param fptr A pointer to a void function, or 0 to set as none
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
     */
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
    void                put_string(const char *str, uint16_t length);
    void                set_busy();
    void                interrupt(FunctionalState enable);

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
