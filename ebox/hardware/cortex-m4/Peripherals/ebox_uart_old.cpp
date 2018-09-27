#include "ebox_mem.h"
#include "ebox_uart.h"

static uint32_t serial_irq_ids[UART_NUM] = {0, 0, 0,0,0};

static uart_irq_handler irq_handler;



static uint8_t busy[UART_NUM];
/**
 *@name     Uart::Uart(USART_TypeDef *USARTx,GPIO *tx_pin,GPIO *rx_pin)
 *@brief    ���ڵĹ��캯��
 *@param    USARTx:  USART1,2,3��UART4,5
 *          tx_pin:  ��������Ӧ��tx����
 *          rx_pin:  ��������Ӧ��rx����
 *@retval   None
*/
Uart::Uart(USART_TypeDef *USARTx, Gpio *tx, Gpio *rx)
{
    this->USARTx = USARTx;
    this->tx = tx;
    this->rx = rx;
}
void Uart::begin(uint32_t baud_rate)
{
    begin(baud_rate,0);
}
void Uart::begin(uint32_t baud_rate,uint8_t use_dma)
{
    uint8_t index;
    this->use_dma = use_dma;
    
    switch((uint32_t)USARTx)
    {
        case (uint32_t)USART1_BASE:
            gpio_af_usart       = GPIO_AF_USART1;
            dma_tx              = &Dma2Stream7;
            dma_channel         = DMA_Channel_4;
            index = NUM_UART1;
            break;
        
        case (uint32_t)USART2_BASE:
            gpio_af_usart       = GPIO_AF_USART2;
            dma_tx              = &Dma1Stream6;
            dma_channel         = DMA_Channel_4;
            index               = NUM_UART2;
            break;
        
        case (uint32_t)USART3_BASE:
            gpio_af_usart       = GPIO_AF_USART3;
            dma_tx              = &Dma1Stream3;
            dma_channel         = DMA_Channel_4;
            index               = NUM_UART3;
            break;   
    
        case (uint32_t)UART4_BASE:
            gpio_af_usart       = GPIO_AF_UART4;
            dma_tx              = &Dma1Stream4;
            dma_channel         = DMA_Channel_4;
            index               = NUM_UART4;
            break;   
    
        case (uint32_t)UART5_BASE:
            gpio_af_usart       = GPIO_AF_UART5;
            dma_tx              = &Dma1Stream7;
            dma_channel         = DMA_Channel_4;
            index               = NUM_UART5;
            break;  
        
        case (uint32_t)USART6_BASE:
            gpio_af_usart       = GPIO_AF_USART6;
            dma_tx              = &Dma2Stream6;//����Dma2Stream7
            dma_channel         = DMA_Channel_5;
            index               = NUM_UART6;
        break;   
    
        
        //����STM32F42xxx��STM32F43xxx����
        #if defined(STM32F427_437xx) || defined(STM32F429_439xx)
        case (uint32_t)UART7_BASE:
            gpio_af_usart       = GPIO_AF_UART7;
            dma_tx              = &Dma1Stream0;
            dma_channel         = DMA_Channel_5;
            index               = NUM_UART7;
            break;  
        
        case (uint32_t)UART8_BASE:
            gpio_af_usart       = GPIO_AF_UART8;
            dma_tx              = &Dma1Stream1;
            dma_channel         = DMA_Channel_5;
            index               = NUM_UART8;
            break;  
        #endif
    }               
    config(baud_rate);
    if(this->use_dma == 1)
    {
        dma_tx->rcc_enable();
        dma_tx->nvic(DISABLE,0,0);
        dma_tx->interrupt(DmaItTc,DISABLE);
        dma_tx->interrupt(DmaItTe,DISABLE);
        dma_tx->interrupt(DmaItHt,DISABLE);
        dma_tx->deInit();
    }
    
    serial_irq_handler(index, Uart::_irq_handler, (uint32_t)this);

}
void Uart::config(uint32_t baud_rate)
{
  	USART_InitTypeDef USART_InitStructure;
    
    tx->mode(AF_PP,gpio_af_usart);
    rx->mode(AF_PP,gpio_af_usart);
    
    rcc_clock_cmd((uint32_t)USARTx,ENABLE);
  	USART_InitStructure.USART_BaudRate = baud_rate;//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USARTx, &USART_InitStructure);
    
    USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);  
   	USART_Cmd(USARTx, ENABLE);
  	USART_ClearFlag(USARTx, USART_FLAG_TC);  
 
    nvic(ENABLE,0,0);
    interrupt(RxIrq,DISABLE);
    interrupt(TcIrq,DISABLE);
}


