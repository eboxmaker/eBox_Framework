/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file n32g030_adc.c
 * @author Nations 
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g030_adc.h"
#include "n32g030_rcc.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup ADC
 * @brief ADC driver modules
 * @{
 */

/** @addtogroup ADC_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup ADC_Private_Defines
 * @{
 */

/* ADC DISC_NUM mask */
#define CTRL1_DISCNUM_Reset ((uint32_t)0xFFFF1FFF)

/* ADC DISC_EN mask */
#define CTRL1_DISC_EN_SET   ((uint32_t)0x00000800)
#define CTRL1_DISC_EN_RESET ((uint32_t)0xFFFFF7FF)

/* ADC INJ_AUTO mask */
#define CTRL1_JAUTO_Set   ((uint32_t)0x00000400)
#define CTRL1_JAUTO_Reset ((uint32_t)0xFFFFFBFF)

/* ADC INJ_DISC_EN mask */
#define CTRL1_INJ_DISC_EN_SET   ((uint32_t)0x00001000)
#define CTRL1_INJ_DISC_EN_RESET ((uint32_t)0xFFFFEFFF)

/* ADC AWDG_CH mask */
#define CTRL1_AWDG_CH_RESET ((uint32_t)0xFFFFFFE0)     //Clear the Analog watchdog channel select bits

/* ADC Analog watchdog enable mode mask */
#define CTRL1_AWDG_MODE_RESET ((uint32_t)0xFF3FFDFF)   //Clear AWDEN, AWDENJ and AWDSGL bits

/* CTRL1 register Mask */
#define CTRL1_CLR_MASK ((uint32_t)0xFFFFFEFF)          //clear scan mode

/* ADC AD_ON mask */
#define CTRL2_AD_ON_SET   ((uint32_t)0x00000001)
#define CTRL2_AD_ON_RESET ((uint32_t)0xFFFFFFFE)

/* ADC DMA mask */
#define CTRL2_DMA_SET   ((uint32_t)0x00000100)
#define CTRL2_DMA_RESET ((uint32_t)0xFFFFFEFF)

/* ADC SOFT_START mask */
#define CTRL2_SOFT_START_SET ((uint32_t)0x00400000)

/* ADC EXT_TRIG mask */
#define CTRL2_EXT_TRIG_SET   ((uint32_t)0x00100000)
#define CTRL2_EXT_TRIG_RESET ((uint32_t)0xFFEFFFFF)

/* ADC Software start mask */
#define CTRL2_EXT_TRIG_SWSTART_SET   ((uint32_t)0x00500000)
#define CTRL2_EXT_TRIG_SWSTART_RESET ((uint32_t)0xFFAFFFFF)

/* ADC INJ_EXT_SEL mask */
#define CTRL2_INJ_EXT_SEL_RESET ((uint32_t)0xFFFF8FFF)

/* ADC INJ_EXT_TRIG mask */
#define CTRL2_INJ_EXT_TRIG_SET   ((uint32_t)0x00008000)
#define CTRL2_INJ_EXT_TRIG_RESET ((uint32_t)0xFFFF7FFF)

/* ADC INJ_SWSTART mask */
#define CTRL2_INJ_SWSTART_SET ((uint32_t)0x00200000)

/* ADC injected software start mask */
#define CTRL2_INJ_EXT_TRIG_JSWSTART_SET   ((uint32_t)0x00208000)
#define CTRL2_INJ_EXT_TRIG_JSWSTART_RESET ((uint32_t)0xFFDF7FFF)

/* ADC TSPD mask */
#define CTRL2_TSVREFE_SET   ((uint32_t)0x00800000)
#define CTRL2_TSVREFE_RESET ((uint32_t)0xFF7FFFFF)

/* CTRL2 register Mask */
#define CTRL2_CLR_MASK ((uint32_t)0xFFF1F7FD)   //Clear CONT, ALIGN and EXTSEL bits

/* ADC SQx mask */
#define SQR4_SEQ_SET ((uint32_t)0x0000000F)
#define SQR3_SEQ_SET ((uint32_t)0x0000000F)
#define SQR2_SEQ_SET ((uint32_t)0x0000000F)
#define SQR1_SEQ_SET ((uint32_t)0x0000000F)

/* RSEQ1 register Mask */
#define RSEQ1_CLR_MASK ((uint32_t)0xFF0FFFFF)  //Clear L bits 

/* ADC JSQx mask */
#define JSEQ_JSQ_SET ((uint32_t)0x0000000F)

/* ADC INJ_LEN mask */
#define JSEQ_INJ_LEN_SET   ((uint32_t)0x00300000)
#define JSEQ_INJ_LEN_RESET ((uint32_t)0xFFCFFFFF)

/* ADC SAMPTx mask */
#define SAMPT1_SMP_SET ((uint32_t)0x0000000F)
#define SAMPT2_SMP_SET ((uint32_t)0x0000000F)
#define SAMPT3_SMP_SET ((uint32_t)0x0000000F)

/* ADC JDATx registers offset */
#define JDAT_OFFSET ((uint8_t)0x28)   //jdata_offset based ADC_INJ_CH_x

/* ADC JSQx_OPASEL mask */
#define JSQ_OPASEL ((uint32_t)0x00000007)

#define OPA_SETUP_TIEM_RESET ((uint32_t)0xFC00FFFF)
/**
 * @}
 */

/** @addtogroup ADC_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup ADC_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup ADC_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup ADC_Private_Functions
 * @{
 */

/**
 * @brief  Deinitializes the ADCx peripheral registers to their default reset values.
 * @param ADCx = ADC, to select the ADC peripheral.
 */
void ADC_DeInit(ADC_Module* ADCx)
{
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));

    if (ADCx == ADC)
    {
    /* Enable ADC reset state */
    RCC_EnableAHBPeriphReset(RCC_AHB_PERIPH_ADC, ENABLE);
    /* Release ADC from reset state */
    RCC_EnableAHBPeriphReset(RCC_AHB_PERIPH_ADC, DISABLE);
    }
}

/**
 * @brief  Initializes the ADC peripheral according to the specified parameters
 *         in the ADC_InitStruct.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_InitStruct pointer to an ADC_InitType structure that contains
 *         the configuration information for the specified ADC peripheral.
 */
