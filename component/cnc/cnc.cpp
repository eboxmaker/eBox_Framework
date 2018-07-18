#include "cnc.h"


int32_t counter = 22100;
uint32_t i;
uint16_t acc_untill;
uint16_t acc_delta=200;


// Calculates the distance (not time) it takes to accelerate from initial_rate to target_rate using the 
// given acceleration:
//估计加速度距离
static float estimate_acceleration_distance(float initial_rate, float target_rate, float acceleration) 
{
  return( (target_rate*target_rate-initial_rate*initial_rate)/(2*acceleration) );
}

/*                        + <- some maximum rate we don't care about
                         /|\
                        / | \                    
                       /  |  + <- final_rate     
                      /   |  |                   
     initial_rate -> +----+--+                   
                          ^  ^                   
                          |  |                   
      intersection_distance  distance                                                                           */
// This function gives you the point at which you must start braking (at the rate of -acceleration) if 
// you started at speed initial_rate and accelerated until this point and want to end at the final_rate after
// a total travel of distance. This can be used to compute the intersection point between acceleration and
// deceleration in the cases where the trapezoid has no plateau (i.e. never reaches maximum speed)
//相交距离
static float intersection_distance(float initial_rate, float final_rate, float acceleration, float distance) 
{
  return( (2*acceleration*distance-initial_rate*initial_rate+final_rate*final_rate)/(4*acceleration) );
}

