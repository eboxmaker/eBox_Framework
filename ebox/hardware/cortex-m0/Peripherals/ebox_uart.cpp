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

#if USE_UART_DMA
#include "ebox_mem.h"
#endif

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
	uint32_t _DataWidth;
	uint32_t _Parity;
	uint32_t _StopBits = 0;        
    rcc_clock_cmd((uint32_t)_USARTx,ENABLE);
    
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

    nvic(ENABLE,0,0);
    _rx_pin->mode(AF_PP_PU,LL_GPIO_AF_1);
    _tx_pin->mode(AF_PP_PU,LL_GPIO_AF_1);    
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
        dma_tx->nvic(DISABLE,0,0);
        dma_tx->interrupt(DmaItTc,DISABLE);
        dma_tx->interrupt(DmaItTe,DISABLE);
        dma_tx->interrupt(DmaItHt,DISABLE);
    }
#endif
    
    interrupt(RxIrq,DISABLE);
    interrupt(TcIrq,DISABLE);
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
        LL_USART_EnableIT_RXNE(_USARTx);
    if(type == TcIrq)
    {
        LL_USART_EnableIT_TC(_USARTx);
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
//        wait_busy();
        if(data_ptr != NULL)
            ebox_free(data_ptr);
        set_busy();
        data_ptr = (char *)ebox_malloc(size);
        if(data_ptr == NULL)
        {
            return 0;
        }
        for(int i = 0; i < size; i++)
            data_ptr[i] = *buffer++;
        dma_write(data_ptr, size);
    }
    else
    #endif
    {
        while(size--)
        {
            while (LL_USART_IsActiveFlag_TXE(_USARTx)== RESET);
            LL_USART_TransmitData8(_USARTx,*buffer++);
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
    return (uint16_t)(_USARTx->RDR & (uint16_t)0x01FF);
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
                if(USART1->ISR & bitShift(6)){
                    busy[0] = 0;
                    LL_USART_ClearFlag_TC(USART1);
                    break;
                }
            }
            break;
    #endif
            
    #if USE_UART2
        case (uint32_t)USART2_BASE:
            while(busy[1] == 1 ){
                if(USART2->ISR & 0X40){
                    busy[1] = 0;
                    LL_USART_ClearFlag_TC(USART2);
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
        if(LL_USART_IsActiveFlag_RXNE(USART1) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART1],RxIrq);
            CLEAR_BIT(USART1->ISR,B10000);//ǿ�����
        }
        if(LL_USART_IsActiveFlag_TC(USART1) == SET)
        {
            busy[0] = 0;   
            irq_handler(serial_irq_ids[NUM_UART1],TcIrq);
            LL_USART_ClearFlag_TC(USART1);
        }        
    }
    #endif


    #if USE_UART2
    void USART2_IRQHandler(void)
    {
        if(LL_USART_IsActiveFlag_RXNE(USART2) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART2],RxIrq);
            // ����ص�������û�ж�ȡ���ݣ��򽫵�ǰ����������׼����һ�ν���
			if (LL_USART_IsActiveFlag_RXNE(USART2) == SET )
			{
				LL_USART_RequestRxDataFlush(USART2);
			}
        }
        if(LL_USART_IsActiveFlag_TC(USART2) == SET)
        {
            busy[1] = 0;
            irq_handler(serial_irq_ids[NUM_UART2],TcIrq);
            // ������ͽ����жϱ�־
			LL_USART_ClearFlag_TC(USART2);
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




