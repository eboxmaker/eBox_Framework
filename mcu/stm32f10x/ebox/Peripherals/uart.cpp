/**
  ******************************************************************************
  * @file    uart.cpp
  * @author  shentq
  * @version V1.2
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
#include "uart.h"


uint8_t busy[5];




static uint32_t serial_irq_ids[UART_NUM] = {0, 0, 0,0,0};

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
    tx_pin->mode(AF_PP);
    rx_pin->mode(INPUT);
}

/**
 *@name     void Uart::begin(uint32_t baud_rate)
 *@brief    ���ڳ�ʼ�����������˲����ʿɿ��⣬��������Ĭ�ϣ�8bit���ݣ�1��ֹͣλ����У�飬��Ӳ��������
 *@param    baud_rate:  �����ʣ�����9600��115200��38400�ȵ�
 *@retval   None
*/
void Uart::begin(uint32_t baud_rate)
{
    uint8_t index;
		USART_InitTypeDef USART_InitStructure;

    if(USE_DMA == 1)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMAʱ��

    switch((uint32_t)_USARTx)
    {
    case (uint32_t)USART1_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        _DMA1_Channelx = DMA1_Channel4;
				index = NUM_UART1;
        break;

    case (uint32_t)USART2_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        _DMA1_Channelx = DMA1_Channel7;
				index = NUM_UART2;
        break;

    case (uint32_t)USART3_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        _DMA1_Channelx = DMA1_Channel2;
				index = NUM_UART3;
        break;

#if defined (STM32F10X_HD)
    case (uint32_t)UART4_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
				index = NUM_UART4;
        break;

    case (uint32_t)UART5_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
				index = NUM_UART5;
        break;
#endif
    }

    serial_irq_handler(index, Uart::_irq_handler, (uint32_t)this);
		
    USART_InitStructure.USART_BaudRate = baud_rate;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(_USARTx, &USART_InitStructure);

    if((_USARTx == USART1 | _USARTx == USART2 | _USARTx == USART3) && (USE_DMA == 1) )
        USART_DMACmd(_USARTx, USART_DMAReq_Tx, ENABLE);
    USART_Cmd(_USARTx, ENABLE);
    interrupt(ENABLE);
}
/**
 *@name     void    begin(uint32_t baud_rate,uint8_t data_bit,uint8_t parity,float stop_bit);
 *@brief    ���ڳ�ʼ�������������и������ò���
 *@param    baud_rate:  �����ʣ�����9600��115200��38400�ȵ�
 *          data_bit:   ����λ����ֻ������8����9
 *          parity:     ����λ��0����У��λ��1��У�飬2żУ��
 *          stop_bit:   ֹͣλ��0.5,1,1.5,2�ĸ���ѡ����
 *@retval   None
*/
void Uart::begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit)
{
    USART_InitTypeDef USART_InitStructure;
    if (USE_DMA == 1)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMAʱ��

    switch((uint32_t)_USARTx)
    {
    case (uint32_t)USART1_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        _DMA1_Channelx = DMA1_Channel4;
        break;

    case (uint32_t)USART2_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        _DMA1_Channelx = DMA1_Channel7;
        break;

    case (uint32_t)USART3_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        _DMA1_Channelx = DMA1_Channel2;
        break;

#if defined (STM32F10X_HD)
    case (uint32_t)UART4_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
        break;

    case (uint32_t)UART5_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
        break;
#endif
    }

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
    if(parity == 0.5)
        USART_InitStructure.USART_StopBits = USART_StopBits_0_5;
    else if(parity == 1)
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
    else if(parity == 1.5)
        USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
    else if(parity == 2)
        USART_InitStructure.USART_StopBits = USART_StopBits_2;

    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(_USARTx, &USART_InitStructure);

    if((_USARTx == USART1 | _USARTx == USART2 | _USARTx == USART3) && (USE_DMA == 1) )
        USART_DMACmd(_USARTx, USART_DMAReq_Tx, ENABLE);
    USART_Cmd(_USARTx, ENABLE);
}

/**
 *@name     void Uart::interrupt(FunctionalState enable)
 *@brief    �����жϿ��ƺ���
 *@param    enable:  ENABLEʹ�ܴ��ڵķ�����ɺͽ����жϣ�DISABLE���ر��������ж�
 *@retval   None
*/
void Uart::interrupt(FunctionalState enable)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    USART_ITConfig(_USARTx, USART_IT_RXNE, enable);
    USART_ITConfig(_USARTx, USART_IT_TC, enable);
    USART_ClearITPendingBit(_USARTx, USART_IT_TC);

    switch((uint32_t)_USARTx)
    {
    case (uint32_t)USART1_BASE:
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        break;

    case (uint32_t)USART2_BASE:
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        break;

    case (uint32_t)USART3_BASE:
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        break;
#if defined (STM32F10X_HD)
    case (uint32_t)UART4_BASE:
        NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        break;
    case (uint32_t)UART5_BASE:
        NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        break;
#endif
    }
    NVIC_Init(&NVIC_InitStructure);
}


