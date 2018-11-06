#ifndef __EBOX_STEP_MOTOR_H
#define __EBOX_STEP_MOTOR_H
#include "ebox_core.h"
#include "mcu.h"
#include "FunctionPointer.h"



#define FULLSTEPS

#define  STEPMOTOR_TIM_FRQ 1000000


/*! \brief Holding data used by timer interrupt for speed ramp calculation.
 *
 *  Contains data used by timer interrupt to calculate speed profile.
 *  Data is written to it by move(), when stepper motor is moving (timer
 *  interrupt running) data is read/updated when calculating a new step_delay
 */
typedef struct
{
    //! What part of the speed ramp we are in.
    unsigned char run_state ;
    //! Direction stepper motor should move.
    unsigned char dir ;
    //! Peroid of next timer delay. At start this value set the accelration rate.
    uint32_t step_delay;
    //! What step_pos to start decelaration
    uint32_t decel_start;
    //! Sets deceleration rate.
    int32_t decel_val;
    //! Minimum time delay (max speed)
    int32_t min_delay;
    //! Counter used when accelerateing/decelerateing to calculate step_delay.
    int32_t accel_count;
} speedRampData;
/*! \Brief Frequency of timer1 in [Hz].
 *
 * Modify this according to frequency used. Because of the prescaler setting,
 * the timer1 frequency is the clock frequency divided by 8.
 */
// Timer/Counter 1 running on 3,686MHz / 8 = 460,75kHz (2,17uS). (T1-FREQ 460750)
#define T1_FREQ STEPMOTOR_TIM_FRQ

//! Number of (full)steps per round on stepper motor in use.
#define FSPR 1600

#ifdef HALFSTEPS
#define SPR (FSPR*2)
#pragma message("[speed_cntr.c] *** Using Halfsteps ***")
#endif
#ifdef FULLSTEPS
#define SPR FSPR
//#pragma message("[speed_cntr.c] *** Using Fullsteps ***")
#endif
#ifndef HALFSTEPS
#ifndef FULLSTEPS
#error FULLSTEPS/HALFSTEPS not defined!
#endif
#endif

// Maths constants. To simplify maths when calculating in speed_cntr_Move().
#define ALPHA (2*PI/SPR)                    // 2*pi/spr
#define A_T_x100 ((long)(ALPHA*T1_FREQ*100))     // (ALPHA / T1_FREQ)*100
#define T1_FREQ_148 ((int)((T1_FREQ*0.676)/100)) // divided by 100 and scaled by 0.676
#define A_SQ (long)(ALPHA*2*10000000000)         // ALPHA*2*10000000000
#define A_x20000 (int)(ALPHA*20000)              // ALPHA*20000

// Speed ramp states
#define STOP  0
#define ACCEL 1
#define DECEL 2
#define RUN   3


#define CW  0
#define CCW 1

class StepMotor
{
public:
    StepMotor(TIM_TypeDef *Timer, Gpio *pin)
    {
        TIMx        = Timer;
        this->pin   = pin;
        status = false;
    };
    void begin();
    void base_init(uint16_t period, uint16_t prescaler);





    void set_frq(uint32_t frq);
    void init_info(Gpio *pin);
    uint32_t get_timer_source_clock();
    void mode_init_oc_toggle(void);
    void mode_init_oc_timing(void);
    void set_ccr(uint16_t ccr);

    void nvic(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);
    void interrupt(FunctionalState enable);

    void move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed);



    void callback();


    static void _irq_handler( uint32_t id);
    void attach(void (*fptr)(void));
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void))
    {
        _irq.attach(tptr, mptr);
    }


public:
    bool status;
    speedRampData srd;

private:
    uint32_t sqrt(u32 x);//开方


private:

    TIM_TypeDef *TIMx;
    Gpio        *pin;
    uint8_t     ch;
    uint16_t    period;//保存溢出值，用于计算占空比
    uint16_t    prescale;//保存占空比值
    uint32_t    ccr;//保存占空比值


protected:
    FunctionPointer _irq;

};

#endif