void ADC_Init(ADC_Module* ADCx, ADC_InitType* ADC_InitStruct)
{
    uint32_t tmpreg1 = 0;
    uint8_t tmpreg2  = 0;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->MultiChEn));
    assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ContinueConvEn));
    assert_param(IsAdcExtTrig(ADC_InitStruct->ExtTrigSelect));
    assert_param(IsAdcDatAlign(ADC_InitStruct->DatAlign));
    assert_param(IsAdcSeqLenValid(ADC_InitStruct->ChsNumber));

    /*---------------------------- ADCx CTRL1 Configuration -----------------*/
    /* Get the ADCx CTRL1 value */
    tmpreg1 = ADCx->CTRL1;
    /* Clear SCAN bits */
    tmpreg1 &= CTRL1_CLR_MASK;
    /* Configure ADCx: scan conversion mode */
    /* Set SCAN bit according to MultiChEn value */
    tmpreg1 |= (uint32_t)( ((uint32_t)ADC_InitStruct->MultiChEn << 8));
    /* Write to ADCx CTRL1 */
    ADCx->CTRL1 = tmpreg1;

    /*---------------------------- ADCx CTRL2 Configuration -----------------*/
    /* Get the ADCx CTRL2 value */
    tmpreg1 = ADCx->CTRL2;
    /* Clear CONT, ALIGN and EXTSEL bits */
    tmpreg1 &= CTRL2_CLR_MASK;
    /* Configure ADCx: external trigger event and continuous conversion mode */
    /* Set ALIGN bit according to DatAlign value */
    /* Set EXTSEL bits according to ExtTrigSelect value */
    /* Set CONT bit according to ContinueConvEn value */
    tmpreg1 |= (uint32_t)(ADC_InitStruct->DatAlign | ADC_InitStruct->ExtTrigSelect
                          | ((uint32_t)ADC_InitStruct->ContinueConvEn << 1));
    /* Write to ADCx CTRL2 */
    ADCx->CTRL2 = tmpreg1;

    /*---------------------------- ADCx RSEQ1 Configuration -----------------*/
    /* Get the ADCx RSEQ1 value */
    tmpreg1 = ADCx->RSEQ1;
    /* Clear L bits */
    tmpreg1 &= RSEQ1_CLR_MASK;
    /* Configure ADCx: regular channel sequence length */
    /* Set L bits according to ChsNumber value */
    tmpreg2 |= (uint8_t)(ADC_InitStruct->ChsNumber - (uint8_t)1);
    tmpreg1 |= (uint32_t)tmpreg2 << 20;
    /* Write to ADCx RSEQ1 */
    ADCx->RSEQ1 = tmpreg1;
}

/**
 * @brief  Fills each ADC_InitStruct member with its default value.
 * @param ADC_InitStruct pointer to an ADC_InitType structure which will be initialized.
 */
void ADC_InitStruct(ADC_InitType* ADC_InitStruct)
{
    /* Reset ADC init structure parameters values */
    /* initialize the MultiChEn member */
    ADC_InitStruct->MultiChEn = DISABLE;
    /* Initialize the ContinueConvEn member */
    ADC_InitStruct->ContinueConvEn = DISABLE;
    /* Initialize the ExtTrigSelect member */
    ADC_InitStruct->ExtTrigSelect = ADC_EXT_TRIGCONV_T1_CC1;
    /* Initialize the DatAlign member */
    ADC_InitStruct->DatAlign = ADC_DAT_ALIGN_R;
    /* Initialize the ChsNumber member */
    ADC_InitStruct->ChsNumber = 1;
}

/**
 * @brief  Enables or disables the specified ADC peripheral.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param Cmd new state of the ADCx peripheral.
 *   This parameter can be: ENABLE or DISABLE.
 */
void ADC_Enable(ADC_Module* ADCx, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        /* Set the AD_ON bit to wake up the ADC from power down mode */
        ADCx->CTRL2 |= CTRL2_AD_ON_SET;
    }
    else
    {
        /* Disable the selected ADC peripheral */
        ADCx->CTRL2 &= CTRL2_AD_ON_RESET;
    }
}

/**
 * @brief  Enables or disables the specified ADC DMA request.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param Cmd new state of the selected ADC DMA transfer.
 *   This parameter can be: ENABLE or DISABLE.
 */
void ADC_EnableDMA(ADC_Module* ADCx, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IsAdcDmaModule(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        /* Enable the selected ADC DMA request */
        ADCx->CTRL2 |= CTRL2_DMA_SET;
    }
    else
    {
        /* Disable the selected ADC DMA request */
        ADCx->CTRL2 &= CTRL2_DMA_RESET;
    }
}

/**
 * @brief  Enables or disables the specified ADC interrupts.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_IT specifies the ADC interrupt sources to be enabled or disabled.
 *   This parameter can be any combination of the following values:
 *     @arg ADC_INT_ENDC End of conversion interrupt mask
 *     @arg ADC_INT_AWD Analog watchdog interrupt mask
 *     @arg ADC_INT_JENDC End of injected conversion interrupt mask
 * @param Cmd new state of the specified ADC interrupts.
 *   This parameter can be: ENABLE or DISABLE.
 */
void ADC_ConfigInt(ADC_Module* ADCx, uint16_t ADC_IT, FunctionalState Cmd)
{
    uint8_t itmask = 0;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    assert_param(IsAdcInt(ADC_IT));
    /* Get the ADC IT index */
    itmask = (uint8_t)ADC_IT;
    if (Cmd != DISABLE)
    {
        /* Enable the selected ADC interrupts */
        ADCx->CTRL1 |= itmask;
    }
    else
    {
        /* Disable the selected ADC interrupts */
        ADCx->CTRL1 &= (~(uint32_t)itmask);
    }
}

/**
 * @brief  Enables or disables the selected ADC software start conversion .
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param Cmd new state of the selected ADC software start conversion.
 *   This parameter can be: ENABLE or DISABLE.
 */
void ADC_EnableSoftwareStartConv(ADC_Module* ADCx, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        /* Enable the selected ADC conversion on external event and start the selected
           ADC conversion */
        ADCx->CTRL2 |= CTRL2_EXT_TRIG_SWSTART_SET;
    }
    else
    {
        /* Disable the selected ADC conversion on external event and stop the selected
           ADC conversion */
        ADCx->CTRL2 &= CTRL2_EXT_TRIG_SWSTART_RESET;
    }
}

/**
 * @brief  Gets the selected ADC Software start conversion Status.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @return The new state of ADC software start conversion (SET or RESET).
 */
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_Module* ADCx)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    /* Check the status of SOFT_START bit */
    if ((ADCx->CTRL2 & CTRL2_SOFT_START_SET) != (uint32_t)RESET)
    {
        /* SOFT_START bit is set */
        bitstatus = SET;
    }
    else
    {
        /* SOFT_START bit is reset */
        bitstatus = RESET;
    }
    /* Return the SOFT_START bit status */
    return bitstatus;
}

/**
 * @brief  Configures the discontinuous mode for the selected ADC regular
 *         group channel.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param Number specifies the discontinuous mode regular channel
 *         count value. This number must be between 1 and 8.
 */
void ADC_ConfigDiscModeChannelCount(ADC_Module* ADCx, uint8_t Number)
{
    uint32_t tmpreg1 = 0;
    uint32_t tmpreg2 = 0;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcSeqDiscNumberValid(Number));
    /* Get the old register value */
    tmpreg1 = ADCx->CTRL1;
    /* Clear the old discontinuous mode channel count */
    tmpreg1 &= CTRL1_DISCNUM_Reset;
    /* Set the discontinuous mode channel count */
    tmpreg2 = Number - 1;
    tmpreg1 |= tmpreg2 << 13;
    /* Store the new register value */
    ADCx->CTRL1 = tmpreg1;
}

