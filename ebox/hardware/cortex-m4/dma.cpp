#include "dma.h"
#include "nvic.h"

static uint32_t dma_irq_ids[DMA_NUM] = {0, 0, 0,0,0,0,0};

static DmaIrqHandler_t irq_handler;


Dma::Dma(DMA_Stream_TypeDef* DMAy_Streamx)
{
    int index;
    this->DMAy_Streamx = DMAy_Streamx;
    if(((uint32_t)DMAy_Streamx - DMA1_Stream1_BASE) <= 0x80 )
        index = ((uint32_t)DMAy_Streamx - DMA1_Stream1_BASE) / 20;
    dma_irq_handler(index, Dma::_irq_handler, (uint32_t)this);

}
void Dma::rcc_enable()
{
    switch((uint32_t)DMAy_Streamx)
    {
        case DMA1_Stream0_BASE:
        case DMA1_Stream1_BASE:
        case DMA1_Stream2_BASE:
        case DMA1_Stream3_BASE:
        case DMA1_Stream4_BASE:
        case DMA1_Stream5_BASE:
        case DMA1_Stream6_BASE:
        case DMA1_Stream7_BASE:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	//使能DMA时钟
        break;
        case DMA2_Stream0_BASE:
        case DMA2_Stream1_BASE:
        case DMA2_Stream2_BASE:
        case DMA2_Stream3_BASE:
        case DMA2_Stream4_BASE:
        case DMA2_Stream5_BASE:
        case DMA2_Stream6_BASE:
        case DMA2_Stream7_BASE:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);	//使能DMA时钟
    }
}
void Dma::rcc_disable()
{
    switch((uint32_t)DMAy_Streamx)
    {
        case DMA1_Stream0_BASE:
        case DMA1_Stream1_BASE:
        case DMA1_Stream2_BASE:
        case DMA1_Stream3_BASE:
        case DMA1_Stream4_BASE:
        case DMA1_Stream5_BASE:
        case DMA1_Stream6_BASE:
        case DMA1_Stream7_BASE:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, DISABLE);	//使能DMA时钟
        break;
        case DMA2_Stream0_BASE:
        case DMA2_Stream1_BASE:
        case DMA2_Stream2_BASE:
        case DMA2_Stream3_BASE:
        case DMA2_Stream4_BASE:
        case DMA2_Stream5_BASE:
        case DMA2_Stream6_BASE:
        case DMA2_Stream7_BASE:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, DISABLE);	//使能DMA时钟
    }
}

void Dma::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
    nvic_irq_set_priority((uint32_t)DMAy_Streamx,0,0,0);
    if(enable != DISABLE)
        nvic_irq_enable((uint32_t)DMAy_Streamx,0);
    else
        nvic_irq_disable((uint32_t)DMAy_Streamx,0);

}
void Dma::interrupt(DmaIrqType DMA_IT,FunctionalState enable)
{
    if(DMA_IT == DmaItTc)
        DMA_ITConfig(DMAy_Streamx,DMA_IT_TC,enable);
    else if(DMA_IT == DmaItTe)
        DMA_ITConfig(DMAy_Streamx,DMA_IT_TE,enable);
    else if(DMA_IT == DmaItHt)
        DMA_ITConfig(DMAy_Streamx,DMA_IT_HT,enable);

}


void Dma::deInit()
{
    DMA_DeInit(DMAy_Streamx); 
}
void Dma::init(DMA_InitTypeDef* DMA_InitStruct)
{
    DMA_Init(DMAy_Streamx,DMA_InitStruct);
}

void Dma::enable()
{
    DMA_Cmd(DMAy_Streamx,ENABLE);

}
void Dma::disable()
{
    DMA_Cmd(DMAy_Streamx,DISABLE);

}
DMA_Stream_TypeDef* Dma::get_dma_ch()
{
    return DMAy_Streamx;
}


void Dma::attach(void (*fptr)(void), DmaIrqType type) 
{
    if (fptr) 
    {
        _irq[type].attach(fptr);
    }
}

