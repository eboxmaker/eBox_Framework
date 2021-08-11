#ifndef __DMA_H
#define __DMA_H

#include "ebox_core.h"
#include "mcu.h"
#include "FunctionPointer.h"
#include "stm32f3xx_ll_dma.h"

//”√ªß≈‰÷√//////////////
#define DMA_NUM (7)
enum DmaIrqType
{
    DmaItTc = 0,
    DmaItTe,
    DmaItHt
};

enum DmaItIndex
{
    DMA1_CH1  = 0,
    DMA1_CH2  = 1,
    DMA1_CH3  = 2,
    DMA1_CH4  = 3,
    DMA1_CH5  = 4,
    DMA1_CH6  = 5,
    DMA1_CH7  = 6,
} ;

typedef void (*DmaIrqHandler_t)(uint32_t id, DmaIrqType type);
class Dma
{

public:
    uint32_t DMAy_Channelx;
public:
    Dma(uint32_t DMAy_Channelx);

    void rcc_enable();
    void rcc_disable();
    void nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority );
    void interrupt(DmaIrqType DMA_IT, FunctionalState enable);


    void deInit();

    void init(LL_DMA_InitTypeDef *DMA_InitStruct);
    void enable();
    void disable();
    void set_current_len(uint16_t len);
    bool get_flag_status();
    void clear_flag();
    uint32_t get_dma_ch();

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
#endif