/**
 * @brief  Enables or disables the discontinuous mode on regular group
 *         channel for the specified ADC
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param Cmd new state of the selected ADC discontinuous mode
 *         on regular group channel.
 *         This parameter can be: ENABLE or DISABLE.
 */
void ADC_EnableDiscMode(ADC_Module* ADCx, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        /* Enable the selected ADC regular discontinuous mode */
        ADCx->CTRL1 |= CTRL1_DISC_EN_SET;
    }
    else
    {
        /* Disable the selected ADC regular discontinuous mode */
        ADCx->CTRL1 &= CTRL1_DISC_EN_RESET;
    }
}

/**
 * @brief  Configures for the selected ADC regular channel its corresponding
 *         rank in the sequencer and its sample time.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_Channel the ADC channel to configure.
 *   This parameter can be one of the following values:
 *     @arg ADC_CH_0 ADC Channel0 selected
 *     @arg ADC_CH_1 ADC Channel1 selected
 *     @arg ADC_CH_2 ADC Channel2 selected
 *     @arg ADC_CH_3 ADC Channel3 selected
 *     @arg ADC_CH_4 ADC Channel4 selected
 *     @arg ADC_CH_5 ADC Channel5 selected
 *     @arg ADC_CH_6 ADC Channel6 selected
 *     @arg ADC_CH_7 ADC Channel7 selected
 *     @arg ADC_CH_8 ADC Channel8 selected
 *     @arg ADC_CH_9 ADC Channel9 selected
 *     @arg ADC_CH_10 ADC Channel10 selected
 *     @arg ADC_CH_11 ADC Channel11 selected
 *     @arg ADC_CH_12 ADC Channel12 selected
 *     @arg ADC_CH_13 ADC Channel13 selected
 *     @arg ADC_CH_14 ADC Channel14 selected
 *     @arg ADC_CH_15 ADC Channel15 selected
 * @param Rank The rank in the regular group sequencer. This parameter must be between 1 to 16.
 * @param ADC_SampleTime The sample time value to be set for the selected channel.
 *   This parameter can be one of the following values:
 *     @arg ADC_SAMP_TIME_6CYCLES5 Sample time equal to 6 cycles
 *     @arg ADC_SAMP_TIME_8CYCLES5 Sample time equal to 8 cycles
 *     @arg ADC_SAMP_TIME_14CYCLES5 Sample time equal to 14 cycles
 *     @arg ADC_SAMP_TIME_20CYCLES5 Sample time equal to 20 cycles
 *     @arg ADC_SAMP_TIME_29CYCLES5 Sample time equal to 29 cycles
 *     @arg ADC_SAMP_TIME_42CYCLES5 Sample time equal to 42 cycles
 *     @arg ADC_SAMP_TIME_56CYCLES5 Sample time equal to 56 cycles
 *     @arg ADC_SAMP_TIME_72CYCLES5 Sample time equal to 72 cycles
 *     @arg ADC_SAMP_TIME_88CYCLES5  Sample time equal to 88 cycles
 *     @arg ADC_SAMP_TIME_120CYCLES5 Sample time equal to 120 cycles
 *     @arg ADC_SAMP_TIME_182CYCLES5 Sample time equal to 182 cycles
 *     @arg ADC_SAMP_TIME_240CYCLES5 Sample time equal to 240 cycles
 *     @arg ADC_SAMP_TIME_300CYCLES5 Sample time equal to 300 cycles
 *     @arg ADC_SAMP_TIME_400CYCLES5 Sample time equal to 400 cycles
 *     @arg ADC_SAMP_TIME_480CYCLES5 Sample time equal to 480 cycles
 *     @arg ADC_SAMP_TIME_600CYCLES5 Sample time equal to 600 cycles
 */

void ADC_ConfigRegularChannel(ADC_Module* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
    uint32_t tmpreg1 = 0, tmpreg2 = 0;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcChannel(ADC_Channel));
    assert_param(IsAdcReqRankValid(Rank));
    assert_param(IsAdcSampleTime(ADC_SampleTime));

    if(ADC_Channel > ADC_CH_7)/* ADC_Channel include in ADC_Channel_[8..15] */
    {
        /* Get the old register value */
        tmpreg1 = ADCx->SAMPT2;
        /* Calculate the mask to clear */
        tmpreg2 = SAMPT2_SMP_SET << (4 * (ADC_Channel - 8));
        /* Clear the old channel sample time */
        tmpreg1 &= ~tmpreg2;
        /* Calculate the mask to set */
        tmpreg2 = (uint32_t)ADC_SampleTime << (4 * (ADC_Channel - 8));
        /* Set the new channel sample time */
        tmpreg1 |= tmpreg2;
        /* Store the new register value */
        ADCx->SAMPT2 = tmpreg1;
    }
    else /* ADC_Channel include in ADC_Channel_[7:0]*/
    {
          /* Get the old register value */
          tmpreg1 = ADCx->SAMPT3;
          /* Calculate the mask to clear */
          tmpreg2 = SAMPT3_SMP_SET << (4 * ADC_Channel);
          /* Clear the old channel sample time */
          tmpreg1 &= ~tmpreg2;
          /* Calculate the mask to set */
          tmpreg2 = (uint32_t)ADC_SampleTime << (4 * ADC_Channel);
          /* Set the new channel sample time */
          tmpreg1 |= tmpreg2;
          /* Store the new register value */
          ADCx->SAMPT3 = tmpreg1;
    }
    /* For Rank 1 to 6 */
    if (Rank < 7)
    {
        /* Get the old register value */
        tmpreg1 = ADCx->RSEQ3;
        /* Calculate the mask to clear */
        tmpreg2 = SQR3_SEQ_SET << (5 * (Rank - 1));
        /* Clear the old SQx bits for the selected rank */
        tmpreg1 &= ~tmpreg2;
        /* Calculate the mask to set */
        tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 1));
        /* Set the SQx bits for the selected rank */
        tmpreg1 |= tmpreg2;
        /* Store the new register value */
        ADCx->RSEQ3 = tmpreg1;
    }
    /* For Rank 7 to 12 */
    else if (Rank < 13)
    {
        /* Get the old register value */
        tmpreg1 = ADCx->RSEQ2;
        /* Calculate the mask to clear */
        tmpreg2 = SQR2_SEQ_SET << (5 * (Rank - 7));
        /* Clear the old SQx bits for the selected rank */
        tmpreg1 &= ~tmpreg2;
        /* Calculate the mask to set */
        tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 7));
        /* Set the SQx bits for the selected rank */
        tmpreg1 |= tmpreg2;
        /* Store the new register value */
        ADCx->RSEQ2 = tmpreg1;
    }
    /* For Rank 13 to 16 */
    else
    {
        /* Get the old register value */
        tmpreg1 = ADCx->RSEQ1;
        /* Calculate the mask to clear */
        tmpreg2 = SQR1_SEQ_SET << (5 * (Rank - 13));
        /* Clear the old SQx bits for the selected rank */
        tmpreg1 &= ~tmpreg2;
        /* Calculate the mask to set */
        tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 13));
        /* Set the SQx bits for the selected rank */
        tmpreg1 |= tmpreg2;
        /* Store the new register value */
        ADCx->RSEQ1 = tmpreg1;
    }
}

