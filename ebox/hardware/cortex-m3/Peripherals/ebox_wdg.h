/**
  ******************************************************************************
  * @file    wdg.h
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __WDG_H
#define __WDG_H
#include "ebox_core.h"
#include "mcu.h"

/**
 * ��ʼ���������Ź�
   �������㷽��
 * pr:��Ƶ��:0~7(ֻ�е� 3 λ��Ч!)
 * ��Ƶ����=4*2^pr.�����ֵֻ���� 256!
 * rlr:��װ�ؼĴ���ֵ:�� 11 λ��Ч.
 * ʱ�����(���):Tout=((4*2^pr)*rlr)/40 = (256*4095)/40 = 26208(ms).
   �������ڲ��Ѿ�������ؼ��㣻
    �������Ϊms��1000����1000ms������1s��ιһ�ι������򽫻Ḵλ
 */
class Iwdg
{
public:
    Iwdg() {};
  /**
	*@brief    �������Ź�,���ʱ26208ms
	*@param    ms ��ʱʱ������λms
	*@retval   NONE
	*/
    void begin(uint16_t ms);
  /**
	*@brief    ι��,�����Ź�ȥ�26208ʱ������ι�������Ҫ����25s
	*@param    none
	*@retval   NONE
	*/
    void feed();
};

#endif