void Uart::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
    nvic_dev_set_priority((uint32_t)USARTx,0,0,0);
    if(enable != DISABLE)
        nvic_dev_enable((uint32_t)USARTx,0);
    else
        nvic_dev_disable((uint32_t)USARTx,0);
}
void Uart::interrupt(IrqType type, FunctionalState enable)
{
    if(type == RxIrq)
        USART_ITConfig(USARTx, USART_IT_RXNE, enable);
    if(type == TcIrq)
    {
        USART_ClearITPendingBit(USARTx, USART_IT_TC);
        USART_ClearFlag(USARTx,USART_FLAG_TC); 
        USART_ITConfig(USARTx, USART_IT_TC, enable);//��ֹ�رշ�������ж�
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
//    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
//    USART_SendData(USARTx, c);
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
    if(use_dma == 1)
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
    {
        while(size--)
        {
            while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
            USART_SendData(USARTx, *buffer++);
        }
    }
	return size;
}

/**
 *@name     uint16_t Uart::dma_write(const char *ptr,uint16_t length)
 *@brief    ����DMA��ʽ�����ַ���������������
 *@param    ptr��       Ҫ���͵��ַ��������ݻ�����
            length��    �������ĳ���
 *@retval   �������ݵĳ���
*/
uint16_t Uart::dma_write(const char *ptr, uint16_t length)
{
	DMA_InitTypeDef DMA_InitStructure;

    dma_tx->deInit();
    
    
    DMA_InitStructure.DMA_Channel = dma_channel; /////////////////////
    //�����ַ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USARTx->DR);/////////////////
    //�ڴ��ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ptr;//////////////////////////
    //dma���䷽��
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    //����DMA�ڴ���ʱ�������ĳ���
    DMA_InitStructure.DMA_BufferSize = length;///////////////////////////////
    //����DMA���������ģʽ��һ������
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //����DMA���ڴ����ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //���������ֳ�
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    //�ڴ������ֳ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    //����DMA�Ĵ���ģʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //����DMA�����ȼ���
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    
    //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ  
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;    
    //ָ����FIFO��ֵˮƽ
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;        
    //ָ����Burstת�������ڴ洫�� 
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;       
    //ָ����Burstת��������Χת�� */  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
    dma_tx->init(&DMA_InitStructure);
    dma_tx->enable();
    return length;
}
/**
 *@name     uint16_t Uart::receive()
 *@brief    ���ڽ������ݣ��˺���ֻ�����û������ж��е���
 *@param    NONE
 *@retval   ���������յ�������
*/
uint16_t Uart::read()
{
    return (uint16_t)(USARTx->DR & (uint16_t)0x01FF);
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
    switch((uint32_t)USARTx)
    {
    case (uint32_t)USART1_BASE:
        while(busy[0] == 1 ){
            if(USART1->SR & 0X40){
                busy[0] = 0;
                USART_ClearITPendingBit(USART1, USART_IT_TC);
                break;
            }
        }
        break;
    case (uint32_t)USART2_BASE:
        while(busy[1] == 1 ){
            if(USART2->SR & 0X40){
                busy[1] = 0;
                USART_ClearITPendingBit(USART2, USART_IT_TC);
                break;
            }
        }
        break;
    case (uint32_t)USART3_BASE:
        while(busy[2] == 1 ){
            if(USART3->SR & 0X40){
                busy[2] = 0;
                USART_ClearITPendingBit(USART3, USART_IT_TC);
                break;
            }
        }
        break;
    case (uint32_t)UART4_BASE:
        while(busy[3] == 1 ){
            if(UART4->SR & 0X40){
                busy[3] = 0;
                USART_ClearITPendingBit(UART4, USART_IT_TC);
                break;
            }
        }
        break;
    case (uint32_t)UART5_BASE:
        while(busy[4] == 1 )
        {
            if(UART5->SR & 0X40)            
            {
                busy[4] = 0;
                USART_ClearITPendingBit(UART5, USART_IT_TC);
                break;
            }
        }
        break;
        
    case (uint32_t)USART6_BASE:
        while(busy[5] == 1 ){
            if(USART6->SR & 0X40){
                busy[5] = 0;
                USART_ClearITPendingBit(USART6, USART_IT_TC);
                break;
            }
        }
        break;
        
    case (uint32_t)UART7_BASE:
        while(busy[6] == 1 )
        {
            if(UART7->SR & 0X40)            
            {
                busy[6] = 0;
                USART_ClearITPendingBit(UART7, USART_IT_TC);
                break;
            }
        }
        break;
        
    case (uint32_t)UART8_BASE:
        while(busy[7] == 1 )
        {
            if(UART8->SR & 0X40)            
            {
                busy[7] = 0;
                USART_ClearITPendingBit(UART8, USART_IT_TC);
                break;
            }
        }
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
    switch((uint32_t)USARTx)
    {
    case (uint32_t)USART1_BASE:
        busy[0] = 1;break;
    case (uint32_t)USART2_BASE:
        busy[1] = 1;break;
    case (uint32_t)USART3_BASE:
        busy[2] = 1;break;
    case (uint32_t)UART4_BASE:
        busy[3] = 1;break;
    case (uint32_t)UART5_BASE:
        busy[4] = 1;break;
    case (uint32_t)USART6_BASE:
        busy[5] = 1;break;
    case (uint32_t)UART7_BASE:
        busy[6] = 1;break;
    case (uint32_t)UART8_BASE:
        busy[7] = 1;break;
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

    void USART6_IRQHandler(void)
    {
        if(USART_GetITStatus(USART6, USART_IT_RXNE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART6],RxIrq);
            USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        }
        if(USART_GetITStatus(USART6, USART_IT_TC) == SET)
        {
            busy[5] = 0;
            irq_handler(serial_irq_ids[NUM_UART6],TcIrq);
            USART_ClearITPendingBit(USART6, USART_IT_TC);
        }
    }

    void UART7_IRQHandler(void)
    {
        if(USART_GetITStatus(UART7, USART_IT_RXNE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART7],RxIrq);
            USART_ClearITPendingBit(UART7, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART7, USART_IT_TC) == SET)
        {
            busy[6] = 0;
            irq_handler(serial_irq_ids[NUM_UART7],TcIrq);
            USART_ClearITPendingBit(UART7, USART_IT_TC);
        }
    }

    void UART8_IRQHandler(void)
    {
        if(USART_GetITStatus(UART8, USART_IT_RXNE) == SET)
        {
            irq_handler(serial_irq_ids[NUM_UART8],RxIrq);
            USART_ClearITPendingBit(UART8, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART8, USART_IT_TC) == SET)
        {
            busy[7] = 0;
            irq_handler(serial_irq_ids[NUM_UART8],TcIrq);
            USART_ClearITPendingBit(UART8, USART_IT_TC);
        }
    }

    void serial_irq_handler(uint8_t index, uart_irq_handler handler, uint32_t id)
    {
        irq_handler = handler;
        serial_irq_ids[index] = id;
    }
}

