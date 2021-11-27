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
 * @file n32g030_gpio.c
 * @author Nations 
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g030_gpio.h"

/** @addtogroup N32G030_StdPeriph_Driver
 * @{
 */

/** @addtogroup GPIO
 * @brief GPIO driver modules
 * @{
 */

/** @addtogroup GPIO_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup GPIO_Private_Defines
 * @{
 */
      
#define GPIO_MODE                       ((uint32_t)0x00000003)
#define EXTI_MODE                       ((uint32_t)0x10000000)
#define GPIO_MODE_IT                    ((uint32_t)0x00010000)
#define GPIO_MODE_EVT                   ((uint32_t)0x00020000)
#define RISING_EDGE                     ((uint32_t)0x00100000) 
#define FALLING_EDGE                    ((uint32_t)0x00200000)
#define GPIO_OUTPUT_TYPE                ((uint32_t)0x00000010)

/**
 * @}
 */

/** @addtogroup GPIO_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup GPIO_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup GPIO_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup GPIO_Private_Functions
 * @{
 */

/**
 * @brief  Deinitializes the GPIOx peripheral registers to their default reset values.
 * @param GPIOx where x can be (A,B,C,D,F) to select the GPIO peripheral.
 */
void GPIO_DeInit(GPIO_Module* GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
    if (GPIOx == GPIOA)
    {
        RCC_EnableAPB2PeriphReset(RCC_APB2_PERIPH_GPIOA, ENABLE);
        RCC_EnableAPB2PeriphReset(RCC_APB2_PERIPH_GPIOA, DISABLE);
    }
    else if (GPIOx == GPIOB)
    {
        RCC_EnableAPB2PeriphReset(RCC_APB2_PERIPH_GPIOB, ENABLE);
        RCC_EnableAPB2PeriphReset(RCC_APB2_PERIPH_GPIOB, DISABLE);
    }
    else if (GPIOx == GPIOC)
    {
        RCC_EnableAPB2PeriphReset(RCC_APB2_PERIPH_GPIOC, ENABLE);
        RCC_EnableAPB2PeriphReset(RCC_APB2_PERIPH_GPIOC, DISABLE);
    }
    else if (GPIOx == GPIOF)
    {
        RCC_EnableAPB2PeriphReset(RCC_APB2_PERIPH_GPIOF, ENABLE);
        RCC_EnableAPB2PeriphReset(RCC_APB2_PERIPH_GPIOF, DISABLE);
    }
    else
    {
        return;
    }
}

/**
 * @brief  Deinitializes the GPIOx peripheral registers to their default reset values.
 * @param GPIOx where x can be (A,B,C,D,F) to select the GPIO peripheral.
 * @param GPIO_Pin specifies the port bit to be written.
  *                This parameter can be one of GPIO_PIN_x where x can be (0..15).
  *                All port bits are not necessarily available on all GPIOs.
 */
void GPIO_DeInitPin(GPIO_Module* GPIOx, uint32_t Pin)
{
    uint32_t pos = 0x00U, currentpin = 0x00U;
    uint32_t tmpregister = 0x00U; 

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN_AVAILABLE(GPIOx, Pin));

    while((Pin >> pos) != 0)
    {
        /* Get the IO position */
        currentpin = (Pin) & (1U << pos);

        if(currentpin)
        {
            /*------------------------- GPIO Mode Configuration --------------------*/
            /* Configure IO Direction in Analog Mode */
            GPIOx->PMODE |= (GPIO_PMODE0 << (pos * 2U));

            /* Configure the default Alternate Function in current IO */ 
            if(pos & 0x08)
                GPIOx->AFH |= ((0x0FUL) << ((pos & 0x07U) * 4U));
            else
                GPIOx->AFL |= ((0x0FUL) << ((pos & 0x07U) * 4U));

            /* Configure the default value IO Output Type */
            GPIOx->POTYPE &= ~(GPIO_POTYPE_POT_0 << pos);

            /* Deactivate the Pull-up and Pull-down resistor for the current IO */
            GPIOx->PUPD &= ~(GPIO_PUPD0 << (pos * 2U));

            /* Configure the default value for IO Speed */
            GPIOx->SR |= (GPIO_SR_SR0 << pos);

            /* Configure the default value for IO Speed */
            GPIOx->DS &= ~(GPIO_DS_DS0 << pos);

            /*------------------------- EXTI Mode Configuration --------------------*/
            /* Clear the External Interrupt or Event for the current IO */
            tmpregister = AFIO->EXTI_CFG[pos >> 2U];
            tmpregister &= ((0x0FUL) << ((pos & 0x03U) * 4U));
            if(tmpregister == (GPIO_GET_INDEX(GPIOx) << ((pos & 0x03U) * 4U)))
            {
                AFIO->EXTI_CFG[pos >> 2U] &= ~((0x0FUL) << ((pos & 0x03U) * 4U));

                /* Clear EXTI line configuration */
                EXTI->IMASK &= ~((uint32_t)currentpin);
                EXTI->EMASK &= ~((uint32_t)currentpin);

                /* Clear Rising Falling edge configuration */
                EXTI->RT_CFG &= ~((uint32_t)currentpin);
                EXTI->FT_CFG &= ~((uint32_t)currentpin);
            }
        } 
        pos++;
    }

}

