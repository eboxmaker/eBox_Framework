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

#if USE_PRINTF
// 是否打印调试信息, 1打印,0不打印
#define debug 1
#endif

#if debug
#include "ebox_uart.h"
extern Uart uart2;
#define DBG(...) uart2.printf(__VA_ARGS__)
#define  CAP_DEBUG(...) DBG("[CAP]  "),DBG(__VA_ARGS__)
#else
#define  CAP_DEBUG(...)
#endif

extern uint16_t t1_overflow_times ;
extern uint16_t t2_overflow_times ;
extern uint16_t t3_overflow_times ;
extern uint16_t t4_overflow_times ;


void InCapture::begin(uint16_t prescaler ,ICMode_t mode)
{
	_period = 0xffff;
	_prescaler = 1;
	_timeClock 	= GetClock()/_prescaler;
	_porlicy = LL_TIM_IC_POLARITY_RISING;
    
        switch((uint32_t)_timx)
    {
    case (uint32_t)TIM1_BASE:
        _overflow_times = &t1_overflow_times;
        _tIndex = TIM1_IT_Update;
        break;
    case (uint32_t)TIM2_BASE:
        _overflow_times = &t2_overflow_times;
        _tIndex = TIM2_IT_Update;
        break;
    case (uint32_t)TIM3_BASE:
        _overflow_times = &t3_overflow_times;
        _tIndex = TIM3_IT_Update;
        break;
//    case (uint32_t)TIM4_BASE:
//        overflow_times = &t4_overflow_times;
//        index = TIM4_IT_Update;
//        break;
//    case (uint32_t)TIM5_BASE:
//        index = TIM5_IT_Update;
//        break;
    case (uint32_t)TIM6_BASE:
        _tIndex = TIM6_IT_Update;
        break;
    case (uint32_t)TIM7_BASE:
        _tIndex = TIM7_IT_Update;
        break;
    }
	
	_enableClock();
	_setPerPsc();
	_enInterrupt();
	
	NVIC_SetPriority(TIM1_CC_IRQn, 0);
	NVIC_EnableIRQ(TIM1_CC_IRQn);

    tim_irq_init(_tIndex+_chNum+TIM_IT_CC_OFFSET,(&InCapture::_irq_handler),(uint32_t)this);
        if(mode == SIMPLE)
    attach(this,&InCapture::simple_event);
else
    attach(this,&InCapture::complex_event);

	_setMode();
	_start();
}

/**
 *@brief    设置为捕获模式
 *@param    none
 *@retval   频率
*/
void InCapture::_setMode(){
   /* Input capture mode configuration */
  /************************************/
  /* Select the active input: IC1 = TI1FP1 */
  LL_TIM_IC_SetActiveInput(_timx, _channel, LL_TIM_ACTIVEINPUT_DIRECTTI);  
  /* Configure the input filter duration: no filter needed */
  LL_TIM_IC_SetFilter(_timx, _channel, LL_TIM_IC_FILTER_FDIV1);
  /* Set input prescaler: prescaler is disabled */
  LL_TIM_IC_SetPrescaler(_timx, _channel, LL_TIM_ICPSC_DIV1);
  /* Select the edge of the active transition on the TI1 channel: rising edge */
  LL_TIM_IC_SetPolarity(_timx, _channel, _porlicy);
	
  /**************************/
  /* TIM1 interrupts set-up */
  /**************************/
  /* Enable the capture/compare interrupt for channel 1 */
  //LL_TIM_EnableIT_CC2(_timx);  
	_CCEnableIT(_timx);
  /***********************/
  /* Start input capture */
  /***********************/
  /* Enable output channel 1 */
  LL_TIM_CC_EnableChannel(_timx, _channel);  
}

/**
 *@brief    设置采样极性
 *@param    none
 *@retval   频率
*/
void InCapture::SetPorlicy(uint8_t porlicy){
	_porlicy = porlicy;
	LL_TIM_IC_SetPolarity(_timx,_channel,_porlicy);
}

uint32_t InCapture::get_capture()
{
    uint32_t    now = 0;
    now = _getCapture( _timx ) + (*_overflow_times) * _period;  //get capture value
    if(now > _last_value)
        _capture = now - _last_value;
    else
        _capture = 0xffffffff + now - _last_value;
    _last_value = now;
    return _capture;    
}

float InCapture::get_zone_time_us()
{
    return get_capture() * 1000000.0 / _timeClock;
}



void InCapture::complex_event()
{
    uint32_t    capture = 0;
    uint32_t    now = 0;
    now = _getCapture( _timx ) + (*_overflow_times) * _period;  //get capture value
    if(now > _last_value)
        capture = now - _last_value;
    else
        capture = 0xffffffff + now - _last_value;
    _last_value = now;
    
    if(_porlicy == TIM_ICPOLARITY_FALLING)//检测到下降沿，测量高电平时间完成
    {
        _high_capture = capture;
        set_polarity_rising();//切换检测上升沿
    }
    else
    {
        _low_capture = capture;
        set_polarity_falling();//切换检测下降沿
    }
    if((_high_capture!= 0) && (_low_capture != 0))
        _available = true;
    
}
void InCapture::simple_event()
{
    uint32_t    now = 0;
    now = _getCapture(_timx) + (*_overflow_times) * _period ;  //get capture value
    if(now > _last_value)
        _capture = now - _last_value;
    else
        _capture = 0xffffffff + now - _last_value;
    _last_value = now;
    
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
        return  (_timeClock/((_high_capture + _low_capture)));
    else
        return (_timeClock/_capture);
}
float InCapture::get_wave_peroid()
{
    _available = false;
    if(_capture == 0)
        return  ((_high_capture + _low_capture)*1000000.0/(_timeClock));
    else
        return  (_capture*1000000.0/(_timeClock));
}

float InCapture::get_wave_high_duty()
{
    _available = false;
    if(_capture == 0)
       return  (_high_capture*100.0/(_high_capture + _low_capture));
    else
        return 0;
}
float InCapture::get_wave_low_duty()
{
    _available = false;
    if(_capture == 0)
        return  (_low_capture*100.0/(_high_capture + _low_capture));
    else
        return  0;
}

float InCapture::get_wave_high_time()
{
    _available = false;
    if(_capture == 0)
        return  ((_high_capture )*1000000.0/(_timeClock));
    else
        return  (_capture*1000000.0/(_timeClock));
}
float InCapture::get_wave_low_time()
{
    _available = false;
    if(_capture == 0)
        return  ((_low_capture)*1000000.0/(_timeClock));
    else
        return  (_capture*1000000.0/(_timeClock));
}


void InCapture::set_count(uint16_t count)
{
//    TIM_SetCounter(this->TIMx, count); //reset couter
}

uint32_t InCapture::get_timer_clock()
{
    return get_timer_source_clock()/_prescaler;
}

uint32_t InCapture::get_timer_source_clock()
{
    uint32_t temp = 0;
    uint32_t timer_clock = 0x00;
    
    if ((uint32_t)this->_timx == TIM1_BASE)
    {
        timer_clock = cpu.clock.pclk2;
    }
    else
    {
        temp = RCC->CFGR;
        if(temp & 0x00000400)//检测PCLK是否进行过分频，如果进行过分频则定时器的频率为PCLK1的两倍
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
		handler->_pirq.call();

}