/**
 * @brief  Enables or disables the ADCx conversion through external trigger.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param Cmd new state of the selected ADC external trigger start of conversion.
 *   This parameter can be: ENABLE or DISABLE.
 */
void ADC_EnableExternalTrigConv(ADC_Module* ADCx, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        /* Enable the selected ADC conversion on external event */
        ADCx->CTRL2 |= CTRL2_EXT_TRIG_SET;
    }
    else
    {
        /* Disable the selected ADC conversion on external event */
        ADCx->CTRL2 &= CTRL2_EXT_TRIG_RESET;
    }
}

/**
 * @brief  Returns the last ADCx conversion result data for regular channel.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @return The Data conversion value.
 */
uint16_t ADC_GetDat(ADC_Module* ADCx)
{
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    /* Return the selected ADC conversion value */
    return (uint16_t)ADCx->DAT;
}

/**
 * @brief  Enables or disables the selected ADC automatic injected group
 *         conversion after regular one.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param Cmd new state of the selected ADC auto injected conversion
 *   This parameter can be: ENABLE or DISABLE.
 */
void ADC_EnableAutoInjectedConv(ADC_Module* ADCx, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        /* Enable the selected ADC automatic injected group conversion */
        ADCx->CTRL1 |= CTRL1_JAUTO_Set;
    }
    else
    {
        /* Disable the selected ADC automatic injected group conversion */
        ADCx->CTRL1 &= CTRL1_JAUTO_Reset;
    }
}

/**
 * @brief  Enables or disables the discontinuous mode for injected group
 *         channel for the specified ADC
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param Cmd new state of the selected ADC discontinuous mode
 *         on injected group channel.
 *   This parameter can be: ENABLE or DISABLE.
 */
void ADC_EnableInjectedDiscMode(ADC_Module* ADCx, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        /* Enable the selected ADC injected discontinuous mode */
        ADCx->CTRL1 |= CTRL1_INJ_DISC_EN_SET;
    }
    else
    {
        /* Disable the selected ADC injected discontinuous mode */
        ADCx->CTRL1 &= CTRL1_INJ_DISC_EN_RESET;
    }
}



/**
 * @brief  Configures the ADCx external trigger for injected channels conversion.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_ExternalTrigInjecConv specifies the ADC trigger to start injected conversion.
 *   This parameter can be one of the following values: 
 *     @arg ADC_EXT_TRIG_INJ_CONV_T1_TRGO    Timer1 TRGO event selected
 *     @arg ADC_EXT_TRIG_INJ_CONV_T1_CC4     Timer1 capture compare4 selected
 *     @arg ADC_EXT_TRIG_INJ_CONV_T3_CC4     Timer3 capture compare4 selected
 *     @arg ADC_EXT_TRIG_INJ_CONV_EXT_INT15_TIM8_CC4   External interrupt line 15 or Timer8 capture compare4 
 *     @arg ADC_EXT_TRIG_INJ_CONV_NONE       Injected conversion started by software and not
 *                                           by external trigger
 */
void ADC_ConfigExternalTrigInjectedConv(ADC_Module* ADCx, uint32_t ADC_ExternalTrigInjecConv)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcExtInjTrig(ADC_ExternalTrigInjecConv));
    /* Get the old register value */
    tmpregister = ADCx->CTRL2;
    /* Clear the old external event selection for injected group */
    tmpregister &= CTRL2_INJ_EXT_SEL_RESET;
    /* Set the external event selection for injected group */
    tmpregister |= ADC_ExternalTrigInjecConv;
    /* Store the new register value */
    ADCx->CTRL2 = tmpregister;
}

/**
 * @brief  Enables or disables the ADCx injected channels conversion through
 *         external trigger
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param Cmd new state of the selected ADC external trigger start of
 *         injected conversion.
 *   This parameter can be: ENABLE or DISABLE.
 */
void ADC_EnableExternalTrigInjectedConv(ADC_Module* ADCx, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        /* Enable the selected ADC external event selection for injected group */
        ADCx->CTRL2 |= CTRL2_INJ_EXT_TRIG_SET;
    }
    else
    {
        /* Disable the selected ADC external event selection for injected group */
        ADCx->CTRL2 &= CTRL2_INJ_EXT_TRIG_RESET;
    }
}

/**
 * @brief  Enables or disables the selected ADC start of the injected
 *         channels conversion.
 * @param ADCx where x can be 1, 2, 3 or 4 to select the ADC peripheral.
 * @param Cmd new state of the selected ADC software start injected conversion.
 *   This parameter can be: ENABLE or DISABLE.
 */
void ADC_EnableSoftwareStartInjectedConv(ADC_Module* ADCx, FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        /* Enable the selected ADC conversion for injected group on external event and start the selected
           ADC injected conversion */
        ADCx->CTRL2 |= CTRL2_INJ_EXT_TRIG_JSWSTART_SET;
    }
    else
    {
        /* Disable the selected ADC conversion on external event for injected group and stop the selected
           ADC injected conversion */
        ADCx->CTRL2 &= CTRL2_INJ_EXT_TRIG_JSWSTART_RESET;
    }
}

/**
 * @brief  Gets the selected ADC Software start injected conversion Status.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @return The new state of ADC software start injected conversion (SET or RESET).
 */
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_Module* ADCx)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    /* Check the status of INJ_SWSTART bit */
    if ((ADCx->CTRL2 & CTRL2_INJ_SWSTART_SET) != (uint32_t)RESET)
    {
        /* INJ_SWSTART bit is set */
        bitstatus = SET;
    }
    else
    {
        /* INJ_SWSTART bit is reset */
        bitstatus = RESET;
    }
    /* Return the INJ_SWSTART bit status */
    return bitstatus;
}

