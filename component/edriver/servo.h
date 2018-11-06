#ifndef __SERVO_H
#define __SERVO_H
#include "ebox_core.h"
#include "ebox_pwm.h"

//舵机驱动
//支持端口：
//定时器引脚对应列表
//通道 CH1  CH2  CH3  CH4
//TIM1 PA8FT  PA9FT  PA10FT PA11FT
//TIM2 PA0  PA1  PA2  PA3
//TIM3 PA6  PA7  PB0  PB1
//TIM4 PB6FT  PB7FT  PB8FT  PB9FT
class Servo
{
    Pwm pwm;
    uint32_t frq;
    uint16_t limLow, limHigh;
    float pct;

public:

    //构建舵机对象，默认频率50Hz
    //标准50Hz时，占空比范围3%~13%
    //标准舵机高电平范围0.5ms~2.5ms
    //此处取limLowMs=0.6ms~limHighMs=2.4ms，留一些余量
    Servo(Gpio *pin, uint32_t frequency = 50, float limLowMs = 0.6, float limHighMs = 2.4);

    //初始化pwm
    void begin();

    //设置百分比
    void setPct(float percent);

    //获取百分比
    float getPct();
};



#endif
