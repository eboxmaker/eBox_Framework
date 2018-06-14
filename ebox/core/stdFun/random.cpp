

/**
  ******************************************************************************
  * @file    common.cpp
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

#include "random.h"
#include "stdlib.h"

/**
 * @brief   
 * 
 * @param   
 *          
 *          
 *          
 * @retval  NONE
 */
void random_seed(unsigned int seed)
{
    srand(seed);
}

/**
 * @brief   ����һ��0~65535�������
 * @param   NONE
 * @retval  0~65535��������
 */
unsigned int random()
{
    return rand();
}
/**
 * @brief   ����һ��С��ĳ��ֵ�������
 * @param   max:����������ֵ����ֵ�Ȳ��������ֵ��1.
 * @retval  ��������
 */
unsigned int random(unsigned int max)
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
unsigned int random(unsigned int min, unsigned int max)
{
    return (min + rand() % (max - min));
}