/**
 * @brief  Configures for the selected ADC injected channel its corresponding
 *         rank in the sequencer and its sample time.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_Channel the ADC channel to configure.
 *   This parameter can be one of the following values:
 *     @arg ADC_CH_0 ADC Channel0 selected
 *     @arg ADC_CH_1 ADC Channel1 selected
 *     @arg ADC_CH_2 ADC Channel2 selected
 *     @arg ADC_CH_3 ADC Channel3 selected
 *     @arg ADC_CH_4 ADC Channel4 selected
 *     @arg ADC_CH_5 ADC Channel5 selected
 *     @arg ADC_CH_6 ADC Channel6 selected
 *     @arg ADC_CH_7 ADC Channel7 selected
 *     @arg ADC_CH_8 ADC Channel8 selected
 *     @arg ADC_CH_9 ADC Channel9 selected
 *     @arg ADC_CH_10 ADC Channel10 selected
 *     @arg ADC_CH_11 ADC Channel11 selected
 *     @arg ADC_CH_12 ADC Channel12 selected
 *     @arg ADC_CH_13 ADC Channel13 selected
 *     @arg ADC_CH_14 ADC Channel14 selected
 *     @arg ADC_CH_15 ADC Channel15 selected
 * @param Rank The rank in the injected group sequencer. This parameter must be between 1 and 4.
 * @param ADC_SampleTime The sample time value to be set for the selected channel.
 *   This parameter can be one of the following values:
 *     @arg ADC_SAMP_TIME_6CYCLES5   Sample time equal to 6 cycles
 *     @arg ADC_SAMP_TIME_8CYCLES5   Sample time equal to 8 cycles
 *     @arg ADC_SAMP_TIME_14CYCLES5  Sample time equal to 14 cycles
 *     @arg ADC_SAMP_TIME_20CYCLES5  Sample time equal to 20 cycles
 *     @arg ADC_SAMP_TIME_29CYCLES5  Sample time equal to 29 cycles
 *     @arg ADC_SAMP_TIME_42CYCLES5  Sample time equal to 42 cycles
 *     @arg ADC_SAMP_TIME_56CYCLES5  Sample time equal to 56 cycles
 *     @arg ADC_SAMP_TIME_72CYCLES5  Sample time equal to 72 cycles
 *     @arg ADC_SAMP_TIME_88CYCLES5  Sample time equal to 88 cycles
 *     @arg ADC_SAMP_TIME_120CYCLES5 Sample time equal to 120 cycles
 *     @arg ADC_SAMP_TIME_182CYCLES5 Sample time equal to 182 cycles
 *     @arg ADC_SAMP_TIME_240CYCLES5 Sample time equal to 240 cycles
 *     @arg ADC_SAMP_TIME_300CYCLES5 Sample time equal to 300 cycles
 *     @arg ADC_SAMP_TIME_400CYCLES5 Sample time equal to 400 cycles
 *     @arg ADC_SAMP_TIME_480CYCLES5 Sample time equal to 480 cycles
 *     @arg ADC_SAMP_TIME_600CYCLES5 Sample time equal to 600 cycles
 */
void ADC_ConfigInjectedChannel(ADC_Module* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
    uint32_t tmpreg1 = 0, tmpreg2 = 0, tmpreg3 = 0;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcChannel(ADC_Channel));
    assert_param(IsAdcInjRankValid(Rank));
    assert_param(IsAdcSampleTime(ADC_SampleTime));


    if (ADC_Channel > ADC_CH_7) /* if ADC_CH_15 ... ADC_CH_8 is selected */
    {
        /* Get the old register value */
        tmpreg1 = ADCx->SAMPT2;
        /* Calculate the mask to clear */
        tmpreg2 = SAMPT2_SMP_SET << (4 * (ADC_Channel - 8));
        /* Clear the old channel sample time */
        tmpreg1 &= ~tmpreg2;
        /* Calculate the mask to set */
        tmpreg2 = (uint32_t)ADC_SampleTime << (4 * (ADC_Channel - 8));
        /* Set the new channel sample time */
        tmpreg1 |= tmpreg2;
        /* Store the new register value */
        ADCx->SAMPT2 = tmpreg1;
    }
    else /* ADC_Channel include in ADC_Channel_[0..7] */
    {
        /* Get the old register value */
        tmpreg1 = ADCx->SAMPT3;
        /* Calculate the mask to clear */
        tmpreg2 = SAMPT3_SMP_SET << (4 * ADC_Channel);
        /* Clear the old channel sample time */
        tmpreg1 &= ~tmpreg2;
        /* Calculate the mask to set */
        tmpreg2 = (uint32_t)ADC_SampleTime << (4 * ADC_Channel);
        /* Set the new channel sample time */
        tmpreg1 |= tmpreg2;
        /* Store the new register value */
        ADCx->SAMPT3 = tmpreg1;
    }
    /* Rank configuration */
    /* Get the old register value */
    tmpreg1 = ADCx->JSEQ;
    /* Get INJ_LEN value: Number = INJ_LEN+1 */
    tmpreg3 = (tmpreg1 & JSEQ_INJ_LEN_SET) >> 20;
    /* Calculate the mask to clear: ((Rank-1)+(4-INJ_LEN-1)) */
    tmpreg2 = JSEQ_JSQ_SET << (5 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)));
    /* Clear the old JSQx bits for the selected rank */
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set: ((Rank-1)+(4-INJ_LEN-1)) */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)));
    /* Set the JSQx bits for the selected rank */
    tmpreg1 |= tmpreg2;
    /* Store the new register value */
    ADCx->JSEQ = tmpreg1;
}

/**
 * @brief  Configures the sequencer length for injected channels
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param Length The sequencer length.
 *   This parameter must be a number between 1 to 4.
 */
void ADC_ConfigInjectedSequencerLength(ADC_Module* ADCx, uint8_t Length)
{
    uint32_t tmpreg1 = 0;
    uint32_t tmpreg2 = 0;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcInjLenValid(Length));

    /* Get the old register value */
    tmpreg1 = ADCx->JSEQ;
    /* Clear the old injected sequnence lenght INJ_LEN bits */
    tmpreg1 &= JSEQ_INJ_LEN_RESET;
    /* Set the injected sequnence lenght INJ_LEN bits */
    tmpreg2 = Length - 1;
    tmpreg1 |= tmpreg2 << 20;
    /* Store the new register value */
    ADCx->JSEQ = tmpreg1;
}

/**
 * @brief  Set the injected channels conversion value offset
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_InjectedChannel the ADC injected channel to set its offset.
 *   This parameter can be one of the following values:
 *     @arg ADC_INJ_CH_1 Injected Channel1 selected
 *     @arg ADC_INJ_CH_2 Injected Channel2 selected
 *     @arg ADC_INJ_CH_3 Injected Channel3 selected
 *     @arg ADC_INJ_CH_4 Injected Channel4 selected
 * @param Offset the offset value for the selected ADC injected channel
 *   This parameter must be a 12bit value.
 */
void ADC_SetInjectedOffsetDat(ADC_Module* ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset)
{
    __IO uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcInjCh(ADC_InjectedChannel));
    assert_param(IsAdcOffsetValid(Offset));

    tmp = (uint32_t)ADCx;
    tmp += ADC_InjectedChannel;

    /* Set the selected injected channel data offset */
    *(__IO uint32_t*)tmp = (uint32_t)Offset;
}

/**
 * @brief  Returns the ADC injected channel conversion result
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_InjectedChannel the converted ADC injected channel.
 *   This parameter can be one of the following values:
 *     @arg ADC_INJ_CH_1 Injected Channel1 selected
 *     @arg ADC_INJ_CH_2 Injected Channel2 selected
 *     @arg ADC_INJ_CH_3 Injected Channel3 selected
 *     @arg ADC_INJ_CH_4 Injected Channel4 selected
 * @return The Data conversion value.
 */
