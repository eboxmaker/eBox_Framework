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
static uint32_t serial_irq_ids[UART_NUM] = {0, 0, 0,0,0};

static uart_irq_handler irq_handler;


char    *_tx_ptr[UART_NUM];

uint16_t _tx_buffer_size[UART_NUM];
uint16_t _tx_buffer_head[UART_NUM];
uint16_t _tx_buffer_tail[UART_NUM];




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
void    Uart::begin(uint32_t baud_rate)
{
    Uart::begin(baud_rate,8,0,1);
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
void Uart::begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit)
{
    USART_InitTypeDef   USART_InitStructure;
        
    rcc_clock_cmd((uint32_t)_USARTx,ENABLE);
    
    switch((uint32_t)_USARTx)
    {
    case (uint32_t)USART1_BASE:
        dma_rx = &Dma1Ch5;
        uart_index = NUM_UART1;
        break;

    case (uint32_t)USART2_BASE:
        uart_index = NUM_UART2;
        break;

    case (uint32_t)USART3_BASE:
        uart_index = NUM_UART3;
        break;

#if defined (STM32F10X_HD)
    case (uint32_t)UART4_BASE:
        uart_index = NUM_UART4;
        break;

    case (uint32_t)UART5_BASE:
        uart_index = NUM_UART5;
        break;
#endif
    }

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



    _tx_buffer_size[uart_index] = TX_BUFFER_SIZE;
    _tx_ptr[uart_index] = (char *)ebox_malloc(_tx_buffer_size[uart_index]);

    _rx_buffer_size = RX_BUFFER_SIZE;
    _rx_ptr = (uint16_t *)ebox_malloc(_rx_buffer_size);

    USART_DMACmd(_USARTx, USART_DMAReq_Rx, ENABLE);
    dma_rx->rcc_enable();
    dma_rx->nvic(DISABLE,0,0);
    dma_rx->interrupt(DmaItTc,DISABLE);
    dma_rx->interrupt(DmaItTe,DISABLE);
    dma_rx->interrupt(DmaItHt,DISABLE);
    
    DMA_InitTypeDef DMA_InitStructure;

    dma_rx->deInit();
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&_USARTx->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) _rx_ptr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = _rx_buffer_size;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    dma_rx->init(&DMA_InitStructure);
    dma_rx->enable();
    
    
    USART_Cmd(_USARTx, ENABLE);
    

    nvic(ENABLE,0,0);
    interrupt(RxIrq,DISABLE);
    interrupt(TxIrq,DISABLE);
    USART_ClearITPendingBit(_USARTx, USART_IT_TXE);
    USART_ClearFlag(_USARTx,USART_FLAG_TXE); 
    _tx_pin->mode(AF_PP);
    _rx_pin->mode(INPUT);

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
 *@brief    �����жϿ��ƺ���
 *@param    enable:  ENABLEʹ�ܴ��ڵķ�����ɺͽ����жϣ�DISABLE���ر��������ж�
 *@retval   None
*/
void Uart::interrupt(IrqType type, FunctionalState enable)
{

    if(type == RxIrq)
        USART_ITConfig(_USARTx, USART_IT_RXNE, enable);
    if(type == TxIrq)
        USART_ITConfig(_USARTx, USART_IT_TXE, enable);
}
int Uart::peek(void)
{
  if (_rx_buffer_head == _rx_buffer_tail) {
    return -1;
  } else {
    return _rx_ptr[_rx_buffer_tail];
  }
}
/**
 *@name     uint16_t Uart::read()
 *@brief    ���ڽ������ݣ��˺���ֻ�����û������ж��е���
 *@param    NONE
 *@retval   ���������յ�������
*/
int Uart::available()
{
  return ((unsigned int)(_rx_buffer_size + (_rx_buffer_size - dma_rx->DMAy_Channelx->CNDTR) - _rx_buffer_tail)) % _rx_buffer_size;
}

