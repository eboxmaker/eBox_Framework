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
 * @file n32g030_comp.h
 * @author Nations 
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __N32G030_COMP_H__
#define __N32G030_COMP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g030.h"
#include <stdbool.h>

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup COMP
 * @{
 */

/** @addtogroup COMP_Exported_Constants
 * @{
 */

typedef enum
{
    COMP_CTRL_CLKSEL_SYSTEMCLK            = (0x0L << 20),
    COMP_CTRL_CLKSEL_LSX                  = (0x1L << 20)
} COMP_CTRL_CLKSEL_ENUM;

typedef enum
{
    COMP_CTRL_PWRMD_NORMAL                = (0x0L << 19),
    COMP_CTRL_PWRMD_LOWPWR                = (0x1L << 19)
} COMP_CTRL_PWRMD_ENUM;

typedef enum
{
    COMP_CTRL_BLKING_NO                   = (0x0L << 14),
    COMP_CTRL_BLKING_TIM1_OC5             = (0x1L << 14),
    COMP_CTRL_BLKING_TIM8_OC5             = (0x2L << 14),
} COMP_CTRL_BLKING_ENUM;

typedef enum
{
    COMP_CTRL_HYST_NO                     = (0x0L << 12),
    COMP_CTRL_HYST_LOW                    = (0x1L << 12),
    COMP_CTRL_HYST_MID                    = (0x2L << 12),
    COMP_CTRL_HYST_HIGH                   = (0x3L << 12),
} COMP_CTRL_HYST_ENUM;

typedef enum
{
    COMP_CTRL_POL_NONINVERTED             = (0x0L << 11),
    COMP_CTRL_POL_INVERTED                = (0x1L << 11)
} COMP_CTRL_POL_ENUM;

typedef enum
{
    COMPx_CTRL_OUTTRG_NC                  = (0x0L << 7),
    // comp out trig
    COMP_CTRL_OUTTRG_TIM1_BKIN           = (0x1L << 7),
    COMP_CTRL_OUTTRG_TIM1_IC1            = (0x2L << 7),
    COMP_CTRL_OUTTRG_TIM1_OCrefclear     = (0x3L << 7),
    COMP_CTRL_OUTTRG_TIM8_IC1            = (0x4L << 7),
    COMP_CTRL_OUTTRG_TIM8_OCrefclear     = (0x5L << 7),
    COMP_CTRL_OUTTRG_TIM3_IC1            = (0x6L << 7),
    COMP_CTRL_OUTTRG_TIM3_OCrefclear     = (0x7L << 7),
    COMP_CTRL_OUTTRG_TIM1_BKIN_TIM8_BKIN = (0xBL << 7),
    COMP_CTRL_OUTTRG_TIM8_BKIN           = (0xCL << 7),
    COMP_CTRL_OUTTRG_LPTIM_ETR           = (0xDL << 7),
} COMP_CTRL_OUTTRG_ENUM;

typedef enum {
    //comp inp sel
    COMP_CTRL_INPSEL_PA1                 = (0x0L << 4),
    COMP_CTRL_INPSEL_PA3                 = (0x1L << 4),
    COMP_CTRL_INPSEL_PA7                 = (0x3L << 4),
    COMPx_CTRL_INPSEL_VREF                = (0x2L << 4),
}COMP_CTRL_INPSEL_ENUM;

typedef enum {
    //comp inm sel
    COMP_CTRL_INMSEL_PA0                 = (0x1L << 1),
    COMP_CTRL_INMSEL_PA4                 = (0x2L << 1),
    COMP_CTRL_INMSEL_PA5                 = (0x3L << 1),
    COMPx_CTRL_INMSEL_VREF                = (0x0L << 1),
}COMP_CTRL_INMSEL_ENUM;

typedef enum
{
    COMP_CTRL_EN_ENABLE                   = (0x1L),
    COMP_CTRL_EN_DISABLE                  = (0x0L),
} COMP_CTRL_EN_ENUM;

typedef enum
{
    COMP_INTEN_CMP1IEN_CFG                = (0x01L << 0),
} COMP_INTEN_CMPXIEN_ENUM;

typedef enum
{
    COMP_INTSTS_CMP1IS_STATUS             = (0x01L << 0),
} COMP_INTSTS_STATUS_ENUM;

typedef enum {
    COMPX_FILC_SAMPW_0                    = (0x0L << 6),
    COMPX_FILC_SAMPW_1                    = (0x1L << 6),
    COMPX_FILC_SAMPW_2                    = (0x2L << 6),
    COMPX_FILC_SAMPW_3                    = (0x3L << 6),
    COMPX_FILC_SAMPW_4                    = (0x4L << 6),
    COMPX_FILC_SAMPW_5                    = (0x5L << 6),
    COMPX_FILC_SAMPW_6                    = (0x6L << 6),
    COMPX_FILC_SAMPW_7                    = (0x7L << 6),
    COMPX_FILC_SAMPW_8                    = (0x8L << 6),
    COMPX_FILC_SAMPW_9                    = (0x9L << 6),
    COMPX_FILC_SAMPW_10                   = (0xAL << 6),
    COMPX_FILC_SAMPW_11                   = (0xBL << 6),
    COMPX_FILC_SAMPW_12                   = (0xCL << 6),
    COMPX_FILC_SAMPW_13                   = (0xDL << 6),
    COMPX_FILC_SAMPW_14                   = (0xEL << 6),
    COMPX_FILC_SAMPW_15                   = (0xFL << 6),
    COMPX_FILC_SAMPW_16                   = (0x10L << 6),
    COMPX_FILC_SAMPW_17                   = (0x11L << 6),
    COMPX_FILC_SAMPW_18                   = (0x12L << 6),
    COMPX_FILC_SAMPW_19                   = (0x13L << 6),
    COMPX_FILC_SAMPW_20                   = (0x14L << 6),
    COMPX_FILC_SAMPW_21                   = (0x15L << 6),
    COMPX_FILC_SAMPW_22                   = (0x16L << 6),
    COMPX_FILC_SAMPW_23                   = (0x17L << 6),
    COMPX_FILC_SAMPW_24                   = (0x18L << 6),
    COMPX_FILC_SAMPW_25                   = (0x19L << 6),
    COMPX_FILC_SAMPW_26                   = (0x1AL << 6),
    COMPX_FILC_SAMPW_27                   = (0x1BL << 6),
    COMPX_FILC_SAMPW_28                   = (0x1CL << 6),
    COMPX_FILC_SAMPW_29                   = (0x1DL << 6),
    COMPX_FILC_SAMPW_30                   = (0x1EL << 6),
    COMPX_FILC_SAMPW_31                   = (0x1FL << 6)
}COMPX_FILC_SAMPW_ENUM;

typedef enum {
    COMPX_FILC_THRESH_0                   = (0x0L << 1),
    COMPX_FILC_THRESH_1                   = (0x1L << 1),
    COMPX_FILC_THRESH_2                   = (0x2L << 1),
    COMPX_FILC_THRESH_3                   = (0x3L << 1),
    COMPX_FILC_THRESH_4                   = (0x4L << 1),
    COMPX_FILC_THRESH_5                   = (0x5L << 1),
    COMPX_FILC_THRESH_6                   = (0x6L << 1),
    COMPX_FILC_THRESH_7                   = (0x7L << 1),
    COMPX_FILC_THRESH_8                   = (0x8L << 1),
    COMPX_FILC_THRESH_9                   = (0x9L << 1),
    COMPX_FILC_THRESH_10                  = (0xAL << 1),
    COMPX_FILC_THRESH_11                  = (0xBL << 1),
    COMPX_FILC_THRESH_12                  = (0xCL << 1),
    COMPX_FILC_THRESH_13                  = (0xDL << 1),
    COMPX_FILC_THRESH_14                  = (0xEL << 1),
    COMPX_FILC_THRESH_15                  = (0xFL << 1),
    COMPX_FILC_THRESH_16                  = (0x10L << 1),
    COMPX_FILC_THRESH_17                  = (0x11L << 1),
    COMPX_FILC_THRESH_18                  = (0x12L << 1),
    COMPX_FILC_THRESH_19                  = (0x13L << 1),
    COMPX_FILC_THRESH_20                  = (0x14L << 1),
    COMPX_FILC_THRESH_21                  = (0x15L << 1),
    COMPX_FILC_THRESH_22                  = (0x16L << 1),
    COMPX_FILC_THRESH_23                  = (0x17L << 1),
    COMPX_FILC_THRESH_24                  = (0x18L << 1),
    COMPX_FILC_THRESH_25                  = (0x19L << 1),
    COMPX_FILC_THRESH_26                  = (0x1AL << 1),
    COMPX_FILC_THRESH_27                  = (0x1BL << 1),
    COMPX_FILC_THRESH_28                  = (0x1CL << 1),
    COMPX_FILC_THRESH_29                  = (0x1DL << 1),
    COMPX_FILC_THRESH_30                  = (0x1EL << 1),
    COMPX_FILC_THRESH_31                  = (0x1FL << 1),
}COMPX_FILC_THRESH_ENUM;

typedef enum
{
    COMPX_FILC_FILEN_ENABLE               = ((uint32_t)0x1L),
    COMPX_FILC_FILEN_DISABLE              = ((uint32_t)0x0L)
} COMPX_FILC_FILEN_ENUM;

typedef enum
{
    COMP_LOCK_CMP1LK_LOCK                 = (0x1L << 0),
    COMP_LOCK_CMP1LK_UNLOCK               = (0x0L << 0),
} COMP_LOCK_CMPXLK_ENUM;

typedef enum
{
    VREF = 0,
} VREFX_ENUM;

typedef enum
{
    COMP_INVREF_VREFSEL_0                = (0x0L << 1),
    COMP_INVREF_VREFSEL_1                = (0x1L << 1),
    COMP_INVREF_VREFSEL_2                = (0x2L << 1),
    COMP_INVREF_VREFSEL_3                = (0x3L << 1),
    COMP_INVREF_VREFSEL_4                = (0x4L << 1),
    COMP_INVREF_VREFSEL_5                = (0x5L << 1),
    COMP_INVREF_VREFSEL_6                = (0x6L << 1),
    COMP_INVREF_VREFSEL_7                = (0x7L << 1),
    COMP_INVREF_VREFSEL_8                = (0x8L << 1),
    COMP_INVREF_VREFSEL_9                = (0x9L << 1),
    COMP_INVREF_VREFSEL_10               = (0xAL << 1),
    COMP_INVREF_VREFSEL_11               = (0xBL << 1),
    COMP_INVREF_VREFSEL_12               = (0xCL << 1),
    COMP_INVREF_VREFSEL_13               = (0xDL << 1),
    COMP_INVREF_VREFSEL_14               = (0xEL << 1),
    COMP_INVREF_VREFSEL_15               = (0xFL << 1),
    COMP_INVREF_VREFSEL_16               = (0x10L << 1),
    COMP_INVREF_VREFSEL_17               = (0x11L << 1),
    COMP_INVREF_VREFSEL_18               = (0x12L << 1),
    COMP_INVREF_VREFSEL_19               = (0x13L << 1),
    COMP_INVREF_VREFSEL_20               = (0x14L << 1),
    COMP_INVREF_VREFSEL_21               = (0x15L << 1),
    COMP_INVREF_VREFSEL_22               = (0x16L << 1),
    COMP_INVREF_VREFSEL_23               = (0x17L << 1),
    COMP_INVREF_VREFSEL_24               = (0x18L << 1),
    COMP_INVREF_VREFSEL_25               = (0x19L << 1),
    COMP_INVREF_VREFSEL_26               = (0x1AL << 1),
    COMP_INVREF_VREFSEL_27               = (0x1BL << 1),
    COMP_INVREF_VREFSEL_28               = (0x1CL << 1),
    COMP_INVREF_VREFSEL_29               = (0x1DL << 1),
    COMP_INVREF_VREFSEL_30               = (0x1EL << 1),
    COMP_INVREF_VREFSEL_31               = (0x1FL << 1),
    COMP_INVREF_VREFSEL_32               = (0x20L << 1),
    COMP_INVREF_VREFSEL_33               = (0x21L << 1),
    COMP_INVREF_VREFSEL_34               = (0x22L << 1),
    COMP_INVREF_VREFSEL_35               = (0x23L << 1),
    COMP_INVREF_VREFSEL_36               = (0x24L << 1),
    COMP_INVREF_VREFSEL_37               = (0x25L << 1),
    COMP_INVREF_VREFSEL_38               = (0x26L << 1),
    COMP_INVREF_VREFSEL_39               = (0x27L << 1),
    COMP_INVREF_VREFSEL_40               = (0x28L << 1),
    COMP_INVREF_VREFSEL_41               = (0x29L << 1),
    COMP_INVREF_VREFSEL_42               = (0x2AL << 1),
    COMP_INVREF_VREFSEL_43               = (0x2BL << 1),
    COMP_INVREF_VREFSEL_44               = (0x2CL << 1),
    COMP_INVREF_VREFSEL_45               = (0x2DL << 1),
    COMP_INVREF_VREFSEL_46               = (0x2EL << 1),
    COMP_INVREF_VREFSEL_47               = (0x2FL << 1),
    COMP_INVREF_VREFSEL_48               = (0x30L << 1),
    COMP_INVREF_VREFSEL_49               = (0x31L << 1),
    COMP_INVREF_VREFSEL_50               = (0x32L << 1),
    COMP_INVREF_VREFSEL_51               = (0x33L << 1),
    COMP_INVREF_VREFSEL_52               = (0x34L << 1),
    COMP_INVREF_VREFSEL_53               = (0x35L << 1),
    COMP_INVREF_VREFSEL_54               = (0x36L << 1),
    COMP_INVREF_VREFSEL_55               = (0x37L << 1),
    COMP_INVREF_VREFSEL_56               = (0x38L << 1),
    COMP_INVREF_VREFSEL_57               = (0x39L << 1),
    COMP_INVREF_VREFSEL_58               = (0x3AL << 1),
    COMP_INVREF_VREFSEL_59               = (0x3BL << 1),
    COMP_INVREF_VREFSEL_60               = (0x3CL << 1),
    COMP_INVREF_VREFSEL_61               = (0x3DL << 1),
    COMP_INVREF_VREFSEL_62               = (0x3EL << 1),
    COMP_INVREF_VREFSEL_63               = (0x3FL << 1)
} COMP_INVREF_VREFXSEL_ENUM;



// COMPx_CTRL
#define COMP_CTRL_CLKSEL_MASK  (0x01L << 20)
#define COMP_CTRL_PWRMODE_MASK (0x01L << 19)
#define COMP_CTRL_OUT_MASK      (0x01L << 17)
#define COMP_CTRL_BLKING_MASK   (0x07L << 14)
#define COMP_CTRL_HYST_MASK     (0x03L << 12)
#define COMP_CTRL_POL_MASK      (0x01L << 11)
#define COMP_CTRL_OUTTRG_MASK   (0x0FL << 7)
#define COMP_CTRL_INPSEL_MASK   (0x03L << 4)
#define COMP_CTRL_INMSEL_MASK   (0x03L << 1)
#define COMP_CTRL_EN_MASK       (0x01L << 0)


/**
 * @}
 */

/** @addtogroup COMP_CTRL_CLKSEL
 * @{
 */
#define IS_COMP_CTRL_CLKSEL(SOURCE) (((SOURCE) == COMP_CTRL_CLKSEL_SYSTEMCLK) || ((SOURCE) == COMP_CTRL_CLKSEL_LSX))


/**
 * @}
 */

/** @addtogroup COMP_CTRL_PWRMD
 * @{
 */
#define IS_COMP_CTRL_PWRMD(SOURCE) (((SOURCE) == COMP_CTRL_PWRMD_NORMAL) || ((SOURCE) == COMP_CTRL_PWRMD_LOWPWR))


/**
 * @}
 */

/** @addtogroup COMP_CTRL_BLKING
 * @{
 */
#define IS_COMP_CTRL_BLKING(SOURCE) (((SOURCE) == COMP_CTRL_BLKING_NO) || ((SOURCE) == COMP_CTRL_BLKING_TIM1_OC5)    \
                                  || ((SOURCE) == COMP_CTRL_BLKING_TIM8_OC5))

