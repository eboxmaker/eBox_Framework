#ifndef __cnc_h
#define __cnc_h
#include "ebox.h"


//²½½ømm
#define MM_PER_STEP 0.01 //mm

#define MAX_SPEED   1000//mm/min

class CNC
{
    public:
        CNC()
        {
            position[X_AXIS] = 0;
            position[Y_AXIS] = 0;
            position[Z_AXIS] = 0;
        };
        void move(float *new_position);
        void move_x_to(float new_x);
        void move_y_to(float new_y);
        uint32_t    mm_to_steps(float mm);
        float       step_to_mm(uint32_t steps);
        
        
        void set_speed(float speed);

// Calculates the distance (not time) it takes to accelerate from initial_rate to target_rate using the 
// given acceleration:
static float estimate_acceleration_distance(float initial_rate, float target_rate, float acceleration) 
{
  return( (target_rate*target_rate-initial_rate*initial_rate)/(2*acceleration) );
}
    
    public:
        float position[3];
        float speed;
        float current_speed;
        float acc;
        float max_acc;//mm/min
};


#endif


