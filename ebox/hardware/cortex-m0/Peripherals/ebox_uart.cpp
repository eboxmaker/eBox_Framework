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



uint16_t _tx_buffer_size[UART_NUM];   // ���ͻ��λ�����size
uint16_t _tx_buffer_head[UART_NUM];   // ������ͷ,ÿд�루д�뻺������һ���ַ�������ƶ�1
uint16_t _tx_buffer_tail[UART_NUM];   // ������β,ÿд����д�봮��TX��һ���ַ�������ƶ�1
uint16_t *_tx_ptr[UART_NUM];          // ������ָ��

uint16_t _rx_buffer_size[UART_NUM];
uint16_t _rx_buffer_head[UART_NUM];
uint16_t _rx_buffer_tail[UART_NUM];
uint16_t *_rx_ptr[UART_NUM];
/**
 *@brief    ���ڵĹ��캯��
 *@param    USARTx:  USART1,2,3��UART4,5
 *          tx_pin:  ��������Ӧ��tx����
 *          rx_pin:  ��������Ӧ��rx����
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
 *@brief    ���ڳ�ʼ�����������˲����ʿɿ��⣬��������Ĭ�ϣ�8bit���ݣ�1��ֹͣλ����У�飬��Ӳ��������
 *@param    baud_rate:  �����ʣ�����9600��115200��38400�ȵ�
 *@param    _use_dma:   �Ƿ�ʹ��DMA��Ĭ��ֵ1��ʹ��DMA��0��������DMA;
 *@retval   None
*/
void    Uart::begin(uint32_t baud_rate, RxMode_t mode)
{
    Uart::begin(baud_rate, 8, 0, 1, mode);
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
void Uart::begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit, RxMode_t mode)
{
//    uint8_t  index;
    uint32_t _DataWidth;
    uint32_t _Parity;
    uint32_t _StopBits = 0;
    rcc_clock_cmd((uint32_t)_USARTx, ENABLE);
		_mode = mode;

#if USE_UART_DMA
    _use_dma = use_dma;
    switch((uint32_t)_USARTx)
    {

#if USE_UART1
    case (uint32_t)USART1_BASE:
        dma_tx = &Dma1Ch2;
        index = NUM_UART1;
        break;
#endif

#if USE_UART2
    case (uint32_t)USART2_BASE:
        dma_tx = &Dma1Ch4;
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
        _index = NUM_UART1;
        break;
#endif

#if USE_UART2
    case (uint32_t)USART2_BASE:
        _index = NUM_UART2;
        break;
#endif

#if USE_UART3
    case (uint32_t)USART3_BASE:
        _index = NUM_UART3;
        break;
#endif

    }
#endif

		_tx_buffer_size[_index] = tx_buffer_size;
    _rx_buffer_size[_index] = rx_buffer_size;

    _tx_ptr[_index] = (uint16_t *)ebox_malloc(_tx_buffer_size[_index]);
    _rx_ptr[_index] = (uint16_t *)ebox_malloc(_rx_buffer_size[_index]);
    serial_irq_handler(_index, Uart::_irq_handler, (uint32_t)this);

    switch(data_bit)
    {
    case 8:
        _DataWidth = LL_USART_DATAWIDTH_8B;
        break;
    case 9:
        _DataWidth = LL_USART_DATAWIDTH_9B;
        break;
    default :
        _DataWidth = LL_USART_DATAWIDTH_8B;
        break;
    }
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
    _rx_pin->mode(AF_PP_PU, LL_GPIO_AF_1);
    _tx_pin->mode(AF_PP_PU, LL_GPIO_AF_1);
    LL_USART_SetTransferDirection(_USARTx, LL_USART_DIRECTION_TX_RX);
    LL_USART_ConfigCharacter(_USARTx, _DataWidth, _Parity, _StopBits);
    LL_USART_SetBaudRate(_USARTx, SystemCoreClock, LL_USART_OVERSAMPLING_16, baud_rate);
    //	��ֹ������
    LL_USART_DisableOverrunDetect(_USARTx);
    LL_USART_Enable(_USARTx);
    /* Polling USART initialisation */
    while ((!(LL_USART_IsActiveFlag_TEACK(_USARTx))) || (!(LL_USART_IsActiveFlag_REACK(_USARTx))))
    {
    }

#if USE_UART_DMA
    if((_USARTx == USART1 || _USARTx == USART2 || _USARTx == USART3) && (_use_dma == 1) )
    {
        LL_USART_EnableDMAReq_TX(_USARTx);
        dma_tx->rcc_enable();
        dma_tx->nvic(DISABLE, 0, 0);
        dma_tx->interrupt(DmaItTc, DISABLE);
        dma_tx->interrupt(DmaItTe, DISABLE);
        dma_tx->interrupt(DmaItHt, DISABLE);
    }
#endif

    interrupt(RxIrq, ENABLE);
    interrupt(TxIrq, DISABLE);
}
/**
 *@brief    ��λ���ڣ������RX������
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
    nvic_dev_set_priority((uint32_t)_USARTx, 0, 0, 0);
    if(enable != DISABLE)
        nvic_dev_enable((uint32_t)_USARTx, 0);
    else
        nvic_dev_disable((uint32_t)_USARTx, 0);
}
///////////////////////////////////////////////////////////////

/**
 *@brief    �����жϿ��ƺ���
 *@param    enable:  ENABLEʹ�ܴ��ڵķ�����ɺͽ����жϣ�DISABLE���ر��������ж�
 *@retval   None
*/
void Uart::interrupt(IrqType type, FunctionalState enable)
{

    if(type == RxIrq)
        LL_USART_EnableIT_RXNE(_USARTx);
    if(type == TxIrq)
    {
			(enable == ENABLE) ? (LL_USART_EnableIT_TXE(_USARTx)): ( LL_USART_DisableIT_TXE(_USARTx)) ;
    }
}
/**
 *@brief    �൱��read������ȡ�󲻴ӻ������������
 *@param    NONE
 *@retval   ����tailָ�������
*/
int Uart::peek(void)
{
    if ((_rx_buffer_size[_index] - LL_DMA_GetDataLength(DMA1,dma_rx->DMAy_Channelx)) == _rx_buffer_tail[_index])
    {
        return -1;
    }
    else
    {
        return _rx_ptr[_index][_rx_buffer_tail[_index]];
    }
}
/**
 *@brief    �жϽ��ջ������Ƿ�������
 *@param    NONE
 *@retval   0 �����ݣ���0����ǰ���������ݳ���
*/
int Uart::available()
{
    if(_mode == RxDMA)
        return ((unsigned int)(_rx_buffer_size[_index] + (_rx_buffer_size[_index] - LL_DMA_GetDataLength(DMA1,dma_rx->DMAy_Channelx)) - _rx_buffer_tail[_index])) % _rx_buffer_size[_index];
    else
        return ((unsigned int)(_rx_buffer_size[_index] + _rx_buffer_head[_index] - _rx_buffer_tail[_index])) % _rx_buffer_size[_index];
}

