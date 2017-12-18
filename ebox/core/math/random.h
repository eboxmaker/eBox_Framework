/**
  ******************************************************************************
  * @file    common.h
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
/**
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *������GPIO��analog��common����Ϲ�ϵ
 *      *������date_time_t����
 *      *ɾ����sys�ģ���������core
 *      *ɾ����parallel_gpio�ģ���������edriver
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __RANDOM_H
#define __RANDOM_H

void            random_seed(unsigned int seed);
unsigned int    random();
unsigned int    random(unsigned int max);
unsigned int    random(unsigned int min, unsigned int max);
#endif
