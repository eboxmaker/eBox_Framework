/*
file   : wdg.h
author : shentq
version: V1.1
date   : 2016/4/6

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __WDG_H
#define __WDG_H
#include "common.h"

/**
 * ��ʼ���������Ź�
   �������㷽��
 * pr:��Ƶ��:0~7(ֻ�е� 3 λ��Ч!)
 * ��Ƶ����=4*2^pr.�����ֵֻ���� 256!
 * rlr:��װ�ؼĴ���ֵ:�� 11 λ��Ч.
 * ʱ�����(���):Tout=((4*2^prer)*rlr)/40 (ms).
   �������ڲ��Ѿ�������ؼ��㣻
    �������Ϊms��1000����1000ms������1s��ιһ�ι������򽫻Ḵλ
 */
class EBOX_IWDG
{
public:
    EBOX_IWDG() {};
    void begin(uint16_t ms);
    void feed();
};

#endif
