#ifndef __cnc_h
#define __cnc_h
#include "ebox.h"


//����mm
#define MM_PER_STEP 0.01 //mm

//����ٶ��ɶ�ʱ����С�ֱ��ʣ�����������(10*TIME_UNIT)���Լ�MCU��仺�����ٶȣ���������С�����ص��ۺ����ơ�
//����Ҫ��һ���ο���MCU�ܲ����ڶ�ʱ���ж�������ʵʱ�ĸ��²���������л�������
//���綨ʱ������Ƶ��1Mhz��100�����������ж�һ�Ρ�MCU���������100us���һ�����л�������
#define MAX_SPEED_MM_MIN    6000.0    //mm/min
//�����ٶ��ɻ�е�ṹ���ơ�
#define MAX_ACC_S           60.0       //(mm/s^2)







//��С�ٶ��ɶ�ʱ�����ֵ���ܳ���65535���ƣ�65535*TIME_UNITʱ��������һ����
#define MIN_SPEED_MM_MIN    (MM_PER_STEP*60.0)/(65535.0*TIME_UNIT)    //mm/min


#define MIN_SPEED_MM_S      (MIN_SPEED_MM_MIN/60.0)    //mm/min
#define MAX_SPEED_MM_S      (MAX_SPEED_MM_MIN/60.0)    //mm/min


//��С���ٶ���c0���ֵ���ܳ���65535���ơ�
#define MIN_ACC_S           (2.0*MM_PER_STEP)/(65535.0*65535.0*TIME_UNIT_POWER)               //(mm/s^2)

#define MIN_ACC_MIN         (MIN_ACC_S*3600.0)  //(mm/min^2)
#define MAX_ACC_MIN         (MAX_ACC_S*3600.0)  //(mm/min^2)
typedef struct
{
    float position[3];
    float acc;
    
    
    uint32_t accelerate_until;
    uint32_t decelerate_after;
    uint32_t delta_steps[3];
    uint32_t delta_mm[3];
    
    uint32_t c0;

}CncBlock_t;
class CNC
{
    public:
        CNC()
        {
            position[X_AXIS] = 0;
            position[Y_AXIS] = 0;
            position[Z_AXIS] = 0;
        };
        void        move(float *new_position);
        void        move_signal_to(uint8_t Axis,float new_x);
        uint32_t    mm_to_steps(float mm);
        float       step_to_mm(uint32_t steps);
        
        
        void set_speed(float speed);

        // Calculates the distance (not time) it takes to accelerate from initial_rate to target_rate using the 
        // given acceleration:
        static float estimate_acceleration_distance(float initial_rate, float target_rate, float acceleration) 
        {
          return( (target_rate*target_rate-initial_rate*initial_rate)/(2*acceleration) );
        }
        uint32_t calculate_c0(uint32_t acc_per_min)
        {
            float acc = acc_per_min / 3600.0;
            uint32_t c0;
            c0 = sqrt(2*MM_PER_STEP/(acc*TIME_UNIT_POWER));
            return c0;
        }
        void block_planner()
        {
        
        }
        
        void print_info()
        {
            uart1.printf("\r\n===========Welcome Free CNC(V1.0)==============\r\n");
            uart1.printf("Steper SPR:      %d(steps per round)\r\n",STEPER_SPR);
            uart1.printf("Steper SPR*DIV:  %d(steps per round)\r\n",STEPER_SPR*STEPER_DIV);
            uart1.printf("Steper Radian:   %0.5fr(%0.1f��)\r\n",RADIAN_PER_STEP,ANGLE_PER_STEP);
            uart1.printf("CNC mm per step: %0.3fmm(%0.0fum)\r\n",MM_PER_STEP,MM_PER_STEP*1000);

            uart1.printf("Time unit:  %6.1fus\r\n",TIME_UNIT*1e6);
            
            uart1.printf("Min Acc:  %6.3fmm/s^2   \t   Max Acc:   %6.3fmm/s^2   \r\n",MIN_ACC_S,MAX_ACC_S);
            uart1.printf("Min Acc:  %6.3fmm/min^2 \t   Max Acc:   %6.3fmm/min^2 \r\n",MIN_ACC_MIN,MAX_ACC_MIN);
            uart1.printf("Min Speed:%6.3fmm/s     \t   Max Speed: %6.3fmm/s\r\n",MIN_SPEED_MM_S,MAX_SPEED_MM_S);
            uart1.printf("Min Speed:%6.3fmm/minute\t   Max Speed: %6.3fmm/minute\r\n",MIN_SPEED_MM_MIN,MAX_SPEED_MM_MIN);
            uart1.printf("===================================================\r\n");

        }
    
    public:
        float position[3];
        float speed;
        float current_speed;
        float acc;
};


#endif


