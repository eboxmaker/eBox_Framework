#include "ebox_uart.h"
#include "nvic.h"


callback_fun_type usart_callback_table[5][2];//֧�ִ��ڵ�rx�ж�
#define uart_tx_length   100
uint8_t uart_tx_buf[uart_tx_length];


static uint32_t serial_irq_ids[UART_NUM] = {0, 0, 0,0,0};

static uart_irq_handler irq_handler;



static uint8_t busy[5];
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
            /* gpio parament
            */
            gpio_af_usart       = GPIO_AF_USART1;
        
            /* Uart parament
            */
            rcc_usart_clock_cmd = RCC_APB2PeriphClockCmd;
            usart_rcc           = RCC_APB2Periph_USART1;
            usart_irq           = USART1_IRQn;
            /* dma parament
            */
            rcc_dma_clock_cmd   = RCC_AHB1PeriphClockCmd;        
            dma_rcc             = RCC_AHB1Periph_DMA2;
            dma_irq             = DMA2_Stream7_IRQn;
            dma                 = DMA2;
            dma_channel         = DMA_Channel_4;
            dma_stream          = DMA2_Stream7;
        
            index = NUM_UART1;

        break;
        
        case (uint32_t)USART2_BASE:
            /* gpio parament
            */
            gpio_af_usart       = GPIO_AF_USART2;
        
            /* Uart parament
            */
            rcc_usart_clock_cmd = RCC_APB1PeriphClockCmd;
            usart_rcc           = RCC_APB1Periph_USART2;
            usart_irq           = USART2_IRQn;
            /* dma parament
            */
            rcc_dma_clock_cmd   = RCC_AHB1PeriphClockCmd;        
            dma_rcc             = RCC_AHB1Periph_DMA1;
            dma                 = DMA1;
            dma_channel         = DMA_Channel_4;
            dma_irq             = DMA1_Stream6_IRQn;
            dma_stream          = DMA1_Stream6;
            index               = NUM_UART2;
        break;
        
        case (uint32_t)USART3_BASE:
            /* gpio parament
            */
            gpio_af_usart       = GPIO_AF_USART3;
        
            /* Uart parament
            */
            rcc_usart_clock_cmd = RCC_APB1PeriphClockCmd;
            usart_rcc           = RCC_APB1Periph_USART3;
            usart_irq           = USART3_IRQn;
            /* dma parament
            */
            rcc_dma_clock_cmd   = RCC_AHB1PeriphClockCmd;        
            dma_rcc             = RCC_AHB1Periph_DMA1;
            dma                 = DMA1;
            dma_channel         = DMA_Channel_4;
            dma_irq             = DMA1_Stream3_IRQn;
            dma_stream          = DMA1_Stream3;
            index               = NUM_UART3;
        break;   
    
        case (uint32_t)UART4_BASE:
            /* gpio parament
            */
            gpio_af_usart       = GPIO_AF_UART4;
        
            /* Uart parament
            */
            rcc_usart_clock_cmd = RCC_APB1PeriphClockCmd;
            usart_rcc           = RCC_APB1Periph_UART4;
            usart_irq           = UART4_IRQn;
            /* dma parament
            */
            rcc_dma_clock_cmd   = RCC_AHB1PeriphClockCmd;        
            dma_rcc             = RCC_AHB1Periph_DMA1;
            dma                 = DMA1;
            dma_channel         = DMA_Channel_4;
            dma_irq             = DMA1_Stream4_IRQn;
            dma_stream          = DMA1_Stream4;
            index               = NUM_UART4;
        break;   
    
        case (uint32_t)UART5_BASE:
            /* gpio parament
            */
            gpio_af_usart       = GPIO_AF_UART5;
        
            /* Uart parament
            */
            rcc_usart_clock_cmd = RCC_APB1PeriphClockCmd;
            usart_rcc           = RCC_APB1Periph_UART5;
            usart_irq           = UART5_IRQn;
            /* dma parament
            */
            rcc_dma_clock_cmd   = RCC_AHB1PeriphClockCmd;        
            dma_rcc             = RCC_AHB1Periph_DMA1;
            dma                 = DMA1;
            dma_channel         = DMA_Channel_4;
            dma_irq             = DMA1_Stream7_IRQn;
            dma_stream          = DMA1_Stream7;
            index               = NUM_UART5;
        break;   
    
    }               
    usart_config(baud_rate);
    if(this->use_dma == 1)
    {
        dma_config();
    }
    
    serial_irq_handler(index, Uart::_irq_handler, (uint32_t)this);

}
void Uart::usart_config(uint32_t baud_rate)
{
  	USART_InitTypeDef USART_InitStructure;
    
    tx->mode(AF_PP,gpio_af_usart);
    rx->mode(AF_PP,gpio_af_usart);
    
    rcc_usart_clock_cmd(usart_rcc,ENABLE);
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
    interrupt(RxIrq,ENABLE);
    interrupt(TcIrq,ENABLE);
}
void Uart::dma_config()
{
	NVIC_InitTypeDef NVIC_InitStructure ;
	DMA_InitTypeDef DMA_InitStructure;

    //���ڷ�DMA����  
    //����DMAʱ��
    rcc_dma_clock_cmd(dma_rcc, ENABLE);//////////////////////
    //DMA�����ж�����
    NVIC_InitStructure.NVIC_IRQChannel = dma_irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //DMAͨ������
    DMA_DeInit(dma_stream);/////////////////////////////
    DMA_InitStructure.DMA_Channel = dma_channel; /////////////////////
    //�����ַ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USARTx->DR);/////////////////
    //�ڴ��ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)this->send_buf;//////////////////////////
    //dma���䷽��
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    //����DMA�ڴ���ʱ�������ĳ���
    DMA_InitStructure.DMA_BufferSize = uart_tx_length;///////////////////////////////
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

    //����DMA1��ͨ��         
    DMA_Init(dma_stream, &DMA_InitStructure);  //////////////////////////////////////////
    //ʹ���ж�
    DMA_ITConfig(dma_stream,DMA_IT_TC,ENABLE); /////////////////////
}
void Uart::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
    nvic_irq_set_priority((uint32_t)USARTx,0,0,0);
    if(enable != DISABLE)
        nvic_irq_enable((uint32_t)USARTx,0);
    else
        nvic_irq_disable((uint32_t)USARTx,0);
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
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
    USART_SendData(USARTx, c);
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
    wait_busy();
    if((USARTx == USART1 | USARTx == USART2 | USARTx == USART3 ) && (use_dma == 1))
    {
        wait_busy();
        if(uart_buf != NULL)
            ebox_free(uart_buf);
        set_busy();
        uart_buf = (char *)ebox_malloc(size);
        if(uart_buf == NULL)
        {
            return 0;
        }
        for(int i = 0; i < size; i++)
            uart_buf[i] = *buffer++;
        dma_send_string(uart_buf, size);
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
 *@name     void Uart::attach_rx_interrupt(void (*callback_fun)(void))
 *@brief    �󶨴��ڽ����ж������õ��û�����
 *@param    callback_fun:  �û�����
 *@retval   None
*/
void Uart::attach_rx_interrupt(void (*callback_fun)(void))
{
    switch((uint32_t)USARTx)
    {
    case (uint32_t)USART1_BASE:
        usart_callback_table[0][0] = callback_fun;
        break;
    case (uint32_t)USART2_BASE:
        usart_callback_table[1][0] = callback_fun;
        break;
    case (uint32_t)USART3_BASE:
        usart_callback_table[2][0] = callback_fun;
        break;
    case (uint32_t)UART4_BASE:
        usart_callback_table[3][0] = callback_fun;
        break;
    case (uint32_t)UART5_BASE:
        usart_callback_table[4][0] = callback_fun;
        break;
    }
}

/**
 *@name     void Uart::attach_tx_interrupt(void (*callback_fun)(void))
 *@brief    �󶨴��ڷ�������ж������õ��û�����
 *@param    callback_fun:  �û�����
 *@retval   None
*/
void Uart::attach_tx_interrupt(void (*callback_fun)(void))
{
    switch((uint32_t)USARTx)
    {
    case (uint32_t)USART1_BASE:
        usart_callback_table[0][1] = callback_fun;
        break;
    case (uint32_t)USART2_BASE:
        usart_callback_table[1][1] = callback_fun;
        break;
    case (uint32_t)USART3_BASE:
        usart_callback_table[2][1] = callback_fun;
        break;
    case (uint32_t)UART4_BASE:
        usart_callback_table[3][1] = callback_fun;
        break;
    case (uint32_t)UART5_BASE:
        usart_callback_table[4][1] = callback_fun;
        break;
    }
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
//�˺����д��Ż���ȥ����һ������
	DMA_SetCurrDataCounter(dma_stream,length);
 	DMA_Cmd(dma_stream,ENABLE);
    return length;
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
//                irq_handler(serial_irq_ids[NUM_UART1],TcIrq);
                USART_ClearITPendingBit(USART1, USART_IT_TC);
                break;
            }
        }
        break;
    case (uint32_t)USART2_BASE:
        while(busy[1] == 1 ){
            if(USART2->SR & 0X40){
                busy[1] = 0;
//                irq_handler(serial_irq_ids[NUM_UART2],TcIrq);
                USART_ClearITPendingBit(USART2, USART_IT_TC);
                break;
            }
        }
        break;
    case (uint32_t)USART3_BASE:
        while(busy[2] == 1 ){
            if(USART3->SR & 0X40){
                busy[2] = 0;
//                irq_handler(serial_irq_ids[NUM_UART3],TcIrq);
                USART_ClearITPendingBit(USART3, USART_IT_TC);
                break;
            }
        }
        break;
    case (uint32_t)UART4_BASE:
        while(busy[3] == 1 ){
            if(UART4->SR & 0X40){
                busy[3] = 0;
//                irq_handler(serial_irq_ids[NUM_UART4],TcIrq);
                USART_ClearITPendingBit(UART4, USART_IT_TC);
                break;
            }
        }
        break;
    case (uint32_t)UART5_BASE:
        while(busy[4] == 1 ){
            if(UART5->SR & 0X40){
                busy[4] = 0;
//                irq_handler(serial_irq_ids[NUM_UART5],TcIrq);
                USART_ClearITPendingBit(UART5, USART_IT_TC);
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
            if(usart_callback_table[1][1] != 0)
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
    void serial_irq_handler(uint8_t index, uart_irq_handler handler, uint32_t id)
    {
        irq_handler = handler;
        serial_irq_ids[index] = id;
    }
}

