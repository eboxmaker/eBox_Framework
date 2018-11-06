#ifndef __cnc_h
#define __cnc_h
#include "ebox.h"

///////////////////可设置参数///////////////////////////////////
//步进mm
#define MM_PER_STEP (double)0.01 //mm

#define MAX_ACC_S   (double)50.00       //(mm/s^2)
////////////////////////////////////////////////////////////////


//最大速度由定时器最小周期。
#define MAX_SPEED_MM_MIN    (double)(MAX_RPM*(STEPER_SPR*STEPER_DIV)*MM_PER_STEP)    //mm/min
//最小速度由定时器最大值不能超过65535限制，65535*TIME_UNIT时长必须走一步。
#define MIN_SPEED_MM_MIN    (double)(MIN_RPM*(STEPER_SPR*STEPER_DIV)*MM_PER_STEP)    //mm/min


#define MIN_SPEED_MM_S      (double)(MIN_SPEED_MM_MIN/60.0)    //mm/min
#define MAX_SPEED_MM_S      (double)(MAX_SPEED_MM_MIN/60.0)    //mm/min


//最小加速度由c0最大值不能超过65535限制。a = 2*s/t^2
#define MIN_ACC_S           (double)(2.0*MM_PER_STEP)/(65535.0*65535.0*TIME_UNIT_POW)               //(mm/s^2)

#define MIN_ACC_MIN         (double)(MIN_ACC_S*3600.0)  //(mm/min^2)
#define MAX_ACC_MIN         (double)(MAX_ACC_S*3600.0)  //(mm/min^2)
typedef struct
{
    //位置参数
    double      position[3];//目的位置

    //速度参数
    double      acc;//加速度mm/min^2
    double      speed;//速度mm/min

    //计算结果
    uint32_t    accelerate_until;
    uint32_t    decelerate_after;
    uint32_t    delta_steps[3];
    double      delta_mm[3];
    uint32_t    max_delta_steps;

    uint32_t    c0;//根据速度参数计算的第一步的计数器溢出值
    uint32_t    step_planed;//已经计算的点数
    double      vn[3];//根据速度参数计算的第n步的速度值
    uint8_t     ctr_bits;//电机控制字

} CncBlock_t;
class CNC
{
public:
    CNC() {};

    //初始化
    void        begin();

    //设置运动速度参数
    void        set_acc(double _acc);
    void        set_speed(double _speed);
    //运动位置模式控制
    void        move(double *new_position);
    void        move_signal_to(uint8_t Axis, double new_x);

    void        draw_line(CncBlock_t *block);
    uint8_t     judge_quadrant(double x_center, double y_center, double x, double y);
    void        dda_circle(double x_center, double y_center, double XStart, double YStart, double XEnd, double YEnd, double radius, int blsCW);
    void        dda_circle( double XEnd, double YEnd, double i,  double j, int blsCW);
    int         guaxiang(double x, double y);
    int         _guaxiang(double x, double y);


    //计算位置运动block参数
    void        calculate_block(CncBlock_t *block);
    double      estimate_acceleration_distance(double initial_rate, double target_rate, double acceleration);
    double      intersection_distance(double initial_rate, double final_rate, double acceleration, double distance);
    uint32_t    calculate_cn(CncBlock_t *block);
    uint32_t    mm_to_steps(double mm);
    double      step_to_mm(uint32_t steps);


    //状态
    bool        is_motion_over();
    bool        is_block_over(CncBlock_t *block);
    double      get_vector_speed();
    double      get_speed();

    //被定时器中断调用更新位置
    void        update_position(uint8_t ctr_bists);

    //log
    void print_position();
    void print_info();
    uint32_t position_step[3];
    double position[3];

private:
    double acc;
    double speed;
    double current_speed;
    uint32_t steps_completed_evnets;
    uint32_t steps_total_events;
};


#endif


