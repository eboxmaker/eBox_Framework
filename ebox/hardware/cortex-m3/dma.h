#ifndef __DMA_H
#define __DMA_H

#include "ebox_core.h"

class Dma
{
    public:
        Dma(DMA_Channel_TypeDef* DMAy_Channelx)
        {
            this->DMAy_Channelx = DMAy_Channelx;
            switch((uint32_t)DMAy_Channelx)
            {
                case DMA1_Channel1_BASE:
                case DMA1_Channel2_BASE:
                case DMA1_Channel3_BASE:
                case DMA1_Channel4_BASE:
                case DMA1_Channel5_BASE:
                case DMA1_Channel6_BASE:
                case DMA1_Channel7_BASE:
                    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA时钟
                break;
                #ifdef STM32F10X_HD
                case DMA2_Channel1_BASE:
                case DMA2_Channel2_BASE:
                case DMA2_Channel3_BASE:
                case DMA2_Channel4_BASE:
                    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	//使能DMA时钟
                #endif

            
            }
            
        }
        
        void nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority );
        void interrupt(uint32_t DMA_IT,FunctionalState enable);


        void deInit();
        void init(DMA_InitTypeDef* DMA_InitStruct);
        void cmd(FunctionalState NewState);

    private:
        DMA_Channel_TypeDef* DMAy_Channelx;

};
#endif

