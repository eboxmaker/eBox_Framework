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
 * @file n32g030_adc.h
 * @author Nations 
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __N32G030_ADC_H__
#define __N32G030_ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g030.h"
#include <stdbool.h>

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup ADC
 * @{
 */

/** @addtogroup ADC_Exported_Types
 * @{
 */

/**
 * @brief  ADC Init structure definition
 */
typedef struct
{

    FunctionalState MultiChEn; /*!< Specifies whether the conversion is performed in
                                           Scan (multichannels) or Single (one channel) mode.
                                           This parameter can be set to ENABLE or DISABLE */

    FunctionalState ContinueConvEn; /*!< Specifies whether the conversion is performed in
                                                 Continuous or Single mode.
                                                 This parameter can be set to ENABLE or DISABLE. */

    uint32_t ExtTrigSelect; /*!< Defines the external trigger used to start the analog
                                        to digital conversion of regular channels. This parameter
                                        can be a value of @ref
                               ADC_external_trigger_sources_for_regular_channels_conversion */

    uint32_t DatAlign; /*!< Specifies whether the ADC data alignment is left or right.
                                 This parameter can be a value of @ref ADC_data_align */

    uint8_t ChsNumber; /*!< Specifies the number of ADC channels that will be converted
                                   using the sequencer for regular channel group.
                                   This parameter must range from 1 to 16. */
} ADC_InitType;
/**
 * @}
 */

/** @addtogroup ADC_Exported_Constants
 * @{
 */

#define IsAdcModule(PERIPH) (((PERIPH) == ADC))

#define IsAdcDmaModule(PERIPH) (((PERIPH) == ADC))



/** @addtogroup ADC_external_trigger_sources_for_regular_channels_conversion
 * @{
 */

#define ADC_EXT_TRIGCONV_T1_CC1              ((uint32_t)0x00000000) 
#define ADC_EXT_TRIGCONV_T1_CC2              ((uint32_t)0x00020000) 
#define ADC_EXT_TRIGCONV_T1_CC3              ((uint32_t)0x00040000) 
#define ADC_EXT_TRIGCONV_T3_TRGO             ((uint32_t)0x00080000) 
#define ADC_EXT_TRIGCONV_EXTI_TIM8_TRGO      ((uint32_t)0x000C0000) 
#define ADC_EXT_TRIGCONV_NONE                ((uint32_t)0x000E0000) 


#define IsAdcExtTrig(REGTRIG)                                                                                         \
    (((REGTRIG) == ADC_EXT_TRIGCONV_T1_CC1) || ((REGTRIG) == ADC_EXT_TRIGCONV_T1_CC2)                                 \
     || ((REGTRIG) == ADC_EXT_TRIGCONV_T1_CC3) || ((REGTRIG) == ADC_EXT_TRIGCONV_T3_TRGO)                             \
     || ((REGTRIG) == ADC_EXT_TRIGCONV_EXTI_TIM8_TRGO)                 \
     || ((REGTRIG) == ADC_EXT_TRIGCONV_NONE))                   
/**
 * @}
 */

/** @addtogroup ADC_data_align
 * @{
 */

#define ADC_DAT_ALIGN_R      ((uint32_t)0x00000000)
#define ADC_DAT_ALIGN_L      ((uint32_t)0x00000800)
#define IsAdcDatAlign(ALIGN) (((ALIGN) == ADC_DAT_ALIGN_R) || ((ALIGN) == ADC_DAT_ALIGN_L))
/**
 * @}
 */

/** @addtogroup ADC_channels
 * @{
 */

#define ADC_CH_0  ((uint8_t)0x00)
#define ADC_CH_1  ((uint8_t)0x01)
#define ADC_CH_2  ((uint8_t)0x02)
#define ADC_CH_3  ((uint8_t)0x03)
#define ADC_CH_4  ((uint8_t)0x04)
#define ADC_CH_5  ((uint8_t)0x05)
#define ADC_CH_6  ((uint8_t)0x06)
#define ADC_CH_7  ((uint8_t)0x07)
#define ADC_CH_8  ((uint8_t)0x08)
#define ADC_CH_9  ((uint8_t)0x09)
#define ADC_CH_10 ((uint8_t)0x0A)
#define ADC_CH_11 ((uint8_t)0x0B)
#define ADC_CH_12 ((uint8_t)0x0C)//vts
#define ADC_CH_13 ((uint8_t)0x0D)//vref_adcbuffer
#define ADC_CH_14 ((uint8_t)0x0E)//vrefp_toadc
#define ADC_CH_15 ((uint8_t)0x0F)//vdda


#define ADC_CH_0_PA0   ((uint8_t)0x00)
#define ADC_CH_1_PA1   ((uint8_t)0x01)
#define ADC_CH_2_PA2   ((uint8_t)0x02)
#define ADC_CH_3_PA3   ((uint8_t)0x03)
#define ADC_CH_4_PA4   ((uint8_t)0x04)
#define ADC_CH_5_PA5   ((uint8_t)0x05)
#define ADC_CH_6_PA6   ((uint8_t)0x06)
#define ADC_CH_7_PA7   ((uint8_t)0x07)
#define ADC_CH_8_PB0   ((uint8_t)0x08)
#define ADC_CH_9_PB1   ((uint8_t)0x09)
#define ADC_CH_10_PB2  ((uint8_t)0x0A)
#define ADC_CH_11_PB10 ((uint8_t)0x0B)


#define ADC_CH_TEMP_SENSOR    ((uint8_t)ADC_CH_12)
#define ADC_CH_VREFINT        ((uint8_t)ADC_CH_13)
#define ADC_CH_VREFP          ((uint8_t)ADC_CH_14)
#define ADC_CH_VDDA          ((uint8_t)ADC_CH_15)

#define IsAdcChannel(CHANNEL)                                                                                          \
    (((CHANNEL) == ADC_CH_0) || ((CHANNEL) == ADC_CH_1) || ((CHANNEL) == ADC_CH_2) || ((CHANNEL) == ADC_CH_3)          \
     || ((CHANNEL) == ADC_CH_4) || ((CHANNEL) == ADC_CH_5) || ((CHANNEL) == ADC_CH_6) || ((CHANNEL) == ADC_CH_7)       \
     || ((CHANNEL) == ADC_CH_8) || ((CHANNEL) == ADC_CH_9) || ((CHANNEL) == ADC_CH_10) || ((CHANNEL) == ADC_CH_11)     \
     || ((CHANNEL) == ADC_CH_12) || ((CHANNEL) == ADC_CH_13) || ((CHANNEL) == ADC_CH_14) || ((CHANNEL) == ADC_CH_15)   )
/**
 * @}
 */

/** @addtogroup ADC_sampling_time
 * @{
 */

#define ADC_SAMP_TIME_6CYCLES5   ((uint8_t)0x00)
#define ADC_SAMP_TIME_8CYCLES5   ((uint8_t)0x01)
#define ADC_SAMP_TIME_14CYCLES5  ((uint8_t)0x02)
#define ADC_SAMP_TIME_20CYCLES5  ((uint8_t)0x03)
#define ADC_SAMP_TIME_29CYCLES5  ((uint8_t)0x04)
#define ADC_SAMP_TIME_42CYCLES5  ((uint8_t)0x05)
#define ADC_SAMP_TIME_56CYCLES5  ((uint8_t)0x06)
#define ADC_SAMP_TIME_72CYCLES5  ((uint8_t)0x07)
#define ADC_SAMP_TIME_88CYCLES5  ((uint8_t)0x08)
#define ADC_SAMP_TIME_120CYCLES5 ((uint8_t)0x09)
#define ADC_SAMP_TIME_182CYCLES5 ((uint8_t)0x0A)
#define ADC_SAMP_TIME_240CYCLES5 ((uint8_t)0x0B)
#define ADC_SAMP_TIME_300CYCLES5 ((uint8_t)0x0C)
#define ADC_SAMP_TIME_400CYCLES5 ((uint8_t)0x0D)
#define ADC_SAMP_TIME_480CYCLES5 ((uint8_t)0x0E)
#define ADC_SAMP_TIME_600CYCLES5 ((uint8_t)0x0F)


#define IsAdcSampleTime(TIME)                                                                 \
        (((TIME) == ADC_SAMP_TIME_6CYCLES5)   || ((TIME) == ADC_SAMP_TIME_8CYCLES5)           \
         || ((TIME) == ADC_SAMP_TIME_14CYCLES5)   || ((TIME) == ADC_SAMP_TIME_20CYCLES5)          \
         || ((TIME) == ADC_SAMP_TIME_29CYCLES5)   || ((TIME) == ADC_SAMP_TIME_42CYCLES5)          \
         || ((TIME) == ADC_SAMP_TIME_56CYCLES5)   || ((TIME) == ADC_SAMP_TIME_72CYCLES5)          \
         || ((TIME) == ADC_SAMP_TIME_88CYCLES5)   || ((TIME) == ADC_SAMP_TIME_120CYCLES5)         \
         || ((TIME) == ADC_SAMP_TIME_182CYCLES5)  || ((TIME) == ADC_SAMP_TIME_240CYCLES5)         \
         || ((TIME) == ADC_SAMP_TIME_300CYCLES5)  || ((TIME) == ADC_SAMP_TIME_400CYCLES5)         \
         || ((TIME) == ADC_SAMP_TIME_480CYCLES5)  || ((TIME) == ADC_SAMP_TIME_600CYCLES5)         \
         )
/**
 * @}
 */

/** @addtogroup ADC_external_trigger_sources_for_injected_channels_conversion
 * @{
 */

#define ADC_EXT_TRIG_INJ_CONV_T1_TRGO            ((uint32_t)0x00000000) 
#define ADC_EXT_TRIG_INJ_CONV_T1_CC4             ((uint32_t)0x00001000) 
#define ADC_EXT_TRIG_INJ_CONV_T3_CC4             ((uint32_t)0x00004000) 
#define ADC_EXT_TRIG_INJ_CONV_EXTI_TIM8_CC4      ((uint32_t)0x00006000) 
#define ADC_EXT_TRIG_INJ_CONV_NONE               ((uint32_t)0x00007000) 


#define IsAdcExtInjTrig(INJTRIG)                                                                                       \
    (((INJTRIG) == ADC_EXT_TRIG_INJ_CONV_T1_TRGO) || ((INJTRIG) == ADC_EXT_TRIG_INJ_CONV_T1_CC4)                       \
     || ((INJTRIG) == ADC_EXT_TRIG_INJ_CONV_T3_CC4)                     \
     || ((INJTRIG) == ADC_EXT_TRIG_INJ_CONV_EXTI_TIM8_CC4) || ((INJTRIG) == ADC_EXT_TRIG_INJ_CONV_NONE))
/**
 * @}
 */

/** @addtogroup ADC_injected_channel_selection
 * @{
 */

#define ADC_INJ_CH_1 ((uint8_t)0x18)
#define ADC_INJ_CH_2 ((uint8_t)0x1C)
#define ADC_INJ_CH_3 ((uint8_t)0x20)
#define ADC_INJ_CH_4 ((uint8_t)0x24)
#define IsAdcInjCh(CHANNEL)                                                                                            \
    (((CHANNEL) == ADC_INJ_CH_1) || ((CHANNEL) == ADC_INJ_CH_2) || ((CHANNEL) == ADC_INJ_CH_3)                         \
     || ((CHANNEL) == ADC_INJ_CH_4))
/**
 * @}
 */

/** @addtogroup ADC_analog_watchdog_selection
 * @{
 */

#define ADC_ANALOG_WTDG_SINGLEREG_ENABLE          ((uint32_t)0x00800200)
#define ADC_ANALOG_WTDG_SINGLEINJEC_ENABLE        ((uint32_t)0x00400200)
#define ADC_ANALOG_WTDG_SINGLEREG_OR_INJEC_ENABLE ((uint32_t)0x00C00200)
#define ADC_ANALOG_WTDG_ALLREG_ENABLE             ((uint32_t)0x00800000)
#define ADC_ANALOG_WTDG_ALLINJEC_ENABLE           ((uint32_t)0x00400000)
#define ADC_ANALOG_WTDG_ALLREG_ALLINJEC_ENABLE    ((uint32_t)0x00C00000)
#define ADC_ANALOG_WTDG_NONE                      ((uint32_t)0x00000000)

#define IsAdcAnalogWatchdog(WATCHDOG)                                                                                  \
    (((WATCHDOG) == ADC_ANALOG_WTDG_SINGLEREG_ENABLE) || ((WATCHDOG) == ADC_ANALOG_WTDG_SINGLEINJEC_ENABLE)            \
     || ((WATCHDOG) == ADC_ANALOG_WTDG_SINGLEREG_OR_INJEC_ENABLE) || ((WATCHDOG) == ADC_ANALOG_WTDG_ALLREG_ENABLE)     \
     || ((WATCHDOG) == ADC_ANALOG_WTDG_ALLINJEC_ENABLE) || ((WATCHDOG) == ADC_ANALOG_WTDG_ALLREG_ALLINJEC_ENABLE)      \
     || ((WATCHDOG) == ADC_ANALOG_WTDG_NONE))
/**
 * @}
 */

/** @addtogroup ADC_interrupts_definition
    the hign 8 bits (02,01,04) do not correspond to CTRL1 register, 
    the main poupose is to match the corresponding flag bit in the sts register 
 * @{
 */

#define ADC_INT_ENDC  ((uint16_t)0x0220)
#define ADC_INT_AWD   ((uint16_t)0x0140)
#define ADC_INT_JENDC ((uint16_t)0x0480)

#define IsAdcInt(IT) ((((IT) & (uint16_t)0xF81F) == 0x00) && ((IT) != 0x00))

#define IsAdcGetInt(IT) (((IT) == ADC_INT_ENDC) || ((IT) == ADC_INT_AWD) || ((IT) == ADC_INT_JENDC))
/**
 * @}
 */

/** @addtogroup ADC_flags_definition
 * @{
 */

#define ADC_FLAG_AWDG      ((uint8_t)0x01)
#define ADC_FLAG_ENDC      ((uint8_t)0x02)
#define ADC_FLAG_JENDC     ((uint8_t)0x04)
#define ADC_FLAG_JSTR      ((uint8_t)0x08)
#define ADC_FLAG_STR       ((uint8_t)0x10)
#define ADC_FLAG_ENDC_ANY   ((uint8_t)0x20)
#define ADC_FLAG_JENDC_ANY  ((uint8_t)0x40)
#define IsAdcClrFlag(FLAG) ((((FLAG) & (uint8_t)0x80) == 0x00) && ((FLAG) != 0x00))
#define IsAdcGetFlag(FLAG)                                                                                             \
    (((FLAG) == ADC_FLAG_AWDG) || ((FLAG) == ADC_FLAG_ENDC) || ((FLAG) == ADC_FLAG_JENDC) || ((FLAG) == ADC_FLAG_JSTR) \
     || ((FLAG) == ADC_FLAG_STR) || ((FLAG) == ADC_FLAG_ENDC_ANY) || ((FLAG) == ADC_FLAG_JENDC_ANY))
/**
 * @}
 */

/** @addtogroup ADC_thresholds
 * @{
 */
#define IsAdcValid(THRESHOLD) ((THRESHOLD) <= 0xFFF)
/**
 * @}
 */

/** @addtogroup ADC_injected_offset
 * @{
 */

#define IsAdcOffsetValid(OFFSET) ((OFFSET) <= 0xFFF)

/**
 * @}
 */

/** @addtogroup ADC_injected_length
 * @{
 */

#define IsAdcInjLenValid(LENGTH) (((LENGTH) >= 0x1) && ((LENGTH) <= 0x4))

/**
 * @}
 */

/** @addtogroup ADC_injected_rank
 * @{
 */

#define IsAdcInjRankValid(RANK) (((RANK) >= 0x1) && ((RANK) <= 0x4))

/**
 * @}
 */

/** @addtogroup ADC_regular_length
 * @{
 */

#define IsAdcSeqLenValid(LENGTH) (((LENGTH) >= 0x1) && ((LENGTH) <= 0x10))
/**
 * @}
 */

/** @addtogroup ADC_regular_rank
 * @{
 */

#define IsAdcReqRankValid(RANK) (((RANK) >= 0x1) && ((RANK) <= 0x10))

/**
 * @}
 */

/** @addtogroup ADC_regular_discontinuous_mode_number
 * @{
 */

#define IsAdcSeqDiscNumberValid(NUMBER) (((NUMBER) >= 0x1) && ((NUMBER) <= 0x8))

/** @addtogroup ADC_Opa_setup_time
 * @{
 */

#define IsAdcOpaSetUpTimeValid(NUMBER)  ((NUMBER) <= 0x2ff)
/**
 * @}
 */
 /** @addtogroup ADC_Opa_vpsel
 * @{
 */

#define IsAdcOpaChannelValid(NUMBER)  ((NUMBER) <= 0x06)
/**
 * @}
 */


/************************** fllowing bit seg in ex register  **********************/
/**@addtogroup ADC_channels_ex_style
 * @{
 */


#define ADC_CH_0_PA0   ((uint8_t)0x00)
#define ADC_CH_1_PA1   ((uint8_t)0x01)
#define ADC_CH_2_PA2   ((uint8_t)0x02)
#define ADC_CH_3_PA3   ((uint8_t)0x03)
#define ADC_CH_4_PA4   ((uint8_t)0x04)
#define ADC_CH_5_PA5   ((uint8_t)0x05)
#define ADC_CH_6_PA6   ((uint8_t)0x06)
#define ADC_CH_7_PA7   ((uint8_t)0x07)
#define ADC_CH_8_PB0   ((uint8_t)0x08)
#define ADC_CH_9_PB1   ((uint8_t)0x09)
#define ADC_CH_10_PB2  ((uint8_t)0x0A)
#define ADC_CH_11_PB10  ((uint8_t)0x0B)

/**
 * @}
 */


/**@addtogroup ADC_ctrl3_definition
 * @{
 */
#define ADC_CTRL3_JENDCAIEN_MSK ((uint32_t)0x01L << 9)
#define ADC_CTRL3_ENDCAIEN_MSK  ((uint32_t)0x01L << 8)
#define ADC_CTRL3_PDRDY_MSK     ((uint32_t)0x01L << 6)
#define ADC_CTRL3_RDY_MSK       ((uint32_t)0x01L << 5)
#define ADC_CTRL3_CKMOD_MSK     ((uint32_t)0x01L << 4)
#define ADC_CTRL3_VREFRDY_MSK   ((uint32_t)0x01L << 2)
#define ADC_CTRL3_VREFEN_MSK    ((uint32_t)0x1L << 1)
#define ADC_CTRL3_REFSEL_MSK    ((uint32_t)0x1L << 0)

typedef enum
{
    ADC_CTRL3_CKMOD_AHB = 0,
    ADC_CTRL3_CKMOD_PLL = 1,
} ADC_CTRL3_CKMOD;


typedef struct
{
    FunctionalState RefSel;
    FunctionalState VrefEn;
    FunctionalState JendcIntEn;
    FunctionalState EndcIntEn;
    ADC_CTRL3_CKMOD ClkMode;
    //FunctionalState CalAtuoLoadEn;
    //bool DifModCal;
    //ADC_CTRL3_RES ResBit;
} ADC_InitTypeEx;

//bit:
typedef enum
{
    ADC_OPAMP_CS_VPSEL_PF0     =  ((uint8_t)0x00),
    ADC_OPAMP_CS_VPSEL_PA0     =  ((uint8_t)0x01),
    ADC_OPAMP_CS_VPSEL_PA1     =  ((uint8_t)0x02),
    ADC_OPAMP_CS_VPSEL_PA4     =  ((uint8_t)0x03),
    ADC_OPAMP_CS_VPSEL_PA7     =  ((uint8_t)0x04),
    ADC_OPAMP_CS_VPSEL_PB0     =  ((uint8_t)0x05),
    ADC_OPAMP_CS_VPSEL_PB14    =  ((uint8_t)0x06),
} ADC_OPAMP_CS_VPSEL;


/** @addtogroup ADC_flags_ex_definition
 * @{
 */
#define ADC_FLAG_RDY    ((uint8_t)0x20)
#define ADC_FLAG_PD_RDY ((uint8_t)0x40)
#define ADC_FLAG_VREF_RDY ((uint8_t)0x04)
#define IS_ADC_GET_READY(FLAG) (((FLAG) == ADC_FLAG_RDY) || ((FLAG) == ADC_FLAG_PD_RDY) || ((FLAG) == ADC_FLAG_VREF_RDY))
/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup ADC_Exported_Functions
 * @{
 */

void ADC_DeInit(ADC_Module* ADCx);
void ADC_Init(ADC_Module* ADCx, ADC_InitType* ADC_InitStruct);
void ADC_InitStruct(ADC_InitType* ADC_InitStruct);
void ADC_Enable(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_EnableDMA(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_ConfigInt(ADC_Module* ADCx, uint16_t ADC_IT, FunctionalState Cmd);
void ADC_EnableSoftwareStartConv(ADC_Module* ADCx, FunctionalState Cmd);
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_Module* ADCx);
void ADC_ConfigDiscModeChannelCount(ADC_Module* ADCx, uint8_t Number);
void ADC_EnableDiscMode(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_ConfigRegularChannel(ADC_Module* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void ADC_EnableExternalTrigConv(ADC_Module* ADCx, FunctionalState Cmd);
uint16_t ADC_GetDat(ADC_Module* ADCx);
void ADC_EnableAutoInjectedConv(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_EnableInjectedDiscMode(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_ConfigExternalTrigInjectedConv(ADC_Module* ADCx, uint32_t ADC_ExternalTrigInjecConv);
void ADC_EnableExternalTrigInjectedConv(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_EnableSoftwareStartInjectedConv(ADC_Module* ADCx, FunctionalState Cmd);
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_Module* ADCx);
void ADC_ConfigInjectedChannel(ADC_Module* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void ADC_ConfigInjectedSequencerLength(ADC_Module* ADCx, uint8_t Length);
void ADC_SetInjectedOffsetDat(ADC_Module* ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset);
uint16_t ADC_GetInjectedConversionDat(ADC_Module* ADCx, uint8_t ADC_InjectedChannel);
void ADC_ConfigAnalogWatchdogWorkChannelType(ADC_Module* ADCx, uint32_t ADC_AnalogWatchdog);
void ADC_ConfigAnalogWatchdogThresholds(ADC_Module* ADCx, uint16_t HighThreshold, uint16_t LowThreshold);
void ADC_ConfigAnalogWatchdogSingleChannel(ADC_Module* ADCx, uint8_t ADC_Channel);
void ADC_EnableTempSensorVrefint(FunctionalState Cmd);
FlagStatus ADC_GetFlagStatus(ADC_Module* ADCx, uint8_t ADC_FLAG);
void ADC_ClearFlag(ADC_Module* ADCx, uint8_t ADC_FLAG);
INTStatus ADC_GetIntStatus(ADC_Module* ADCx, uint16_t ADC_IT);
void ADC_ClearIntPendingBit(ADC_Module* ADCx, uint16_t ADC_IT);

void ADC_InitEx(ADC_Module* ADCx, ADC_InitTypeEx* ADC_InitStructEx);
FlagStatus ADC_GetFlagStatusNew(ADC_Module* ADCx, uint8_t ADC_FLAG_NEW);

void ADC_ConfigClk(ADC_CTRL3_CKMOD ADC_ClkMode, uint32_t RCC_ADCHCLKPrescaler);

void ADC_ConfigOPAControl(ADC_Module* ADCx, uint8_t ADC_OPAChannel, uint8_t Rank, FunctionalState Cmd);
void ADC_ConfigOPASetUpTime(ADC_Module* ADCx, uint16_t ADC_OPASetUpTime);

void ADC_SetOPACtrl(uint32_t value);
uint32_t ADC_GetOPACtrl(void);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /*__N32G030_ADC_H__ */

/**
 * @}
 */
/**
 * @}
 */
