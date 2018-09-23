#include "dma.h"

static uint32_t dma_irq_ids[DMA_NUM] = {0, 0, 0,0,0,0,0};

static DmaIrqHandler_t irq_handler;



/**
 *@name     Dma::Dma(DMA_Stream_TypeDef* DMAy_Streamx)
 *@brief    Dma构造函数，传递DMAxStreamx参数和中断指针
 *@param    DMAy_Streamx :  DMA1_Stream0
 *@retval   NONE
*/
Dma::Dma(DMA_Channel_TypeDef* DMAy_Channelx)
{
    int index;
    this->DMAy_Channelx = DMAy_Channelx;
    if(((uint32_t)DMAy_Channelx - DMA1_Channel1_BASE) <= 0x80 )
        index = ((uint32_t)DMAy_Channelx - DMA1_Channel1_BASE) / 20;
    dma_irq_handler(index, Dma::_irq_handler, (uint32_t)this);

}
/**
 *@name     void Dma::rcc_enable()
 *@brief    开启Dma时钟
 *@param    NONE
 *@retval   NONE
*/
void Dma::rcc_enable()
{
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
/**
 *@name     void Dma::rcc_disable()
 *@brief    关闭Dma时钟
 *@param    NONE
 *@retval   NONE
*/
void Dma::rcc_disable()
{
    switch((uint32_t)DMAy_Channelx)
    {
        case DMA1_Channel1_BASE:
        case DMA1_Channel2_BASE:
        case DMA1_Channel3_BASE:
        case DMA1_Channel4_BASE:
        case DMA1_Channel5_BASE:
        case DMA1_Channel6_BASE:
        case DMA1_Channel7_BASE:
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE);	//使能DMA时钟
        break;
        #ifdef STM32F10X_HD
        case DMA2_Channel1_BASE:
        case DMA2_Channel2_BASE:
        case DMA2_Channel3_BASE:
        case DMA2_Channel4_BASE:
        case DMA2_Channel5_BASE:
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, DISABLE);	//使能DMA时钟
        #endif
    }
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
        DMA_ITConfig(DMAy_Channelx,DMA_IT_TC,enable);
    else if(DMA_IT == DmaItTe)
        DMA_ITConfig(DMAy_Channelx,DMA_IT_TE,enable);
    else if(DMA_IT == DmaItHt)
        DMA_ITConfig(DMAy_Channelx,DMA_IT_HT,enable);

}


/**
 *@name     void Dma::deInit()
 *@brief    Dma传输设置为默认值
 *@param    NONE
 *@retval   NONE
*/
void Dma::deInit()
{
    DMA_DeInit(DMAy_Channelx); 
}

/**
 *@name     void Dma::init(DMA_InitTypeDef* DMA_InitStruct)
 *@brief    Dma传输设置为DMA_InitStruct
 *@param    DMA_InitStruct  :  初始化结构体
 *@retval   NONE
*/
void Dma::init(DMA_InitTypeDef* DMA_InitStruct)
{
    DMA_Init(DMAy_Channelx,DMA_InitStruct);
}


/**
 *@name     void Dma::enable()
 *@brief    Dma开启传输
 *@param    NONE
 *@retval   NONE
*/
void Dma::enable()
{
    DMA_Cmd(DMAy_Channelx,ENABLE);

}