/**
 *@brief    ��ȡһ���ֽڣ��������ǰ����
 *@param    NONE
 *@retval   -1  �����ݣ���������ǰ����
*/
int Uart::read()
{
    if(_mode == RxDMA)
    {
        if (_rx_buffer_tail[_index] == (_rx_buffer_size[_index] - LL_DMA_GetDataLength(DMA1,dma_rx->DMAy_Channelx)))
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
 *@brief    �Ƿ����д��
 *@param    NONE
 *@retval   ����д�������
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
 *@brief    �ֶ���tx����������ȫ��д��
 *@param    NONE
 *@retval   NONE
*/
void Uart::flush()
{
    uint8_t major, minor;
    while(_tx_buffer_head[_index] != _tx_buffer_tail[_index] )
    {
        //���ȫ���жϱ��ر�,����ʹ���жϱ��رա��������жϺ�����,���ֶ�����
        //        if(__get_PRIMASK() || ((_USARTx->CR1 & USART_FLAG_TXE) == 0))
        if(__get_PRIMASK() || ((_USARTx->CR1 & USART_ISR_TXE) == 0) || (__get_IPSR() != 0))
        {
            nvic_irq_get_priority((IRQn_Type)__get_IPSR(), &major, &minor);
            if(major <= preemption_priority )//�������ȼ����ڻ���������жϷ�����
            {
                //                interrupt(TxIrq,DISABLE);//�ڼ����رմ����ж�
                while(LL_USART_IsActiveFlag_TXE(_USARTx) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
                tx_bufferx_one(_USARTx, _index);
            }
        }
        //���ߵȴ������жϷ�������������ݵĴ���
    }
}


/**
 *@brief    ����һ���ֽ�
 *@param    c��Ҫ���͵��ַ�
 *@retval   1
*/
size_t Uart::write(uint8_t c)
{
    uint16_t i = (_tx_buffer_head[_index] + 1) % _tx_buffer_size[_index];//����ͷ��λ��
    // head = tail, �������������ȷ���
    while (i == _tx_buffer_tail[_index]) 
    {
        interrupt(TxIrq,DISABLE);//�ڼ����رմ����ж�				
        while(LL_USART_IsActiveFlag_TXE(_USARTx) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
        tx_bufferx_one(_USARTx,_index);
        interrupt(TxIrq,ENABLE);//��������
    }
    // ���������ݣ��ƶ�head
    _tx_ptr[_index][_tx_buffer_head[_index]] = c;
    _tx_buffer_head[_index] = i;

    interrupt(TxIrq,ENABLE);//��������  
  	return 1;
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
    LL_DMA_InitTypeDef DMA_InitStructure;

    dma_tx->deInit();

    DMA_InitStructure.PeriphOrM2MSrcAddress  = (uint32_t)&_USARTx->TDR;             //�����ַ
    DMA_InitStructure.MemoryOrM2MDstAddress  = (uint32_t) str;                      //mem��ַ
    DMA_InitStructure.Direction              = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;   //���䷽���ڴ浽����
    DMA_InitStructure.Mode                   = LL_DMA_MODE_NORMAL;                  //����ģʽ
    DMA_InitStructure.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;           //�����ַ����ģʽ(����)
    DMA_InitStructure.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;             //�洢����ַ����ģʽ
    DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;              //�������ݿ��8bit
    DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;              //�洢�����ݿ��8bit
    DMA_InitStructure.NbData                 = length;                              //���䳤��
    DMA_InitStructure.Priority               = LL_DMA_PRIORITY_HIGH;                //ͨ�����ȼ���

    dma_tx->init(&DMA_InitStructure);
    dma_tx->enable();
    return length;
}
#endif

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
      *@brief    �ӻ�����д��һ���ַ�
      *@param    uart�����ڣ� index ���� IT ����
      *@retval   1
      */
    void tx_bufferx_one(USART_TypeDef* uart,uint8_t index)
    {
        if (_tx_buffer_head[index] == _tx_buffer_tail[index])//�������ֱ�ӷ���
        {   
						LL_USART_DisableIT_TXE(uart);					
						return;
				}
        unsigned char c = _tx_ptr[index][_tx_buffer_tail[index]];   // ȡ���ַ�
        _tx_buffer_tail[index] = (_tx_buffer_tail[index] + 1) % _tx_buffer_size[index];
        uart->TDR = (c & (uint16_t)0x01FF);
				//LL_USART_TransmitData8(uart,c);
        if (_tx_buffer_head[index] == _tx_buffer_tail[index])//�����������������
        {
            // Buffer empty, so disable interrupts
            //USART_ITConfig(uart, USART_IT_TXE, DISABLE);
						LL_USART_DisableIT_TXE(uart);
        }
    }
    /**
      *@brief    ����һ���ַ����뻺����
      *@param    uart�����ڣ� index ���� IT ����
      *@retval   1
      */
    void rx_buffer_one(USART_TypeDef* uart,uint8_t index)
    {
        uint16_t i = (_rx_buffer_head[index] + 1) % _rx_buffer_size[index];//����ͷ��λ��
        if(i == _rx_buffer_tail[index]) //������λ��������˾��޸�һ��tail�������������ϵ�һ������
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
            CLEAR_BIT(USART1->ISR, B10000); //ǿ�����
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
            // ����ص�������û�ж�ȡ���ݣ��򽫵�ǰ����������׼����һ�ν���
            LL_USART_RequestRxDataFlush(USART2);
        }
			
        if(LL_USART_IsActiveFlag_TXE(USART2) == SET)
        {
						tx_bufferx_one(USART2, NUM_UART2);
            irq_handler(serial_irq_ids[NUM_UART2], TxIrq);
            // ������ͽ����жϱ�־
            LL_USART_IsActiveFlag_TXE(USART2);
        }
    }
#endif


#if USE_UART3
//    void USART3_IRQHandler(void)
//    {
//        if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
//        {
//            irq_handler(serial_irq_ids[NUM_UART3], RxIrq);
//            USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//        }
//        if(USART_GetITStatus(USART3, USART_IT_TC) == SET)
//        {
//            busy[2] = 0;
//            irq_handler(serial_irq_ids[NUM_UART3], TcIrq);
//            USART_ClearITPendingBit(USART3, USART_IT_TC);
//        }
//    }
#endif

    void serial_irq_handler(uint8_t index, uart_irq_handler handler, uint32_t id)
    {
        irq_handler = handler;
        serial_irq_ids[index] = id;
    }
}




