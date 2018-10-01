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
  *@brief    ���캯��
  *@param    port port; pin pin
  *@retval   None
  */
mcuGpio::mcuGpio(GPIO_TypeDef *port, uint16_t pin)
{
  uint8_t temp1,temp2;
  _port = port;
  _pin = pin;
  temp1 = (((uint32_t)port - APB2PERIPH_BASE)>>10) - 2;

  for (int i = 0; i <= 15; i ++)
  {
    if (((_pin >> i) & 0xfffe) == 0)
    {
      temp2 = i ;
      break;
    }
  }
  this->id = (PIN_ID_t)(temp1*16 + temp2);
}
/**
  *@brief    GPIOģʽ����
  *@param    mode:PIN_MODEö�ٱ�������
  *@retval   None
  */
void mcuGpio::mode(PIN_MODE mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  rcc_clock_cmd((uint32_t)_port,ENABLE);

  switch ((uint8_t)mode)
  {
  /*analog input mode
   */
  case AIN:
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    break;

  /* digital input mode
   */
  case INPUT:
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    break;

  case INPUT_PD:
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    break;

  case INPUT_PU:
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    break;

  /*digital output mode
   */
  case OUTPUT_OD:
  case OUTPUT_OD_PU:
  case OUTPUT_OD_PD:
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    break;

  case OUTPUT:
  case OUTPUT_PP:
  case OUTPUT_PP_PU:
  case OUTPUT_PP_PD:
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    break;
    
  /*af mode 
   */
  case AF_OD:
  case AF_OD_PU:
  case AF_OD_PD:
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    break;

  case AF_PP:
  case AF_PP_PU:
  case AF_PP_PD:
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    break;
  /* if parament is other mode,set as INPUT mode
   */
  default:
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    break;
  }
  GPIO_InitStructure.GPIO_Pin = _pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(_port, &GPIO_InitStructure);   //��ʼ��GPIOC�˿�
}
/**
  *@brief    GPIOģʽ����,f1����af_configration����
  *@param    mode: PIN_MODEö�ٱ������ͣ�af_configration �ڶ�����
  *@retval   None
  */
void mcuGpio::mode(PIN_MODE mode,uint8_t af_configration)
{
  mcuGpio::mode(mode);
}
/**
 *@brief    GPIO����ߵ�ƽ
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::set()
{
  _port->BSRR = _pin;
}

/**
 *@brief    GPIO����͵�ƽ
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::reset()
{
  _port->BRR = _pin;
}


/**
 *@brief    GPIO�������
 *@param    val��1������ߵ�ƽ��0������͵�ƽ
 *@retval   NONE
*/
void mcuGpio::write(uint8_t val)
{
  // val==0,ִ��_port->BRR = _pin������_port->BSRR = _pin
  (val==0)?(_port->BRR = _pin):(_port->BSRR = _pin);
}


/**
 *@brief    GPIO����ߵ�ƽ
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::read(uint8_t *val)
{
  *val = _port->IDR & _pin;
}


/**
 *@brief    ��ȡGPIO����״̬
 *@param    NONE
 *@retval   �������ŵ�ǰ�ĵ�ƽ״̬
*/
uint8_t mcuGpio::read(void)
{
  return ((_port->IDR & _pin) == _pin)?(1):(0);
}

/**
 *@brief    GPIO�����ƽ��ת
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::toggle()
{
  _port->ODR ^= _pin;
}

// �൱��read()
int mcuGpio::operator =(mcuGpio&)
{
  return ((_port->IDR & _pin) == _pin)?(1):(0);
}
// �൱��read()
mcuGpio::operator int()
{
  return ((_port->IDR & _pin) == _pin)?(1):(0);
}
// �൱��write()
mcuGpio mcuGpio::operator= ( int value)
{
  (value==0)?(_port->BRR = _pin):(_port->BSRR = _pin);
  return *this;
}
