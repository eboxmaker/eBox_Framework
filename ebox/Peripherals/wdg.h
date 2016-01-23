#ifndef __WDG_H
#define __WDG_H
#include "common.h"

/**
 * 初始化独立看门狗
   基本计算方法
 * pr:分频数:0~7(只有低 3 位有效!)
 * 分频因子=4*2^pr.但最大值只能是 256!
 * rlr:重装载寄存器值:低 11 位有效.
 * 时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
   本函数内部已经做了相关计算；
    输入参数为ms；1000代表1000ms；请在1s内喂一次狗。否则将会复位
 */
class EBOX_IWDG
{
    public:
        EBOX_IWDG(){};
        void begin(uint16_t ms);
        void feed();

};

#endif
