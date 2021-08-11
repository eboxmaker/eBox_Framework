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

// ��ȡPin,����ֵ0-15
#define GETPIN(A) 	 	  (uint16_t)(1<<(A&0x0f))
// ��ȡ�˿�����������0,1,2,3,4,5
//#define GETPORTINDEX(A)   ((A)&0xf0)>>4
#define GETPORT(A)   (GPIO_TypeDef*)(((((A)&0xf0))<<6)+AHB2PERIPH_BASE)

// �˺����ᱻ parallel��gpio.cpp���ã������Ƴ�
void port_mode(GPIO_TypeDef* port,uint32_t pin, PinMode_t mode)
{    
#if ENABLE_USESWD
		if(SWD_PORT == port)
		{
			pin = pin & ~SWD_PIN;
		}
//		#error "ע��:��ǰ���ý�ֹ�û�ʹ��SW�˿ڣ�Ĭ��ΪPA13��PA14"
#endif

    LL_GPIO_InitTypeDef GPIO_InitStructure;   
    rcc_clock_cmd((uint32_t)port, ENABLE);

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

    GPIO_InitStructure.Pin = pin;
    GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(port, &GPIO_InitStructure);
}

/**
  *@brief    ���캯��
  *@param    PinId_t pin_id
  *@retval   None
  */
mcuGpio::mcuGpio(PinId_t pin_id){
	id = pin_id;
  _port = GETPORT(id);
  _pin = GETPIN(id);
}
/**
  *@brief    GPIOģʽ����
  *@param    mode:PinMode_tö�ٱ�������
  *@retval   None
  */
void mcuGpio::mode(PinMode_t mode)
{
  port_mode(_port,_pin,mode);
}


/**
  *@brief    GPIOģʽ����
  *@param    mode: PinMode_tö�ٱ������ͣ�af_configration �ڶ�����
  *@retval   None
  */
void mcuGpio::mode(PinMode_t mode, uint8_t af_configration)
{
    mcuGpio::mode(mode);
    if (_pin < LL_GPIO_PIN_8)
        LL_GPIO_SetAFPin_0_7(_port, _pin, af_configration);
    else
        LL_GPIO_SetAFPin_8_15(_port, _pin, af_configration);
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
    (val == 0) ? (_port->BRR = _pin) : (_port->BSRR = _pin);
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
    return ((_port->IDR & _pin) == _pin) ? (1) : (0);
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
int mcuGpio::operator =(mcuGpio &)
{
    return ((_port->IDR & _pin) == _pin) ? (1) : (0);
}
// �൱��read()
mcuGpio::operator int()
{
    return ((_port->IDR & _pin) == _pin) ? (1) : (0);
}
// �൱��write()
mcuGpio mcuGpio::operator= ( int value)
{
    (value == 0) ? (_port->BRR = _pin) : (_port->BSRR = _pin);
    return *this;
}