/**
 * @brief  Deinitializes the Alternate Functions (remap, event control
 *   and EXTI configuration) registers to their default reset values.
 */
void GPIO_AFIOInitDefault(void)
{
    RCC_EnableAPB2PeriphReset(RCC_APB2_PERIPH_AFIO, ENABLE);
    RCC_EnableAPB2PeriphReset(RCC_APB2_PERIPH_AFIO, DISABLE);
}

/**
 * @brief  Initializes the GPIOx peripheral according to the specified
 *         parameters in the GPIO_InitStruct.
 * @param GPIOx where x can be (A,B,C,D,F) to select the GPIO peripheral.
 * @param GPIO_InitStruct pointer to a GPIO_InitType structure that
 *         contains the configuration information for the specified GPIO peripheral.
 */
 
void GPIO_InitPeripheral(GPIO_Module* GPIOx, GPIO_InitType * GPIO_InitStruct)
{
    uint32_t pos = 0x00U, currentpin = 0x00U;
    uint32_t tmpregister = 0x00U; 

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN_AVAILABLE(GPIOx, GPIO_InitStruct->Pin));
    // assert_param(IS_GPIO_AF(GPIO_InitStruct->GPIO_Alternate)); 
    assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
    assert_param(IS_GPIO_PULL(GPIO_InitStruct->GPIO_Pull));
    assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
    assert_param(IS_GPIO_CURRENT(GPIO_InitStruct->GPIO_Current));

    while(((GPIO_InitStruct->Pin) >> pos) != 0)
    {
        /* Get the IO position */
        currentpin = (GPIO_InitStruct->Pin) & (1U << pos);

        if(currentpin)
        {
            /*--------------------- GPIO Mode Configuration ------------------------*/
            /* In case of Alternate function mode selection */
            if((GPIO_InitStruct->GPIO_Mode == GPIO_MODE_AF_PP) || (GPIO_InitStruct->GPIO_Mode == GPIO_MODE_AF_OD))
            {
                /* Check the Alternate function parameters */
                assert_param(IS_GPIO_AF(GPIO_InitStruct->GPIO_Alternate));

                /* Configure Alternate function mapped with the current IO */
                if(pos & 0x08)
                {
                    tmpregister = GPIOx->AFH;
                    tmpregister &= ~((uint32_t)0xF << ((uint32_t)(pos & (uint32_t)0x07) * 4U));
                    tmpregister |= ((uint32_t)(GPIO_InitStruct->GPIO_Alternate) << ((uint32_t)(pos & (uint32_t)0x07) * 4U)) ;
                    GPIOx->AFH = tmpregister;
                }
                else
                {
                    tmpregister = GPIOx->AFL;
                    tmpregister &= ~((uint32_t)0xF << ((uint32_t)(pos & (uint32_t)0x07) * 4U)) ;
                    tmpregister |= ((uint32_t)(GPIO_InitStruct->GPIO_Alternate) << ((uint32_t)(pos & (uint32_t)0x07) * 4U)) ;
                    GPIOx->AFL = tmpregister;
                }
            }

            /* In case of Output or Alternate function mode selection */
            if ((GPIO_InitStruct->GPIO_Mode == GPIO_MODE_OUTPUT_PP) || (GPIO_InitStruct->GPIO_Mode == GPIO_MODE_OUTPUT_OD)
                 ||(GPIO_InitStruct->GPIO_Mode == GPIO_MODE_AF_PP) || (GPIO_InitStruct->GPIO_Mode == GPIO_MODE_AF_OD))
            {
                /* Configure the IO Output Type */
                tmpregister = GPIOx->POTYPE;
                tmpregister &= ~(GPIO_POTYPE_POT_0 << pos);
                tmpregister |= (((GPIO_InitStruct->GPIO_Mode >> 4U) & 0x01U) << pos);
                GPIOx->POTYPE = tmpregister;
            }

            /*---------------------------- GPIO Mode Configuration -----------------------*/
            /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
            tmpregister = GPIOx->PMODE;
            tmpregister &= ~(GPIO_PMODE0 << (pos * 2U));
            tmpregister |= (((GPIO_InitStruct->GPIO_Mode & 0x03U) << (pos * 2U)));
            GPIOx->PMODE = tmpregister;

            /* Configure pull-down mode */
            tmpregister = GPIOx->PUPD;
            tmpregister &= ~(GPIO_PUPD0 << (pos * 2U));
            tmpregister |= (GPIO_InitStruct->GPIO_Pull << (pos * 2U));
            GPIOx->PUPD = tmpregister;

            /* Configure slew rate */
            tmpregister = GPIOx->SR;
            tmpregister &= ~(GPIO_SR_SR0 << pos);
            tmpregister |= (GPIO_InitStruct->GPIO_Speed << pos);
            GPIOx->SR = tmpregister;

            /* Configure driver current */
            tmpregister = GPIOx->DS;
            tmpregister &= ~(GPIO_DS_DS0 << pos);
            tmpregister |= (GPIO_InitStruct->GPIO_Current << pos);
            GPIOx->DS = tmpregister;

            /*--------------------- EXTI Mode Configuration ------------------------*/
            /* Configure the External Interrupt or event for the current IO */
            if((GPIO_InitStruct->GPIO_Mode & EXTI_MODE) == EXTI_MODE) 
            {
                tmpregister = AFIO->EXTI_CFG[pos >> 2U];
                tmpregister &= ~(0x0FUL << ((pos & 0x03U) * 4U));
                tmpregister |= (GPIO_GET_INDEX(GPIOx) << ((pos & 0x03U) * 4U));
                AFIO->EXTI_CFG[pos >> 2U] = tmpregister;

                /* Clear EXTI line configuration */
                tmpregister = EXTI->IMASK;
                tmpregister &= ~((uint32_t)currentpin);
                if((GPIO_InitStruct->GPIO_Mode & GPIO_MODE_IT) == GPIO_MODE_IT)
                {
                    tmpregister |= currentpin;
                }
                EXTI->IMASK = tmpregister;

                tmpregister = EXTI->EMASK;
                tmpregister &= ~((uint32_t)currentpin);
                if((GPIO_InitStruct->GPIO_Mode & GPIO_MODE_EVT) == GPIO_MODE_EVT)
                {
                    tmpregister |= currentpin;
                }
                EXTI->EMASK = tmpregister;

                /* Clear Rising Falling edge configuration */
                tmpregister = EXTI->RT_CFG;
                tmpregister &= ~((uint32_t)currentpin);
                if((GPIO_InitStruct->GPIO_Mode & RISING_EDGE) == RISING_EDGE)
                {
                    tmpregister |= currentpin;
                }
                EXTI->RT_CFG = tmpregister;

                tmpregister = EXTI->FT_CFG;
                tmpregister &= ~((uint32_t)currentpin);
                if((GPIO_InitStruct->GPIO_Mode & FALLING_EDGE) == FALLING_EDGE)
                {
                    tmpregister |= currentpin;
                }
                EXTI->FT_CFG = tmpregister;
            }
        }
        pos++;      
    }
}

