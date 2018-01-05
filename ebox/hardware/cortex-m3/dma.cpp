#include "dma.h"
#include "nvic.h"

void Dma::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
    nvic_irq_set_priority((uint32_t)DMAy_Channelx,0,0,0);
    if(enable != DISABLE)
        nvic_irq_enable((uint32_t)DMAy_Channelx,0);
    else
        nvic_irq_disable((uint32_t)DMAy_Channelx,0);

}
void Dma::interrupt(uint32_t DMA_IT,FunctionalState enable)
{
    DMA_ITConfig(DMAy_Channelx,DMA_IT,enable);
}


void Dma::deInit()
{
    DMA_DeInit(DMAy_Channelx); 
}
void Dma::init(DMA_InitTypeDef* DMA_InitStruct)
{
    DMA_Init(DMAy_Channelx,DMA_InitStruct);
}
void Dma::cmd(FunctionalState NewState)
{
    DMA_Cmd(DMAy_Channelx,NewState);
}
