/**
  ******************************************************************************
  * @file    incapture.cpp
  * @author  shentq
  * @version V2.1
  * @date    2017/07/23
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
#include "ebox_in_capture.h"
#include "ebox_timer_it.h"
extern uint16_t t1_overflow_times ;
extern uint16_t t2_overflow_times ;
extern uint16_t t3_overflow_times ;
extern uint16_t t4_overflow_times ;


InCapture::InCapture(Gpio *capture_pin)
{
    this->capture_pin = capture_pin;
    this->period = 0xffff;
    this->prescaler = 1;
    this->polarity = TIM_ICPOLARITY_FALLING;
    high_capture = 0;
    low_capture = 0;
    _capture = 0;
}
void InCapture::begin(uint16_t prescaler ,ICMode_t mode)
{
	  uint8_t index;
    capture_pin->mode(INPUT_PU);
    init_info(this->capture_pin);

    this->prescaler = prescaler;
    base_init(this->period, this->prescaler);
    timer_clock = get_timer_clock();
    switch(ch)
    {
    case TIM_Channel_1:
        _get_capture = TIM_GetCapture1;
        _set_polarity = TIM_OC1PolarityConfig;
        break;
    case TIM_Channel_2:
        _get_capture = TIM_GetCapture2;
        _set_polarity = TIM_OC2PolarityConfig;
        break;
    case TIM_Channel_3:
        _get_capture = TIM_GetCapture3;
        _set_polarity = TIM_OC3PolarityConfig;
        break;
    case TIM_Channel_4:
        _get_capture = TIM_GetCapture4;
        _set_polarity = TIM_OC4PolarityConfig;
        break;
    }
    switch((uint32_t)TIMx)
    {
    case (uint32_t)TIM1_BASE:
        overflow_times = &t1_overflow_times;
        if(ch== TIM_Channel_1) index = TIM1_IT_CC1;
        if(ch== TIM_Channel_2) index = TIM1_IT_CC2;
        if(ch== TIM_Channel_3) index = TIM1_IT_CC3;
        if(ch== TIM_Channel_4) index = TIM1_IT_CC4;		
        break;
    case (uint32_t)TIM2_BASE:
        overflow_times = &t2_overflow_times;
        if(ch== TIM_Channel_1) index = TIM2_IT_CC1;
        if(ch== TIM_Channel_2) index = TIM2_IT_CC2;
        if(ch== TIM_Channel_3) index = TIM2_IT_CC3;
        if(ch== TIM_Channel_4) index = TIM2_IT_CC4;	
        break;
    case (uint32_t)TIM3_BASE:
        overflow_times = &t3_overflow_times;
        if(ch== TIM_Channel_1) index = TIM3_IT_CC1;
        if(ch== TIM_Channel_2) index = TIM3_IT_CC2;
        if(ch== TIM_Channel_3) index = TIM3_IT_CC3;
        if(ch== TIM_Channel_4) index = TIM3_IT_CC4;	
        break;
    case (uint32_t)TIM4_BASE:
        overflow_times = &t4_overflow_times;
        if(ch== TIM_Channel_1) index = TIM4_IT_CC1;
        if(ch== TIM_Channel_2) index = TIM4_IT_CC2;
        if(ch== TIM_Channel_3) index = TIM4_IT_CC3;
        if(ch== TIM_Channel_4) index = TIM4_IT_CC4;	
        break;

    }
    tim_irq_init(index,(&InCapture::_irq_handler),(uint32_t)this);
    if(mode == SIMPLE)
        attach(this,&InCapture::simple_event);
    else
        attach(this,&InCapture::complex_event);

}
void InCapture::base_init(uint16_t period, uint16_t prescaler)
{
    this->period = period;//����period
    this->prescaler = prescaler;//����prescaler

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIMx_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    switch((uint32_t)this->TIMx)
    {
    case (uint32_t)TIM1_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//
        break;
    case (uint32_t)TIM2_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//
        break;
    case (uint32_t)TIM3_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//
        break;
    case (uint32_t)TIM4_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//
        break;
    }

    TIM_TimeBaseStructure.TIM_Period = this->period - 1; //ARR
    TIM_TimeBaseStructure.TIM_Prescaler = this->prescaler - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIMx, ENABLE);


    //��ʼ��TIMx���벶����� ͨ��2
    TIMx_ICInitStructure.TIM_Channel = ch; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
    TIMx_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//�����ز���
    TIMx_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
    TIMx_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ
    TIMx_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(this->TIMx, &TIMx_ICInitStructure);
    TIM_ClearFlag(this->TIMx, TIM_FLAG_Update);


    //�жϷ����ʼ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�1��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);   //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    switch(ch)
    {
    case TIM_Channel_1:
        TIM_ClearFlag(this->TIMx, TIM_FLAG_CC1);
        TIM_ITConfig(this->TIMx,  TIM_IT_CC1 | TIM_IT_Update, ENABLE);   //ʹ��TIMx��ͨ��2�����ж�
        break;
    case TIM_Channel_2:
        TIM_ClearFlag(this->TIMx, TIM_FLAG_CC2);
        TIM_ITConfig(this->TIMx,  TIM_IT_CC2 | TIM_IT_Update, ENABLE);   //ʹ��TIMx��ͨ��2�����ж�
        break;
    case TIM_Channel_3:
        TIM_ClearFlag(this->TIMx, TIM_FLAG_CC3);
        TIM_ITConfig(this->TIMx,  TIM_IT_CC3 | TIM_IT_Update, ENABLE);   //ʹ��TIMx��ͨ��2�����ж�
        break;
    case TIM_Channel_4:
        TIM_ClearFlag(this->TIMx, TIM_FLAG_CC4);
        TIM_ITConfig(this->TIMx,  TIM_IT_CC4 | TIM_IT_Update, ENABLE);   //ʹ��TIMx��ͨ��2�����ж�
        break;
    }

    TIM_Cmd(TIMx, ENABLE); //

}
void InCapture::init_info(Gpio *capture_pin)
{
    switch((uint8_t)capture_pin->id)
    {
        case PA0_ID :
            TIMx = TIM2;
            ch = TIM_Channel_1;//irq = TIM2_IRQn;
            break;
        case PA1_ID:
            TIMx = TIM2;
            ch = TIM_Channel_2;//irq = TIM2_IRQn;
            break;
        case PA2_ID:
            TIMx = TIM2;
            ch = TIM_Channel_3;//irq = TIM2_IRQn;
            break;
        case PA3_ID:
            TIMx = TIM2;
            ch = TIM_Channel_4;//irq = TIM2_IRQn;
            break;        
    
        case PA6_ID:
            TIMx = TIM3;
            ch = TIM_Channel_1;//irq = TIM3_IRQn;
            break;
        case PA7_ID:
            TIMx = TIM3;
            ch = TIM_Channel_2;//irq = TIM3_IRQn;
            break;
        
        
        case PB0_ID:
            TIMx = TIM3;
            ch = TIM_Channel_3;//irq = TIM3_IRQn;
            break;
        case PB1_ID:
            TIMx = TIM3;
            ch = TIM_Channel_4;//irq = TIM3_IRQn;
            break;

        case PB6_ID:
            TIMx = TIM4;
            ch = TIM_Channel_1;//irq = TIM4_IRQn;
            break;
        case PB7_ID:
            TIMx = TIM4;
            ch = TIM_Channel_2;//irq = TIM4_IRQn;
            break;
        case PB8_ID:
            TIMx = TIM4;
            ch = TIM_Channel_3;//irq = TIM4_IRQn;
            break;
        case PB9_ID:
            TIMx = TIM4;
            ch = TIM_Channel_4;//irq = TIM4_IRQn;
            break;
    }
    

}

void InCapture::set_polarity_falling()
{
    this->polarity = TIM_ICPOLARITY_FALLING;
    _set_polarity(this->TIMx, this->polarity);//����Ϊ�½��ز���

}
void InCapture::set_polarity_rising()
{
    this->polarity = TIM_ICPOLARITY_RISING;
    _set_polarity(this->TIMx, this->polarity);//����Ϊ�½��ز���

}
uint32_t InCapture::get_capture()
{
    uint32_t    now = 0;
    now = _get_capture( this->TIMx ) + (*overflow_times) * this->period;  //get capture value
    if(now > last_value)
        _capture = now - last_value;
    else
        _capture = 0xffffffff + now - last_value;
    last_value = now;
    return _capture;    
}

float InCapture::get_zone_time_us()
{
    return get_capture() * 1000000.0 / timer_clock;
}



void InCapture::complex_event()
{
    uint32_t    capture = 0;
    uint32_t    now = 0;
    now = _get_capture( this->TIMx ) + (*overflow_times) * this->period;  //get capture value
    if(now > last_value)
        capture = now - last_value;
    else
        capture = 0xffffffff + now - last_value;
    last_value = now;
    
    if(polarity == TIM_ICPOLARITY_FALLING)//��⵽�½��أ������ߵ�ƽʱ�����
    {
        high_capture = capture;
        set_polarity_rising();//�л����������
    }
    else
    {
        low_capture = capture;
        set_polarity_falling();//�л�����½���
    }
    if((high_capture!= 0) && (low_capture != 0))
        _available = true;
    
}
void InCapture::simple_event()
{
    uint32_t    now = 0;
    now = _get_capture( this->TIMx ) + (*overflow_times) * this->period;  //get capture value
    if(now > last_value)
        _capture = now - last_value;
    else
        _capture = 0xffffffff + now - last_value;
    last_value = now;
    
    _available = true;  
}

bool InCapture::available()
{
    return _available;
}
float InCapture::get_wave_frq()
{
    _available = false;
    if(_capture == 0)
        return  (timer_clock/((high_capture + low_capture)));
    else
        return (timer_clock/_capture);
}
float InCapture::get_wave_peroid()
{
    _available = false;
    if(_capture == 0)
        return  ((high_capture + low_capture)*1000000.0/(timer_clock));
    else
        return  (_capture*1000000.0/(timer_clock));
}

float InCapture::get_wave_high_duty()
{
    _available = false;
    if(_capture == 0)
       return  (high_capture*100.0/(high_capture + low_capture));
    else
        return 0;
}
float InCapture::get_wave_low_duty()
{
    _available = false;
    if(_capture == 0)
        return  (low_capture*100.0/(high_capture + low_capture));
    else
        return  0;
}

float InCapture::get_wave_high_time()
{
    _available = false;
    if(_capture == 0)
        return  ((high_capture )*1000000.0/(timer_clock));
    else
        return  (_capture*1000000.0/(timer_clock));
}
float InCapture::get_wave_low_time()
{
    _available = false;
    if(_capture == 0)
        return  ((low_capture)*1000000.0/(timer_clock));
    else
        return  (_capture*1000000.0/(timer_clock));
}


void InCapture::set_count(uint16_t count)
{
    TIM_SetCounter(this->TIMx, count); //reset couter
}

uint32_t InCapture::get_timer_clock()
{
    return get_timer_source_clock()/this->prescaler;
}

uint32_t InCapture::get_timer_source_clock()
{
    uint32_t temp = 0;
    uint32_t timer_clock = 0x00;
    
    if ((uint32_t)this->TIMx == TIM1_BASE)
    {
        timer_clock = cpu.clock.pclk2;
    }
    else
    {
        temp = RCC->CFGR;
        if(temp & 0x00000400)//���PCLK�Ƿ���й���Ƶ��������й���Ƶ��ʱ����Ƶ��ΪPCLK1������
            timer_clock = cpu.clock.pclk1 * 2;
        else
            timer_clock = cpu.clock.pclk1 ;
    }
    return timer_clock;
}
uint32_t InCapture::get_detect_max_frq()
{
    
    switch(get_timer_source_clock())
    {
        case 72000000:
            return 180000;
        case 8000000:
            return 18000;
        default:
            return 0;
    }

}
uint32_t InCapture::get_detect_min_frq()
{
    return 0;
}

uint8_t InCapture::get_detect_min_pulse_us()
{
    switch(get_timer_source_clock())
    {
        case 72000000:
            return 4;
        case 8000000:
            return 30;
        default:
            return 0;
    }
}


void InCapture::_irq_handler( uint32_t id){ 
		InCapture *handler = (InCapture*)id;
		handler->_irq.call();

}


void InCapture::attach(void (*fptr)(void)) {
    if (fptr) {
        _irq.attach(fptr);
		}
}

