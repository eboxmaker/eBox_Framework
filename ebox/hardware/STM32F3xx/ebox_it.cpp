#include "ebox_core.h"
#include "mcu.h"
#include "ebox_timer_it.h"

uint16_t t2_overflow_times = 0;
uint16_t t3_overflow_times = 0;
uint16_t t4_overflow_times = 0;

static tim_irq_handler irq_handler;
static uint32_t tim_irq_ids[TIM_IRQ_ID_NUM];//保存对象this指针

//index:定时器保存this指针数组列表中的索引值，实现将特定的对象绑定到特定的入口
//handler：类的静态方法
//id:对象的this指针
int tim_irq_init(uint8_t index, tim_irq_handler handler, uint32_t id)
{
    tim_irq_ids[index] = id;//将对象this指针保存至列表中
    irq_handler =  handler;//类的静态方法
    return 0;
}

void tim_irq_callback(uint8_t index)
{
    irq_handler(tim_irq_ids[index]);//寻找到对象的回调函数入口
}

extern "C" {
//    void TIM1_BRK_TIM9_IRQHandler()
//    {
//        if(TIM_GetITStatus(TIM9, TIM_IT_Update) == SET)
//        {
//            tim_irq_callback(TIM9_IT_Update);
//            TIM_ClearITPendingBit(TIM9, TIM_FLAG_Update);
//        }

//    }
//    void TIM1_UP_TIM10_IRQHandler(void)
//    {

//        if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
//        {
//            tim_irq_callback(TIM1_IT_Update);
//            TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
//        }
//        if(TIM_GetITStatus(TIM10, TIM_IT_Update) == SET)
//        {
//            tim_irq_callback(TIM10_IT_Update);
//            TIM_ClearITPendingBit(TIM10, TIM_FLAG_Update);
//        }
//    }
//    //    void TIM1_TRG_COM_TIM11_IRQHandler()
//    //    {
//    //        if(TIM_GetITStatus(TIM11 , TIM_IT_Update) == SET)
//    //        {
//    //            if(timx_cb_table[10][0] != 0)
//    //                timx_cb_table[10][0]();
//    //            TIM_ClearITPendingBit(TIM11 , TIM_FLAG_Update);
//    //        }
//    //
//    //    }
//    void TIM1_CC_IRQHandler()
//    {

//    }
//    //    void TIM8_BRK_TIM12_IRQHandler()
//    //    {
//    //        if(TIM_GetITStatus(TIM12 , TIM_IT_Update) == SET)
//    //        {
//    //            if(timx_cb_table[11][0] != 0)
//    //                timx_cb_table[11][0]();
//    //            TIM_ClearITPendingBit(TIM12 , TIM_FLAG_Update);
//    //        }
//    //
//    //    }
//    void TIM8_UP_TIM13_IRQHandler()
//    {
//        if(TIM_GetITStatus(TIM8, TIM_IT_Update) == SET)
//        {
//            tim_irq_callback(TIM8_IT_Update);
//            TIM_ClearITPendingBit(TIM8, TIM_FLAG_Update);
//        }
//        if(TIM_GetITStatus(TIM13, TIM_IT_Update) == SET)
//        {
//            tim_irq_callback(TIM13_IT_Update);
//            TIM_ClearITPendingBit(TIM13, TIM_FLAG_Update);
//        }
//    }
//    //    void TIM8_TRG_COM_TIM14_IRQHandler()
//    //    {
//    //        if(TIM_GetITStatus(TIM14 , TIM_IT_Update) == SET)
//    //        {
//    //            if(timx_cb_table[13][0] != 0)
//    //                timx_cb_table[13][0]();
//    //            TIM_ClearITPendingBit(TIM14 , TIM_FLAG_Update);
//    //        }
//    //
//    //    }


//    void TIM2_IRQHandler(void)
//    {

//        if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//        {
//            t2_overflow_times++;
//            tim_irq_callback(TIM2_IT_Update);
//            TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
//        }
//        if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)
//        {
//            tim_irq_callback(TIM2_IT_CC1);
//            TIM_ClearITPendingBit(TIM2, TIM_FLAG_CC1);
//        }
//        if(TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET)
//        {
//            tim_irq_callback(TIM2_IT_CC2);
//            TIM_ClearITPendingBit(TIM2, TIM_FLAG_CC2);
//        }
//        if(TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET)
//        {
//            tim_irq_callback(TIM2_IT_CC3);
//            TIM_ClearITPendingBit(TIM2, TIM_FLAG_CC3);
//        }
//        if(TIM_GetITStatus(TIM2, TIM_IT_CC4) == SET)
//        {
//            tim_irq_callback(TIM2_IT_CC4);
//            TIM_ClearITPendingBit(TIM2, TIM_FLAG_CC4);
//        }


//    }
//    void TIM3_IRQHandler(void)
//    {
//        if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
//        {
//            t3_overflow_times++;
//            tim_irq_callback(TIM3_IT_Update);
//            TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
//        }
//        if(TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET)
//        {
//            tim_irq_callback(TIM3_IT_CC1);
//            TIM_ClearITPendingBit(TIM2, TIM_FLAG_CC1);
//        }
//        if(TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET)
//        {
//            tim_irq_callback(TIM3_IT_CC2);
//            TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC2);
//        }
//        if(TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET)
//        {
//            tim_irq_callback(TIM3_IT_CC3);
//            TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC3);
//        }
//        if(TIM_GetITStatus(TIM3, TIM_IT_CC4) == SET)
//        {
//            tim_irq_callback(TIM3_IT_CC4);
//            TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC4);
//        }
//    }
//    void TIM4_IRQHandler(void)
//    {
//        if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
//        {
//            t4_overflow_times++;
//            tim_irq_callback(TIM4_IT_Update);
//            TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
//        }
//        if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET)
//        {
//            tim_irq_callback(TIM4_IT_CC1);
//            TIM_ClearITPendingBit(TIM4, TIM_FLAG_CC1);
//        }
//        if(TIM_GetITStatus(TIM4, TIM_IT_CC2) == SET)
//        {
//            tim_irq_callback(TIM4_IT_CC2);
//            TIM_ClearITPendingBit(TIM4, TIM_FLAG_CC2);
//        }
//        if(TIM_GetITStatus(TIM4, TIM_IT_CC3) == SET)
//        {
//            tim_irq_callback(TIM4_IT_CC3);
//            TIM_ClearITPendingBit(TIM4, TIM_FLAG_CC3);
//        }
//        if(TIM_GetITStatus(TIM4, TIM_IT_CC4) == SET)
//        {
//            tim_irq_callback(TIM4_IT_CC4);
//            TIM_ClearITPendingBit(TIM4, TIM_FLAG_CC4);
//        }
//    }

//    void TIM5_IRQHandler(void)
//    {
//        if(TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)
//        {
//            tim_irq_callback(TIM5_IT_Update);
//            TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
//        }
//        if(TIM_GetITStatus(TIM5, TIM_IT_CC1) == SET)
//        {
//            tim_irq_callback(TIM5_IT_CC1);
//            TIM_ClearITPendingBit(TIM5, TIM_FLAG_CC1);
//        }
//        if(TIM_GetITStatus(TIM5, TIM_IT_CC2) == SET)
//        {
//            tim_irq_callback(TIM5_IT_CC2);
//            TIM_ClearITPendingBit(TIM5, TIM_FLAG_CC2);
//        }
//        if(TIM_GetITStatus(TIM5, TIM_IT_CC3) == SET)
//        {
//            tim_irq_callback(TIM5_IT_CC3);
//            TIM_ClearITPendingBit(TIM5, TIM_FLAG_CC3);
//        }
//        if(TIM_GetITStatus(TIM5, TIM_IT_CC4) == SET)
//        {
//            tim_irq_callback(TIM5_IT_CC4);
//            TIM_ClearITPendingBit(TIM5, TIM_FLAG_CC4);
//        }
//    }
//    void TIM6_DAC_IRQHandler(void)
//    {
//        if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
//        {
//            tim_irq_callback(TIM6_IT_Update);
//            TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
//        }
//    }
//    void TIM7_IRQHandler(void)
//    {
//        if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)
//        {
//            tim_irq_callback(TIM7_IT_Update);
//            TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
//        }
//    }



//    void DMA2_Stream7_IRQHandler(void)
//    {
//        if(DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7) != RESET)
//        {
//            DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
//            DMA_Cmd(DMA2_Stream7, DISABLE);
//        }
//    }
//    void DMA1_Stream6_IRQHandler(void)
//    {
//        if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) != RESET)
//        {
//            DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
//            DMA_Cmd(DMA1_Stream6, DISABLE);
//        }
//    }
//    void DMA1_Stream3_IRQHandler(void)
//    {
//        if(DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3) != RESET)
//        {
//            DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);
//            DMA_Cmd(DMA1_Stream3, DISABLE);
//        }
//    }

//    void DMA1_Stream4_IRQHandler(void)
//    {
//        if(DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF4) != RESET)
//        {
//            DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);
//            DMA_Cmd(DMA1_Stream4, DISABLE);
//        }
//    }
//    void DMA1_Stream7_IRQHandler(void)
//    {
//        if(DMA_GetITStatus(DMA1_Stream7, DMA_IT_TCIF7) != RESET)
//        {
//            DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);
//            DMA_Cmd(DMA1_Stream7, DISABLE);
//        }
//    }
}










/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}
