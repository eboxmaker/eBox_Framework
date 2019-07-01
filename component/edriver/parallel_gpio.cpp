/**
  ******************************************************************************
  * @file    parallel_gpio.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
  *     2019/6/10  cat_li  修改实现，使之可以操作任意IO，同一个端口连续IO，同一个
  *       类 Gpios 可以操作最多16个任意IO组合
  *       类 Port 可以操作同一个端口的连续IO，不连续IO
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
#include "parallel_gpio.h"
#include "ebox_core.h"

Gpios::Gpios(Gpio **pin,uint8_t num)
{
    _num = num;
    for (uint8_t i=0;i<_num;i++)
    {
        _bit[i] = pin[i];
    }
}

/**
*@brief    Gpio组模式设置
*@param    mode:   PIN_MODE枚举变量类型
*@retval   None
*/
void Gpios::mode(PinMode_t mode)
{

    for (uint8_t i=0;i<_num;i++)
    {
        (_bit[i])->mode(mode);
    }
}

/**
*@name     void ParallelGpio::write(uint8_t data)
*@brief    Gpio组输出数据
*@param    data：输出数据
*@retval   NONE
*/
void Gpios::write(uint16_t val)
{
    for (uint8_t i=0;i<_num;i++)
    {
        _bit[i]->write(val & (0x01 << i));
    }
}

/**
*@name     void ParallelGpio::write(uint8_t data)
*@brief    读取Gpio组输入数据
*@param    NONE
*@retval   读取Gpio组的数据
*/
void Gpios::read(uint16_t *val)
{
    *val = 0;
    for (uint8_t i=0;i<_num;i++)
    {
        *val |= _bit[i]->read() << i;
    }
}

uint16_t Gpios::read(void)
{
    uint16_t r;
    read(&r);
    return r;
}

//-----------------------------------------------------------------------------------//

Port::Port(uint32_t port, uint8_t pinnum, uint8_t pinoffset)
{
    _mask = (0xffff>>(16-pinnum))<<pinoffset;
    _offset = pinoffset;
    _port = (GPIO_TypeDef *)port;
}
// 该函数实现在ebox_gpio.cpp中，不同平台实现有差异
extern void port_mode(GPIO_TypeDef* port,uint32_t pin, PinMode_t mode);
void Port::mode(PinMode_t mode)
{
    port_mode(_port,_mask,mode);
}

void Port::write(uint16_t val)
{
    //LL_GPIO_WriteOutputPort(_port,_mask&val);
    //	_offset == 0 ? _port->ODR = (_port->ODR & ~_mask) | (val & _mask) : _port->ODR = (_port->ODR & ~_mask) | ((val<<_offset) & _mask);
    _port->ODR = (_port->ODR & ~_mask) | ((val<<_offset) & _mask);
}

uint16_t Port::read(void)
{
    //	return _offset== 0 ? _port->IDR & _mask:((_port->IDR & _mask) >> _offset);
    return (_port->IDR & _mask) >> _offset;
}

void Port::read(uint16_t *val)
{
    *val = (_port->IDR & _mask) >> _offset;
}