/**
 * @}
 */

/** @addtogroup COMP_CTRL_HYST
 * @{
 */
#define IS_COMP_CTRL_HYST(SOURCE) (((SOURCE) == COMP_CTRL_HYST_NO) || ((SOURCE) == COMP_CTRL_HYST_LOW)    \
                                || ((SOURCE) == COMP_CTRL_HYST_MID) || ((SOURCE) == COMP_CTRL_HYST_HIGH))

/**
 * @}
 */

/** @addtogroup COMP_CTRL_POL
 * @{
 */
#define IS_COMP_CTRL_POL(SOURCE) (((SOURCE) == COMP_CTRL_POL_NONINVERTED) || ((SOURCE) == COMP_CTRL_POL_INVERTED))

/**
 * @}
 */

/** @addtogroup COMP_CTRL_OUTTRG
 * @{
 */
#define IS_COMPX_CTRL_OUTTRG(SOURCE) (((SOURCE) == COMPx_CTRL_OUTTRG_NC) || ((SOURCE) == COMP_CTRL_OUTTRG_TIM1_BKIN)  \
                                   || ((SOURCE) == COMP_CTRL_OUTTRG_TIM1_IC1) || ((SOURCE) == COMP_CTRL_OUTTRG_TIM1_OCrefclear)  \
                                   || ((SOURCE) == COMP_CTRL_OUTTRG_TIM8_IC1) || ((SOURCE) == COMP_CTRL_OUTTRG_TIM8_OCrefclear)  \
                                   || ((SOURCE) == COMP_CTRL_OUTTRG_TIM3_IC1) || ((SOURCE) == COMP_CTRL_OUTTRG_TIM3_OCrefclear)  \
                                   || ((SOURCE) == COMP_CTRL_OUTTRG_TIM1_BKIN_TIM8_BKIN) || ((SOURCE) == COMP_CTRL_OUTTRG_TIM8_BKIN)  \
                                   || ((SOURCE) == COMP_CTRL_OUTTRG_LPTIM_ETR) )

