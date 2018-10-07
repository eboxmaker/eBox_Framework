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
 *  �¼ܹ��Ĵ�������
 *  1�����շ�ʽ��
 *     A��DMA+���λ�����(Ĭ��ģʽ)���ڹر������жϵ��������Ȼ��������ʹ�ã����Ǵ���ģʽÿһ������
 *        ����ռһ��DMAͨ�����û���Ҫע���κ�ʱ��Ҫʹ�ô���ʹ�õ�DMAͨ����������ʹ�����֮������
 *        �Ļָ�DMA���������ú�CNTRֵ��
 *     B���ж�+���λ��������û����ܹر�ȫ���жϺͽ����жϷ����޷���������
 *  2�����ͷ�ʽ���ж�+���λ��������û�д��ʵ�����򻺳�����д�����ݣ�Ȼ����
 *      TX�жϣ���Ƭ�����Զ������������ݴ����ȥֱ����ɣ����ر�TX�жϡ�
 *      A���ر�ȫ���жϣ�����flush������Ȼ������ȷ�Ľ�������������ɣ����ر�TX�жϡ�
 *         ���������flush��������Ῠ�ڻ������С����ǲ�δ��ʧ�����û������㹻��
 *         ������ʹ��������֮���û��ٷ����µ����ݣ��������ɵ����ݽ��ᱻ�Դ˷���
 *         ��ȥ�����ǻ�����һֱ����N�����ݿ������棬ֱ������A��ǰ���������ߵ���flush
 *         ��ᷢ�������������
 *      B���������жϷ����������η��������ȼ����ڻ��ߵ��ڴ������ȼ�����º�Aһ����
 *         ���η����������ȼ����ڴ���ʱ�Դ�����Ӱ��
 *      C�������TX�жϱ��رգ�����û�������û�з����������������ر���TX�жϣ����
 *         ���»��λ������������棬�û�����ͨ���ٴε���д�뺯�����¿�ʼ���䣬���ߵ���
 *         flush����ɡ�
 *   3�����������ڴ���ö�̬���䷽ʽ��      
 *   4����ȡ���û���Ҫ��ĳ��ѭ����ͨ��available�жϻ������ж��ٸ��ֽ���Ҫ��ȡ��Ȼ��һ��
 *            �Զ�ȡ��ѭ��n��read���� ���������ݻ���һ��һ���Ķ�ȥ��һ���ж�available
 *            ����һ��read���� ����
 *            ���գ����û���ȡ���ݵ��ٶȱȽ��յ���������������������ܻᷢ�����ݶ�ʧ���µ�
 *            ������Ḳ���ϵ�����
 *   5��д�룺�û��ɵ���Print�ṩ������API�ӿڽ���д�룬д��֮ǰ����ͨ��availableForWrite�ж�
 *            ���������ж����ֽڿ���д�룬���������ɵ���flush��ˢһ�Σ����������ϵͳ��Ȼ�ܹ�
 *            ������д����������
 *   6��RxIrq�жϣ�RxIrq�ж���DMA+���λ������Ľ��շ�ʽ�£��޷�ʹ�ã����ж�+���λ������Ľ��շ�ʽ�¿���
 *                 ����ʹ�á�
 *   6��TxIrq�жϣ�ÿ����һ���ֽ��ж�һ�Σ��û���������ʹ�ã������鲻Ҫ�ڻص���������д����ռ��ʱ��
 *                 �ĺ��������û�����flush�����ڷ������ֶ�ǿ��ִ�еģ���ᶪʧ�˲����жϣ�����û�   
 *                 д�������������availableForWriteʱ��Ҳ�ᶪʧ�����жϡ������������ʹ�ã����Խ���
 *                 �û�����Ҫʹ��TxIrq�жϵ�����£���Ҫ����flush����Ҫд�����availableForWrite����
 *                 �����������ȫ��֤�жϲ��ᶪʧ��
 */


//�û�����//////////////

#define USE_UART1 1
#define USE_UART2 1
#define USE_UART3 1
#define USE_UART4 0
#define USE_UART5 0
#define USE_UART6 0
#define USE_UART7 0
#define USE_UART8 0
#define UART_NUM (USE_UART1 + USE_UART2 + USE_UART3 + USE_UART4 + USE_UART5 + USE_UART6 + USE_UART7 + USE_UART8)


enum IrqType {
    RxIrq = 0,
    TxIrq = 1
};

typedef enum  {
    RxDMA = 0,
    RxIt = 1,
}RxMode_t;

enum Uart_It_Index{
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

class Uart:public Stream
{
public:
    Uart(USART_TypeDef *USARTx, Gpio *tx_pin, Gpio *rx_pin, uint16_t tx_buffer_size = 128, uint16_t rx_buffer_size = 256);

    //initial uart
    void    begin(uint32_t baud_rate,RxMode_t mode = RxDMA);
    void    begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit,RxMode_t mode = RxDMA);
    void    end();
    void    nvic(FunctionalState enable, uint8_t preemption_priority = 3, uint8_t sub_priority = 3);


    virtual int     available();
    virtual int     peek(void);
    virtual int     read();
    virtual int     availableForWrite();
    virtual void    flush();
    virtual size_t  write(uint8_t c);
    inline  size_t  write(unsigned long n) { return write((uint8_t)n); }
    inline  size_t  write(long n) { return write((uint8_t)n); }
    inline  size_t  write(unsigned int n) { return write((uint8_t)n); }
    inline  size_t  write(int n) { return write((uint8_t)n); }
//    virtual size_t  write(const uint8_t *buffer, size_t size);
    using       Print::write;



    /** Attach a function to call whenever a serial interrupt is generated
     *
     *  @param fptr A pointer to a void function, or 0 to set as none
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    //attach user event
    void attach(void (*fptr)(void), IrqType type);
    void interrupt(IrqType type,FunctionalState enable);

//    void printf(const char *fmt, ...);

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
    Gpio                *_tx_pin;
    Gpio                *_rx_pin;
    uint8_t             gpio_af_usart;
    uint16_t            rx_buffer_size;
    uint16_t            tx_buffer_size;

    RxMode_t            mode;
    Dma                 *dma_rx;
    uint32_t            dma_channel;
    uint8_t             index;
    uint8_t             preemption_priority;
    uint8_t             sub_priority;

protected:
    FunctionPointer _irq[2];
};


#ifdef __cplusplus
extern "C" {
#endif
    void tx_bufferx_one(USART_TypeDef* uart,uint8_t uart_num);

void serial_irq_handler(uint8_t index, uart_irq_handler handler, uint32_t id);
#ifdef __cplusplus
}
#endif
	
#endif
