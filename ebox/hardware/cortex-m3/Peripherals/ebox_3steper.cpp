#include "ebox_3steper.h"
#include "ebox_timer_it.h"
#include "ebox.h"


#include "bsp_ebox.h"

void Steper::begin()
{

    uint8_t index;

    cycleRing.begin(64);
    bitsRing.begin(64);
    //    init_info(pin);


    switch((uint32_t)TIMx)
    {
    case (uint32_t)TIM1_BASE:
        index = TIM1_IT_Update;
        break;
    case (uint32_t)TIM2_BASE:
        index = TIM2_IT_Update;
        break;
    case (uint32_t)TIM3_BASE:
        index = TIM3_IT_Update;
        break;
    case (uint32_t)TIM4_BASE:
        index = TIM4_IT_Update;
        break;

    }

    tim_irq_init(index, (&Steper::_irq_handler), (uint32_t)this);


    Xpwm->mode(AF_PP);
    Ypwm->mode(AF_PP);
    //    Zpwm->mode(AF_PP);

    prescale = get_timer_source_clock() * TIME_UNIT; //
    period = 65535;
    ccr = 0;
    base_init(period, prescale);
    mode_init_oc_pwm();
    set_ccr(10);
    nvic(ENABLE, 0, 0);
    attach(this, &Steper::callback);
    interrupt(ENABLE);

}

void Steper::timer_start()
{
    TIM_Cmd(TIMx, ENABLE);
    TIMx->CNT = 0;

}
void Steper::timer_stop()
{

    TIMx->CCER &= 0xefff;
    TIMx->CCER &= 0xfeff;
    TIM_Cmd(TIMx, ENABLE);
    TIMx->CNT = 0;
    ctr_bits = 0;
}



void Steper::base_init(uint16_t period, uint16_t prescaler)
{
    this->period = period;//更新period


    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    if(TIMx == TIM1 ||  TIMx == TIM8 )
    {
        TIM_CtrlPWMOutputs(TIMx, ENABLE);
    }

    rcc_clock_cmd((uint32_t)TIMx, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = this->period - 1; //ARR
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_ARRPreloadConfig(TIMx, DISABLE);
    TIM_Cmd(TIMx, DISABLE); //

}

void Steper::mode_init_oc_pwm()
{


    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OC1Init(TIMx, &TIM_OCInitStructure);
    TIM_OC2Init(TIMx, &TIM_OCInitStructure);
    TIM_OC3Init(TIMx, &TIM_OCInitStructure);
    TIM_OC4Init(TIMx, &TIM_OCInitStructure);

}

void Steper::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority)
{
    nvic_dev_set_priority((uint32_t)TIMx, 0, 0, 0);
    if(enable != DISABLE)
        nvic_dev_enable((uint32_t)TIMx, 0);
    else
        nvic_dev_disable((uint32_t)TIMx, 0);
}
void Steper::interrupt(FunctionalState enable)
{
    TIM_ClearITPendingBit(TIMx, TIM_FLAG_Update); //必须加，否则开启中断会立即产生一次中断
    TIM_ITConfig(TIMx, TIM_IT_Update, enable);
}
//duty:0-1000对应0%-100.0%
void Steper::set_ccr(uint16_t pulse)
{

    TIMx->CCR1 = pulse;
    TIMx->CCR2 = pulse;
    TIMx->CCR3 = pulse;
    TIMx->CCR4 = pulse;

}

uint32_t Steper::get_timer_source_clock()
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
        if(temp & 0x00000400)//检测PCLK是否进行过分频，如果进行过分频则定时器的频率为PCLK1的两倍
            timer_clock = cpu.clock.pclk1 * 2;
        else
            timer_clock = cpu.clock.pclk1 ;
    }
    return timer_clock;
}

bool Steper::write_buffer(uint16_t counter, uint8_t ctr)
{
    if(!cycleRing.isfull())
    {
        no_interrupts();
        bitsRing.write(ctr);
        cycleRing.write(counter);
        interrupts();
        return true;
    }
    return false;
}

void Steper::callback()
{

    no_interrupts();
    if(cycleRing.available())
    {
        TIMx->ARR = cycleRing.read();
        ctr_bits = bitsRing.read();
        if(ctr_bits & X_STEP_BIT_MASK)
            TIMx->CCER |= 0x0100;
        else
            TIMx->CCER &= 0xfeff;

        if(ctr_bits & Y_STEP_BIT_MASK)
            TIMx->CCER |= 0x1000;
        else
            TIMx->CCER &= 0xefff;
        cnc.update_position(ctr_bits);
        //    uart1.printf("%X\r\n", ctr_bits);
    }
    //    else
    //    {
    //        ctr_bits = 0;
    //        TIMx->CCER &= 0xefff;
    //        TIMx->CCER &= 0xfeff;
    //        TIM_Cmd(TIMx, DISABLE); //
    //        uart1.printf("stop\r\n");
    //    }
    interrupts();

}



void Steper::_irq_handler( uint32_t id)
{
    Steper *handler = (Steper *)id;
    handler->_irq.call();
}

void Steper::attach(void (*fptr)(void))
{
    if (fptr)
    {
        _irq.attach(fptr);
    }
}


uint32_t Steper::sqrt(u32 x)//开方
{
    register u32 xr;  // result register
    register u32 q2;  // scan-bit register
    register u8 f;   // flag (one bit)

    xr = 0;                     // clear result
    q2 = 0x40000000L;           // higest possible result bit
    do
    {
        if((xr + q2) <= x)
        {
            x -= xr + q2;
            f = 1;                  // set flag
        }
        else
        {
            f = 0;                  // clear flag
        }
        xr >>= 1;
        if(f)
        {
            xr += q2;               // test flag
        }
    }
    while(q2 >>= 2);            // shift twice
    if(xr < x)
    {
        return xr + 1;            // add for rounding
    }
    else
    {
        return xr;
    }
}
Steper steper(TIM4);
