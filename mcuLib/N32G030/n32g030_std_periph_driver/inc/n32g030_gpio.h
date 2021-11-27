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
 * @file n32g030_gpio.h
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __N32G030_GPIO_H__
#define __N32G030_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g030.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup GPIO
 * @{
 */

/** @addtogroup GPIO_Exported_Types
 * @{
 */

/**
 * @brief  GPIO Init structure definition
 */
typedef struct
{
    uint32_t Pin; /*!< Specifies the GPIO pins to be configured.
                        This parameter can be any value of @ref GPIO_pins_define */
    
    uint32_t GPIO_Mode; /*!< Specifies the operating mode for the selected pins.
                                    This parameter can be a value of @ref GPIO_mode_define */

    uint32_t GPIO_Pull;  /*!< Specifies the Pull-up or Pull-Down activation for the selected pins.
                                    This parameter can be a value of @ref GPIO_pull_define */

    uint32_t GPIO_Speed; /*!< Specifies the slew rate for the selected pins.
                                    This parameter can be a value of @ref GPIO_speed_define */

    uint32_t GPIO_Current; /*!<Driving current of the select pins>.
                                    This paramter can be a value of @ref GPIO_current_define */
    
    uint32_t GPIO_Alternate; /*!< Specifies the alternate function for the selected pins 
                                   This parameter can be a value of @ref GPIOEx_Alternate_function_selection */ 
} GPIO_InitType;

/**
 * @brief  Bit_SET and Bit_RESET enumeration
 */

typedef enum
{
    Bit_RESET = 0,
    Bit_SET
} Bit_OperateType;

#define IS_GPIO_BIT_OPERATE(OPERATE) (((OPERATE) == Bit_RESET) || ((OPERATE) == Bit_SET))

/**
 * @}
 */

/** @addtogroup GPIO_Exported_Constants GPIO Exported Constants
 * @{
 */

#define IS_GPIO_ALL_PERIPH(PERIPH)                                                                                     \
    (((PERIPH) == GPIOA) || ((PERIPH) == GPIOB) || ((PERIPH) == GPIOC) || ((PERIPH) == GPIOF))

#define GPIO_GET_INDEX(PERIPH) (((PERIPH) == (GPIOA))? 0 :\
                                ((PERIPH) == (GPIOB))? 1 :\
                                ((PERIPH) == (GPIOC))? 2 :5)

#define GPIO_GET_PERIPH(INDEX) (((INDEX)==((uint8_t)0x00))? GPIOA :\
                                ((INDEX)==((uint8_t)0x01))? GPIOB :\
                                ((INDEX)==((uint8_t)0x02))? GPIOC : GPIOF)

/** @addtogroup GPIO_pins_define Pin definition
 * @{
 */
#define GPIO_PIN_0   ((uint16_t)0x0001U) /*!< Pin 0 selected */
#define GPIO_PIN_1   ((uint16_t)0x0002U) /*!< Pin 1 selected */
#define GPIO_PIN_2   ((uint16_t)0x0004U) /*!< Pin 2 selected */
#define GPIO_PIN_3   ((uint16_t)0x0008U) /*!< Pin 3 selected */
#define GPIO_PIN_4   ((uint16_t)0x0010U) /*!< Pin 4 selected */
#define GPIO_PIN_5   ((uint16_t)0x0020U) /*!< Pin 5 selected */
#define GPIO_PIN_6   ((uint16_t)0x0040U) /*!< Pin 6 selected */
#define GPIO_PIN_7   ((uint16_t)0x0080U) /*!< Pin 7 selected */
#define GPIO_PIN_8   ((uint16_t)0x0100U) /*!< Pin 8 selected */
#define GPIO_PIN_9   ((uint16_t)0x0200U) /*!< Pin 9 selected */
#define GPIO_PIN_10  ((uint16_t)0x0400U) /*!< Pin 10 selected */
#define GPIO_PIN_11  ((uint16_t)0x0800U) /*!< Pin 11 selected */
#define GPIO_PIN_12  ((uint16_t)0x1000U) /*!< Pin 12 selected */
#define GPIO_PIN_13  ((uint16_t)0x2000U) /*!< Pin 13 selected */
#define GPIO_PIN_14  ((uint16_t)0x4000U) /*!< Pin 14 selected */
#define GPIO_PIN_15  ((uint16_t)0x8000U) /*!< Pin 15 selected */
#define GPIO_PIN_ALL ((uint16_t)0xFFFFU) /*!< All pins selected */

