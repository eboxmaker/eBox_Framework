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
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 0
#endif

#if debug
#define  PWM_DEBUG(...) DBG("[PWM]  "),DBG(__VA_ARGS__)
#else
#define  PWM_DEBUG(...)
#endif

/**
 *@brief    ����TIMΪPWMģʽ
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
 *@brief    ����PWM���
 *@param    frq  Ҫ�����Ƶ�ʣ���λhz. 0-���Ƶ�ʡ�
 *		   duty ռ�ձ� 0-1000
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
 *@brief    ����Ƶ��,��ͬ���������ռ�ձ�;���Ƶ�ʿ�ͨ��GetMaxFrequency()���
 *@param    frq  Ҫ�����Ƶ�ʣ���λhz. 0-���Ƶ�ʡ�
 *@retval   None
*/
void Pwm::SetFrequency(uint32_t frq)
{
    if (frq >= GetMaxFrequency())//����Ƶ�ʣ���֤����1%����
        frq = GetMaxFrequency();

    _calculate(frq);

    _accuracy = ((_period >= 100) && (_period < 1000)) ? 1 : 0;
    _setPerPsc();
    SetDutyCycle(_duty);
}

/**
 *@brief    ����PWMռ�ձ�.Ƶ��С�ڵ���ʮ��֮һ���Ƶ��ʱ���ֱ���Ϊǧ��֮һ
 *		   Ƶ����ʮ��֮һ���Ƶ�ʺ����Ƶ��֮�䣬�ֱ���Ϊ�ٷ�֮һ
 *@param    duty 0-1000 ��Ӧ0%-100%
 *@retval   none
*/
void Pwm::SetDutyCycle(uint16_t duty)
{
    uint16_t pulse = 0;
    float percent;		// �ٷֱ�

    _duty = duty > 1000 ? 1000 : duty;
    // �ٷ�֮һ����
    if(_accuracy)
    {
        _duty = (_duty < 10 && _duty != 0) ? 10 : duty;

    }

    percent = (_duty / 1000.0);
    pulse = (uint16_t) (( percent  * _period ));
    PWM_DEBUG("DutyCycle is : %.1f%% , pulse is : %d \r\n", percent * 100, pulse);
    _OCsetCompare(_timx, pulse);
    /* Force update generation ǿ�Ƹ��� */
    LL_TIM_GenerateEvent_UPDATE(_timx);
}

/**
 *@brief    ���ü���
 *@param    flag:  1 HIGH  0 LOW
 *@retval   None
*/
void Pwm::SetPorlicy(uint8_t porlicy)
{
    LL_TIM_OC_SetPolarity(_timx, _channel, (porlicy == 1) ? (LL_TIM_OCPOLARITY_HIGH) : (LL_TIM_OCPOLARITY_LOW));
    /* Force update generation ǿ�Ƹ��� */
    LL_TIM_GenerateEvent_UPDATE(_timx);
}

/**
 *@brief    ��ȡPWM���Ƶ��
 *@param    none
 *@retval   ���Ƶ��
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

