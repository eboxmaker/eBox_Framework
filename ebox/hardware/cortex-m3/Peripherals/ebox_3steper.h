#ifndef __3steper_h
#define __3steper_h

#include "ebox_core.h"
#include "mcu.h"
#include "FunctionPointer.h"

//! Number of (full)steps per round on stepper motor in use.
#define SPR 200 

//步进角
#define RADIAN_PER_STEP (2*PI/SPR)                    // 2*pi/spr


//typedef struct 
//{
//    uint32_t 

//}block_t;
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define X_STEMP_BIT 0
#define Y_STEMP_BIT 1
#define Z_STEMP_BIT 2
#define X_DIR_BIT   3
#define Y_DIR_BIT   4
#define Z_DIR_BIT   5


class Steper
{
    public:
        Steper(TIM_TypeDef *Timer)
        {
            TIMx        = Timer;
            status = false;
            cycleRing.begin(32);
            bitsRing.begin(32);
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
        bool write_buffer(uint16_t counter,uint8_t ctr);
        
        
        void move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed);
        
        
        
        void callback();
        static void _irq_handler( uint32_t id);
        void attach(void (*fptr)(void));
        template<typename T>
        void attach(T* tptr, void (T::*mptr)(void)) {
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
