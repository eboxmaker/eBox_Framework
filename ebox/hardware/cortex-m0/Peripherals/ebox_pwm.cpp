/**
  ******************************************************************************
  * @file    pwm.cpp
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
#include "ebox_pwm.h"

#if USE_PRINTF
// 是否打印调试信息, 1打印,0不打印
#define debug 0
#endif

#if debug
#define  PWM_DEBUG(...) DBG("[PWM]  "),DBG(__VA_ARGS__)
#else
#define  PWM_DEBUG(...)
#endif

/**
 *@brief    设置TIM为PWM模式
 *@param    NONE
 *@retval   NONE
*/
void Pwm::_setMode(void)
{
    LL_TIM_EnableARRPreload(_timx);
    /*********************************/
    /* Output waveform configuration */
    /*********************************/
    /* Set output mode */
    /* Reset value is LL_TIM_OCMODE_FROZEN */
    LL_TIM_OC_SetMode(_timx, _channel, LL_TIM_OCMODE_PWM1);
    LL_TIM_OC_EnablePreload(_timx, _channel);
    /* Enable output channel 1 */
    LL_TIM_CC_EnableChannel(_timx, _channel);
}

/**
 *@brief    启动PWM输出
 *@param    frq  要输出的频率，单位hz. 0-最大频率。
 *		   duty 占空比 0-1000
 *@retval   NONE
*/
void Pwm::begin(uint32_t frq, uint16_t duty)
{
    _enableClock();
    _setMode();
    SetFrequency(frq);
    SetDutyCycle(duty);
    if (IS_TIM_BREAK_INSTANCE(_timx))
    {
        LL_TIM_EnableAllOutputs(_timx);
    }
    _start();
}

/**
 *@brief    设置频率,并同步计算更新占空比;最大频率可通过GetMaxFrequency()获得
 *@param    frq  要输出的频率，单位hz. 0-最大频率。
 *@retval   None
*/
void Pwm::SetFrequency(uint32_t frq)
{
    if (frq >= GetMaxFrequency())//控制频率，保证其有1%精度
        frq = GetMaxFrequency();

    _calculate(frq);

    _accuracy = ((_period >= 100) && (_period < 1000)) ? 1 : 0;
    _setPerPsc();
    SetDutyCycle(_duty);
}

/**
 *@brief    设置PWM占空比.频率小于等于十分之一最大频率时，分辨率为千分之一
 *		   频率在十分之一最大频率和最大频率之间，分辨率为百分之一
 *@param    duty 0-1000 对应0%-100%
 *@retval   none
*/
void Pwm::SetDutyCycle(uint16_t duty)
{
    uint16_t pulse = 0;
    float percent;		// 百分比

    _duty = duty > 1000 ? 1000 : duty;
    // 百分之一精度
    if(_accuracy)
    {
        _duty = (_duty < 10 && _duty != 0) ? 10 : duty;

    }

    percent = (_duty / 1000.0);
    pulse = (uint16_t) (( percent  * _period ));
    PWM_DEBUG("DutyCycle is : %.1f%% , pulse is : %d \r\n", percent * 100, pulse);
    _OCsetCompare(_timx, pulse);
    /* Force update generation 强制更新 */
    LL_TIM_GenerateEvent_UPDATE(_timx);
}

/**
 *@brief    设置极性
 *@param    flag:  1 HIGH  0 LOW
 *@retval   None
*/
void Pwm::SetPorlicy(uint8_t porlicy)
{
    LL_TIM_OC_SetPolarity(_timx, _channel, (porlicy == 1) ? (LL_TIM_OCPOLARITY_HIGH) : (LL_TIM_OCPOLARITY_LOW));
    /* Force update generation 强制更新 */
    LL_TIM_GenerateEvent_UPDATE(_timx);
}

/**
 *@brief    获取PWM最大频率
 *@param    none
 *@retval   最大频率
*/
uint32_t Pwm::GetMaxFrequency(void)
{
    return GetClock();
}

float Pwm::get_accuracy()
{
    switch(_accuracy)
    {
    case 0:
        return 0;
    case 1:
        return 0.001;
    case 2:
        return 0.01;
    case 3:
        return 0.02;

    }
    return 0.001;
}

