#include "dma.h"

static uint32_t dma_irq_ids[DMA_NUM] = {0, 0, 0,0,0,0,0};

static DmaIrqHandler_t irq_handler;



/**
 *@name     Dma::Dma(DMA_Stream_TypeDef* DMAy_Streamx)
 *@brief    Dma构造函数，传递DMAxStreamx参数和中断指针
 *@param    DMAy_Streamx :  DMA1_Stream0
 *@retval   NONE
*/
Dma::Dma(uint32_t DMAy_Channelx)
{
//    int index;
    this->DMAy_Channelx = DMAy_Channelx;
//    if(((uint32_t)DMAy_Channelx - DMA1_Channel1_BASE) <= 0x80 )
//        index = ((uint32_t)DMAy_Channelx - DMA1_Channel1_BASE) / 20;
    dma_irq_handler(DMAy_Channelx, Dma::_irq_handler, (uint32_t)this);

}
/**
 *@name     void Dma::rcc_enable()
 *@brief    开启Dma时钟
 *@param    NONE
 *@retval   NONE
*/
void Dma::rcc_enable()
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
//    switch((uint32_t)DMAy_Channelx)
//    {
//        case DMA1_Channel1_BASE:
//        case DMA1_Channel2_BASE:
//        case DMA1_Channel3_BASE:
//        case DMA1_Channel4_BASE:
//        case DMA1_Channel5_BASE:
//        case DMA1_Channel6_BASE:
//        case DMA1_Channel7_BASE:
//            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA时钟
//        break;
//        #ifdef STM32F10X_HD
//        case DMA2_Channel1_BASE:
//        case DMA2_Channel2_BASE:
//        case DMA2_Channel3_BASE:
//        case DMA2_Channel4_BASE:
//            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	//使能DMA时钟
//        #endif
//    }
}
/**
 *@name     void Dma::rcc_disable()
 *@brief    关闭Dma时钟
 *@param    NONE
 *@retval   NONE
*/
void Dma::rcc_disable()
{
      LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    
//    switch((uint32_t)DMAy_Channelx)
//    {
//        case DMA1_Channel1_BASE:
//        case DMA1_Channel2_BASE:
//        case DMA1_Channel3_BASE:
//        case DMA1_Channel4_BASE:
//        case DMA1_Channel5_BASE:
//        case DMA1_Channel6_BASE:
//        case DMA1_Channel7_BASE:
//            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE);	//使能DMA时钟
//        break;
//        #ifdef STM32F10X_HD
//        case DMA2_Channel1_BASE:
//        case DMA2_Channel2_BASE:
//        case DMA2_Channel3_BASE:
//        case DMA2_Channel4_BASE:
//            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, DISABLE);	//使能DMA时钟
//        #endif
//    }
}


/**
 *@name     void Dma::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
 *@brief    Dma中断的优先级和允许位设置
 *@param    enable              :  ENABLE or DISABLE
            preemption_priority :  0-3
            sub_priority        :  0-3
 *@retval   NONE
*/
void Dma::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
    nvic_dev_set_priority((uint32_t)DMAy_Channelx,0,0,0);
    if(enable != DISABLE)
        nvic_dev_enable((uint32_t)DMAy_Channelx,0);
    else
        nvic_dev_disable((uint32_t)DMAy_Channelx,0);

}


/**
 *@name     void Dma::interrupt(DmaIrqType DMA_IT,FunctionalState enable)
 *@param    DMA_IT  :  中断类型，DmaItTc or DmaItHt
            enable  : ENABLE or DISABLE
 *@retval   NONE
*/
void Dma::interrupt(DmaIrqType DMA_IT,FunctionalState enable)
{
    if(DMA_IT == DmaItTc)
        LL_DMA_EnableIT_TC(DMA1, DMAy_Channelx);
        //DMA_ITConfig(DMAy_Channelx,DMA_IT_TC,enable);
    else if(DMA_IT == DmaItTe)
        LL_DMA_EnableIT_TE(DMA1, DMAy_Channelx);
        //DMA_ITConfig(DMAy_Channelx,DMA_IT_TE,enable);
    else if(DMA_IT == DmaItHt)
//        DMA_ITConfig(DMAy_Channelx,DMA_IT_HT,enable);
        LL_DMA_EnableIT_HT(DMA1, DMAy_Channelx);

}


/**
 *@name     void Dma::deInit()
 *@brief    Dma传输设置为默认值
 *@param    NONE
 *@retval   NONE
*/
void Dma::deInit()
{
//    DMA_DeInit(DMAy_Channelx); 
    LL_DMA_DeInit(DMA1,DMAy_Channelx);
}

