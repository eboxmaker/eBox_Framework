#ifndef __3steper_h
#define __3steper_h

#include "ebox_core.h"
#include "mcu.h"
#include "FunctionPointer.h"

///////////////////可设置参数///////////////////////////////////

//! Number of (full)steps per round on stepper motor in use.
#define STEPER_SPR      200             //电机一圈需要走多少步
#define STEPER_DIV      2               //电机的细分控制
#define TIME_UNIT       (1e-6)          //单位：s；描述了定时器的时钟周期
//定时器最小周期，驱动脉冲宽度(10*TIME_UNIT)，以及MCU填充缓冲区速度，缓冲区大小，机械特性等因素的综合限制。
//最重要的参考：
//一、MCU能不能在定时器中断周期内实时的更新步进电机运行缓冲区。
//比如定时器计数频率1Mhz。100个计数周期中断一次。MCU则必须满足100us填充一次运行缓冲区。
//二、电机在特定负载下的最高速度，硬件驱动的最快相应速度
#define MIN_PERIOD      (500*TIME_UNIT)  //单位：s。决定了定时器最高的中断周期，进而电机的最高速度
#define MAX_PERIOD      (65535*TIME_UNIT)//单位：s
//////////////////////////////////////////////////////////////

//步进角
#define RADIAN_PER_STEP (2.0*PI/(STEPER_SPR*STEPER_DIV))  // 2*pi/spr
#define ANGLE_PER_STEP (360.0/(STEPER_SPR*STEPER_DIV))  // 2*pi/spr

#define TIME_UNIT_POW   (TIME_UNIT*TIME_UNIT)            //单位：s；描述了定时器的时钟周期

#define MAX_RPM         (60/MIN_PERIOD)/(STEPER_SPR*STEPER_DIV)//一分钟走NSteps/StepsPerRound圈
#define MIN_RPM         (60/MAX_PERIOD)/(STEPER_SPR*STEPER_DIV)//一分钟走NSteps/StepsPerRound圈
//typedef struct
//{
//    uint32_t

//}block_t;
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define X_STEP_BIT  0
#define Y_STEP_BIT  1
#define Z_STEPE_BIT 2
#define X_DIR_BIT   3
#define Y_DIR_BIT   4
#define Z_DIR_BIT   5

#define X_STEP_BIT_MASK  (1<<0)
#define Y_STEP_BIT_MASK  (1<<1)
#define Z_STEPE_BIT_MASK (1<<2)
#define X_DIR_BIT_MASK   (1<<3)
#define Y_DIR_BIT_MASK   (1<<4)
#define Z_DIR_BIT_MASK   (1<<5)
class Steper
{
public:
    Steper(TIM_TypeDef *Timer)
    {
        TIMx        = Timer;
        status = false;

    };
    void begin();

    void base_init(uint16_t period, uint16_t prescaler);
    void mode_init_oc_pwm(void);
    void set_ccr(uint16_t ccr);
    void nvic(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);
    void interrupt(FunctionalState enable);
    void timer_start();
    void timer_stop();
    uint32_t get_timer_source_clock();
    bool write_buffer(uint16_t counter, uint8_t ctr);


    void move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed);

    void print_info()
    {

    }

    void callback();
    static void _irq_handler( uint32_t id);
    void attach(void (*fptr)(void));
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void))
    {
        _irq.attach(tptr, mptr);
    }


public:
    RingBufUint16   cycleRing;
    RingBufUint8    bitsRing;
    uint8_t         ctr_bits;
    bool status;
    Gpio        *Xpwm;
    Gpio        *Ypwm;
    Gpio        *Zpwm;
    Gpio        *Xdir;
    Gpio        *Ydir;
    Gpio        *Zdir;
    Gpio        *Xen;
    Gpio        *Yen;
    Gpio        *Zen;

private:
    uint32_t sqrt(u32 x);//开方


private:

    TIM_TypeDef *TIMx;
    uint8_t     ch;
    uint16_t    period;//保存溢出值，用于计算占空比
    uint16_t    prescale;//保存占空比值
    uint32_t    ccr;//保存占空比值



protected:
    FunctionPointer _irq;

};
extern Steper steper;

#endif
