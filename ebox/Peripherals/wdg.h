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
        EBOX_IWDG(){};
        void begin(uint16_t ms);
        void feed();

};

#endif