/**
 * @}
 */

/** @addtogroup COMP_CTRL_INPSEL
 * @{
 */
#define IS_COMPX_CTRL_INPSEL(SOURCE) (((SOURCE) == COMP_CTRL_INPSEL_PA1) || ((SOURCE) == COMP_CTRL_INPSEL_PA3)  \
                                   || ((SOURCE) == COMP_CTRL_INPSEL_PA7) || ((SOURCE) == COMPx_CTRL_INPSEL_VREF))

/**
 * @}
 */

/** @addtogroup COMP_CTRL_INPSEL
 * @{
 */
#define IS_COMPX_CTRL_INMSEL(SOURCE) (((SOURCE) == COMP_CTRL_INMSEL_PA0) || ((SOURCE) == COMP_CTRL_INMSEL_PA4)  \
                                   || ((SOURCE) == COMP_CTRL_INMSEL_PA5) || ((SOURCE) == COMPx_CTRL_INMSEL_VREF))

//COMPx_FILC
#define COMP_FILC_SAMPW_MASK         (0x1FL<<6) //Low filter sample window size. Number of samples to monitor is SAMPWIN+1.
#define COMP_FILC_THRESH_MASK        (0x1FL<<1) //For proper operation, the value of THRESH must be greater than SAMPWIN / 2.
#define COMP_FILC_FILEN_MASK         (0x01L<<0) //Filter enable.