/**
 * @brief  Fills each GPIO_InitStruct member with its default value.
 * @param GPIO_InitStruct pointer to a GPIO_InitType structure which will
 *         be initialized.
 */
void GPIO_InitStruct(GPIO_InitType* GPIO_InitStruct)
{
    /* Reset GPIO init structure parameters values */
    GPIO_InitStruct->Pin            = GPIO_PIN_ALL;
    GPIO_InitStruct->GPIO_Alternate = GPIO_NO_AF;
    GPIO_InitStruct->GPIO_Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct->GPIO_Pull      = GPIO_NO_PULL;
    GPIO_InitStruct->GPIO_Speed     = GPIO_SPEED_LOW;
    GPIO_InitStruct->GPIO_Current   = GPIO_DC_HIGH;
}

/**
 * @brief  Reads the specified input port pin.
 * @param GPIOx where x can be (A,B,C,D,F) to select the GPIO peripheral.
 * @param Pin specifies the port bit to read.
 *   This parameter can be GPIO_Pin_x where x can be (0..15).
 * @return The input port pin value.
 */
uint8_t GPIO_ReadInputDataBit(GPIO_Module* GPIOx, uint16_t Pin)
{
    uint8_t bitstatus = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN_AVAILABLE(GPIOx, Pin));

    if ((GPIOx->PID & Pin) != (uint32_t)Bit_RESET)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}