#define GPIOA_PIN_AVAILABLE  GPIO_PIN_ALL
#define GPIOB_PIN_AVAILABLE  GPIO_PIN_ALL
#define GPIOC_PIN_AVAILABLE  (GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)
#define GPIOF_PIN_AVAILABLE  (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |\
                               GPIO_PIN_6 | GPIO_PIN_7)
/**
 * @}
 */
#define IS_GPIO_PIN(PIN) ((((PIN) & (uint16_t)0x00) == 0x00) && ((PIN) != (uint16_t)0x00))

#define IS_GET_GPIO_PIN(PIN)                                                                                           \
    (((PIN) == GPIO_PIN_0) || ((PIN) == GPIO_PIN_1) || ((PIN) == GPIO_PIN_2) || ((PIN) == GPIO_PIN_3)                  \
     || ((PIN) == GPIO_PIN_4) || ((PIN) == GPIO_PIN_5) || ((PIN) == GPIO_PIN_6) || ((PIN) == GPIO_PIN_7)               \
     || ((PIN) == GPIO_PIN_8) || ((PIN) == GPIO_PIN_9) || ((PIN) == GPIO_PIN_10) || ((PIN) == GPIO_PIN_11)             \
     || ((PIN) == GPIO_PIN_12) || ((PIN) == GPIO_PIN_13) || ((PIN) == GPIO_PIN_14) || ((PIN) == GPIO_PIN_15))


#define IS_GPIO_PIN_AVAILABLE(__INSTANCE__,__PIN__)  \
           ((((__INSTANCE__) == GPIOA) && (((__PIN__) & (GPIOA_PIN_AVAILABLE)) != 0) && (((__PIN__) | (GPIOA_PIN_AVAILABLE)) == (GPIOA_PIN_AVAILABLE))) || \
            (((__INSTANCE__) == GPIOB) && (((__PIN__) & (GPIOB_PIN_AVAILABLE)) != 0) && (((__PIN__) | (GPIOB_PIN_AVAILABLE)) == (GPIOB_PIN_AVAILABLE))) || \
            (((__INSTANCE__) == GPIOC) && (((__PIN__) & (GPIOC_PIN_AVAILABLE)) != 0) && (((__PIN__) | (GPIOC_PIN_AVAILABLE)) == (GPIOC_PIN_AVAILABLE))) || \
            (((__INSTANCE__) == GPIOF) && (((__PIN__) & (GPIOF_PIN_AVAILABLE)) != 0) && (((__PIN__) | (GPIOF_PIN_AVAILABLE)) == (GPIOF_PIN_AVAILABLE))))

/** @addtogroup GPIO_mode_define Mode definition
  *        Values convention: 0xW0yz00YZ
  *           - W  : GPIO mode or EXTI Mode
  *           - y  : External IT or Event trigger detection
  *           - z  : IO configuration on External IT or Event
  *           - Y  : Output type (Push Pull or Open Drain)
  *           - Z  : IO Direction mode (Input, Output, Alternate or Analog)
 * @{
 */
#define GPIO_MODE_INPUT                 ((uint32_t)0x00000000U) /*!< Input Floating Mode */
#define GPIO_MODE_OUTPUT_PP             ((uint32_t)0x00000001U) /*!< Output Push Pull Mode */
#define GPIO_MODE_OUTPUT_OD             ((uint32_t)0x00000011U) /*!< Output Open Drain Mode */
#define GPIO_MODE_AF_PP                 ((uint32_t)0x00000002U) /*!< Alternate Function Push Pull Mode */  
#define GPIO_MODE_AF_OD                 ((uint32_t)0x00000012U) /*!< Alternate Function Open Drain Mode */

#define GPIO_MODE_ANALOG                ((uint32_t)0x00000003U)  /*!< Analog Mode */

