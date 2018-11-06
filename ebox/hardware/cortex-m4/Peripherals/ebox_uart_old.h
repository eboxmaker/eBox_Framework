#ifndef __EBOX_UART_H
#define __EBOX_UART_H
#include "ebox_core.h"
#include "mcu.h"
#include "FunctionPointer.h"
#include "dma.h"
#include "print.h"

//”√ªß≈‰÷√//////////////

#define UART_NUM (8)

enum IrqType
{
    RxIrq = 0,
    TcIrq
};
enum Uart_It_Index
{
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


class Uart: public Print
{
public:
    Uart(USART_TypeDef *USARTx, Gpio *tx, Gpio *rx);

    void    begin(uint32_t baud_rate);
    void    begin(uint32_t baud_rate, uint8_t use_dma);
    void    begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit);


    //write method
    virtual size_t  write(uint8_t c);
    virtual size_t  write(const uint8_t *buffer, size_t size);
    using   Print::write;

    //read method
    uint16_t    read();

    void    wait_busy();


    void    nvic(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);
    void    attach(void (*fptr)(void), IrqType type);
    void    interrupt(IrqType type, FunctionalState enable);

    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void), IrqType type)
    {
        if((mptr != NULL) && (tptr != NULL))
        {
            _irq[type].attach(tptr, mptr);
        }
    }
    static void _irq_handler(uint32_t id, IrqType irq_type);
protected:
    FunctionPointer _irq[2];
private:
    char                *tx_buf_ptr;
    Gpio                *tx;
    Gpio                *rx;
    uint8_t             gpio_af_usart;

    USART_TypeDef       *USARTx;
    void                config(uint32_t baud_rate);

    uint8_t             use_dma;
    uint32_t            dma_channel;
    uint16_t            dma_write(const char *buf, uint16_t length);

    void                set_busy();


    Dma                 *dma_tx;

};


#ifdef __cplusplus
extern "C" {
#endif

void serial_irq_handler(uint8_t index, uart_irq_handler handler, uint32_t id);
#ifdef __cplusplus
}
#endif

#endif
