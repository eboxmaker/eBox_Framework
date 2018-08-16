#ifndef __3steper_h
#define __3steper_h

#include "ebox_core.h"
#include "mcu.h"
#include "FunctionPointer.h"

///////////////////�����ò���///////////////////////////////////

//! Number of (full)steps per round on stepper motor in use.
#define STEPER_SPR      200             //���һȦ��Ҫ�߶��ٲ�
#define STEPER_DIV      2               //�����ϸ�ֿ���
#define TIME_UNIT       (1e-6)          //��λ��s�������˶�ʱ����ʱ������
//��ʱ����С���ڣ�����������(10*TIME_UNIT)���Լ�MCU��仺�����ٶȣ���������С����е���Ե����ص��ۺ����ơ�
//����Ҫ�Ĳο���
//һ��MCU�ܲ����ڶ�ʱ���ж�������ʵʱ�ĸ��²���������л�������
//���綨ʱ������Ƶ��1Mhz��100�����������ж�һ�Ρ�MCU���������100us���һ�����л�������
//����������ض������µ�����ٶȣ�Ӳ�������������Ӧ�ٶ�
#define MIN_PERIOD      (500*TIME_UNIT)  //��λ��s�������˶�ʱ����ߵ��ж����ڣ��������������ٶ�
#define MAX_PERIOD      (65535*TIME_UNIT)//��λ��s
//////////////////////////////////////////////////////////////

//������
#define RADIAN_PER_STEP (2.0*PI/(STEPER_SPR*STEPER_DIV))  // 2*pi/spr
#define ANGLE_PER_STEP (360.0/(STEPER_SPR*STEPER_DIV))  // 2*pi/spr

#define TIME_UNIT_POW   (TIME_UNIT*TIME_UNIT)            //��λ��s�������˶�ʱ����ʱ������

#define MAX_RPM         (60/MIN_PERIOD)/(STEPER_SPR*STEPER_DIV)//һ������NSteps/StepsPerRoundȦ
#define MIN_RPM         (60/MAX_PERIOD)/(STEPER_SPR*STEPER_DIV)//һ������NSteps/StepsPerRoundȦ
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
        bool write_buffer(uint16_t counter,uint8_t ctr);
        
        
        void move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed);
        
        void print_info()
        {
        
        }
        
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
        uint32_t sqrt(u32 x);//����


    private:

        TIM_TypeDef *TIMx;
        uint8_t     ch;
        uint16_t    period;//�������ֵ�����ڼ���ռ�ձ�
        uint16_t    prescale;//����ռ�ձ�ֵ
        uint32_t    ccr;//����ռ�ձ�ֵ
    
    
    
    protected:
        FunctionPointer _irq;

};
extern Steper steper;

#endif
