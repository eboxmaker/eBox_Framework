#ifndef __EBOX_SLAVE_SPI_H
#define __EBOX_SLAVE_SPI_H

#include "ebox_core.h"
#include "ringbuf.h"
#include "FunctionPointer.h"

//”√ªß≈‰÷√//////////////
#define SPI_NUM (3)

enum SpiIrqType {
		SpiRxIrq = 0,
		SpiTcIrq
};

enum SpiItIndex{
    SPI1_ID  = 0,
    SPI2_ID  = 1,
    SPI3_ID  = 2,
    SPI4_ID  = 1,
    SPI5_ID  = 2,
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

    void    set_rx_dma();
    void    enable_rx_dma();
    void    disable_rx_dma();


    void    set_tx_dma();
    void    enable_tx_dma();
    void    disable_tx_dma();
    void    enable_dma_tc_irq();
    void    disable_dma_tc_irq();

    void    enable_rx_int();
    void    disable_rx_int();





    void    wait_tx_over();

    void    enable_rx_irq();
    void    disable_rx_irq();

    uint8_t transfer(uint8_t data);
    
    uint8_t read_buf_pool[100];
    uint8_t write_buf_pool[100];
    RINGBUF read_buf;
    RINGBUF write_buf;
//    virtual uint8_t read_config(void);

//    virtual uint8_t transfer(uint8_t data);

//    virtual int8_t  write(uint8_t data);
//    virtual int8_t  write(uint8_t *data, uint16_t data_length);

//    virtual uint8_t read();
//    virtual int8_t  read(uint8_t  *recv_data);
//    virtual int8_t  read(uint8_t *recv_data, uint16_t data_length);
//public:
//    virtual int8_t take_spi_right(SpiConfig_t *spi_config);
//    virtual int8_t release_spi_right(void);
    uint8_t dma_buf[256];
    
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
    void attach(T* tptr, void (T::*mptr)(void), SpiIrqType type) {
        if((mptr != NULL) && (tptr != NULL)) {
            _irq[type].attach(tptr, mptr);
        }
    }
		
    static void _irq_handler(uint32_t id, SpiIrqType irq_type);
    void    nvic(FunctionalState enable,uint8_t preemption_priority = 0, uint8_t sub_priority = 0);
    uint8_t     *xfet;
    uint16_t     xlen;

private:
    SPI_TypeDef *spi;
    uint8_t     busy;
    DMA_Channel_TypeDef *_DMA1_Channelx;
protected:
    FunctionPointer _irq[2];

};

#ifdef __cplusplus
extern "C" {
#endif

void spi_irq_handler(uint8_t index, SpiIrqHandler_t handler, uint32_t id);
#ifdef __cplusplus
}
#endif
#endif


