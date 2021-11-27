#ifndef __DMA_H
#define __DMA_H

#include "ebox_core.h"
#include "mcu.h"
#include "FunctionPointer.h"


#define EBOX_DEBUG_DMA_ENABLE       true
#define EBOX_DEBUG_DMA_ENABLE_ERR   false

#if EBOX_DEBUG_SPI_ENABLE
#define dmaDebug(...)  ebox_printf("[DMA]:%d: ",__LINE__),ebox_printf(__VA_ARGS__ )
#else
#define dmaDebug(...)
#endif

#if EBOX_DEBUG_SPI_ENABLE_ERR
#define dmaDebugErr(fmt, ...)  ebox_printf("[DMA err]:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define dmaDebugErr(fmt, ...)
#endif


//”√ªß≈‰÷√//////////////
#if defined (STM32F10X_HD)
    #define DMA_NUM (12)
#else 
    #define DMA_NUM (7)
#endif
enum DmaIrqType
{
    DmaItTc = 0,
    DmaItTe,
    DmaItHt
};

enum DmaItIndex
{
    DmaItIndex_CH1  = 0,
    DmaItIndex_CH2  = 1,
    DmaItIndex_CH3  = 2,
    DmaItIndex_CH4  = 3,
    DmaItIndex_CH5  = 4,
    DmaItIndex_CH6  = 5,
    DmaItIndex_CH7  = 6,
    Dma2ItIndex_CH1,
    Dma2ItIndex_CH2,
    Dma2ItIndex_CH3,
    Dma2ItIndex_CH4,
    Dma2ItIndex_CH5,
} ;

typedef void (*DmaIrqHandler_t)(uint32_t id, DmaIrqType type);
class Dma
{
public:
    DMA_ChannelType *DMAy_Channelx;


public:
    Dma(DMA_ChannelType *DMAy_Channelx);

    void rcc_enable();
    void rcc_disable();
    void nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority );
    void interrupt(DmaIrqType DMA_IT, FunctionalState enable);


    void deInit();

    void init(DMA_InitType *DMA_InitStruct);
    void enable();
    void disable();
    void set_current_len(uint16_t len);
    uint16_t get_current_len();
    void wait();
    bool get_flag_status();
    void clear_flag();

    DMA_ChannelType *get_dma_ch();



    /** Attach a function to call whenever a serial interrupt is generated
    *
    *  @param fptr A pointer to a void function, or 0 to set as none
    *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
    */
    //attach user event
    void attach(void (*fptr)(void), DmaIrqType type);
    //    void interrupt(DmaIrqType type, FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);


    /** Attach a member function to call whenever a serial interrupt is generated
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void), DmaIrqType type)
    {
        if((mptr != NULL) && (tptr != NULL))
        {
            _irq[type].attach(tptr, mptr);
        }
    }

    static void _irq_handler(uint32_t id, DmaIrqType irq_type);



protected:
    FunctionPointer _irq[3];

};

#ifdef __cplusplus
extern "C" {
#endif

void dma_irq_handler(uint8_t index, DmaIrqHandler_t handler, uint32_t id);
#ifdef __cplusplus
}
#endif

extern Dma Dma1Ch1;
extern Dma Dma1Ch2;
extern Dma Dma1Ch3;
extern Dma Dma1Ch4;
extern Dma Dma1Ch5;
extern Dma Dma1Ch6;
extern Dma Dma1Ch7;
#if defined (STM32F10X_HD)

extern Dma Dma2Ch1;
extern Dma Dma2Ch2;
extern Dma Dma2Ch3;
extern Dma Dma2Ch4;
extern Dma Dma2Ch5;


#endif

#endif

