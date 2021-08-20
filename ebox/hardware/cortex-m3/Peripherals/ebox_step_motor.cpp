#include "ebox_step_motor.h"
#include "ebox_timer_it.h"



void StepMotor::begin()
{

    uint8_t index;

    init_info(pin);


    switch((uint32_t)TIMx)
    {
    case (uint32_t)TIM1_BASE:
        if(ch == TIM_Channel_1) index = TIM1_IT_CC1;
        if(ch == TIM_Channel_2) index = TIM1_IT_CC2;
        if(ch == TIM_Channel_3) index = TIM1_IT_CC3;
        if(ch == TIM_Channel_4) index = TIM1_IT_CC4;
        break;
    case (uint32_t)TIM2_BASE:
        if(ch == TIM_Channel_1) index = TIM2_IT_CC1;
        if(ch == TIM_Channel_2) index = TIM2_IT_CC2;
        if(ch == TIM_Channel_3) index = TIM2_IT_CC3;
        if(ch == TIM_Channel_4) index = TIM2_IT_CC4;
        break;
    case (uint32_t)TIM3_BASE:
        if(ch == TIM_Channel_1) index = TIM3_IT_CC1;
        if(ch == TIM_Channel_2) index = TIM3_IT_CC2;
        if(ch == TIM_Channel_3) index = TIM3_IT_CC3;
        if(ch == TIM_Channel_4) index = TIM3_IT_CC4;
        break;
    case (uint32_t)TIM4_BASE:
        if(ch == TIM_Channel_1) index = TIM4_IT_CC1;
        if(ch == TIM_Channel_2) index = TIM4_IT_CC2;
        if(ch == TIM_Channel_3) index = TIM4_IT_CC3;
        if(ch == TIM_Channel_4) index = TIM4_IT_CC4;
        break;

    }

    tim_irq_init(index, (&StepMotor::_irq_handler), (uint32_t)this);


    pin->mode(AF_PP);

    prescale = get_timer_source_clock() / STEPMOTOR_TIM_FRQ;
    period = 65535;
    ccr = 0;
    base_init(period, prescale);

    nvic(ENABLE, 0, 0);
    attach(this, &StepMotor::callback);
    interrupt(DISABLE);

}


void StepMotor::move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed)
{
    //! Number of steps before we hit max speed.
    unsigned int max_s_lim;
    //! Number of steps before we must start deceleration (if accel does not hit max speed).
    unsigned int accel_lim;

    // Set direction from sign on step value.
    if(step < 0)
    {
        srd.dir = CCW;
        step = -step;
    }
    else
    {
        srd.dir = CW;
    }

    // If moving only 1 step.
    if(step == 1)
    {
        // Move one step...
        srd.accel_count = -1;
        // ...in DECEL state.
        srd.run_state = DECEL;
        // Just a short delay so main() can act on 'running'.
        srd.step_delay = 1000;
        status = true;

        //    OCR1A = 10;
        //    // Run Timer/Counter 1 with prescaler = 8.
        //    TCCR1B |= ((0<<CS12)|(1<<CS11)|(0<<CS10));
        set_ccr(100);
        TIM_Cmd(TIMx, ENABLE);
    }
    // Only move if number of steps to move is not zero.
    else if(step != 0)
    {
        // Refer to documentation for detailed information about these calculations.

        // Set max speed limit, by calc min_delay to use in timer.
        // min_delay = (alpha / tt)/ w
        srd.min_delay = A_T_x100 / speed;

        // Set accelration by calc the first (c0) step delay .
        // step_delay = 1/tt * sqrt(2*alpha/accel)
        // step_delay = ( tfreq*0.676/100 )*100 * sqrt( (2*alpha*10000000000) / (accel*100) )/10000
        srd.step_delay = (T1_FREQ_148 * sqrt(A_SQ / accel)) / 100;

        // Find out after how many steps does the speed hit the max speed limit.
        // max_s_lim = speed^2 / (2*alpha*accel)
        max_s_lim = (long)speed * speed / (long)(((long)A_x20000 * accel) / 100);
        // If we hit max speed limit before 0,5 step it will round to 0.
        // But in practice we need to move atleast 1 step to get any speed at all.
        if(max_s_lim == 0)
        {
            max_s_lim = 1;
        }

        // Find out after how many steps we must start deceleration.
        // n1 = (n1+n2)decel / (accel + decel)
        accel_lim = ((long)step * decel) / (accel + decel);
        // We must accelrate at least 1 step before we can start deceleration.
        if(accel_lim == 0)
        {
            accel_lim = 1;
        }

        // Use the limit we hit first to calc decel.
        if(accel_lim <= max_s_lim)
        {
            srd.decel_val = accel_lim - step;
        }
        else
        {
            srd.decel_val = -(max_s_lim * accel / decel);
        }
        // We must decelrate at least 1 step to stop.
        if(srd.decel_val == 0)
        {
            srd.decel_val = -1;
        }

        // Find step to start decleration.
        srd.decel_start = step + srd.decel_val;

        // If the maximum speed is so low that we dont need to go via accelration state.
        if(srd.step_delay <= srd.min_delay)
        {
            srd.step_delay = srd.min_delay;
            srd.run_state = RUN;
        }
        else
        {
            srd.run_state = ACCEL;
        }

        // Reset counter.
        srd.accel_count = 0;
        status = true;
        //    OCR1A = 10;
        //    // Set Timer/Counter to divide clock by 8
        //    TCCR1B |= ((0<<CS12)|(1<<CS11)|(0<<CS10));
        //    ebox_printf("===========\n");
        //    ebox_printf("step:%d\n",step);
        //    ebox_printf("min_delay:%d\n",srd.min_delay);
        //    ebox_printf("step_delay:%d\n",srd.step_delay);
        //    ebox_printf("max_s_lim:%d\n",max_s_lim);
        //    ebox_printf("accel_lim:%d\n",accel_lim);
        //    ebox_printf("decel_val:%d(%d)\n",srd.decel_val,-10);
        //    ebox_printf("decel_start:%d\n",srd.decel_start);
        //    ebox_printf("decel_start:%d\n",srd.run_state);
        //    ebox_printf_flush();
        mode_init_oc_toggle();
        set_ccr(100);
        interrupt(ENABLE);

    }

}