void Dma::_irq_handler(uint32_t id, DmaIrqType irq_type)
{
    Dma *handler = (Dma*)id;
    handler->_irq[irq_type].call();
}



#ifdef __cplusplus
extern "C" {
#endif    
    void dma_irq_handler(uint8_t index, DmaIrqHandler_t handler, uint32_t id)
    {
        irq_handler = handler;
        dma_irq_ids[index] = id;
    }
    
//    void dma_irq_handler(uint8_t index, DmaIrqHandler_t handler, uint32_t id)
//    {
//        irq_handler = handler;
//        dma_irq_ids[index] = id;
//    }
//    
//    
//    
//    void DMA1_Stream1_IRQHandler(void)
//    {
//        if(DMA_GetITStatus(DMA1_Stream1,DMA_IT_TCIF1) == SET)
//        {
//            irq_handler(dma_irq_ids[DMA1_CH1],DmaItTc);
//            DMA_ClearITPendingBit(DMA1_Stream1,DMA_IT_TCIF1);
//            return ;
//        }
//    }
//    void DMA1_Stream2_IRQHandler(void)
//    {
//        if(DMA_GetITStatus(DMA1_IT_TC2) == SET)
//        {
//            irq_handler(dma_irq_ids[DMA1_CH2],DmaItTc);
//            DMA_ClearITPendingBit(DMA1_IT_TC2);
//            
//        }
//    }
//    void DMA1_Stream3_IRQHandler(void)
//    {
//        if(DMA_GetITStatus(DMA1_IT_TC3) == SET)
//        {
//            irq_handler(dma_irq_ids[DMA1_CH3],DmaItTc);
//            DMA_ClearITPendingBit(DMA1_IT_TC3);
//            return ;
//        }
//    }
//    void DMA1_Stream4_IRQHandler(void)
//    {
//        if(DMA_GetITStatus(DMA1_IT_TC4) == SET)
//        {
//            irq_handler(dma_irq_ids[DMA1_CH4],DmaItTc);
//            DMA_ClearITPendingBit(DMA1_IT_TC4);
//            
//        }
//    }
//    void DMA1_Stream5_IRQHandler(void)
//    {
//        if(DMA_GetITStatus(DMA1_IT_TC5) == SET)
//        {
//            irq_handler(dma_irq_ids[DMA1_CH5],DmaItTc);
//            DMA_ClearITPendingBit(DMA1_IT_TC5);
//            return ;
//        }
//    }
//    void DMA1_Stream6_IRQHandler(void)
//    {
//        if(DMA_GetITStatus(DMA1_IT_TC6) == SET)
//        {
//            irq_handler(dma_irq_ids[DMA1_CH6],DmaItTc);
//            DMA_ClearITPendingBit(DMA1_IT_TC6);
//            
//        }
//    }
//    void DMA1_Stream7_IRQHandler(void)
//    {
//        if(DMA_GetITStatus(DMA1_IT_TC7) == SET)
//        {
//            irq_handler(dma_irq_ids[DMA1_CH7],DmaItTc);
//            DMA_ClearITPendingBit(DMA1_IT_TC7);
//            return ;
//        }
//    }



    
    
#ifdef __cplusplus
}
#endif

Dma Dma1Stream0(DMA1_Stream0);
Dma Dma1Stream1(DMA1_Stream1);
Dma Dma1Stream2(DMA1_Stream2);
Dma Dma1Stream3(DMA1_Stream3);
Dma Dma1Stream4(DMA1_Stream4);
Dma Dma1Stream5(DMA1_Stream5);
Dma Dma1Stream6(DMA1_Stream6);
Dma Dma1Stream7(DMA1_Stream7);

Dma Dma2Stream0(DMA2_Stream0);
Dma Dma2Stream1(DMA2_Stream1);
Dma Dma2Stream2(DMA2_Stream2);
Dma Dma2Stream3(DMA2_Stream3);
Dma Dma2Stream4(DMA2_Stream4);
Dma Dma2Stream5(DMA2_Stream5);
Dma Dma2Stream6(DMA2_Stream6);
Dma Dma2Stream7(DMA2_Stream7);
    
