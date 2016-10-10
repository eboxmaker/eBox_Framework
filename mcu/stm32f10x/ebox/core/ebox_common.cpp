
/**
  ******************************************************************************
  * @file    common.cpp
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
#include "ebox_common.h"
#include "stdlib.h"

/** @defgroup common 
  * @brief common
  * @{
  */
extern uint16_t AD_value[];


/** @defgroup common����
  * @{
  */


/**
 * @brief   
 * 
 * @param   
 *          
 *          
 *          
 * @retval  NONE
 */
void random_seed(uint16_t seed)
{
    srand(seed);
}

/**
 * @brief   ����һ��0~65535�������
 * @param   NONE
 * @retval  0~65535��������
 */
uint16_t random()
{
    return rand();
}
/**
 * @brief   ����һ��С��ĳ��ֵ�������
 * @param   max:����������ֵ����ֵ�Ȳ��������ֵ��1.
 * @retval  ��������
 */
uint16_t random(uint16_t max)
{
    return (rand() % max);
}

/**
 * @name    
 * @brief   
 * 
 * @param   
 *          
 *          
 *          
 * @retval  NONE
 */
uint16_t random(uint16_t min, uint16_t max)
{
    return (min + rand() % (max - min));
}
/**
 *@brief    Gpio����ʱ���������һ��u8���͵����ݣ����ڴ���ͨ�š�
 *@param    data_pin��  ������������
            clock_pin:  ʱ���źŵ�����
            bit_order:  ���ݴ�С�˿���LSB_FIRST����λ�ȷ��ͣ�MSB_FIRST��λ�ȷ���
 *@retval   ���յ�������
*/
uint8_t shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order)
{
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i)
    {
        clock_pin->write(HIGH);
        if (bit_order == LSB_FIRST)
            value |= data_pin->read() << i;
        else
            value |= data_pin->read() << (7 - i);
        clock_pin->write(LOW);
    }
    return value;
}


/**
 * @brief   Gpio����ʱ���������һ��u8���͵����ݣ����ڴ���ͨ�š�
 * @param   data_pin��  �����������
 *          clock_pin:  ʱ���źŵ�����
 *          bit_order:  ���ݴ�С�˿���LSB_FIRST����λ�ȷ��ͣ�MSB_FIRST��λ�ȷ���
 *          val��       Ҫ���͵�����
 * @retval  NONE
 */
void shift_out(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order, uint8_t val)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        if (bit_order == LSB_FIRST)
            data_pin->write(!!(val & (1 << i)));
        else
            data_pin->write(!!(val & (1 << (7 - i))));

        clock_pin->write(HIGH);
        clock_pin->write(LOW);

    }
}

/**
  * @}
  */
/**
  * @}
  */