int Uart::read()
{
    if (_rx_buffer_tail == (_rx_buffer_size - dma_rx->DMAy_Channelx->CNDTR)) 
    {
        return -1;
    } else 
    {
        int c = _rx_ptr[_rx_buffer_tail];
        _rx_buffer_tail = (_rx_buffer_tail + 1) % _rx_buffer_size;
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
        //���ȫ���жϱ��ر�,����ʹ���жϱ��رա��������жϺ�����,���ֶ�����
//        if(__get_PRIMASK() || ((_USARTx->CR1 & USART_FLAG_TXE) == 0))
        if(__get_PRIMASK() || ((_USARTx->CR1 & USART_FLAG_TXE) == 0) || (__get_IPSR() != 0))
        {   
            nvic_irq_get_priority((IRQn_Type)__get_IPSR(),&major,&minor);
            if(major <= preemption_priority )//�������ȼ����ڻ���������жϷ�����
            {
//                interrupt(TxIrq,DISABLE);//�ڼ����رմ����ж�
                while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
                tx_bufferx_one(_USARTx,uart_index);
            }
        }
        //���ߵȴ������жϷ�������������ݵĴ���
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

    uint16_t i = (_tx_buffer_head[uart_index] + 1) % _tx_buffer_size[uart_index];//����ͷ��λ��

    while (i == _tx_buffer_tail[uart_index]) //������λ��������˾͵���һ�η���
    {
        interrupt(TxIrq,DISABLE);//�ڼ����رմ����ж�
        while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
        tx_bufferx_one(_USARTx,uart_index);
        interrupt(TxIrq,ENABLE);//��������
    }

    _tx_ptr[uart_index][_tx_buffer_head[uart_index]] = c;
    _tx_buffer_head[uart_index] = i;

    interrupt(TxIrq,ENABLE);//��������
  
  return 1;
}


///**
// *@name     size_t Uart::write(const uint8_t *buffer, size_t size)
// *@brief    ���ڷ�ʽ����һ��������
// *@param    buffer:   ������ָ��
//            size��    ��������С
// *@retval   �ѷ��͵�����
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
    void tx_bufferx_one(USART_TypeDef* uart,uint8_t uart_num)
    {
        if (_tx_buffer_head[uart_num] == _tx_buffer_tail[uart_num])//�������ֱ�ӷ���
            return;
        unsigned char c = _tx_ptr[uart_num][_tx_buffer_tail[uart_num]];
        _tx_buffer_tail[uart_num] = (_tx_buffer_tail[uart_num] + 1) % _tx_buffer_size[uart_num];
        uart->DR = (c & (uint16_t)0x01FF);
        if (_tx_buffer_head[uart_num] == _tx_buffer_tail[uart_num])//�����������������
        {
            // Buffer empty, so disable interrupts
            USART_ITConfig(uart, USART_IT_TXE, DISABLE);
        }
    }

    void USART1_IRQHandler(void)
    {
        if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
        {
            USART_ClearITPendingBit(USART1, USART_IT_RXNE);
            irq_handler(serial_irq_ids[NUM_UART1],RxIrq);
        }      
        if(USART_GetITStatus(USART1, USART_IT_TXE) == SET)
        {
            tx_bufferx_one(USART1,NUM_UART1);
            irq_handler(serial_irq_ids[NUM_UART1],TxIrq);
        }        
    }
    void USART2_IRQHandler(void)
    {
        if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART2],RxIrq);
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART2, USART_IT_TXE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART2],TxIrq);
            USART_ClearITPendingBit(USART2, USART_IT_TXE);
        }
    }
    void USART3_IRQHandler(void)
    {
        if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART3],RxIrq);
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART3, USART_IT_TXE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART3],TxIrq);
            USART_ClearITPendingBit(USART3, USART_IT_TXE);
        }
    }
#if defined (STM32F10X_HD)
    void UART4_IRQHandler(void)
    {
        if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART4],RxIrq);
            USART_ClearITPendingBit(UART4, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART4, USART_IT_TC) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART4],TcIrq);
            USART_ClearITPendingBit(UART4, USART_IT_TC);
        }
    }
    void UART5_IRQHandler(void)
    {
        if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART5],RxIrq);
            USART_ClearITPendingBit(UART5, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART5, USART_IT_TC) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART5],TcIrq);
            USART_ClearITPendingBit(UART5, USART_IT_TC);
        }
    }
#endif
		
void serial_irq_handler(uint8_t uart_index, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[uart_index] = id;
}
}