CncBlock_t block;
void draw_line( uint32_t x1, uint32_t y1)
{
    uint8_t temp_ctr_bit = 0;
    uint32_t x0 = 0;
    uint32_t y0 = 0;
    int32_t dx,             // difference in x's
        dy,             // difference in y's
        dx2,            // dx,dy * 2
        dy2,
        x_inc,          // amount in pixel space to move during drawing
        y_inc,          // amount in pixel space to move during drawing
        error,          // the discriminant i.e. error i.e. decision variable
        index;          // used for looping


   // set_xy(x0, y0);
    dx = x1 - x0; //计算x距离
    dy = y1 - y0; //计算y距离

    if (dx >= 0){ x_inc = 1; }
    else{ x_inc = -1; dx = -dx; }

    if (dy >= 0){ y_inc = 1; }
    else{ y_inc = -1; dy = -dy; }

    dx2 = dx << 1;
    dy2 = dy << 1;
    steper.timer_start();

    if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
    {
        acc_untill = dx/2;
        //且线的点数等于x距离，以x轴递增画点
        // initialize error term
        error = dy2 - dx;

        // draw the line
        for (index = 0; index <= dx; index++) //要画的点数不会超过x距离
        {
            //画点
            //draw_pixel(x0, y0, color);

            // test if error has overflowed
            if (error >= 0) //是否需要增加y坐标值
            {
                error -= dx2;

                // move to next line
                y0 += y_inc; //增加y坐标值
                bit_set(temp_ctr_bit,Y_STEP_BIT);

            } // end if error overflowed
            else
            {
                bit_clear(temp_ctr_bit,Y_STEP_BIT);
            }

            // adjust the error term
            error += dy2;

            // move to the next pixel
            x0 += x_inc; //x坐标值每次画点后都递增1
            bit_set(temp_ctr_bit,X_STEP_BIT);
            while(steper.bitsRing.isfull())
            {
                //do run time
            }
            steper.write_buffer(counter,temp_ctr_bit);
            if(index < acc_untill)
                counter -= acc_delta;
            else
                counter += acc_delta;

            uart1.printf("TIMER:BIT:%D\r\n",steper.cycleRing.available());
            uart1.printf("X>Y:%05d:counter:%05d(%02d)\r\n",index,counter,temp_ctr_bit);
        } // end for
    } // end if |slope| <= 1
    else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
    {
        acc_untill = dy/2;
        //以y轴为递增画点
        // initialize error term
        error = dx2 - dy;

        // draw the line
        for (index = 0; index <= dy; index++)
        {
            // set the pixel
            //draw_pixel(x0, y0, color);

            // test if error overflowed
            if (error >= 0)
            {
                error -= dy2;

                // move to next line
                x0 += x_inc;
                bit_set(temp_ctr_bit,X_STEP_BIT);

            } // end if error overflowed
            else
            {
                bit_clear(temp_ctr_bit,X_STEP_BIT);
            }

            // adjust the error term
            error += dx2;

            // move to the next pixel
            y0 += y_inc;
            bit_set(temp_ctr_bit,Y_STEP_BIT);
            while(steper.bitsRing.isfull())
            {
                //do run time
            }
            steper.write_buffer(counter,temp_ctr_bit);
            if(index < acc_untill)
                counter -= acc_delta;
            else
                counter += acc_delta;
            uart1.printf("TIMER:BIT:%D\r\n",steper.cycleRing.available());
            uart1.printf("Y>X:%05d:counter:%05d(%02d)\r\n",index,counter,temp_ctr_bit);
        } // end for
    } // end else |slope| > 1
}
void draw_line(CncBlock_t *block)
{
    uint8_t temp_ctr_bit = 0;
    uint32_t x0 = 0;
    uint32_t y0 = 0;
    int32_t dx,             // difference in x's
        dy,             // difference in y's
        dx2,            // dx,dy * 2
        dy2,
        x_inc,          // amount in pixel space to move during drawing
        y_inc,          // amount in pixel space to move during drawing
        error,          // the discriminant i.e. error i.e. decision variable
        index;          // used for looping


    uint32_t steps_completed = 0;
    temp_ctr_bit = block->ctr_bits;
    counter = block->c0;
   // set_xy(x0, y0);
    dx = block->delta_steps[X_AXIS]; //计算x距离
    dy = block->delta_steps[Y_AXIS]; //计算y距离

    if(block->ctr_bits & X_DIR_BIT_MASK){ x_inc = 1; }
    else{ x_inc = -1; }
    if(block->ctr_bits & Y_DIR_BIT_MASK){ y_inc = 1; }
    else{ y_inc = -1; }


    dx2 = dx << 1;
    dy2 = dy << 1;
    steper.timer_start();
    steps_completed++;
    if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
    {
        //且线的点数等于x距离，以x轴递增画点
        // initialize error term
        error = dy2 - dx;

        // draw the line
        for (index = 0; index < dx; index++) //要画的点数不会超过x距离
        {
            //画点
            //draw_pixel(x0, y0, color);

            // test if error has overflowed
            if (error >= 0) //是否需要增加y坐标值
            {
                error -= dx2;

                // move to next line
                y0 += y_inc; //增加y坐标值
                bit_set(temp_ctr_bit,Y_STEP_BIT);

            } // end if error overflowed
            else
            {
                bit_clear(temp_ctr_bit,Y_STEP_BIT);
            }

            // adjust the error term
            error += dy2;

            // move to the next pixel
            x0 += x_inc; //x坐标值每次画点后都递增1
            bit_set(temp_ctr_bit,X_STEP_BIT);
            if(steps_completed < block->accelerate_until)
            {
                counter = block->c0*(sqrt(float(steps_completed+1)) - sqrt(float(steps_completed)));
                uart1.printf("X>Y:%05d\t+\t%05d\t0x%2x\r\n",index,counter,temp_ctr_bit);

            }
            else if(steps_completed < block->decelerate_after)
            {
                counter = counter;
                uart1.printf("X>Y:%05d\t=\t%05d\t0x%2x\r\n",index,counter,temp_ctr_bit);

            }
            else
            {
                counter = block->c0*(sqrt(float(block->delta_steps[X_AXIS] - steps_completed + 1)) - sqrt(float(block->delta_steps[X_AXIS] - steps_completed)));
                uart1.printf("X>Y:%05d\t-\t%05d\t0x%2x\r\n",index,counter,temp_ctr_bit);

            }
            while(steper.bitsRing.isfull())
            {
                //do run time
            }
            steper.write_buffer(counter,temp_ctr_bit);


            steps_completed++;

        } // end for
    } // end if |slope| <= 1
    else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
    {
        //以y轴为递增画点
        // initialize error term
        error = dx2 - dy;

        // draw the line
        for (index = 0; index < dy; index++)
        {
            // set the pixel
            //draw_pixel(x0, y0, color);

            // test if error overflowed
            if (error >= 0)
            {
                error -= dy2;

                // move to next line
                x0 += x_inc;
                bit_set(temp_ctr_bit,X_STEP_BIT);

            } // end if error overflowed
            else
            {
                bit_clear(temp_ctr_bit,X_STEP_BIT);
            }

            // adjust the error term
            error += dx2;

            // move to the next pixel
            y0 += y_inc;
            bit_set(temp_ctr_bit,Y_STEP_BIT);
            
            
            
            
            if(steps_completed < block->accelerate_until)
            {
                counter = block->c0*(sqrt(float(steps_completed+1)) - sqrt(float(steps_completed)));
                uart1.printf("Y>X:%05d\t+\t%05d\t0x%2x\r\n",index,counter,temp_ctr_bit);

            }
            else if(steps_completed < block->decelerate_after)
            {
                counter = counter;
                uart1.printf("Y>X:%05d\t=\t%05d\t0x%2x\r\n",index,counter,temp_ctr_bit);

            }
            else
            {
                counter = block->c0*(sqrt(float(block->delta_steps[Y_AXIS] - steps_completed + 1)) - sqrt(float(block->delta_steps[Y_AXIS] - steps_completed)));
                uart1.printf("Y>X:%05d\t-\t%05d\t0x%2x\r\n",index,counter,temp_ctr_bit);

            }
            while(steper.bitsRing.isfull())
            {
                //do run time
            }
            steper.write_buffer(counter,temp_ctr_bit);


            steps_completed++;        } // end for
    } // end else |slope| > 1
}