#define GPIO_MODE_IT_RISING             ((uint32_t)0x10110000U)
#define GPIO_MODE_IT_FALLING            ((uint32_t)0x10210000U)
#define GPIO_MODE_IT_RISING_FALLING     ((uint32_t)0x10310000U)

#define GPIO_MODE_EVT_RISING            ((uint32_t)0x10132000U)
#define GPIO_MODE_EVT_FALLING           ((uint32_t)0x10120000U)
#define GPIO_MODE_EVT_RISING_FALLING    ((uint32_t)0x10320000U)
/**
 * @}
 */
#define IS_GPIO_MODE(__MODE__) (((__MODE__) == GPIO_MODE_INPUT)              ||\
                                ((__MODE__) == GPIO_MODE_OUTPUT_PP)          ||\
                                ((__MODE__) == GPIO_MODE_OUTPUT_OD)          ||\
                                ((__MODE__) == GPIO_MODE_AF_PP)              ||\
                                ((__MODE__) == GPIO_MODE_AF_OD)              ||\
                                ((__MODE__) == GPIO_MODE_IT_RISING)          ||\
                                ((__MODE__) == GPIO_MODE_IT_FALLING)         ||\
                                ((__MODE__) == GPIO_MODE_IT_RISING_FALLING)  ||\
                                ((__MODE__) == GPIO_MODE_EVT_RISING)         ||\
                                ((__MODE__) == GPIO_MODE_EVT_FALLING)        ||\
                                ((__MODE__) == GPIO_MODE_EVT_RISING_FALLING) ||\
                                ((__MODE__) == GPIO_MODE_ANALOG))

/**
 * @addtogroup GPIO_pull_define Pull definition
 * @brief GPIO Pull-Up or Pull-Down Activation
 * @{
 */
#define GPIO_NO_PULL   ((uint32_t)0x00000000U)
#define GPIO_PULL_UP   ((uint32_t)0x00000001U)
#define GPIO_PULL_DOWN ((uint32_t)0x00000002U)
/**
  * @}
  */
#define IS_GPIO_PULL(__PULL__) (((__PULL__) == GPIO_NO_PULL) || ((__PULL__) == GPIO_PULL_UP) || \
                                ((__PULL__) == GPIO_PULL_DOWN))

/**
 * @addtogroup  GPIO_speed_define Speed definition
 * @brief GPIO Output Maximum frequency
 * @{
 */
#define GPIO_SPEED_HIGH   ((uint32_t)0x00000000U)
#define GPIO_SPEED_LOW    ((uint32_t)0x00000001U)
/**
 * @}
 */
#define IS_GPIO_SPEED(_SPEED_)                           \
    (((_SPEED_) == GPIO_SPEED_HIGH) || ((_SPEED_) == GPIO_SPEED_LOW))

/**
 * @addtogroup GPIO_current_define Current definition
 * @brief GPIO Driver Strength Configuration
 * @{
 */
#define GPIO_DC_HIGH ((uint32_t)0x00000000U)
#define GPIO_DC_LOW  ((uint32_t)0x00000001U)
/**
  * @}
  */
#define IS_GPIO_CURRENT(CURRENT)  \
        (((CURRENT) == GPIO_DC_HIGH)||((CURRENT) == GPIO_DC_LOW))

/** @addtogroup GPIO_Port_Sources
 * @{
 */

#define GPIOA_PORT_SOURCE ((uint8_t)0x00)
#define GPIOB_PORT_SOURCE ((uint8_t)0x01)
#define GPIOC_PORT_SOURCE ((uint8_t)0x02)
#define GPIOF_PORT_SOURCE ((uint8_t)0x05)
/**
 * @}
 */
/**
 * @}
 */
#define IS_GPIO_REMAP_PORT_SOURCE(PORTSOURCE)                                                                           \
    (((PORTSOURCE) == GPIOA_PORT_SOURCE) || ((PORTSOURCE) == GPIOB_PORT_SOURCE) || ((PORTSOURCE) == GPIOC_PORT_SOURCE) \
      || ((PORTSOURCE) == GPIOF_PORT_SOURCE)) 
 
 
#define IS_GPIO_EXTI_PORT_SOURCE(PORTSOURCE)                                                                           \
    (((PORTSOURCE) == GPIOA_PORT_SOURCE) || ((PORTSOURCE) == GPIOB_PORT_SOURCE) || ((PORTSOURCE) == GPIOC_PORT_SOURCE) \
      || ((PORTSOURCE) == GPIOF_PORT_SOURCE))