//COMPx_FILP
#define COMP_FILP_CLKPSC_MASK        (uint16_t)(0xFFFF) //Low filter sample clock prescale.
#define IS_COMP_FILP_CLKPSC(SOURCE) ((SOURCE) <= COMP_FILP_CLKPSC_MASK)

//COMP_INTEN
#define COMP_INTEN_CMPIEN_MSK        (0x01L << 0) // This bit control Interrput enable of COMP.

//COMP_INTSTS
#define COMP_INTSTS_INTSTS_MSK       (0x01L << 0) // This bit control Interrput enable of COMP.

//COMP_INVREF
#define COMP_INVREF_VREFSEL_MSK     (0x2FL << 1) // Vref Voltage scaler value.
#define COMP_INVREF_VREFEN_ENABLE   (0x01L << 0)
#define COMP_INVREF_VREFEN_DISABLE  (0x00L << 0)

//COMP_LOCK
#define COMP_LOCK_CMP1LK_MASK        (0x1L << 0)

/**
 * @}
 */

/**
 * @brief  COMP Init structure definition
 */

typedef struct
{
    // COMP_CTRL
    COMP_CTRL_CLKSEL_ENUM  ClockSelect; // only COMP have this bit
    COMP_CTRL_PWRMD_ENUM   LowPoweMode; // only COMP have this bit

    COMP_CTRL_BLKING_ENUM  Blking;      /*see @ref COMP_CTRL_BLKING */
    COMP_CTRL_HYST_ENUM    Hyst;
    COMP_CTRL_POL_ENUM     PolRev;      // out polarity reverse
    COMP_CTRL_OUTTRG_ENUM  OutTrg;
    COMP_CTRL_INPSEL_ENUM  InpSel;
    COMP_CTRL_INMSEL_ENUM  InmSel;
    //COMP_CTRL_EN En;
    // filter
    COMPX_FILC_SAMPW_ENUM  SampWindow;           // 5bit
    COMPX_FILC_THRESH_ENUM Thresh;               // 5bit ,need > SampWindow/2
    COMPX_FILC_FILEN_ENUM  FilterEn;
    // filter psc
    uint16_t ClkPsc;
} COMP_InitType;

