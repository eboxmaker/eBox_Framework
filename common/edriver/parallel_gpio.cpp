/**
  ******************************************************************************
  * @file    parallel_gpio.cpp
  * @author  shentq
  * @version V1.2
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

/**
 ************************************************************************
 * @brief 
 Ŀǰ��������Ч�ʱȽϵͣ����ǿ���֧������IO��ϣ�
 * ���ʹ��������GPIO����ʹ�ÿ���ODR�Ĵ����ķ����ӿ��д�ٶȣ�
 * #define DATAOUT  GPIOB->ODR             //PB[0..7]--(LCD)D0~D7
 * DATAOUT &= 0XFF00;
 * DATAOUT |= dat;
 ************************************************************************
 */

#include "parallel_gpio.h"

/**
 *@name     void ParallelGpio::mode(PIN_MODE mode)
 *@brief    Gpio��ģʽ����
 *@param    mode:   PIN_MODEö�ٱ�������
 *@retval   None
*/
void ParallelGpio::mode(PIN_MODE mode)
{
    bit[0]->mode(mode);
    bit[1]->mode(mode);
    bit[2]->mode(mode);
    bit[3]->mode(mode);
    bit[4]->mode(mode);
    bit[5]->mode(mode);
    bit[6]->mode(mode);
    bit[7]->mode(mode);
}

/**
 *@name     void ParallelGpio::write(uint8_t data)
 *@brief    Gpio���������
 *@param    data���������
 *@retval   NONE
*/
void ParallelGpio::write(uint8_t data)
{
    bit[0]->write(data & (0x01 << 0));
    bit[1]->write(data & (0x01 << 1));
    bit[2]->write(data & (0x01 << 2));
    bit[3]->write(data & (0x01 << 3));
    bit[4]->write(data & (0x01 << 4));
    bit[5]->write(data & (0x01 << 5));
    bit[6]->write(data & (0x01 << 6));
    bit[7]->write(data & (0x01 << 7));
}

/**
 *@name     void ParallelGpio::write(uint8_t data)
 *@brief    ��ȡGpio����������
 *@param    NONE
 *@retval   ��ȡGpio�������
*/
uint8_t ParallelGpio::read()
{
    uint8_t data = 0;
    data |= *bit[0] << 0;
    data |= *bit[1] << 1;
    data |= *bit[2] << 2;
    data |= *bit[3] << 3;
    data |= *bit[4] << 4;
    data |= *bit[5] << 5;
    data |= *bit[6] << 6;
    data |= *bit[7] << 7;
    return data;
}