uint16_t ADC_GetInjectedConversionDat(ADC_Module* ADCx, uint8_t ADC_InjectedChannel)
{
    __IO uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcInjCh(ADC_InjectedChannel));

    tmp = (uint32_t)ADCx;
    tmp += ADC_InjectedChannel + JDAT_OFFSET;

    /* Returns the selected injected channel conversion data value */
    return (uint16_t)(*(__IO uint32_t*)tmp);
}

/**
 * @brief  Enables or disables the analog watchdog on single/all regular
 *         or injected channels
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_AnalogWatchdog the ADC analog watchdog configuration.
 *   This parameter can be one of the following values:
 *     @arg ADC_ANALOG_WTDG_SINGLEREG_ENABLE Analog watchdog on a single regular channel
 *     @arg ADC_ANALOG_WTDG_SINGLEINJEC_ENABLE Analog watchdog on a single injected channel
 *     @arg ADC_ANALOG_WTDG_SINGLEREG_OR_INJEC_ENABLE Analog watchdog on a single regular or injected channel
 *     @arg ADC_ANALOG_WTDG_ALLREG_ENABLE Analog watchdog on  all regular channel
 *     @arg ADC_ANALOG_WTDG_ALLINJEC_ENABLE Analog watchdog on  all injected channel
 *     @arg ADC_ANALOG_WTDG_ALLREG_ALLINJEC_ENABLE Analog watchdog on all regular and injected channels
 *     @arg ADC_ANALOG_WTDG_NONE No channel guarded by the analog watchdog
 */
void ADC_ConfigAnalogWatchdogWorkChannelType(ADC_Module* ADCx, uint32_t ADC_AnalogWatchdog)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcAnalogWatchdog(ADC_AnalogWatchdog));
    /* Get the old register value */
    tmpregister = ADCx->CTRL1;
    /* Clear AWDEN, AWDENJ and AWDSGL bits */
    tmpregister &= CTRL1_AWDG_MODE_RESET;
    /* Set the analog watchdog enable mode */
    tmpregister |= ADC_AnalogWatchdog;
    /* Store the new register value */
    ADCx->CTRL1 = tmpregister;
}

/**
 * @brief  Configures the high and low thresholds of the analog watchdog.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param HighThreshold the ADC analog watchdog High threshold value.
 *   This parameter must be a 12bit value.
 * @param LowThreshold the ADC analog watchdog Low threshold value.
 *   This parameter must be a 12bit value.
 */
void ADC_ConfigAnalogWatchdogThresholds(ADC_Module* ADCx, uint16_t HighThreshold, uint16_t LowThreshold)
{
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcValid(HighThreshold));
    assert_param(IsAdcValid(LowThreshold));
    /* Set the ADCx high threshold */
    ADCx->WDGHIGH = HighThreshold;
    /* Set the ADCx low threshold */
    ADCx->WDGLOW = LowThreshold;
}

/**
 * @brief  Configures the analog watchdog guarded single channel
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_Channel the ADC channel to configure for the analog watchdog.
 *   This parameter can be one of the following values:
 *     @arg ADC_CH_0 ADC Channel0 selected
 *     @arg ADC_CH_1 ADC Channel1 selected
 *     @arg ADC_CH_2 ADC Channel2 selected
 *     @arg ADC_CH_3 ADC Channel3 selected
 *     @arg ADC_CH_4 ADC Channel4 selected
 *     @arg ADC_CH_5 ADC Channel5 selected
 *     @arg ADC_CH_6 ADC Channel6 selected
 *     @arg ADC_CH_7 ADC Channel7 selected
 *     @arg ADC_CH_8 ADC Channel8 selected
 *     @arg ADC_CH_9 ADC Channel9 selected
 *     @arg ADC_CH_10 ADC Channel10 selected
 *     @arg ADC_CH_11 ADC Channel11 selected
 *     @arg ADC_CH_12 ADC Channel12 selected
 *     @arg ADC_CH_13 ADC Channel13 selected
 *     @arg ADC_CH_14 ADC Channel14 selected
 *     @arg ADC_CH_15 ADC Channel15 selected
 */
void ADC_ConfigAnalogWatchdogSingleChannel(ADC_Module* ADCx, uint8_t ADC_Channel)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcChannel(ADC_Channel));
    /* Get the old register value */
    tmpregister = ADCx->CTRL1;
    /* Clear the Analog watchdog channel select bits */
    tmpregister &= CTRL1_AWDG_CH_RESET;
    /* Set the Analog watchdog channel */
    tmpregister |= ADC_Channel;
    /* Store the new register value */
    ADCx->CTRL1 = tmpregister;
}

/**
 * @brief  Enables or disables the temperature sensor and Vrefint channel.
 * @param Cmd new state of the temperature sensor.
 *   This parameter can be: ENABLE or DISABLE.
 */
void ADC_EnableTempSensorVrefint(FunctionalState Cmd)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Cmd));
    if (Cmd != DISABLE)
    {
        /* Enable the temperature sensor and Vrefint channel*/
        ADC->CTRL2 |= CTRL2_TSVREFE_SET;
    }
    else
    {
        /* Disable the temperature sensor and Vrefint channel*/
        ADC->CTRL2 &= CTRL2_TSVREFE_RESET;
    }
}

/**
 * @brief  Checks whether the specified ADC flag is set or not.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_FLAG specifies the flag to check.
 *   This parameter can be one of the following values:
 *     @arg ADC_FLAG_AWDG Analog watchdog flag
 *     @arg ADC_FLAG_ENDC End of conversion flag
 *     @arg ADC_FLAG_JENDC End of injected group conversion flag
 *     @arg ADC_FLAG_JSTR Start of injected group conversion flag
 *     @arg ADC_FLAG_STR Start of regular group conversion flag
 * @return The new state of ADC_FLAG (SET or RESET).
 */
FlagStatus ADC_GetFlagStatus(ADC_Module* ADCx, uint8_t ADC_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcGetFlag(ADC_FLAG));
    /* Check the status of the specified ADC flag */
    if ((ADCx->STS & ADC_FLAG) != (uint8_t)RESET)
    {
        /* ADC_FLAG is set */
        bitstatus = SET;
    }
    else
    {
        /* ADC_FLAG is reset */
        bitstatus = RESET;
    }
    /* Return the ADC_FLAG status */
    return bitstatus;
}

/**
 * @brief  Clears the ADCx's pending flags.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_FLAG specifies the flag to clear.
 *   This parameter can be any combination of the following values:
 *     @arg ADC_FLAG_AWDG Analog watchdog flag
 *     @arg ADC_FLAG_ENDC End of conversion flag
 *     @arg ADC_FLAG_JENDC End of injected group conversion flag
 *     @arg ADC_FLAG_JSTR Start of injected group conversion flag
 *     @arg ADC_FLAG_STR Start of regular group conversion flag
 */
