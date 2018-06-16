#include "ebox_common.h"

callback_fun_type timx_cb_table[15][5];
uint16_t t2_overflow_times = 0;
uint16_t t3_overflow_times = 0;
uint16_t t4_overflow_times = 0;


extern "C" {
    void TIM1_BRK_TIM9_IRQHandler()
    {
        if(TIM_GetITStatus(TIM9 , TIM_IT_Update) == SET)
        {
            if(timx_cb_table[8][0] != 0)
                timx_cb_table[8][0]();
            TIM_ClearITPendingBit(TIM9 , TIM_FLAG_Update);
        }
   
    }
    void TIM1_UP_TIM10_IRQHandler(void)
    {

        if(TIM_GetITStatus(TIM1 , TIM_IT_Update) == SET)
        {
            if(timx_cb_table[0][0] != 0)
                timx_cb_table[0][0]();
            TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM10 , TIM_IT_Update) == SET)
        {
            if(timx_cb_table[9][0] != 0)
                timx_cb_table[9][0]();
            TIM_ClearITPendingBit(TIM10 , TIM_FLAG_Update);
        }
    }
    void TIM1_TRG_COM_TIM11_IRQHandler()
    {
        if(TIM_GetITStatus(TIM11 , TIM_IT_Update) == SET)
        {
            if(timx_cb_table[10][0] != 0)
                timx_cb_table[10][0]();
            TIM_ClearITPendingBit(TIM11 , TIM_FLAG_Update);
        }
        
    }
    void TIM8_BRK_TIM12_IRQHandler()
    {
        if(TIM_GetITStatus(TIM12 , TIM_IT_Update) == SET)
        {
            if(timx_cb_table[11][0] != 0)
                timx_cb_table[11][0]();
            TIM_ClearITPendingBit(TIM12 , TIM_FLAG_Update);
        }
   
    }
    void TIM8_UP_TIM13_IRQHandler()
    {
        if(TIM_GetITStatus(TIM8 , TIM_IT_Update) == SET)
        {
            if(timx_cb_table[7][0] != 0)
                timx_cb_table[7][0]();
            TIM_ClearITPendingBit(TIM8 , TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM13 , TIM_IT_Update) == SET)
        {
            if(timx_cb_table[12][0] != 0)
                timx_cb_table[12][0]();
            TIM_ClearITPendingBit(TIM13 , TIM_FLAG_Update);
        }
    }
    void TIM8_TRG_COM_TIM14_IRQHandler()
    {
        if(TIM_GetITStatus(TIM14 , TIM_IT_Update) == SET)
        {
            if(timx_cb_table[13][0] != 0)
                timx_cb_table[13][0]();
            TIM_ClearITPendingBit(TIM14 , TIM_FLAG_Update);
        }
        
    }
    
    
    void TIM2_IRQHandler(void)
    {

        if(TIM_GetITStatus(TIM2 , TIM_IT_Update) == SET)
        {
            t2_overflow_times++;
            if(timx_cb_table[1][0] != 0)
                timx_cb_table[1][0]();
            TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM2 , TIM_IT_CC1) == SET)
        {
            if(timx_cb_table[1][1] != 0)
                timx_cb_table[1][1]();
            TIM_ClearITPendingBit(TIM2 , TIM_FLAG_CC1);
        }
        if(TIM_GetITStatus(TIM2 , TIM_IT_CC2) == SET)
        {
            if(timx_cb_table[1][2] != 0)
                timx_cb_table[1][2]();
            TIM_ClearITPendingBit(TIM2 , TIM_FLAG_CC2);
        }
        if(TIM_GetITStatus(TIM2 , TIM_IT_CC3) == SET)
        {
            if(timx_cb_table[1][3] != 0)
                timx_cb_table[1][3]();
            TIM_ClearITPendingBit(TIM2 , TIM_FLAG_CC3);
        }
        if(TIM_GetITStatus(TIM2 , TIM_IT_CC4) == SET)
        {
            if(timx_cb_table[1][4] != 0)
                timx_cb_table[1][4]();
            TIM_ClearITPendingBit(TIM2 , TIM_FLAG_CC4);
        }


    }
    void TIM3_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM3 , TIM_IT_Update) == SET)
        {
            t3_overflow_times++;
            if(timx_cb_table[2][0] != 0)
                timx_cb_table[2][0]();
            TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM3 , TIM_IT_CC1) == SET)
        {
            if(timx_cb_table[2][1] != 0)
                timx_cb_table[2][1]();
            TIM_ClearITPendingBit(TIM2 , TIM_FLAG_CC1);
        }
        if(TIM_GetITStatus(TIM3 , TIM_IT_CC2) == SET)
        {
            if(timx_cb_table[2][2] != 0)
                timx_cb_table[2][2]();
            TIM_ClearITPendingBit(TIM3 , TIM_FLAG_CC2);
        }
        if(TIM_GetITStatus(TIM3 , TIM_IT_CC3) == SET)
        {
            if(timx_cb_table[2][3] != 0)
                timx_cb_table[2][3]();
            TIM_ClearITPendingBit(TIM3 , TIM_FLAG_CC3);
        }
        if(TIM_GetITStatus(TIM3 , TIM_IT_CC4) == SET)
        {
            if(timx_cb_table[2][4] != 0)
                timx_cb_table[2][4]();
            TIM_ClearITPendingBit(TIM3 , TIM_FLAG_CC4);
        }
    }
    void TIM4_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM4 , TIM_IT_Update) == SET)
        {
            t4_overflow_times++;
            if(timx_cb_table[3][0] != 0)
                timx_cb_table[3][0]();
            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM4 , TIM_IT_CC1) == SET)
        {
            if(timx_cb_table[3][1] != 0)
                timx_cb_table[3][1]();
            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_CC1);
        }
        if(TIM_GetITStatus(TIM4 , TIM_IT_CC2) == SET)
        {
            if(timx_cb_table[3][2] != 0)
                timx_cb_table[3][2]();
            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_CC2);
        }
        if(TIM_GetITStatus(TIM4 , TIM_IT_CC3) == SET)
        {
            if(timx_cb_table[3][3] != 0)
                timx_cb_table[3][3]();
            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_CC3);
        }
        if(TIM_GetITStatus(TIM4 , TIM_IT_CC4) == SET)
        {
            if(timx_cb_table[3][4] != 0)
                timx_cb_table[3][4]();
            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_CC4);
        }
    }

    void TIM5_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM5 , TIM_IT_Update) == SET)
        {
            if(timx_cb_table[4][0] != 0)
                timx_cb_table[4][0]();
            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM5 , TIM_IT_CC1) == SET)
        {
            if(timx_cb_table[4][1] != 0)
                timx_cb_table[4][1]();
            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC1);
        }
        if(TIM_GetITStatus(TIM5 , TIM_IT_CC2) == SET)
        {
            if(timx_cb_table[4][2] != 0)
                timx_cb_table[4][2]();
            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC2);
        }
        if(TIM_GetITStatus(TIM5 , TIM_IT_CC3) == SET)
        {
            if(timx_cb_table[4][3] != 0)
                timx_cb_table[4][3]();
            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC3);
        }
        if(TIM_GetITStatus(TIM5 , TIM_IT_CC4) == SET)
        {
            if(timx_cb_table[4][4] != 0)
                timx_cb_table[4][4]();
            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC4);
        }
    }
    void TIM6_DAC_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM6 , TIM_IT_Update) == SET)
        {
            if(timx_cb_table[5][0] != 0)
                timx_cb_table[5][0]();
            TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);
        }
    }
    void TIM7_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM7 , TIM_IT_Update) == SET)
        {
            if(timx_cb_table[6][0] != 0)
                timx_cb_table[6][0]();
            TIM_ClearITPendingBit(TIM7 , TIM_FLAG_Update);
        }
    }
    
    
    
    void DMA2_Stream7_IRQHandler(void)
    {
        if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7) != RESET) 
        {
            DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
            DMA_Cmd(DMA2_Stream7,DISABLE);
        }
    }
    void DMA1_Stream6_IRQHandler(void)
    {
        if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6) != RESET) 
        {
            DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
            DMA_Cmd(DMA1_Stream6,DISABLE);
        }
    }
    void DMA1_Stream3_IRQHandler(void)
    {
        if(DMA_GetITStatus(DMA1_Stream3,DMA_IT_TCIF3) != RESET) 
        {
            DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);
            DMA_Cmd(DMA1_Stream3,DISABLE);
        }
    }

    void DMA1_Stream4_IRQHandler(void)
    {
        if(DMA_GetITStatus(DMA1_Stream4,DMA_IT_TCIF4) != RESET) 
        {
            DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
            DMA_Cmd(DMA1_Stream4,DISABLE);
        }
    }
    void DMA1_Stream7_IRQHandler(void)
    {
        if(DMA_GetITStatus(DMA1_Stream7,DMA_IT_TCIF7) != RESET) 
        {
            DMA_ClearFlag(DMA1_Stream7,DMA_FLAG_TCIF7);
            DMA_Cmd(DMA1_Stream7,DISABLE);
        }
    }
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
