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
#include "ebox_uart.h"
extern Uart uart1;
#define DBG(...) uart1.printf(__VA_ARGS__)
#define  PWM_DEBUG(...) DBG("[PWM]  "),DBG(__VA_ARGS__)
#else
#define  PWM_DEBUG(...)
#endif

/**
 *@brief    ����TIMΪPWMģʽ
 *@param    NONE
 *@retval   NONE
*/
void Pwm::_setMode(void){
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
void Pwm::begin(uint32_t frq,uint16_t duty){
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

	_accuracy = ((_period >= 100) && (_period < 1000))?1:0;
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
	
	_duty = duty>1000?1000:duty;
	// �ٷ�֮һ����
	if(_accuracy){
		_duty = (_duty<10 && _duty!=0)?10:duty;
		
	}

	percent = (_duty/1000.0);
	pulse = (uint16_t) (( percent  * _period ));
	PWM_DEBUG("DutyCycle is : %.1f%% , pulse is : %d \r\n",percent*100,pulse);
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
	LL_TIM_OC_SetPolarity(_timx,_channel,(porlicy == 1)?(LL_TIM_OCPOLARITY_HIGH):(LL_TIM_OCPOLARITY_LOW));
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

float Pwm::get_accuracy(){
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


//Pwm::Pwm(Gpio *pwm_pin)
//{
//    //	if(isPwmPin(PWMpin))
//    //	{
//    this->pwm_pin = pwm_pin;
//    //	}
//}
//void Pwm::begin(uint32_t frq, uint16_t duty)
//{
//    this->duty = duty;

//    init_info(pwm_pin);
//    pwm_pin->mode(AF_PP);

//    set_oc_polarity(1);
//    set_frq(frq);
//    _set_duty(duty);
//}
//void Pwm::base_init(uint16_t period, uint16_t prescaler)
//{
//    this->period = period;//����period


//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

//    if(TIMx == TIM1 ||  TIMx == TIM8 )
//    {
//        TIM_CtrlPWMOutputs(TIMx,ENABLE); 
//    }
//        
//    rcc_clock_cmd((uint32_t)TIMx,ENABLE);
//    TIM_TimeBaseStructure.TIM_Period = this->period - 1; //ARR
//    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //
//    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

//    TIM_ARRPreloadConfig(TIMx, ENABLE);
//    TIM_Cmd(TIMx, ENABLE); //

//}
//void Pwm::init_info(Gpio *pwm_pin)
//{
//    switch((uint8_t)pwm_pin->id)
//    {
//    //TIM2
//    case PA0_ID:
//        TIMx = TIM2;
//        ch = TIMxCH1;//irq = TIM2_IRQn;
//        break;
//    case PA1_ID:
//        TIMx = TIM2;
//        ch = TIMxCH2;//irq = TIM2_IRQn;
//        break;
//    case PA2_ID:
//        TIMx = TIM2;
//        ch = TIMxCH3;//irq = TIM2_IRQn;
//        break;
//    case PA3_ID:
//        TIMx = TIM2;
//        ch = TIMxCH4;//irq = TIM2_IRQn;
//        break;
//    
//    //TIM3
//    case PA6_ID:
//        TIMx = TIM3;
//        ch = TIMxCH1;//irq = TIM3_IRQn;
//        break;
//    case PA7_ID:
//        TIMx = TIM3;
//        ch = TIMxCH2;//irq = TIM3_IRQn;
//        break;
//    
//    //TIM1
//    case PA8_ID:
//        TIMx = TIM1;
//        ch = TIMxCH1;//irq = TIM4_IRQn;
//        break;
//    case PA9_ID:
//        TIMx = TIM1;
//        ch = TIMxCH2;//irq = TIM4_IRQn;
//        break;
//    case PA10_ID:
//        TIMx = TIM1;
//        ch = TIMxCH3;//irq = TIM4_IRQn;
//        break;
//    case PA11_ID:
//        TIMx = TIM1;
//        ch = TIMxCH4;//irq = TIM4_IRQn;
//        break;

//    //TIM4
//    case PB6_ID:
//        TIMx = TIM4;
//        ch = TIMxCH1;//irq = TIM4_IRQn;
//        break;
//    case PB7_ID:
//        TIMx = TIM4;
//        ch = TIMxCH2;//irq = TIM4_IRQn;
//        break;
//    case PB8_ID:
//        TIMx = TIM4;
//        ch = TIMxCH3;//irq = TIM4_IRQn;
//        break;
//    case PB9_ID:
//        TIMx = TIM4;
//        ch = TIMxCH4;//irq = TIM4_IRQn;
//        break;
//    
//    //TIM3
//    case PB0_ID:
//        TIMx = TIM3;
//        ch = TIMxCH3;//irq = TIM3_IRQn;
//        break;
//    case PB1_ID:
//        TIMx = TIM3;
//        ch = TIMxCH4;//irq = TIM3_IRQn;
//        break;
//    }
//}

//void Pwm::set_oc_polarity(uint8_t flag)
//{

//    if(flag == 1)
//        this->oc_polarity = TIM_OCPolarity_High;
//    else if(flag == 0)
//        this->oc_polarity = TIM_OCPolarity_Low;
//    _set_duty(duty);

//}
//void Pwm::set_frq(uint32_t frq)
//{
//    uint32_t period  = 0;
//    uint32_t prescaler = 1;
//    
//    if(frq >= get_max_frq())//����Ƶ�ʣ���֤����1%����
//        frq = get_max_frq();

//    
//    //ǧ��֮һ���ȷ��䷽��
//    for(; prescaler <= 0xffff; prescaler++)
//    {
//        period = get_timer_source_clock() / prescaler / frq;
//        if(period <= 65535)
//        {
//            if((0xffff >= period) && (period >= 1000))
//            {
//                accuracy = 1;
//                break;
//            }
//        }
//    }
//    
//    if(prescaler == 65536)//�����㷨����ʧ��
//    {
//        //�ٷ�֮һ���䷽��
//        for(prescaler = 1; prescaler <= 0xffff; prescaler++)
//        {
//            period = get_timer_source_clock() / prescaler / frq;
//            if(period <= 65535)
//            {
//                if((0xffff >= period) && (period >= 100))
//                {
//                    accuracy = 2;
//                    break;
//                }         
//            }
//        }
//    } 
//    if(prescaler == 65536)//�����㷨����ʧ��
//    {
//        //�ٷ�֮�����䷽��
//        for(prescaler = 1; prescaler <= 0xffff; prescaler++)
//        {
//            period = get_timer_source_clock() / prescaler / frq;
//            if(period <= 65535)
//            {
//                if((0xffff >= period) && (period >= 50))
//                {
//                    accuracy = 3;
//                    break;
//                }   
//            }            
//        }
//    }     
//    base_init(period, prescaler);
//    _set_duty(duty);

//}
////duty:0-1000��Ӧ0%-100.0%
//void Pwm::_set_duty(uint16_t duty)
//{

//    this->duty = duty;

//    uint16_t pulse = 0;
//    float percent;

//    if(this->duty > 1000)
//        this->duty = 1000;
//    percent = this->duty / 1000.0;

//    pulse = (uint16_t) (( percent  * period ));

//    TIM_OCInitTypeDef  TIM_OCInitStructure;
//    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_OCPolarity = oc_polarity;
//    TIM_OCInitStructure.TIM_Pulse = pulse;
//    switch(ch)
//    {
//    case TIMxCH1:
//        TIM_OC1Init(TIMx, &TIM_OCInitStructure);
//        break;
//    case TIMxCH2:
//        TIM_OC2Init(TIMx, &TIM_OCInitStructure);
//        break;
//    case TIMxCH3:
//        TIM_OC3Init(TIMx, &TIM_OCInitStructure);
//        break;
//    case TIMxCH4:
//        TIM_OC4Init(TIMx, &TIM_OCInitStructure);
//        break;
//    }


//}

////duty:0-1000��Ӧ0%-100.0%
//void Pwm::set_duty(uint16_t duty)
//{
//    this->duty = duty;

//    uint16_t pulse = 0;
//    float percent;

//    if(this->duty > 1000)
//        this->duty = 1000;
//    percent = this->duty / 1000.0;

//    pulse = (uint16_t) (( percent  * period ));

//    switch(ch)
//    {
//    case TIMxCH1:
//        TIMx->CCR1 = pulse;
//        break;
//    case TIMxCH2:
//        TIMx->CCR2 = pulse;
//        break;
//    case TIMxCH3:
//        TIMx->CCR3 = pulse;
//        break;
//    case TIMxCH4:
//        TIMx->CCR4 = pulse;
//        break;
//    }

//}



//uint32_t Pwm::get_timer_source_clock()
//{
//    uint32_t temp = 0;
//    uint32_t timer_clock = 0x00;
//    
//    if ((uint32_t)this->TIMx == TIM1_BASE)
//    {
//        timer_clock = cpu.clock.pclk2;
//    }
//    else
//    {
//        temp = RCC->CFGR;
//        if(temp & 0x00000400)//���PCLK�Ƿ���й���Ƶ��������й���Ƶ��ʱ����Ƶ��ΪPCLK1������
//            timer_clock = cpu.clock.pclk1 * 2;
//        else
//            timer_clock = cpu.clock.pclk1 ;
//    }
//    return timer_clock;
//}
//uint32_t Pwm::get_max_frq()
//{
//    return get_timer_source_clock()/50;

//}
//float Pwm::get_accuracy()
//{
//    
//    switch(accuracy)
//    {
//        case 0:
//            return 0;
//        case 1:
//            return 0.001;
//        case 2:
//            return 0.01;
//        case 3:
//            return 0.02;

//    }
//    return 0.001;


//}


//duty:0-1000��Ӧ0%-100.0%
//void analog_write(Gpio *pwm_pin, uint16_t duty)
//{
//    //	if(isPwmPin(PWMpin))
//    //	{
//    Pwm p(pwm_pin);
//    p.begin(1000, duty);
//    //p.SetFrq(1000,1);
//    //			p._set_duty(duty);

//    //	}
//    //	else
//    //	{
//    //	;
//    //	}
//}