void StepMotor::base_init(uint16_t period, uint16_t prescaler)
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

    TIM_ARRPreloadConfig(TIMx, ENABLE);
    TIM_Cmd(TIMx, ENABLE); //

}
//duty:0-1000对应0%-100.0%
void StepMotor::mode_init_oc_toggle()
{


    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    switch(ch)
    {
    case TIM_Channel_1:
        TIM_OC1Init(TIMx, &TIM_OCInitStructure);
        break;
    case TIM_Channel_2:
        TIM_OC2Init(TIMx, &TIM_OCInitStructure);
        break;
    case TIM_Channel_3:
        TIM_OC3Init(TIMx, &TIM_OCInitStructure);
        break;
    case TIM_Channel_4:
        TIM_OC4Init(TIMx, &TIM_OCInitStructure);
        break;
    }
}
void StepMotor::mode_init_oc_timing()
{


    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    switch(ch)
    {
    case TIM_Channel_1:
        TIM_OC1Init(TIMx, &TIM_OCInitStructure);
        break;
    case TIM_Channel_2:
        TIM_OC2Init(TIMx, &TIM_OCInitStructure);
        break;
    case TIM_Channel_3:
        TIM_OC3Init(TIMx, &TIM_OCInitStructure);
        break;
    case TIM_Channel_4:
        TIM_OC4Init(TIMx, &TIM_OCInitStructure);
        break;
    }
}

void StepMotor::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority)
{
    nvic_dev_set_priority((uint32_t)TIMx, 0, 0, 0);
    if(enable != DISABLE)
        nvic_dev_enable((uint32_t)TIMx, 0);
    else
        nvic_dev_disable((uint32_t)TIMx, 0);
}
void StepMotor::interrupt(FunctionalState enable)
{
    //TIM_ClearITPendingBit(TIMx , TIM_FLAG_Update);//必须加，否则开启中断会立即产生一次中断
    switch(ch)
    {
    case TIM_Channel_1:
        TIM_ITConfig(TIMx, TIM_IT_CC1, enable);
        break;
    case TIM_Channel_2:
        TIM_ITConfig(TIMx, TIM_IT_CC2, enable);
        break;
    case TIM_Channel_3:
        TIM_ITConfig(TIMx, TIM_IT_CC3, enable);
        break;
    case TIM_Channel_4:
        TIM_ITConfig(TIMx, TIM_IT_CC4, enable);
        break;
    }

}
//duty:0-1000对应0%-100.0%
void StepMotor::set_ccr(uint16_t diff)
{

    ccr = diff + TIMx->CNT;
    if(ccr > 0xffff)
        ccr -= 0xffff;
    switch(ch)
    {
    case TIM_Channel_1:
        TIMx->CCR1 = ccr;
        break;
    case TIM_Channel_2:
        TIMx->CCR2 = ccr;
        break;
    case TIM_Channel_3:
        TIMx->CCR3 = ccr;
        break;
    case TIM_Channel_4:
        TIMx->CCR4 = ccr;
        break;
    }

}