/** @addtogroup GPIO_Pin_sources
 * @{
 */
#define GPIO_PIN_SOURCE0  ((uint8_t)0x00)
#define GPIO_PIN_SOURCE1  ((uint8_t)0x01)
#define GPIO_PIN_SOURCE2  ((uint8_t)0x02)
#define GPIO_PIN_SOURCE3  ((uint8_t)0x03)
#define GPIO_PIN_SOURCE4  ((uint8_t)0x04)
#define GPIO_PIN_SOURCE5  ((uint8_t)0x05)
#define GPIO_PIN_SOURCE6  ((uint8_t)0x06)
#define GPIO_PIN_SOURCE7  ((uint8_t)0x07)
#define GPIO_PIN_SOURCE8  ((uint8_t)0x08)
#define GPIO_PIN_SOURCE9  ((uint8_t)0x09)
#define GPIO_PIN_SOURCE10 ((uint8_t)0x0A)
#define GPIO_PIN_SOURCE11 ((uint8_t)0x0B)
#define GPIO_PIN_SOURCE12 ((uint8_t)0x0C)
#define GPIO_PIN_SOURCE13 ((uint8_t)0x0D)
#define GPIO_PIN_SOURCE14 ((uint8_t)0x0E)
#define GPIO_PIN_SOURCE15 ((uint8_t)0x0F)
/**
 * @}
 */
#define IS_GPIO_PIN_SOURCE(PINSOURCE)                                                                                  \
    (((PINSOURCE) == GPIO_PIN_SOURCE0) || ((PINSOURCE) == GPIO_PIN_SOURCE1) || ((PINSOURCE) == GPIO_PIN_SOURCE2)       \
     || ((PINSOURCE) == GPIO_PIN_SOURCE3) || ((PINSOURCE) == GPIO_PIN_SOURCE4) || ((PINSOURCE) == GPIO_PIN_SOURCE5)    \
     || ((PINSOURCE) == GPIO_PIN_SOURCE6) || ((PINSOURCE) == GPIO_PIN_SOURCE7) || ((PINSOURCE) == GPIO_PIN_SOURCE8)    \
     || ((PINSOURCE) == GPIO_PIN_SOURCE9) || ((PINSOURCE) == GPIO_PIN_SOURCE10) || ((PINSOURCE) == GPIO_PIN_SOURCE11)  \
     || ((PINSOURCE) == GPIO_PIN_SOURCE12) || ((PINSOURCE) == GPIO_PIN_SOURCE13) || ((PINSOURCE) == GPIO_PIN_SOURCE14) \
     || ((PINSOURCE) == GPIO_PIN_SOURCE15))

/** @addtogroup GPIOx_Alternate_function_selection Alternate function selection
 * @{
 */

/*
 * Alternate function AF0
 */
#define GPIO_AF0        ((uint8_t)0x00U)
#define GPIO_AF0_SPI1   (GPIO_AF0)  /* SPI1 Alternate Function mapping */
#define GPIO_AF0_SPI2   (GPIO_AF0)  /* SPI2 Alternate Function mapping */ 
#define GPIO_AF0_I2S    (GPIO_AF0)  /* I2S Alternate Function mapping */
#define GPIO_AF0_I2C2   (GPIO_AF0)  /* I2C2 Alternate Function mapping */
#define GPIO_AF0_SWD    (GPIO_AF0)  /* SWD Alternate Function mapping */
#define GPIO_AF0_OSC    (GPIO_AF0)  /* OSC Alternate Function mapping */ 

/*
 * Alternate function AF1
 */
#define GPIO_AF1          ((uint8_t)0x01U)
#define GPIO_AF1_USART1   (GPIO_AF1)  /* USART1 Alternate Function mapping */
#define GPIO_AF1_USART2   (GPIO_AF1)  /* USART2 Alternate Function mapping */
#define GPIO_AF1_TIM1     (GPIO_AF1)  /* TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM8     (GPIO_AF1)  /* TIM8 Alternate Function mapping */
#define GPIO_AF1_TIM3     (GPIO_AF1)  /* TIM3 Alternate Function mapping */
#define GPIO_AF1_I2C1     (GPIO_AF1)  /* I2C1 Alternate Function mapping */
#define GPIO_AF1_RTC      (GPIO_AF1)  /* RTC Alternate Function mapping */