void CNC::move(float *new_position)
{
    float delta_mm[3];
    uint32_t delta_steps[3];
    delta_mm[X_AXIS] = new_position[X_AXIS] - this->position[X_AXIS];
    delta_mm[Y_AXIS] = new_position[Y_AXIS] - this->position[Y_AXIS];
    delta_steps[X_AXIS] = mm_to_steps(delta_mm[X_AXIS]);
    delta_steps[Y_AXIS] = mm_to_steps(delta_mm[Y_AXIS]);

    block.acc = MAX_ACC_MIN;
    block.position[0] = new_position[0];
    block.position[1] = new_position[1];
    block.position[2] = new_position[2];
    calculate_block(&block);
    draw_line(&block);
    
}
void CNC::move_signal_to(uint8_t Axis,float new_value)
{
    uint32_t c0,c;
    uint8_t ctr_bits = 0;
    float delta_mm;
    uint32_t delta_steps,accelerate_until,decelerate_after;
    uint32_t step_counter = 0;
    
    
    delta_mm = new_value - this->position[Axis];
    if(delta_mm == 0)return;
    else if(delta_mm < 0) { bit_clear(ctr_bits,Axis+3);  delta_mm = -delta_mm; }
    else{ bit_set(ctr_bits,Axis+3); }
    delta_steps = mm_to_steps(delta_mm);
    int32_t accelerate_steps = 
        mm_to_steps(estimate_acceleration_distance(0, MAX_SPEED_MM_MIN, MAX_ACC_MIN));
    int32_t decelerate_steps = 
        mm_to_steps(estimate_acceleration_distance(MAX_SPEED_MM_MIN, 0, -MAX_ACC_MIN));    
    // Calculate the size of Plateau of Nominal Rate. 
    int32_t plateau_steps = delta_steps-accelerate_steps-decelerate_steps;
    if (plateau_steps < 0) {  
        accelerate_steps = ceil(
        intersection_distance(0,0, MAX_ACC_MIN, delta_steps));
        accelerate_steps = max(accelerate_steps,0); // Check limits due to numerical round-off
        accelerate_steps = min(accelerate_steps,delta_steps);
        plateau_steps = 0;
    }  
    accelerate_until = accelerate_steps;
    decelerate_after = accelerate_steps+plateau_steps;
    
    c0 = calculate_c0(MAX_ACC_MIN);
    
    uart1.printf("delta_mm:%f;\r\n", delta_mm);
    uart1.printf("all:%05d\r\n", delta_steps);
    uart1.printf("+:%05d\r\n", accelerate_until);
    uart1.printf("=:%05d\r\n", plateau_steps);
    uart1.printf("-:%05d\r\n", decelerate_after);
    uart1.printf("c0:%05d\r\n", c0);


    steper.timer_start();
    step_counter++;
    bit_set(ctr_bits,Axis);
    steper.write_buffer(c0,ctr_bits);
        uart1.printf("steps:%3d\t + \t%04d;0X%X\r\n", step_counter,c0,ctr_bits);
    for(; step_counter < accelerate_until; step_counter++)
    {

        c = c0*(sqrt(float(step_counter+1)) - sqrt(float(step_counter)));
        while(steper.bitsRing.isfull())
        {
            //do run time
        }
        steper.write_buffer(c,ctr_bits);
    }
        uart1.printf("steps:%3d\t + \t%04d;0X%X\r\n", step_counter,c,ctr_bits);
    for(; step_counter < decelerate_after; step_counter++)
    {
        while(steper.bitsRing.isfull())
        {
            //do run time
        }
        steper.write_buffer(c,ctr_bits);

    }
        uart1.printf("steps:%3d\t = \t%04d;0X%X\r\n", step_counter,c,ctr_bits);
    for(; step_counter < delta_steps; step_counter++)
    {
        c = c0*(sqrt(float(delta_steps - step_counter+1)) - sqrt(float(delta_steps - step_counter)));
        while(steper.bitsRing.isfull())
        {
            //do run time
        }
        steper.write_buffer(c,ctr_bits);


    }
        uart1.printf("steps:%3d\t - \t%04d;0X%X\r\n", step_counter,c,ctr_bits);
}

uint32_t CNC::mm_to_steps(float mm)
{
    return floor(mm/MM_PER_STEP);
}
float CNC::step_to_mm(uint32_t steps)
{
    return (steps*MM_PER_STEP);
}


void CNC::set_speed(float speed)
{
    if(speed > MAX_SPEED_MM_MIN)
        this->speed = MAX_SPEED_MM_MIN;
    else
        this->speed = speed;

}