/**
 * @brief  Reads the specified GPIO input data port.
 * @param GPIOx where x can be (A,B,C,D,F) to select the GPIO peripheral.
 * @return GPIO input data port value.
 */
uint16_t GPIO_ReadInputData(GPIO_Module* GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    return ((uint16_t)GPIOx->PID);
}

/**
 * @brief  Reads the specified output data port bit.
 * @param GPIOx where x can be (A,B,C,D,F) to select the GPIO peripheral.
 * @param Pin specifies the port bit to read.
 *   This parameter can be GPIO_Pin_x where x can be (0..15).
 * @return The output port pin value.
 */
uint8_t GPIO_ReadOutputDataBit(GPIO_Module* GPIOx, uint16_t Pin)
{
    uint8_t bitstatus = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN_AVAILABLE(GPIOx, Pin));

    if ((GPIOx->POD & Pin) != (uint32_t)Bit_RESET)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}

/**
 * @brief  Reads the specified GPIO output data port.
 * @param GPIOx where x can be (A,B,C,D,F) to select the GPIO peripheral.
 * @return GPIO output data port value.
 */
uint16_t GPIO_ReadOutputData(GPIO_Module* GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    return ((uint16_t)GPIOx->POD);
}

/**
 * @brief  Sets the selected data port bits.
 * @param GPIOx where x can be (A,B,C,D,F) to select the GPIO peripheral.
 * @param Pin specifies the port bits to be written.
 *   This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 */
void GPIO_SetBits(GPIO_Module* GPIOx, uint16_t Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN_AVAILABLE(GPIOx, Pin));

    GPIOx->PBSC = Pin;
}

/**
 * @brief  Clears the selected data port bits.
 * @param GPIOx where x can be (A,B,C,D,F) to select the GPIO peripheral.
 * @param Pin specifies the port bits to be written.
 *   This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 */
void GPIO_ResetBits(GPIO_Module* GPIOx, uint16_t Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN_AVAILABLE(GPIOx, Pin));

    GPIOx->PBC = Pin;
}

/**
 * @brief  Sets or clears the selected data port bit.
 * @param GPIOx where x can be (A,B,C,D,F) to select the GPIO peripheral.
 * @param Pin specifies the port bit to be written.
 *   This parameter can be one of GPIO_Pin_x where x can be (0..15).
 * @param BitCmd specifies the value to be written to the selected bit.
 *   This parameter can be one of the Bit_OperateType enum values:
 *     @arg Bit_RESET to clear the port pin
 *     @arg Bit_SET to set the port pin
 */
void GPIO_WriteBit(GPIO_Module* GPIOx, uint16_t Pin, Bit_OperateType BitCmd)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN_AVAILABLE(GPIOx, Pin));
    assert_param(IS_GPIO_BIT_OPERATE(BitCmd));

    if (BitCmd != Bit_RESET)
    {
        GPIOx->PBSC = Pin;
    }
    else
    {
        GPIOx->PBC = Pin;
    }
}

/**
 * @brief  Writes data to the specified GPIO data port.
 * @param GPIOx where x can be (A,B,C,D,F) to select the GPIO peripheral.
 * @param PortVal specifies the value to be written to the port output data register.
 */
void GPIO_Write(GPIO_Module* GPIOx, uint16_t PortVal)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    GPIOx->POD = PortVal;
}