/*
 * Alternate function AF2
 */
#define GPIO_AF2          ((uint8_t)0x02U)
#define GPIO_AF2_TIM1     (GPIO_AF2)  /* TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM8     (GPIO_AF2)  /* TIM8 Alternate Function mapping */
#define GPIO_AF2_TIM3     (GPIO_AF2)  /* TIM3 Alternate Function mapping */
#define GPIO_AF2_USART1   (GPIO_AF2)  /* USART1 Alternate Function mapping */
#define GPIO_AF2_LPUART   (GPIO_AF2)  /* LPUART Alternate Function mapping */
#define GPIO_AF2_SPI2     (GPIO_AF2)  /* SPI2 Alternate Function mapping */
#define GPIO_AF2_RTC      (GPIO_AF2)  /* RTC Alternate Function mapping */


/*
 * Alternate function AF3
 */
#define GPIO_AF3          ((uint8_t)0x03U)
#define GPIO_AF3_TIM1     (GPIO_AF3)  /* TIM1 Alternate Function mapping */
#define GPIO_AF3_TIM8     (GPIO_AF3)  /* TIM8 Alternate Function mapping */
#define GPIO_AF3_USART2   (GPIO_AF3)  /* USART2 Alternate Function mapping */
#define GPIO_AF3_EVENTOUT (GPIO_AF3)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF3_RTC      (GPIO_AF3)  /* RTC Alternate Function mapping */

/*
 * Alternate function AF4
 */
#define GPIO_AF4          ((uint8_t)0x04U)
#define GPIO_AF4_USART1   (GPIO_AF4)  /* USART1 Alternate Function mapping */ 
#define GPIO_AF4_USART2   (GPIO_AF4)  /* USART2 Alternate Function mapping */
#define GPIO_AF4_LPUART   (GPIO_AF4)  /* LPUART Alternate Function mapping */
#define GPIO_AF4_TIM1     (GPIO_AF4)  /* TIM1 Alternate Function mapping */
#define GPIO_AF4_TIM8     (GPIO_AF4)  /* TIM8 Alternate Function mapping */
#define GPIO_AF4_SPI2     (GPIO_AF4)  /* SPI2 Alternate Function mapping */ 

/*
 * Alternate function AF5
 */
#define GPIO_AF5          ((uint8_t)0x05U)
#define GPIO_AF5_USART2   (GPIO_AF5)  /* USART2 Alternate Function mapping */
#define GPIO_AF5_LPUART   (GPIO_AF5)  /* LPUART Alternate Function mapping */
#define GPIO_AF5_TIM1     (GPIO_AF5)  /* TIM1 Alternate Function mapping */
#define GPIO_AF5_TIM8     (GPIO_AF5)  /* TIM8 Alternate Function mapping */ 
#define GPIO_AF5_TIM3     (GPIO_AF5)  /* TIM3 Alternate Function mapping */
#define GPIO_AF5_SPI1     (GPIO_AF5)  /* SPI1 Alternate Function mapping */
#define GPIO_AF5_MCO      (GPIO_AF5)  /* RCC MCO Alternate Function mapping */ 

/*
 * Alternate function AF6
 */
#define GPIO_AF6          ((uint8_t)0x06U)
#define GPIO_AF6_USART1   (GPIO_AF6)  /* USART1 Alternate Function mapping */ 
#define GPIO_AF6_LPUART   (GPIO_AF6)  /* LPUART Alternate Function mapping */
#define GPIO_AF6_LPTIM    (GPIO_AF6)  /* LPTIM Alternate Function mapping */
#define GPIO_AF6_I2C1     (GPIO_AF6)  /* I2C1 Alternate Function mapping */

/*
 * Alternate function AF7
 */
