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

/**
  *@brief    构造函数
  *@param    port port; pin pin
  *@retval   None
  */
mcuGpio::mcuGpio(GPIO_TypeDef *port, uint16_t pin)
{
  uint8_t temp1,temp2;
  _port = port;
  _pin = pin;
  switch ((uint32_t)(port))
  {
  case (uint32_t)GPIOA_BASE:
    temp1 = 0;
    break;
  case (uint32_t)GPIOB_BASE:
    temp1 = 1;
    break;
  case (uint32_t)GPIOC_BASE:
    temp1 = 2;
    break;
  case (uint32_t)GPIOD_BASE:
    temp1 = 3;
    break;
#if !(defined(STM32F030x6)||defined(STM32F031x6))
  case (uint32_t)GPIOE_BASE:
    temp1 = 4;
    break;
#endif
  case (uint32_t)GPIOF_BASE:
    temp1 = 5;
    break;
  default:
    temp1 = 0;
    break;
  }
  for (int i = 0; i <= 15; i ++)
  {
    if ((_pin >> i) == 0)
    {
      temp2 = i -1;
      break;
    }
  }
  id = (PIN_ID_t)(temp1*16 + temp2);
}
/**
  *@brief    GPIO模式设置
  *@param    mode:PIN_MODE枚举变量类型
  *@retval   None
  */
void mcuGpio::mode(PIN_MODE mode)
{
  LL_GPIO_InitTypeDef GPIO_InitStructure;
  
  rcc_clock_cmd((uint32_t)_port,ENABLE);

  switch ((uint8_t)mode)
  {
  /*analog input mode
   */
  case AIN:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_NO;
    break;

  /* digital input mode
   */
  case INPUT:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_NO;
    break;
    
  case INPUT_PD:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_DOWN;
    break;

  case INPUT_PU:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_UP;
    break;

  /*digital output mode
   */
  case OUTPUT_OD:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_NO;
    break;

  case OUTPUT_OD_PU:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_UP;
    break;

  case OUTPUT_OD_PD:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_DOWN;
    break;

  case OUTPUT:
  case OUTPUT_PP:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_NO;
    break;

  case OUTPUT_PP_PU:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_UP;
    break;

  case OUTPUT_PP_PD:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_DOWN;
    break;
  /*af mode
   */
  case AF_OD:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_NO;
    break;

  case AF_OD_PU:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_UP;
    break;

  case AF_OD_PD:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_DOWN;
    break;

  case AF_PP:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_NO;
    break;

  case AF_PP_PU:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_UP;
    break;

  case AF_PP_PD:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_DOWN;
    break;
   /* if parament is other mode,set as INPUT mode
    */
  default:
    GPIO_InitStructure.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_NO;
    break;
  }

  GPIO_InitStructure.Pin = _pin;
  GPIO_InitStructure.Speed = LL_GPIO_SPEED_HIGH;
  LL_GPIO_Init(_port, &GPIO_InitStructure);
}


/**
  *@brief    GPIO模式设置
  *@param    mode: PIN_MODE枚举变量类型，af_configration 第二功能
  *@retval   None
  */
void mcuGpio::mode(PIN_MODE mode,uint8_t af_configration)
{
  mcuGpio::mode(mode);
  if (_pin< LL_GPIO_PIN_8)
    LL_GPIO_SetAFPin_0_7(_port,_pin,af_configration);
  else
    LL_GPIO_SetAFPin_8_15(_port,_pin,af_configration);
}
/**
 *@brief    GPIO输出高电平
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::set()
{
  _port->BSRR = _pin;
}

/**
 *@brief    GPIO输出低电平
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::reset()
{
  _port->BRR = _pin;
}


/**
 *@brief    GPIO输出设置
 *@param    val：1：输出高电平；0：输出低电平
 *@retval   NONE
*/
void mcuGpio::write(uint8_t val)
{
  // val==0,执行_port->BRR = _pin，否则，_port->BSRR = _pin
  (val==0)?(_port->BRR = _pin):(_port->BSRR = _pin);
}


/**
 *@brief    GPIO输出高电平
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::read(uint8_t *val)
{
  *val = _port->IDR & _pin;
}


/**
 *@brief    读取GPIO引脚状态
 *@param    NONE
 *@retval   返回引脚当前的电平状态
*/
uint8_t mcuGpio::read(void)
{
  return ((_port->IDR & _pin) == _pin)?(1):(0);
}

/**
 *@brief    GPIO输出电平翻转
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::toggle()
{
  _port->ODR ^= _pin;
}

// 相当与read()
int mcuGpio::operator =(mcuGpio&)
{
  return ((_port->IDR & _pin) == _pin)?(1):(0);
}
// 相当与read()
mcuGpio::operator int()
{
  return ((_port->IDR & _pin) == _pin)?(1):(0);
}
// 相当与write()
mcuGpio mcuGpio::operator= ( int value)
{
  (value==0)?(_port->BRR = _pin):(_port->BSRR = _pin);
  return *this;
}