void ADC_ClearFlag(ADC_Module* ADCx, uint8_t ADC_FLAG)
{
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcClrFlag(ADC_FLAG));
    /* Clear the selected ADC flags */
    ADCx->STS &= ~(uint32_t)ADC_FLAG;
}

/**
 * @brief  Checks whether the specified ADC interrupt has occurred or not.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_IT specifies the ADC interrupt source to check.
 *   This parameter can be one of the following values:
 *     @arg ADC_INT_ENDC End of conversion interrupt mask
 *     @arg ADC_INT_AWD Analog watchdog interrupt mask
 *     @arg ADC_INT_JENDC End of injected conversion interrupt mask
 * @return The new state of ADC_IT (SET or RESET).
 */
INTStatus ADC_GetIntStatus(ADC_Module* ADCx, uint16_t ADC_IT)
{
    INTStatus bitstatus = RESET;
    uint32_t itmask = 0, enablestatus = 0;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcGetInt(ADC_IT));
    /* Get the ADC IT index */
    itmask = ADC_IT >> 8;
    /* Get the ADC_IT enable bit status */
    enablestatus = (ADCx->CTRL1 & (uint8_t)ADC_IT);
    /* Check the status of the specified ADC interrupt */
    if (((ADCx->STS & itmask) != (uint32_t)RESET) && enablestatus)
    {
        /* ADC_IT is set */
        bitstatus = SET;
    }
    else
    {
        /* ADC_IT is reset */
        bitstatus = RESET;
    }
    /* Return the ADC_IT status */
    return bitstatus;
}

/**
 * @brief  Clears the ADCx's interrupt pending bits.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_IT specifies the ADC interrupt pending bit to clear.
 *   This parameter can be any combination of the following values:
 *     @arg ADC_INT_ENDC End of conversion interrupt mask
 *     @arg ADC_INT_AWD Analog watchdog interrupt mask
 *     @arg ADC_INT_JENDC End of injected conversion interrupt mask
 */
void ADC_ClearIntPendingBit(ADC_Module* ADCx, uint16_t ADC_IT)
{
    uint8_t itmask = 0;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcInt(ADC_IT));
    /* Get the ADC IT index */
    itmask = (uint8_t)(ADC_IT >> 8);
    /* Clear the selected ADC interrupt pending bits */
    ADCx->STS &= ~(uint32_t)itmask;
}

/**
 * @brief  Initializes the ADCx peripheral according to the specified parameters
 *         in the ADC_InitStructEx.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_InitStructEx pointer to an ADC_InitTypeEx structure that contains
 *         the configuration information for the specified ADC peripheral.
 */
void ADC_InitEx(ADC_Module* ADCx, ADC_InitTypeEx* ADC_InitStructEx)
{
    uint32_t tmpregister = 0;

    /*intial ADC_CTRL3 once initiall config*/
    tmpregister = ADCx->CTRL3;
   
    if (ADC_InitStructEx->RefSel)
        tmpregister |= ADC_CTRL3_REFSEL_MSK;
    else
          tmpregister &= (~ADC_CTRL3_REFSEL_MSK);
    
    if (ADC_InitStructEx->VrefEn)
        tmpregister |= ADC_CTRL3_VREFEN_MSK;
    else
        tmpregister &= (~ADC_CTRL3_VREFEN_MSK);

    if (ADC_InitStructEx->JendcIntEn)
        tmpregister |= ADC_CTRL3_JENDCAIEN_MSK;
    else
        tmpregister &= (~ADC_CTRL3_JENDCAIEN_MSK);

    if (ADC_InitStructEx->EndcIntEn)
        tmpregister |= ADC_CTRL3_ENDCAIEN_MSK;
    else
        tmpregister &= (~ADC_CTRL3_ENDCAIEN_MSK);

    tmpregister &= (~ADC_CTRL3_CKMOD_MSK);
    if(ADC_InitStructEx->ClkMode==ADC_CTRL3_CKMOD_PLL)
    tmpregister |= ADC_CTRL3_CKMOD_MSK;

    ADCx->CTRL3 = tmpregister;
}
/**
 * @brief  Checks whether the specified ADC flag is set or not.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_FLAG_NEW specifies the flag to check.
 *   This parameter can be one of the following values:
 *     @arg ADC_FLAG_RDY     ADC ready flag
 *     @arg ADC_FLAG_PD_RDY  ADC powerdown ready flag
 * @return The new state of ADC_FLAG_NEW (SET or RESET).
 */
FlagStatus ADC_GetFlagStatusNew(ADC_Module* ADCx, uint8_t ADC_FLAG_NEW)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcGetFlag(ADC_FLAG_NEW));
    /* Check the status of the specified ADC flag */
    if ((ADCx->CTRL3 & ADC_FLAG_NEW) != (uint8_t)RESET)
    {
        /* ADC_FLAG_NEW is set */
        bitstatus = SET;
    }
    else
    {
        /* ADC_FLAG_NEW is reset */
        bitstatus = RESET;
    }
    /* Return the ADC_FLAG_NEW status */
    return bitstatus;
}