#define GPIO_AF7          ((uint8_t)0x07U)
#define GPIO_AF7_TIM8     (GPIO_AF7)  /* TIM8 Alternate Function mapping */ 
#define GPIO_AF7_LPTIM    (GPIO_AF7)  /* LPTIM Alternate Function mapping */
#define GPIO_AF7_I2C1     (GPIO_AF7)  /* I2C1 Alternate Function mapping */
#define GPIO_AF7_I2C2     (GPIO_AF7)  /* I2C2 Alternate Function mapping */

/*
 * Alternate function AF8
 */
#define GPIO_AF8          ((uint8_t)0x08U)
#define GPIO_AF8_COMP    (GPIO_AF8)  /* COMP Alternate Function mapping */
#define GPIO_AF8_LPTIM    (GPIO_AF8)  /* LPTIM Alternate Function mapping */
#define GPIO_AF8_SPI2     (GPIO_AF8)  /* SPI2 Alternate Function mapping */
  
/*
 * Alternate function AF9
 */
#define GPIO_AF9          ((uint8_t)0x09U)
#define GPIO_AF9_LPUART   (GPIO_AF9)  /* LPUART Alternate Function mapping */
#define GPIO_AF9_RTC      (GPIO_AF9)  /* RTC Alternate Function mapping */
#define GPIO_AF9_LPTIM    (GPIO_AF9)  /* LPTIM Alternate Function mapping */ 
#define GPIO_AF9_SPI2     (GPIO_AF9)  /* SPI2 Alternate Function mapping */
  
/*
 * Alternate function AF10
 */
#define GPIO_AF10           ((uint8_t)0x0AU)
#define GPIO_AF10_USART2    (GPIO_AF10)  /* USART2 Alternate Function mapping */
#define GPIO_AF10_LPUART    (GPIO_AF10)  /* LPUART Alternate Function mapping */
#define GPIO_AF10_TIM8     (GPIO_AF10)  /* TIM8 Alternate Function mapping */ 
#define GPIO_AF10_TIM3     (GPIO_AF10)  /* TIM3 Alternate Function mapping */
 
/*
 * Alternate function AF11
 */
#define GPIO_AF11         ((uint8_t)0x0BU)
#define GPIO_AF11_LPUART  (GPIO_AF11)  /* LPUART Alternate Function mapping */
#define GPIO_AF11_BEEPER  (GPIO_AF11)  /* BEEPER Alternate Function mapping */
#define GPIO_AF11_RTC     (GPIO_AF11)  /*RTC Alternate Function mapping */

/*
 * Alternate function AF12
 */
#define GPIO_AF12       ((uint8_t)0x0CU)    /* NON Alternate Function mapping */   

/*
 * Alternate function AF13
 */
#define GPIO_AF13           ((uint8_t)0x0DU)
#define GPIO_AF13_TIM8     (GPIO_AF13)  /* TIM8 Alternate Function mapping */ 

/*
 * Alternate function AF15
 */
#define GPIO_AF15   ((uint8_t)0x0FU)  /* NON Alternate Function mapping */

#define GPIO_NO_AF  (GPIO_AF15)
/**
  * @}
  */


/**
 *  IS_GPIO_AF macro definition
 */
#define IS_GPIO_AF(AF)         ((AF) <= (uint8_t)0x0FU) 
/**
 * @}
 */

/** @addtogroup AFIO_Remap_define
 * @{
 */

#define IS_AFIO_REMAP(_RMP_) ((_RMP_) == AFIO_RMP_TIM4CH2)
/**
 * @}
 */

/** @defgroup GPIO Alternate function remaping
 * @{
 */
#define AFIO_SPI1_NSS   (0x00400000UL)
#define AFIO_SPI2_NSS   (0x00800000UL)
#define IS_AFIO_SPIX(_PARAMETER_) \
            (((_PARAMETER_) == AFIO_SPI1_NSS) || ((_PARAMETER_) == AFIO_SPI2_NSS))

#define AFIO_SPI_NSS_High_IMPEDANCE (0x0UL)
#define AFIO_SPI_NSS_High_LEVEL     (0x1UL)
#define IS_AFIO_SPI_NSS(_PARAMETER_) \
            (((_PARAMETER_) == AFIO_SPI_NSS_High_IMPEDANCE) ||((_PARAMETER_) == AFIO_SPI_NSS_High_LEVEL))


