/**
  ******************************************************************************
  * @file    gpio.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "ebox_gpio.h"
#include "rcc.h"

 /**
 *@brief    ADC的DMA配置，将ADC1配置为DMA自动传输模式。转换结果自动的传输到内存
 *@param    NONE
 *@retval   NONE
*/
mcuGpio::mcuGpio(GPIO_TypeDef *port, uint16_t pin)
{
    uint8_t temp1,temp2;
    this->port = port;
    this->pin = pin;
    temp1 = (((uint32_t)port - APB2PERIPH_BASE)>>10) - 2;

    for(int i = 0; i <= 15; i ++)
    {
        if((this->pin >> i) == 0)
        {
            temp2 = i -1;
            break;
        }
    }
    this->id = (PIN_ID_t)(temp1*16 + temp2);
}
/**
 *@brief    GPIO模式设置
 *@param    mode:   PIN_MODE枚举变量类型
 *@retval   None
*/
void mcuGpio::mode(PIN_MODE mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    rcc_clock_cmd((uint32_t)port,ENABLE);

    switch((uint8_t)mode)
    {
        /*analog input mode
        */
        case AIN:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;break;
        
        /* digital input mode
        */
        case INPUT:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;break;
        
        case INPUT_PD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;break;
            
        case INPUT_PU:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;break;
        
        /*digital output mode
        */
        case OUTPUT_OD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;break;
            
        case OUTPUT_OD_PU:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;break;
            
        case OUTPUT_OD_PD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;break;
            
        case OUTPUT:
        case OUTPUT_PP:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;break;
        
        case OUTPUT_PP_PU:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;break;
        
        case OUTPUT_PP_PD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;break;
        
        
        /*af mode
        */
        case AF_OD:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;break;
            
        case AF_OD_PU:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;break;
            
        case AF_OD_PD:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;break;
            
        case AF_PP:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;break;
        
        case AF_PP_PU:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;break;
        
        case AF_PP_PD:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;break;
        /* if parament is other mode,set as INPUT mode
        */
       default:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;break;
    }
    GPIO_InitStructure.GPIO_Pin = this->pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(this->port, &GPIO_InitStructure);   //初始化GPIOC端口
}
/**
 *@brief    GPIO输出高电平
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::set()
{
    this->port->BSRR = this->pin;
}

/**
 *@brief    GPIO输出低电平
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::reset()
{
    this->port->BRR = this->pin;
}


/**
 *@brief    GPIO输出设置
 *@param    val：1：输出高电平；0：输出低电平
 *@retval   NONE
*/
void mcuGpio::write(uint8_t val)
{
    if(val == 0)
        this->port->BRR = this->pin;
    else
        this->port->BSRR = this->pin;
}


/**
 *@brief    GPIO输出高电平
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::read(uint8_t *val)
{
    *val = this->port->IDR & this->pin;
}


/**
 *@brief    读取GPIO引脚状态
 *@param    NONE
 *@retval   返回引脚当前的电平状态
*/
uint8_t mcuGpio::read(void)
{
    if(this->port->IDR & this->pin)
        return 1;
    return  0;
}

/**
 *@brief    GPIO输出电平翻转
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::toggle()
{
    port->ODR ^= this->pin;
}

int mcuGpio::operator =(mcuGpio&) 
{
    return read();
}

mcuGpio::operator int()
{
    return read();
}

mcuGpio mcuGpio::operator= ( int value)
{
    write(value);
    return *this;
}



