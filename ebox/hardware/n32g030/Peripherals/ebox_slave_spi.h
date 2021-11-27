#ifndef __EBOX_SLAVE_SPI_H
#define __EBOX_SLAVE_SPI_H

#include "ebox_core.h"
#include "FunctionPointer.h"

#include "dma.h"

//”√ªß≈‰÷√//////////////
#define SPI_NUM (3)

enum SpiIrqType
{
    SpiItRx = 0,
    SpiItTc,
    SpiItRc
};

enum SpiItIndex
{
    SPI1_ID  = 0,
    SPI2_ID  = 1,
    SPI3_ID  = 2,
    SPI4_ID  = 3,
    SPI5_ID  = 4,
} ;

typedef void (*SpiIrqHandler_t)(uint32_t id, SpiIrqType type);

class	SlaveSpi
{
public:
    SlaveSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi);

    void    begin ();
    void    config(SpiConfig_t *spi_config);

    void    write(uint8_t data);
    uint8_t read();
    void    enable_rx_int();
    void    disable_rx_int();

    void    set_rx_dma();
    void    enable_rx_dma();
    void    disable_rx_dma();
    void    enable_dma_rx_int();
    void    disable_dma_rx_int();


    void    set_tx_dma();
    void    enable_tx_dma();
    void    disable_tx_dma();
    void    enable_dma_tx_int();
    void    disable_dma_tx_int();


    void    int_rx_event();
    void    dma_rx_over_event();
    void    dma_tx_over_event();

    uint8_t read_buf_pool[500];
    uint8_t write_buf_pool[500];

    uint8_t cmd_buf_pool[500];

    RINGBUF read_buf;
    RINGBUF write_buf;
    RINGBUF cmd_buf;

    uint8_t     *xfet;
    uint16_t     xlen;

    /** Attach a function to call whenever a serial interrupt is generated
     *
     *  @param fptr A pointer to a void function, or 0 to set as none
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    //attach user event
    void attach(void (*fptr)(void), SpiIrqType type);
    void interrupt(SpiIrqType type, FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);


    /** Attach a member function to call whenever a serial interrupt is generated
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void), SpiIrqType type)
    {
        if((mptr != NULL) && (tptr != NULL))
        {
            _irq[type].attach(tptr, mptr);
        }
    }

    static void _irq_handler(uint32_t id, SpiIrqType irq_type);

private:
    SPI_TypeDef *spi;
    Dma         *dma_tx;
    Dma         *dma_rx;

protected:
    FunctionPointer _irq[3];

};

#ifdef __cplusplus
extern "C" {
#endif

void spi_irq_handler(uint8_t index, SpiIrqHandler_t handler, uint32_t id);
#ifdef __cplusplus
}
#endif

#endif


