

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

#include "../core/math/random.h"
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
 * @brief   产生一个0~65535的随机数
 * @param   NONE
 * @retval  0~65535随机数结果
 */
unsigned int random()
{
    return rand();
}
/**
 * @brief   产生一个小于某个值得随机数
 * @param   max:随机数的最大值，此值比产生的最大值大1.
 * @retval  随机数结果
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