typedef enum
{
    AFIO_ADC_ETRI = 0U,
    AFIO_ADC_ETRR = 1U
}AFIO_ADC_ETRType;

typedef enum
{
    AFIO_ADC_TRIG_EXTI_0 = 0U,
    AFIO_ADC_TRIG_EXTI_1 = 1U,
    AFIO_ADC_TRIG_EXTI_2,
    AFIO_ADC_TRIG_EXTI_3,
    AFIO_ADC_TRIG_EXTI_4,
    AFIO_ADC_TRIG_EXTI_5,
    AFIO_ADC_TRIG_EXTI_6,
    AFIO_ADC_TRIG_EXTI_7,
    AFIO_ADC_TRIG_EXTI_8,
    AFIO_ADC_TRIG_EXTI_9,
    AFIO_ADC_TRIG_EXTI_10,
    AFIO_ADC_TRIG_EXTI_11,
    AFIO_ADC_TRIG_EXTI_12,
    AFIO_ADC_TRIG_EXTI_13,
    AFIO_ADC_TRIG_EXTI_14,
    AFIO_ADC_TRIG_EXTI_15,
    AFIO_ADC_TRIG_TIM8_TRGO,
    AFIO_ADC_TRIG_TIM8_CH4
}AFIO_ADC_Trig_RemapType;

#define IS_AFIO_ADC_ETR(_PARAMETER_) \
            (((_PARAMETER_) == AFIO_ADC_ETRI) ||((_PARAMETER_) == AFIO_ADC_ETRR))

#define IS_AFIO_ADC_ETRI(_PARAMETER_) \
            (((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_0) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_1)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_2) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_3)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_4) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_5)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_6) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_7)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_8) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_9)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_10) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_11)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_12) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_13)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_14) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_15)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_TIM8_CH4))

#define IS_AFIO_ADC_ETRR(_PARAMETER_) \
            (((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_0) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_1)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_2) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_3)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_4) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_5)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_6) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_7)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_8) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_9)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_10) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_11)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_12) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_13) ||\
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_14) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_15)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_TIM8_TRGO))

 /**
 * @}
 */

/** @addtogroup GPIO_Exported_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup GPIO_Exported_Functions
 * @{
 */

void GPIO_DeInit(GPIO_Module* GPIOx);
void GPIO_DeInitPin(GPIO_Module* GPIOx, uint32_t GPIO_Pin);
void GPIO_AFIOInitDefault(void);
void GPIO_InitPeripheral(GPIO_Module* GPIOx, GPIO_InitType* GPIO_InitStruct);
void GPIO_InitStruct(GPIO_InitType* GPIO_InitStruct);
uint8_t GPIO_ReadInputDataBit(GPIO_Module* GPIOx, uint16_t Pin);
uint16_t GPIO_ReadInputData(GPIO_Module* GPIOx);
uint8_t GPIO_ReadOutputDataBit(GPIO_Module* GPIOx, uint16_t Pin);
uint16_t GPIO_ReadOutputData(GPIO_Module* GPIOx);
void GPIO_SetBits(GPIO_Module* GPIOx, uint16_t Pin);
void GPIO_ResetBits(GPIO_Module* GPIOx, uint16_t Pin);
void GPIO_WriteBit(GPIO_Module* GPIOx, uint16_t Pin, Bit_OperateType BitCmd);
void GPIO_Write(GPIO_Module* GPIOx, uint16_t PortVal);
void GPIO_TogglePin(GPIO_Module* GPIOx, uint16_t Pin);
void GPIO_ConfigPinLock(GPIO_Module* GPIOx, uint16_t Pin);

void GPIO_ConfigEXTILine(uint8_t PortSource, uint8_t PinSource);
void GPIO_ConfigPinRemap(uint8_t PortSource, uint8_t PinSource, uint32_t AlternateFunction);
void AFIO_ConfigSPINSSMode(uint32_t AFIO_SPIx_NSS, uint32_t SpiNssMode);
void AFIO_ConfigADCExternalTrigRemap(AFIO_ADC_ETRType ADCETRType, AFIO_ADC_Trig_RemapType ADCTrigRemap);

#ifdef __cplusplus
}
#endif

#endif /* __N32G030_GPIO_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