/** @addtogroup COMP_Exported_Functions
 * @{
 */

void COMP_DeInit(void);
void COMP_StructInit(COMP_InitType* COMP_InitStruct);
void COMP_Init( COMP_InitType* COMP_InitStruct);
void COMP_Enable( COMP_CTRL_EN_ENUM Cmd);
void COMP_SetInpSel( COMP_CTRL_INPSEL_ENUM Inpsel);
void COMP_SetInmSel( COMP_CTRL_INMSEL_ENUM Inmsel);
void COMP_SetOutTrig( COMP_CTRL_OUTTRG_ENUM OutTrig);                                          
void COMP_ConfigInt(COMP_INTEN_CMPXIEN_ENUM COMPx_INT, FunctionalState Cmd);                                                 // return see @COMP_INTSTS_CMPIS
INTStatus COMP_GetIntStatus(COMP_INTSTS_STATUS_ENUM status);
FlagStatus COMP_GetCompOutStatus(void);
void COMP_ConfigCTRLLock(COMP_LOCK_CMPXLK_ENUM COMPx_LOCK, FunctionalState Cmd);                                              // see @COMP_LOCK_CMPLK
void COMP_ConfigVREFx(VREFX_ENUM Vrefx, COMP_INVREF_VREFXSEL_ENUM COMP_VREFx, FunctionalState Cmd);                                            // see @COMP_INTEN_CMPIEN

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /*__N32G030_ADC_H */
/**
 * @}
 */
/**
 * @}
 */
