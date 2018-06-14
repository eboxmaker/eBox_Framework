#ifndef __EBOX_UART_H
#define __EBOX_UART_H
#include "ebox_core.h"
#include "mcu.h"
//用户配置//////////////
#define UART_MAX_SEND_BUF 128


class Uart:public Print
{
public:
    Uart(USART_TypeDef *USARTx, Gpio *tx, Gpio *rx);

    void    begin(uint32_t baud_rate);
    void    begin(uint32_t baud_rate,uint8_t use_dma);
    void    begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit);
    void    attach_rx_interrupt(void (*callback_fun)(void));
    void    attach_tx_interrupt(void (*callback_fun)(void));

    int 	put_char(uint16_t ch);
    void 	put_string(const char *str);
    void    printf_length(const char *str, uint16_t length);
    void    printf(const char *fmt, ...); //需要注意缓冲区溢出
    void    wait_busy();

    //write method
    virtual size_t  write(uint8_t c);
    virtual size_t  write(const uint8_t *buffer, size_t size);
    using   Print::write;


    uint16_t            receive();   

private:
    char                *uart_buf;
    Gpio                *tx;
    Gpio                *rx;
    uint8_t             gpio_af_usart;

    USART_TypeDef       *USARTx;
    void                (*rcc_usart_clock_cmd)(uint32_t rcc,FunctionalState state);
    uint32_t            usart_rcc;
    uint8_t             usart_irq;
    void                usart_config(uint32_t baud_rate);

    uint8_t             use_dma;
    void                (*rcc_dma_clock_cmd)(uint32_t rcc,FunctionalState state);
    uint32_t            dma_rcc;//;
    uint8_t             dma_irq;//DMA2_Stream7_IRQn;
    DMA_TypeDef         *dma;
    uint32_t            dma_channel;
    DMA_Stream_TypeDef  *dma_stream;
    void                dma_config();
    uint16_t            dma_send_string(const char *str, uint16_t length);

    char                send_buf[UART_MAX_SEND_BUF];
    void                put_string(const char *str, uint16_t length);
    void                set_busy();
    void                interrupt(FunctionalState enable);
};


#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
     
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
