/**
  ******************************************************************************
  * @file    *.h
  * @author  shentq
  * @version V1.2
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
/**
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *优化内部结构，降低频繁中断带来的额外计算开销
 *      *单独增加了波形基本测量工具，简化用户的波形测量编程
 */

/* Define to prevent recursive inclusion -------------------------------------*/


#ifndef __IN_CAPTURE_H
#define __IN_CAPTURE_H
#include "ebox_common.h"
#include "FunctionPointer.h"

#define TIM_ICPOLARITY_FALLING TIM_ICPolarity_Falling
#define TIM_ICPOLARITY_RISING  TIM_ICPolarity_Rising
/*
1.支持TIM2，3，4的ch1,2,3,4.共计12个通道
2.支持测量周期、频率、高级用法支持测量占空比
3.默认测量下降沿，可通过函数设置测量边沿模式
4.定时器计数器最大值为0xffff,为了突破这个限制，
    在本例程中，使用了update溢出中断调用tx_overflow_times可以将计数器
    拓展至2^32。大大提高测量范围，可以实现最高频率（1分频）测量周期低于120s的信号。
    如果使用2分频，可测量周期低于240s的信号。以此类推。
5.关于分频系数和脉冲宽度测量的计算关系，要遵循一个原则：在不溢出的情况下尽量使用低分频系数（高TIM时钟）去检测对象
6.关于get_capture()和测量时间结果转换的关系；
    时间(us)=get_capture()/(72/prescaler);
    时间(ms)=get_capture()/(72000/prescaler);
    时间(s)=get_capture()/(72000000/prescaler);
    如果直接使用get_zone_time_us()方法则可直接得到一个计算好的值。可以省去手工计算的过程。
    此处提供了两种获取边沿宽度的方法，一种是按定时器脉冲数，一种是按时间单位注意其区别。


7.如果使用某个定时器通道用于输入捕获，则该定时器所有通道都必须是输入捕获模式，不能再设置为其他工作模式
重点：
    在采用低分频系数的时候，可以保证测量精度，但是会增大定时器溢出频率，进而增大cpu开销，
    在采用高分频系数的时候，测量精度较低，但是会降低定时器溢出频率，进而降低cpu开销，
    stm32在72M主频下，最高可测160Khz的信号。如果再大，将无法测量。
    

*/

class InCapture
{
public:
    InCapture(Gpio *capture_pin);
    void        begin();//使用默认参数，分频系数为1；最大量程为120s
    void        begin(uint16_t prescaler = 1);
    void        set_count(uint16_t count);
    void        set_polarity_falling();
    void        set_polarity_rising();


    //需要用户在中断中处理更精细的任务，处理状态机等事务，比如红外解码，超声波测距
    uint32_t    get_capture();//
    float       get_zone_time_us();//

    //波形的基本的测量工具
    void        complex_event();//适用于要求测量占空比的情况，但是最短脉宽不能低于4us****
    void        simple_event();//适用于所有情况，执行效率高，最高可测180K,但是不能测量占空比
    float       get_wave_frq();///<波形的频率
    float       get_wave_peroid();///<波形的周期
    float       get_wave_high_duty();///<波形的高电平占空比
    float       get_wave_low_duty();///<波形的低电平占空比
    float       get_wave_high_time();///<波形的高电平时间
    float       get_wave_low_time();///<波形的低电平时间
    bool        available();///<波形的测量完成
    
    //绑定中断

		
	void attach(void (*fptr)(void));
    template<typename T>
    void attach(T* tptr, void (T::*mptr)(void)) {
        _irq.attach(tptr, mptr);
    }

	static void _irq_handler( uint32_t id);
    
    uint32_t    get_timer_clock();
    uint32_t    get_timer_source_clock();
    uint32_t    get_detect_max_frq();
    uint32_t    get_detect_min_frq();
    uint8_t     get_detect_min_pulse_us();

    uint8_t     polarity;
		
		

private:
    Gpio        *capture_pin;
    TIM_TypeDef *TIMx;
    uint8_t     ch;
    uint16_t    period;//保存溢出值，用于计算占空比
    uint16_t    prescaler;//保存溢出值，用于计算占空比
    uint32_t    _capture;
    uint32_t    high_capture;
    uint32_t    low_capture;

    uint16_t    *overflow_times;
    uint32_t    last_value;
    bool        _available;
    uint32_t    timer_clock;

    void        init_info(Gpio *capture_pin);
    void        base_init(uint16_t Period, uint16_t Prescaler);

    uint16_t    (*_get_capture)(TIM_TypeDef *TIMx);
    void        (*_set_polarity)(TIM_TypeDef *TIMx, uint16_t TIM_OCPolarity); //设置为下降沿或者上升沿捕获
protected:
    FunctionPointer _irq;
};
#endif