void StepMotor::init_info(Gpio *pin)
{
    switch((uint8_t)pin->id)
    {
    case PA0_ID :
        //            TIMx = TIM2;
        ch = TIM_Channel_1;//irq = TIM2_IRQn;
        break;
    case PA1_ID:
        //            TIMx = TIM2;
        ch = TIM_Channel_2;//irq = TIM2_IRQn;
        break;
    case PA2_ID:
        //            TIMx = TIM2;
        ch = TIM_Channel_3;//irq = TIM2_IRQn;
        break;
    case PA3_ID:
        //            TIMx = TIM2;
        ch = TIM_Channel_4;//irq = TIM2_IRQn;
        break;

    case PA6_ID:
        //            TIMx = TIM3;
        ch = TIM_Channel_1;//irq = TIM3_IRQn;
        break;
    case PA7_ID:
        //            TIMx = TIM3;
        ch = TIM_Channel_2;//irq = TIM3_IRQn;
        break;


    case PB0_ID:
        //            TIMx = TIM3;
        ch = TIM_Channel_3;//irq = TIM3_IRQn;
        break;
    case PB1_ID:
        //            TIMx = TIM3;
        ch = TIM_Channel_4;//irq = TIM3_IRQn;
        break;

    case PB6_ID:
        //            TIMx = TIM4;
        ch = TIM_Channel_1;//irq = TIM4_IRQn;
        break;
    case PB7_ID:
        //            TIMx = TIM4;
        ch = TIM_Channel_2;//irq = TIM4_IRQn;
        break;
    case PB8_ID:
        //            TIMx = TIM4;
        ch = TIM_Channel_3;//irq = TIM4_IRQn;
        break;
    case PB9_ID:
        //            TIMx = TIM4;
        ch = TIM_Channel_4;//irq = TIM4_IRQn;
        break;
    }


}
uint32_t StepMotor::get_timer_source_clock()
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



void StepMotor::callback()
{
    static uint8_t counter = 0 ;
    static uint32_t step_counter = 0 ;
    // Holds next delay period.
    static uint16_t new_step_delay;
    // Remember the last step delay used when accelrating.
    static uint16_t last_accel_delay;
    // Counting steps when moving.
    static uint32_t step_count = 0;
    // Keep track of remainder from new_step-delay calculation to incrase accurancy
    static int32_t rest = 0;


    set_ccr(srd.step_delay);

    counter++;
    step_counter++;
    //    uart1.printf("%d(%d)\t",step_counter,ch);
    if(counter >= 2)
    {
        switch(srd.run_state)
        {
        case STOP:
            //                ebox_printf("STOP%D\t",step_count);
            step_count = 0;
            rest = 0;
            // Stop Timer/Counter 1.
            //TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));
            interrupt(DISABLE);
            mode_init_oc_timing();
            status = false;
            break;

        case ACCEL:
            //sm_driver_StepCounter(srd.dir);
            step_count++;
            //                ebox_printf("ACCEL:%D\t",step_count);
            srd.accel_count++;
            new_step_delay = srd.step_delay - (((2 * (long)srd.step_delay) + rest) / (4 * srd.accel_count + 1));
            rest = ((2 * (long)srd.step_delay) + rest) % (4 * srd.accel_count + 1);
            // Chech if we should start decelration.
            if(step_count >= srd.decel_start)
            {
                //                    ebox_printf("ACCEL TO DECEL:%D\t",step_count);
                srd.accel_count = srd.decel_val;
                srd.run_state = DECEL;
            }
            // Chech if we hitted max speed.
            else if(new_step_delay <= srd.min_delay)
            {
                //                    ebox_printf("ACCEL TO RUN:%D\t",step_count);
                last_accel_delay = new_step_delay;
                new_step_delay = srd.min_delay;
                rest = 0;
                srd.run_state = RUN;
            }
            break;

        case RUN:
            //sm_driver_StepCounter(srd.dir);
            step_count++;
            //                ebox_printf("RUN :%D\t",step_count);
            new_step_delay = srd.min_delay;
            // Chech if we should start decelration.
            if(step_count >= srd.decel_start)
            {
                //                    ebox_printf("RUN TO DECEL:%D\t",step_count);
                srd.accel_count = srd.decel_val;
                // Start decelration with same delay as accel ended with.
                new_step_delay = last_accel_delay;
                srd.run_state = DECEL;
            }
            break;

        case DECEL:
            //sm_driver_StepCounter(srd.dir);
            step_count++;
            //                ebox_printf("DECEL:%D\t",step_count);
            srd.accel_count++;
            new_step_delay = srd.step_delay - ((long)((2 * srd.step_delay) + rest) / (4 * srd.accel_count + 1));
            rest = ((long)(2 * srd.step_delay) + rest) % (4 * srd.accel_count + 1);
            // Check if we at last step
            if(srd.accel_count >= 0)
            {
                srd.run_state = STOP;
            }
            break;
        }
        srd.step_delay = new_step_delay;

        //        uart1.printf(":%d\n",srd.step_delay);
        counter = 0;
    }
    //    ebox_printf("\n");


}



void StepMotor::_irq_handler( uint32_t id)
{
    StepMotor *handler = (StepMotor *)id;
    handler->_irq.call();
}

void StepMotor::attach(void (*fptr)(void))
{
    if (fptr)
    {
        _irq.attach(fptr);
    }
}


uint32_t StepMotor::sqrt(u32 x)//开方
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