/**
 *@name     void Dma::init(DMA_InitTypeDef* DMA_InitStruct)
 *@brief    Dma传输设置为DMA_InitStruct
 *@param    DMA_InitStruct  :  初始化结构体
 *@retval   NONE
*/
void Dma::init(LL_DMA_InitTypeDef* DMA_InitStruct)
{
    //DMA_Init(DMAy_Channelx,DMA_InitStruct);
    LL_DMA_Init(DMA1,DMAy_Channelx,DMA_InitStruct);
}


/**
 *@name     void Dma::enable()
 *@brief    Dma开启传输
 *@param    NONE
 *@retval   NONE
*/
void Dma::enable()
{
 //   DMA_Cmd(DMAy_Channelx,ENABLE);
    LL_DMA_EnableChannel(DMA1,DMAy_Channelx);

}

/**
 *@name     void Dma::enable()
 *@brief    Dma关闭传输
 *@param    NONE
 *@retval   NONE
*/
void Dma::disable()
{
//    DMA_Cmd(DMAy_Channelx,DISABLE);
    LL_DMA_DisableChannel(DMA1,DMAy_Channelx);
}

/**
 *@name     void Dma::enable()
 *@brief    获取Dma流信息
 *@param    NONE
 *@retval   DMAy_Streamx
*/
uint32_t Dma::get_dma_ch()
{
    return DMAy_Channelx;
}


/**
 *@name     void Dma::attach(void (*fptr)(void), DmaIrqType type) 
 *@brief    绑定中断回调函数
 *@param    NONE
 *@retval   NONE
*/
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
    
    
    
    void DMA1_Channel1_IRQHandler(void)
    {
        if(LL_DMA_IsActiveFlag_TC1(DMA1) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH1],DmaItTc);
            //DMA_ClearITPendingBit(DMA1_IT_TC1);
            LL_DMA_ClearFlag_GI1(DMA1);
            return ;
        }
    }
    void DMA1_Channel2_IRQHandler(void)
    {
        if(LL_DMA_IsActiveFlag_TC2(DMA1) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH2],DmaItTc);
            //DMA_ClearITPendingBit(DMA1_IT_TC2);
            LL_DMA_ClearFlag_GI2(DMA1);
        }
    }
    void DMA1_Channel3_IRQHandler(void)
    {
        if(LL_DMA_IsActiveFlag_TC3(DMA1) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH3],DmaItTc);
            LL_DMA_ClearFlag_GI3(DMA1);
            return ;
        }
    }
//    void DMA1_Channel4_IRQHandler(void)
//    {
//        if(LL_DMA_IsActiveFlag_TC4(DMA1) == SET)
//        {
//            irq_handler(dma_irq_ids[DMA1_CH4],DmaItTc);
//            LL_DMA_ClearFlag_GI4(DMA1);            
//        }
//    }
    void DMA1_Channel5_IRQHandler(void)
    {
        if(LL_DMA_IsActiveFlag_TC5(DMA1) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH5],DmaItTc);
            LL_DMA_ClearFlag_GI5(DMA1);
            return ;
        }
    }
    void DMA1_Channel6_IRQHandler(void)
    {
        if(LL_DMA_IsActiveFlag_TC6(DMA1) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH6],DmaItTc);
            LL_DMA_ClearFlag_GI6(DMA1);
            
        }
    }
    void DMA1_Channel4_5_6_7_IRQnHandler(void)
    {
      if(LL_DMA_IsActiveFlag_TC4(DMA1) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH4],DmaItTc);
            LL_DMA_ClearFlag_GI4(DMA1);            
        }
        if(LL_DMA_IsActiveFlag_TC7(DMA1) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH7],DmaItTc);
            LL_DMA_ClearFlag_GI7(DMA1);
            return ;
        }
    }



    
    
#ifdef __cplusplus
}
#endif

Dma Dma1Ch1(LL_DMA_CHANNEL_1);
Dma Dma1Ch2(LL_DMA_CHANNEL_2);
Dma Dma1Ch3(LL_DMA_CHANNEL_3);
Dma Dma1Ch4(LL_DMA_CHANNEL_4);
Dma Dma1Ch5(LL_DMA_CHANNEL_5);
Dma Dma1Ch6(LL_DMA_CHANNEL_6);
Dma Dma1Ch7(LL_DMA_CHANNEL_7);