/**
  * @brief  Toggles the specified GPIO pins.
  * @param GPIOx Where x can be (A,B,C,D,F) to select the GPIO peripheral.
  *              All port bits are not necessarily available on all GPIOs.
  * @param GPIO_Pin Specifies the pins to be toggled.
  * @retval None
  */
void GPIO_TogglePin(GPIO_Module *GPIOx, uint16_t Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_PIN_AVAILABLE(GPIOx, Pin));

    GPIOx->POD ^= Pin;
}

/**
 * @brief  Locks GPIO Pins configuration registers.
 * @param GPIOx where x can be (A,B,C,D,F) to select the GPIO peripheral.
 * @param Pin specifies the port bit to be written.
 *   This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 */
void GPIO_ConfigPinLock(GPIO_Module* GPIOx, uint16_t Pin)
{
    uint32_t tmp = 0x00010000;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN_AVAILABLE(GPIOx, Pin));

    tmp |= Pin;
    /* Set LCKK bit */
    GPIOx->PLOCK = tmp;
    /* Reset LCKK bit */
    GPIOx->PLOCK = Pin;
    /* Set LCKK bit */
    GPIOx->PLOCK = tmp;
    /* Read LCKK bit*/
    tmp = GPIOx->PLOCK;
    /* Read LCKK bit*/
    tmp = GPIOx->PLOCK;
}

/**
 * @brief  Selects the GPIO pin used as EXTI Line.
 * @param PortSource selects the GPIO port to be used as source for EXTI lines.
 *   This parameter can be GPIO_PortSourceGPIOx where x can be (A..D).
 * @param PinSource specifies the EXTI line to be configured.
 *   This parameter can be GPIO_PinSourcex where x can be (0..15).
 */
void GPIO_ConfigEXTILine(uint8_t PortSource, uint8_t PinSource)
{
    uint32_t port = (uint32_t)PortSource;

    /* Check the parameters */
    assert_param(IS_GPIO_EXTI_PORT_SOURCE(PortSource));
    assert_param(IS_GPIO_PIN_SOURCE(PinSource));

    AFIO->EXTI_CFG[(PinSource >> 0x02)] &= ~(0x0FUL << ((PinSource & 0x03U) * 4U));
    AFIO->EXTI_CFG[(PinSource >> 0x02)] |= (port << ((PinSource & 0x03U) * 4U));
}

/**
 * @brief  Changes the mapping of the specified pin.
 * @param PortSource selects the GPIO port to be used.
 * @param PinSource specifies the pin for the remaping.
 *   This parameter can be GPIO_PinSourcex where x can be (0..15).
 * @param AlternateFunction specifies the alternate function for the remaping.
 */
void GPIO_ConfigPinRemap(uint8_t PortSource, uint8_t PinSource, uint32_t AlternateFunction)
{
    uint32_t tmp = 0x00, tmpregister = 0x00;
    GPIO_Module *GPIOx;
    /* Check the parameters */
    assert_param(IS_GPIO_REMAP_PORT_SOURCE(PortSource));
    assert_param(IS_GPIO_PIN_SOURCE(PinSource));
    assert_param(IS_GPIO_AF(AlternateFunction));
    /*Get Peripheral point*/
    GPIOx = GPIO_GET_PERIPH(PortSource);
    /**/
     if(PinSource & (uint8_t)0x08)
    {
        tmp = (uint32_t)(PinSource & (uint8_t)0x07);
        /*Read GPIO_AFH register*/
        tmpregister  = GPIOx->AFH;
        /*Reset corresponding bits*/
        tmpregister &=~((uint32_t)0x0F <<(tmp*4U));
        /*Set corresponding bits*/
        tmpregister |= ((uint32_t)(AlternateFunction) << (tmp * 4U));
        /*Write to the GPIO_AFH register*/
        GPIOx->AFH = tmpregister;
    }
    else
    {
        tmp = (uint32_t)(PinSource & (uint8_t)0x07);
        /*Read GPIO_AFL register*/
        tmpregister  = GPIOx->AFL;
        /*Reset corresponding bits*/
        tmpregister &=~((uint32_t)0x0F <<(tmp*4U));
        /*Set corresponding bits*/
        tmpregister |= ((uint32_t)(AlternateFunction) << (tmp * 4U));
        /*Write to the GPIO_AFL register*/
        GPIOx->AFL = tmpregister;
    }
}