/**
 * @brief  Configures the ADCHCLK prescaler.
 * @param RCC_ADCHCLKPrescaler specifies the ADCHCLK prescaler.
 *   This parameter can be on of the following values:
 *     @arg RCC_ADCHCLK_DIV1 ADCHCLKPRE[3:0] = 0000, HCLK Clock Divided By 1
 *     @arg RCC_ADCHCLK_DIV2 ADCHCLKPRE[3:0] = 0001, HCLK Clock Divided By 2
 *     @arg RCC_ADCHCLK_DIV4 ADCHCLKPRE[3:0] = 0010, HCLK Clock Divided By 4
 *     @arg RCC_ADCHCLK_DIV6 ADCHCLKPRE[3:0] = 0011, HCLK Clock Divided By 6
 *     @arg RCC_ADCHCLK_DIV8 ADCHCLKPRE[3:0] = 0100, HCLK Clock Divided By 8
 *     @arg RCC_ADCHCLK_DIV10 ADCHCLKPRE[3:0] = 0101, HCLK Clock Divided By 10
 *     @arg RCC_ADCHCLK_DIV12 ADCHCLKPRE[3:0] = 0110, HCLK Clock Divided By 12
 *     @arg RCC_ADCHCLK_DIV16 ADCHCLKPRE[3:0] = 0111, HCLK Clock Divided By 16
 *     @arg RCC_ADCHCLK_DIV32 ADCHCLKPRE[3:0] = 1000, HCLK Clock Divided By 32
 *     @arg RCC_ADCHCLK_DIV_OTHERS ADCHCLKPRE[3:0] = others, HCLK Clock Divided By 32

 *     @arg RCC_ADCPLLCLK_DISABLE ADCPLLCLKPRES[4:0] = 0xxxx, ADC Pll Clock Disable
 *     @arg RCC_ADCPLLCLK_DIV1 ADCPLLCLKPRES[4:0] = 10000, Pll Clock Divided By 1
 *     @arg RCC_ADCPLLCLK_DIV2 ADCPLLCLKPRES[4:0] = 10001, Pll Clock Divided By 2
 *     @arg RCC_ADCPLLCLK_DIV4 ADCPLLCLKPRES[4:0] = 10010, Pll Clock Divided By 4
 *     @arg RCC_ADCPLLCLK_DIV6 ADCPLLCLKPRES[4:0] = 10011, Pll Clock Divided By 6
 *     @arg RCC_ADCPLLCLK_DIV8 ADCPLLCLKPRES[4:0] = 10100, Pll Clock Divided By 8
 *     @arg RCC_ADCPLLCLK_DIV10 ADCPLLCLKPRES[4:0] = 10101, Pll Clock Divided By 10
 *     @arg RCC_ADCPLLCLK_DIV12 ADCPLLCLKPRES[4:0] = 10110, Pll Clock Divided By 12
 *     @arg RCC_ADCPLLCLK_DIV16 ADCPLLCLKPRES[4:0] = 10111, Pll Clock Divided By 16
 *     @arg RCC_ADCPLLCLK_DIV32 ADCPLLCLKPRES[4:0] = 11000, Pll Clock Divided By 32
 *     @arg RCC_ADCPLLCLK_DIV64 ADCPLLCLKPRES[4:0] = 11001, Pll Clock Divided By 64
 *     @arg RCC_ADCPLLCLK_DIV128 ADCPLLCLKPRES[4:0] = 11010, Pll Clock Divided By 128
 *     @arg RCC_ADCPLLCLK_DIV256 ADCPLLCLKPRES[4:0] = 11011, Pll Clock Divided By 256
 *     @arg RCC_ADCPLLCLK_DIV_OTHERS ADCPLLCLKPRES[4:0] = others, Pll Clock Divided By 256
 */
void ADC_ConfigClk(ADC_CTRL3_CKMOD ADC_ClkMode, uint32_t RCC_ADCHCLKPrescaler)
{
    if(ADC_ClkMode==ADC_CTRL3_CKMOD_AHB){
        RCC_ConfigAdcPllClk(RCC_ADCPLLCLK_DIV1, DISABLE);
        RCC_ConfigAdcHclk(RCC_ADCHCLKPrescaler);
    }else{
        RCC_ConfigAdcPllClk(RCC_ADCHCLKPrescaler, ENABLE);
        RCC_ConfigAdcHclk(RCC_ADCHCLK_DIV1);
    }
}

/**
 * @brief  Configures for the selected OPA channel that ADC can be controled 
 *         and its corresponding rank in the sequencer and FunctionalState.
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_Channel the ADC channel to configure.
 *   This parameter can be one of the following values:
 *     @arg ADC_OPAMP_CS_VPSEL_PF0  OPA PF0 selected
 *     @arg ADC_OPAMP_CS_VPSEL_PA0  OPA PA0 selected
 *     @arg ADC_OPAMP_CS_VPSEL_PA1  OPA PA1 selected
 *     @arg ADC_OPAMP_CS_VPSEL_PA4  OPA PA4 selected
 *     @arg ADC_OPAMP_CS_VPSEL_PA7  OPA PA7 selected
 *     @arg ADC_OPAMP_CS_VPSEL_PB0  OPA PB0 selected
 *     @arg ADC_OPAMP_CS_VPSEL_PB14 OPA PB14 selected
 * @param Rank The rank in the injected group sequencer. This parameter must be between 1 and 4.
 * @param Cmd  This parameter can be: ENABLE or DISABLE.
 */
void ADC_ConfigOPAControl(ADC_Module* ADCx, uint8_t ADC_OPAChannel, uint8_t Rank, FunctionalState Cmd)
{
    uint32_t tmpreg1 = 0, tmpreg2 = 0, tmpreg3 = 0, tmpreg4 = 0;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcOpaChannelValid(ADC_OPAChannel));
    assert_param(IsAdcInjRankValid(Rank));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    tmpreg1 = ADCx->JSEQ;
    /* Get INJ_LEN value: Number = INJ_LEN+1 */
    tmpreg3 = (tmpreg1 & JSEQ_INJ_LEN_SET) >> 20;    
    /* Calculate the mask to clear: ((Rank-1)+(4-INJ_LEN-1)) */
    tmpreg2 = JSQ_OPASEL << (3 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)) + 4); 

    tmpreg1 = ADCx->OPACTRL;
    /* Clear the old JSQx_OPASEL bits for the selected rank */
    tmpreg1 &= ~tmpreg2;
    /* Set the JSQx_OPASEL bits for the selected rank */
    tmpreg2 = (uint32_t)ADC_OPAChannel << (3 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)) + 4);
    /* Set the JSQx_OPASEL bits for the selected rank */
    tmpreg1 |= tmpreg2;
    /* Store the new register value */
    ADCx->OPACTRL = tmpreg1;

    /* Calculate the enable mask to clear: ((Rank-1)+(4-INJ_LEN-1)) */
    tmpreg4 = 1 << ((uint8_t)((Rank + 3) - (tmpreg3 + 1)));
    /* Clear the old JSQx_OPAEN bits for the selected rank */
    tmpreg1 &= ~tmpreg4;    
    
    /* Set the JSQx_OPAEN bits for the selected rank */
    tmpreg4 = (uint32_t)Cmd << ((uint8_t)((Rank + 3) - (tmpreg3 + 1)));

    /* Set the JSQx_OPAEN bits for the selected rank */
    tmpreg1 |= tmpreg4;  
    /* Store the new register value */
    ADCx->OPACTRL = tmpreg1;
}

/**
 * @brief  Configures the opa setup time
 * @param ADCx = ADC, to select the ADC peripheral.
 * @param ADC_OPASetUpTime The OPASetUpTime
 *   This parameter must be a number between 0 to 1023.
 */
void ADC_ConfigOPASetUpTime(ADC_Module* ADCx, uint16_t ADC_OPASetUpTime)
{
    uint32_t tmpreg1 = 0;
    uint32_t tmpreg2 = 0;
    /* Check the parameters */
    assert_param(IsAdcModule(ADCx));
    assert_param(IsAdcOpaSetUpTimeValid(ADC_OPASetUpTime));

    /* Get the old register value */
    tmpreg1 = ADCx->OPACTRL;
    /* Clear the old OPA setup time */
    tmpreg1 &= OPA_SETUP_TIEM_RESET;
    /* Set the opa setup time*/
    tmpreg2 = ADC_OPASetUpTime;
    tmpreg1 |= tmpreg2 << 16;
    /* Store the new register value */
    ADCx->OPACTRL = tmpreg1;
}

void ADC_SetOPACtrl(uint32_t value)
{
    ADC->OPACTRL=value;
}
uint32_t ADC_GetOPACtrl(void)
{
    return ADC->OPACTRL;
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