/**
 *@name     uint16_t Uart::receive()
 *@brief    ���ڽ������ݣ��˺���ֻ�����û������ж��е���
 *@param    NONE
 *@retval   ���������յ�������
*/
uint16_t Uart::receive()
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
uint16_t Uart::dma_send_string(const char *str, uint16_t length)
{
    DMA_DeInit(_DMA1_Channelx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    _DMA1_Channelx->CPAR = (uint32_t)&_USARTx->DR; //�����ַ
    _DMA1_Channelx->CMAR = (uint32_t) str; //mem��ַ
    _DMA1_Channelx->CNDTR = length ; //���䳤��
    _DMA1_Channelx->CCR = (0 << 14) | // �Ǵ洢�����洢��ģʽ
                          (2 << 12) | // ͨ�����ȼ���
                          (0 << 11) | // �洢�����ݿ��8bit
                          (0 << 10) | // �洢�����ݿ��8bit
                          (0 <<  9) | // �������ݿ��8bit
                          (0 <<  8) | // �������ݿ��8bit
                          (1 <<  7) | // �洢����ַ����ģʽ
                          (0 <<  6) | // �����ַ����ģʽ(����)
                          (0 <<  5) | // ��ѭ��ģʽ
                          (1 <<  4) | // �Ӵ洢����
                          (1 <<  3) | // �Ƿ�����������ж�
                          (0 <<  2) | // �Ƿ�����봫���ж�
                          (0 <<  1) | // �Ƿ�����������ж�
                          (1);        // ͨ������
    return length;
}

/**
 *@name     int Uart::put_char(uint16_t ch)
 *@brief    ���ڷ�ʽ����һ���ֽ�
 *@param    ch��    Ҫ���͵��ַ�
 *@retval   �ѷ��͵�����
*/
int Uart::put_char(uint16_t ch)
{
    while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
    USART_SendData(_USARTx, ch);
    return ch;
}

/**
 *@name     void Uart::put_string(const char *str,uint16_t length)
 *@brief    ���ڷ�ʽ�����ַ���������������
 *@param    str��       Ҫ���͵��ַ��������ݻ�����
            length��    �������ĳ���
 *@retval   NONE
*/
void Uart::put_string(const char *str, uint16_t length)
{
    if((_USARTx == USART1 | _USARTx == USART2 | _USARTx == USART3 ) && (USE_DMA == 1))
    {
        dma_send_string(str, length);
    }
    else
    {
        while(length--)
        {
            while(USART_GetFlagStatus(_USARTx, USART_FLAG_TXE) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
            USART_SendData(_USARTx, *str++);
        }
    }
}

/**
 *@name     void Uart::put_string(const char *str)
 *@brief    ���ڷ�ʽ����һ���ֽ�
 *@param    str��   Ҫ���͵��ַ���,ֱ������'\0'�����û��'\0'������󻺳�������
 *@retval   �ѷ��͵�����
*/
void Uart::put_string(const char *str)
{
    uint16_t i = 0;
    uint16_t length = 0;

    wait_busy();
    set_busy();
    while(str[i++] != '\0')
    {
        length++;
        if(length >= UART_MAX_SEND_BUF)
        {
            length = UART_MAX_SEND_BUF;
            break;
        }
    };
    put_string(str, length);

}

/////////////////////////////////////////////////
/**
 *@name     int Uart::put_char(uint16_t ch)
 *@brief    ���ڷ�ʽ����һ���ֽ�
 *@param    str��       Ҫ���͵��ַ��������ݻ�����
            length��    �������ĳ���
 *@retval   �ѷ��͵�����
*/
void Uart::printf_length(const char *str, uint16_t length)
{
    wait_busy();
    set_busy();
    if(length >= UART_MAX_SEND_BUF)
        length = UART_MAX_SEND_BUF;
    for(int i = 0; i < length; i++)
        send_buf[i] = *str++;
    put_string(send_buf, length);
}

/**
 *@name     int Uart::put_char(uint16_t ch)
 *@brief    ���ڷ�ʽ����һ���ֽ�
 *@param    str��       Ҫ���͵��ַ��������ݻ�����
            length��    �������ĳ���
 *@retval   �ѷ��͵�����
*/
void Uart::printf(const char *fmt, ...)
{
    uint16_t i = 0;
    uint16_t length = 0;

    wait_busy();
    set_busy();
    va_list va_params;
    va_start(va_params, fmt);
    vsprintf(send_buf, fmt, va_params); //�����ڴ�����ķ���
    va_end(va_params);

    while(send_buf[i++] != '\0')
    {
        length++;
        if(length >= UART_MAX_SEND_BUF)
        {
            length = UART_MAX_SEND_BUF;
            break;
        }
    };
    put_string(send_buf, length);
}


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
    case (uint32_t)USART1_BASE:
        while(busy[0] == 1);
        break;
    case (uint32_t)USART2_BASE:
        while(busy[1] == 1);
        break;
    case (uint32_t)USART3_BASE:
        while(busy[2] == 1);
        break;
    case (uint32_t)UART4_BASE:
        while(busy[3] == 1);
        break;
    case (uint32_t)UART5_BASE:
        while(busy[4] == 1);
        break;
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
    case (uint32_t)USART1_BASE:
        busy[0] = 1;
        break;
    case (uint32_t)USART2_BASE:
        busy[1] = 1;
        break;
    case (uint32_t)USART3_BASE:
        busy[2] = 1;
        break;
    case (uint32_t)UART4_BASE:
        busy[3] = 1;
        break;
    case (uint32_t)UART5_BASE:
        busy[4] = 1;
        break;
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

    //void DMA1_Channel4_IRQHandler(void)
    //	{

    //		DMA_Cmd(DMA1_Channel4,DISABLE);
    //		DMA_ClearFlag(DMA1_FLAG_TC4);

    //	}
    //	void DMA1_Channel7_IRQHandler(void)
    //	{
    //		DMA_Cmd(DMA1_Channel7,DISABLE);
    //		DMA_ClearFlag(DMA1_FLAG_TC7);
    //	}
    //	void DMA1_Channel2_IRQHandler(void)
    //	{
    //		DMA_Cmd(DMA1_Channel2,DISABLE);
    //		DMA_ClearFlag(DMA1_FLAG_TC2);
    //	}
		
void serial_irq_handler(uint8_t index, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[index] = id;
}
}




