/**
  ******************************************************************************
  * @file    pwm.h
  * @author  shentq
  * @version V2.1
  * @date    2016/11/04
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

#ifndef __EBOX_PULSE_H
#define __EBOX_PULSE_H

#include "ebox_core.h"
#include "mcu.h"

/*
	1.支持TIM1,2,3,4下的CH1-4通道。共计16个通道
	2.可以随时调用pwm的参数修改函数。调用setfrq函数不会改变占空比,但是会影响同定时器下其他通道的工作。
    3.pwm所占用的定时器的时钟源频率可以通过get_timer_clock()获得；
    4.PWM最大频率输出可以通过get_max_frq()获得；
    5.PWM当前输出的精度可以通过get_accuracy()获得；
	注意：  1.暂时没有完美支持remap引脚。
                如果使用remap引脚需要自己在执行完初始化函数后再执行相应的remap函数。然后将需要remap的引脚初始化为AF_PP模式
                详细内容请参考pwm&remap.cpp
            2.使用了相应的timx为pwm服务后，该tim就不能再初始化成另外的功能
            3.初始化pwm后，只有调用setDuty函数后才会输出pwm
            4.如果duty大于等于1000则输出持续高电平

    定时器引脚对应列表
    通道 CH1  CH2  CH3  CH4
    TIM1 PA8  PA9  PA10 PA11
    TIM2 PA0  PA1  PA2  PA3
    TIM3 PA6  PA7  PA0  PA1
    TIM4 PB6  PB7  PB8  PB9
*/
/**
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *修复了set_duty时导致波形输出畸形
 *      *修复了芯片主频变动导致定时器PWM频率变化的bug，支持任意主频下正常工作
 *      *增加了get_timer_clock(),get_max_frq(),get_accuracy()三个函数接口
 * -shentq                  -version 2.0(2016/11/4)
 *      *增加对定时器1的支持
 */

#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04

class Pulse
{
public:
    typedef struct {
        uint16_t    prescaler;//分频
        uint16_t    max_counter;//结束时的counter
        uint16_t    delay_counter;//启动时的counter
    }Config_t;
    
    Config_t cfg;
    Gpio        *pin;
public:
    Pulse(Gpio *pwm_pin);
    void begin(uint16_t percent);
    void end(void);

    void start();
    void stop();
    void set_oc_polarity(uint8_t flag);//设置输出极性1：比较匹配后输出低电平（默认设置），0：比较匹配后输出高电平

    void set_delay(uint16_t counter);
    void set_reload(uint16_t value);
    void set_phase_front(uint16_t percent);
    void set_phase_after(uint16_t percent);
    uint32_t get_timer_source_clock();

    float get_delay_us();
    void enable_pin(void);
    void disable_pin(void);
private:
    TIM_TypeDef *TIMx;
    uint8_t     ch;
    uint16_t    duty;//保存占空比值
    uint16_t    oc_polarity;

    void init_info(Gpio *pwm_pin);
    void base_init(uint16_t Period, uint16_t Prescaler);
    void oc_mode_init();
    //-------------------------------------------------------------
    uint16_t percent;
    uint16_t arr;


};
void analog_write(Gpio *pwm_pin, uint16_t duty);

#endif