/**
 * @brief  Selects the alternate function SPIx NSS mode.
 * @param AFIO_SPIx_NSS choose which SPI configuration.
 *   This parameter can be AFIO_SPI1_NSS, AFIO_SPI2_NSS.
 * @param SpiNssMode specifies the SPI_NSS mode to be configured.
 *   This parameter can be AFIO_SPI_NSS_High_IMPEDANCE and AFIO_SPI_NSS_High_LEVEL.
 */
void AFIO_ConfigSPINSSMode(uint32_t AFIO_SPIx_NSS, uint32_t SpiNssMode)
{
    uint32_t tmpregister = 0x00U;

    /* Check the parameters */
    assert_param(IS_AFIO_SPIX(AFIO_SPIx_NSS));
    assert_param(IS_AFIO_SPI_NSS(SpiNssMode));

    tmpregister = AFIO->CFG;
    if(SpiNssMode != AFIO_SPI_NSS_High_IMPEDANCE)
    {
        tmpregister |= AFIO_SPIx_NSS;
    }
    else 
    {
        tmpregister &= ~AFIO_SPIx_NSS;
    }
    AFIO->CFG = tmpregister;
}

/**
 * @brief  Configur ADC external trigger.
 * @param ADCETRType choose whether to configure rule conversion or injection conversion .
 *   This parameter can be AFIO_ADC_ETRI and AFIO_ADC_ETRR.
 * @param ADCTrigRemap specifies the external trigger line be configured.
 *   This parameter can be AFIO_ADC_TRIG_EXTI_x where x can be (0..15) or AFIO_ADC_TRIG_TIM8_CHy where y can be(3..4).
 */
void AFIO_ConfigADCExternalTrigRemap(AFIO_ADC_ETRType ADCETRType, AFIO_ADC_Trig_RemapType ADCTrigRemap)
{
    uint32_t tmpregister = 0x00U;

    /* Check the parameters */
    assert_param(IS_AFIO_ADC_ETR(ADCETRType));

    if(ADCETRType == AFIO_ADC_ETRI)
    {
        /* Check the parameters */
        assert_param(IS_AFIO_ADC_ETRI(ADCTrigRemap));

        tmpregister = AFIO->CFG;

        /* clear AFIO_CFG register ETRI bit*/
        tmpregister &= ~AFIO_CFG_ADC_ETRI;
        /* if ADCETRType is AFIO_ADC_ETRI then ADCTrigRemap cannot be AFIO_ADC_TRIG_TIM8_CH3*/
        if(ADCTrigRemap == AFIO_ADC_TRIG_TIM8_CH4)
        {
            /* select TIM8_CH4 line to connect*/
            tmpregister |= AFIO_CFG_ADC_ETRI;
        }
        else
        {
            /* clear AFIO_CFG register EXTI_ETRI bit*/
            tmpregister &= ~AFIO_CFG_EXTI_ETRI;
            /* select which external line is connected*/
            tmpregister |= (ADCTrigRemap << 15U);
        }
        AFIO->CFG = tmpregister;
    }
    else
    {
        if(ADCETRType == AFIO_ADC_ETRR)
        {
            /* Check the parameters */
            assert_param(IS_AFIO_ADC_ETRR(ADCTrigRemap));

            tmpregister = AFIO->CFG;
            /* clear AFIO_CFG register ETRR bit*/
            tmpregister &= ~AFIO_CFG_ADC_ETRR;
            /* if ADCETRType is AFIO_ADC_ETRR then ADCTrigRemap cannot be AFIO_ADC_TRIG_TIM8_CH4*/
            if(ADCTrigRemap == AFIO_ADC_TRIG_TIM8_TRGO)
            {
                /* select TIM8_CH3 line to connect*/
                tmpregister |= AFIO_CFG_ADC_ETRR;
            }
            else
            {
                /* clear AFIO_CFG register EXTI_ETRR bit*/
                tmpregister &= ~AFIO_CFG_EXTI_ETRR;
                /* select which external line is connected*/
                tmpregister |= (ADCTrigRemap << 11U);
            }
            AFIO->CFG = tmpregister;
        }
    }
}

/**
 * @}
 */

/**
 * @}
 */
