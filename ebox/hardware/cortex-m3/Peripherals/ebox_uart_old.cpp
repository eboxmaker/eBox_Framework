/**
  ******************************************************************************
  * @file    uart.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief   
  *         2018/8/6  ��Tx pin ���÷��ڴ��ڳ�ʼ��֮�󣬷�ֹ����0x00
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
 *@brief    ���ڵĹ��캯��
 *@param    USARTx:  USART1,2,3��UART4,5
 *          tx_pin:  ��������Ӧ��tx����
 *          rx_pin:  ��������Ӧ��rx����
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
 *@brief    ���ڳ�ʼ�����������˲����ʿɿ��⣬��������Ĭ�ϣ�8bit���ݣ�1��ֹͣλ����У�飬��Ӳ��������
 *@param    baud_rate:  �����ʣ�����9600��115200��38400�ȵ�
 *@param    _use_dma:   �Ƿ�ʹ��DMA��Ĭ��ֵ1��ʹ��DMA��0��������DMA;
 *@retval   None
*/
void    Uart::begin(uint32_t baud_rate,uint8_t use_dma)
{
    Uart::begin(baud_rate,8,0,1,use_dma);
}


/**
 *@name     void    begin(uint32_t baud_rate,uint8_t data_bit,uint8_t parity,float stop_bit,uint8_t _use_dma);
 *@brief    ���ڳ�ʼ�������������и������ò���
 *@param    baud_rate:  �����ʣ�����9600��115200��38400�ȵ�
 *          data_bit:   ����λ����ֻ������8����9
 *          parity:     ����λ��0����У��λ��1��У�飬2żУ��
 *          stop_bit:   ֹͣλ��0.5,1,1.5,2�ĸ���ѡ����
 *          _use_dma:   �Ƿ�ʹ��DMA��Ĭ��ֵ1��ʹ��DMA��0��������DMA;
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
 *@brief    �����жϿ��ƺ���
 *@param    enable:  ENABLEʹ�ܴ��ڵķ�����ɺͽ����жϣ�DISABLE���ر��������ж�
 *@retval   None
*/
void Uart::interrupt(IrqType type, FunctionalState enable)
{

    if(type == RxIrq)
        USART_ITConfig(_USARTx, USART_IT_RXNE, enable);
    if(type == TcIrq)
    {
        USART_ITConfig(_USARTx, USART_IT_TC, enable);//��ֹ�رշ�������ж�
    }
}
/**
 *@name     size_t Uart::write(uint8_t c)
 *@brief    ���ڷ�ʽ����һ���ֽ�
 *@param    ch��    Ҫ���͵��ַ�
 *@retval   �ѷ��͵�����
*/
size_t Uart::write(uint8_t c)
{
//    while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
//    USART_SendData(_USARTx, c);
//    return 1;
    write(&c,1);
    return 1;
}

/**
 *@name     size_t Uart::write(const uint8_t *buffer, size_t size)
 *@brief    ���ڷ�ʽ����һ��������
 *@param    buffer:   ������ָ��
            size��    ��������С
 *@retval   �ѷ��͵�����
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
            while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
            USART_SendData(_USARTx, *buffer++);
        }
    }
	return size;
}


/**
 *@name     uint16_t Uart::read()
 *@brief    ���ڽ������ݣ��˺���ֻ�����û������ж��е���
 *@param    NONE
 *@retval   ���������յ�������
*/
uint16_t Uart::read()
{
    return (uint16_t)(_USARTx->DR & (uint16_t)0x01FF);
}






/**
 *@name     uint16_t Uart::dma_send_string(const char *str,uint16_t length)
 *@brief    ����DMA��ʽ�����ַ���������������
 *@param    str��       Ҫ���͵��ַ��������ݻ�����
            length��    �������ĳ���
 *@retval   �������ݵĳ���
*/
#if USE_UART_DMA
uint16_t Uart::dma_write(const char *str, uint16_t length)
{
//    DMA_DeInit(_DMA1_Channelx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
//    _DMA1_Channelx->CPAR = (uint32_t)&_USARTx->DR; //�����ַ
//    _DMA1_Channelx->CMAR = (uint32_t) str; //mem��ַ
//    _DMA1_Channelx->CNDTR = length ; //���䳤��
//    _DMA1_Channelx->CCR = (0 << 14) | // �Ǵ洢�����洢��ģʽ
//                          (2 << 12) | // ͨ�����ȼ���
//                          (0 << 11) | // �洢�����ݿ��8bit
//                          (0 << 10) | // �洢�����ݿ��8bit
//                          (0 <<  9) | // �������ݿ��8bit
//                          (0 <<  8) | // �������ݿ��8bit
//                          (1 <<  7) | // �洢����ַ����ģʽ
//                          (0 <<  6) | // �����ַ����ģʽ(����)
//                          (0 <<  5) | // ��ѭ��ģʽ
//                          (1 <<  4) | // �Ӵ洢����
//                          (0 <<  3) | // �Ƿ�����������ж�
//                          (0 <<  2) | // �Ƿ�����봫���ж�
//                          (0 <<  1) | // �Ƿ�����������ж�
//                          (0);        // ͨ������
    
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
 *@brief    ���ڷ�ʽ����һ���ֽ�
 *@param    str��       Ҫ���͵��ַ��������ݻ�����
            length��    �������ĳ���
 *@retval   �ѷ��͵�����
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
 *@brief    ���ڷ�ʽ����һ���ֽ�
 *@param    str��       Ҫ���͵��ַ��������ݻ�����
            length��    �������ĳ���
 *@retval   �ѷ��͵�����
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