/**
 *@name     void Dma::enable()
 *@brief    Dma关闭传输
 *@param    NONE
 *@retval   NONE
*/
void Dma::disable()
{
    DMA_Cmd(DMAy_Channelx,DISABLE);

}
void Dma::set_current_len(uint16_t len)
{

    DMA_SetCurrDataCounter(DMAy_Channelx, len);
}
bool Dma::get_flag_status()
{
    switch((uint32_t)DMAy_Channelx)
    {
        case (uint32_t)DMA1_Channel1:
            return DMA_GetFlagStatus(DMA1_FLAG_TC1);
        case (uint32_t)DMA1_Channel2:
            return DMA_GetFlagStatus(DMA1_FLAG_TC2);
        case (uint32_t)DMA1_Channel3:
            return DMA_GetFlagStatus(DMA1_FLAG_TC3);
        case (uint32_t)DMA1_Channel4:
            return DMA_GetFlagStatus(DMA1_FLAG_TC4);
        case (uint32_t)DMA1_Channel5:
            return DMA_GetFlagStatus(DMA1_FLAG_TC5);
        case (uint32_t)DMA1_Channel6:
            return DMA_GetFlagStatus(DMA1_FLAG_TC6);
        case (uint32_t)DMA1_Channel7:
            return DMA_GetFlagStatus(DMA1_FLAG_TC7);
        default:
            break;
    }
}
void Dma::clear_flag()
{
    switch((uint32_t)DMAy_Channelx)
    {
        case (uint32_t)DMA1_Channel1:
             DMA_ClearFlag(DMA1_FLAG_TC1);break;
        case (uint32_t)DMA1_Channel2:
             DMA_ClearFlag(DMA1_FLAG_TC2);break;
        case (uint32_t)DMA1_Channel3:
             DMA_ClearFlag(DMA1_FLAG_TC3);break;
        case (uint32_t)DMA1_Channel4:
             DMA_ClearFlag(DMA1_FLAG_TC4);break;
        case (uint32_t)DMA1_Channel5:
             DMA_ClearFlag(DMA1_FLAG_TC5);break;
        case (uint32_t)DMA1_Channel6:
             DMA_ClearFlag(DMA1_FLAG_TC6);break;
        case (uint32_t)DMA1_Channel7:
             DMA_ClearFlag(DMA1_FLAG_TC7);break;
        default:
            break;
    }
}

/**
 *@name     void Dma::enable()
 *@brief    获取Dma流信息
 *@param    NONE
 *@retval   DMAy_Streamx
*/
DMA_Channel_TypeDef* Dma::get_dma_ch()
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
        if(DMA_GetITStatus(DMA1_IT_TC1) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH1],DmaItTc);
            DMA_ClearITPendingBit(DMA1_IT_TC1);
            return ;
        }
    }
    void DMA1_Channel2_IRQHandler(void)
    {
        if(DMA_GetITStatus(DMA1_IT_TC2) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH2],DmaItTc);
            DMA_ClearITPendingBit(DMA1_IT_TC2);
            
        }
    }
    void DMA1_Channel3_IRQHandler(void)
    {
        if(DMA_GetITStatus(DMA1_IT_TC3) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH3],DmaItTc);
            DMA_ClearITPendingBit(DMA1_IT_TC3);
            return ;
        }
    }
    void DMA1_Channel4_IRQHandler(void)
    {
        if(DMA_GetITStatus(DMA1_IT_TC4) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH4],DmaItTc);
            DMA_ClearITPendingBit(DMA1_IT_TC4);
            
        }
    }
    void DMA1_Channel5_IRQHandler(void)
    {
        if(DMA_GetITStatus(DMA1_IT_TC5) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH5],DmaItTc);
            DMA_ClearITPendingBit(DMA1_IT_TC5);
            return ;
        }
    }
    void DMA1_Channel6_IRQHandler(void)
    {
        if(DMA_GetITStatus(DMA1_IT_TC6) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH6],DmaItTc);
            DMA_ClearITPendingBit(DMA1_IT_TC6);
            
        }
    }
    void DMA1_Channel7_IRQHandler(void)
    {
        if(DMA_GetITStatus(DMA1_IT_TC7) == SET)
        {
            irq_handler(dma_irq_ids[DMA1_CH7],DmaItTc);
            DMA_ClearITPendingBit(DMA1_IT_TC7);
            return ;
        }
    }



    
    
#ifdef __cplusplus
}
#endif
#if USE_DMA
Dma Dma1Ch1(DMA1_Channel1);
Dma Dma1Ch2(DMA1_Channel2);
Dma Dma1Ch3(DMA1_Channel3);
Dma Dma1Ch4(DMA1_Channel4);
Dma Dma1Ch5(DMA1_Channel5);
Dma Dma1Ch6(DMA1_Channel6);
Dma Dma1Ch7(DMA1_Channel7);
#endif
