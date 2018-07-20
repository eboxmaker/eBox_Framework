#include "cnc.h"

CncBlock_t block;

void CNC::begin()
{
    position[X_AXIS] = 0;
    position[Y_AXIS] = 0;
    position[Z_AXIS] = 0;
    set_acc(MAX_ACC_MIN);
    set_speed(MAX_SPEED_MM_MIN);
}
//设置运动速度参数
void CNC::set_acc(double _acc)
{
    if(_acc > MAX_ACC_MIN)
        this->acc = MAX_SPEED_MM_MIN;
    else
        this->acc = _acc;

}

void CNC::set_speed(double _speed)
{
    if(_speed > MAX_SPEED_MM_MIN)
        this->speed = MAX_SPEED_MM_MIN;
    else
        this->speed = _speed;
}
//运动位置模式控制

void CNC::move(double *new_position)
{


    steps_completed = 0;
    block.acc = acc;
    block.speed = speed;
    block.position[0] = new_position[0];
    block.position[1] = new_position[1];
    block.position[2] = new_position[2];
    calculate_block(&block);
    draw_line(&block);
    
}
void CNC::move_signal_to(uint8_t Axis,double new_value)
{
    
    
    block.acc = acc;
    block.speed = speed;
    for(int i = 0; i < 3; i++)
    {
        if(i == Axis) block.position[i] = new_value;
        else block.position[i] = this->position[i];
    }
    block.step_planed = 0;
    


    bit_set(block.ctr_bits,Axis);
    steps_completed = 0;
    calculate_block(&block);    
    


    steper.timer_start();
    for(; block.step_planed < block.max_delta_steps; block.step_planed++)
    {

        uint32_t cn = calculate_cn(&block);
        while(steper.bitsRing.isfull())
        {

            //do run time
        }
        steper.write_buffer(cn,block.ctr_bits);
//        uart1.printf("steps:0X%X\r\n", block.ctr_bits);
    }
}
void  CNC::draw_circle(double x0, double y0, double r)
{
int i,s;
    double angle[37];
    for(i = 0; i < 37;i++)
    {
        angle[i] = 10.0*i/180*PI;
    }
    double x[37],y[37];
    for(i = 0; i < 37;i++)
    {
        x[i] =  x0 + r*cos(angle[i]);
        y[i] =  y0 + r*cos(angle[i]);
    }
    for(s = 0; s < 36; s++)
    {
       // draw_line();
    }
    
}
void CNC::draw_line(CncBlock_t *block)
{
int32_t counter = 22100;
    uint8_t temp_ctr_bit = 0;
    uint32_t x0 = 0;
    uint32_t y0 = 0;
    int32_t dx,         // difference in x's
        dy,             // difference in y's
        dx2,            // dx,dy * 2
        dy2,
        x_inc,          // amount in pixel space to move during drawing
        y_inc,          // amount in pixel space to move during drawing
        error,          // the discriminant i.e. error i.e. decision variable
        index;          // used for looping


    temp_ctr_bit = block->ctr_bits;
    counter = block->c0;
   // set_xy(x0, y0);
    dx = block->delta_steps[X_AXIS]; //计算x距离
    dy = block->delta_steps[Y_AXIS]; //计算y距离
    uart1.printf("dx :%5d;\t dy:%5d\r\n",dx,dy);

    if(block->ctr_bits & X_DIR_BIT_MASK){ x_inc = 1; }
    else{ x_inc = -1; }
    if(block->ctr_bits & Y_DIR_BIT_MASK){ y_inc = 1; }
    else{ y_inc = -1; }


    dx2 = dx << 1;
    dy2 = dy << 1;
    steper.timer_start();
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
            
            
            counter = calculate_cn(block);
            while(steper.bitsRing.isfull())
            {
                //do run time
            }
            steper.write_buffer(counter,temp_ctr_bit);


            block->step_planed++;

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
            
            
            counter = calculate_cn(block);

            while(steper.bitsRing.isfull())
            {
                //do run time
            }
            steper.write_buffer(counter,temp_ctr_bit);


            block->step_planed++;        
        } // end for
    } // end else |slope| > 1
    uart1.printf("steps planed:%5d\r\n",block->step_planed);

}

//计算位置运动block参数

// Calculates the distance (not time) it takes to accelerate from initial_rate to target_rate using the 
// given acceleration:
double CNC::estimate_acceleration_distance(double initial_rate, double target_rate, double acceleration) 
{
  return( (target_rate*target_rate-initial_rate*initial_rate)/(2*acceleration) );
}
double CNC::intersection_distance(double initial_rate, double final_rate, double acceleration, double distance) 
{
  return( (2*acceleration*distance-initial_rate*initial_rate+final_rate*final_rate)/(4*acceleration) );
}
void CNC::calculate_block(CncBlock_t *block)
{
    double max_mm;
    
    //检查参数
    if(block->speed > MAX_SPEED_MM_MIN) block->speed = MAX_SPEED_MM_MIN;
    if(block->acc > MAX_ACC_MIN) block->acc = MAX_ACC_MIN;
    
    block->delta_mm[X_AXIS] = (block->position[X_AXIS] - position[X_AXIS]);
    block->delta_mm[Y_AXIS] = block->position[Y_AXIS] - position[Y_AXIS];
    block->delta_mm[Z_AXIS] = block->position[Z_AXIS] - position[Z_AXIS];
    
    if(block->delta_mm[X_AXIS] < 0 ){bit_clear( block->ctr_bits,X_DIR_BIT);block->delta_mm[X_AXIS] = -block->delta_mm[X_AXIS];}
    else{bit_set( block->ctr_bits,X_DIR_BIT);}
    if(block->delta_mm[Y_AXIS] < 0 ){bit_clear( block->ctr_bits,Y_DIR_BIT);block->delta_mm[Y_AXIS] = -block->delta_mm[Y_AXIS];}
    else{bit_set( block->ctr_bits,Y_DIR_BIT);}
    if(block->delta_mm[Z_AXIS] < 0 ){bit_clear( block->ctr_bits,Z_DIR_BIT);block->delta_mm[Z_AXIS] = -block->delta_mm[Z_AXIS];}
    else{bit_set( block->ctr_bits,Z_DIR_BIT);}
    
    max_mm = max3v(block->delta_mm[X_AXIS],block->delta_mm[Y_AXIS],block->delta_mm[Z_AXIS]);
    block->delta_steps[X_AXIS] = mm_to_steps(block->delta_mm[X_AXIS]);
    block->delta_steps[Y_AXIS] = mm_to_steps(block->delta_mm[Y_AXIS]);
    block->delta_steps[Z_AXIS] = mm_to_steps(block->delta_mm[Z_AXIS]);
    block->max_delta_steps = max3v(block->delta_steps[X_AXIS],block->delta_steps[Y_AXIS],block->delta_steps[Z_AXIS]);
    int32_t accelerate_steps = 
        mm_to_steps(estimate_acceleration_distance(0, block->speed, block->acc));
    int32_t decelerate_steps = 
        mm_to_steps(estimate_acceleration_distance(block->speed, 0, -block->acc));    
    // Calculate the size of Plateau of Nominal Rate. 
    int32_t plateau_steps = block->max_delta_steps-accelerate_steps-decelerate_steps;
    if (plateau_steps < 0) {  
        accelerate_steps = ceil(
        intersection_distance(0,0, block->acc, block->max_delta_steps));
        accelerate_steps = max(accelerate_steps,0); // Check limits due to numerical round-off
        accelerate_steps = min(accelerate_steps,block->max_delta_steps);
        plateau_steps = 0;
    }  
    block->accelerate_until = accelerate_steps;
    block->decelerate_after = accelerate_steps+plateau_steps;
    //计算c0
    double acc = block->acc / 3600.0;
    block->c0 = sqrt(2*MM_PER_STEP/(acc*TIME_UNIT_POWER));    
    block->step_planed = 0;
    
    
    uart1.printf("\r\n===block===\r\n");
    uart1.printf("block positon:%0.5f,%0.5f\r\n", block->position[X_AXIS],block->position[Y_AXIS]);
    uart1.printf("all:%05d(%0.5f)\r\n", block->max_delta_steps,max_mm);
    uart1.printf("+:%05d\r\n", block->accelerate_until);
    uart1.printf("=:%05d\r\n", plateau_steps);
    uart1.printf("-:%05d\r\n", block->decelerate_after);
    uart1.printf("c0:%05d\r\n", block->c0);
    
}

uint32_t CNC::calculate_cn(CncBlock_t *block)
{
    static uint32_t cn = 65536;
    if( block->step_planed < block->accelerate_until)
    {
        cn = block->c0*(sqrt(double(block->step_planed+1)) - sqrt(double(block->step_planed)));
    //             uart1.printf("steps:%3d\t + \t%04d;\r\n", block->step_planed,cn);
    }
    else if(block->step_planed < block->decelerate_after)
    {
        cn = cn;
    //             uart1.printf("steps:%3d\t = \t%04d;\r\n", block->step_planed,cn);
    }
    else if(block->step_planed < block->max_delta_steps)
    {
        cn = block->c0*(sqrt(double(block->max_delta_steps - block->step_planed+1)) - sqrt(double(block->max_delta_steps - block->step_planed)));
    //             uart1.printf("steps:%3d\t - \t%04d;\r\n", block->step_planed,cn);
    }
    else
        cn = cn;
    return cn;
}
uint32_t CNC::mm_to_steps(double mm)
{

    return round(mm/MM_PER_STEP);
}
double CNC::step_to_mm(uint32_t steps)
{
    return (steps*MM_PER_STEP);
}







//状态
bool CNC::is_motion_over()
{
    if(is_block_over(&block))
        return true;
    else
        return false;
}

bool CNC::is_block_over(CncBlock_t *block)
{
    if(steps_completed == block->max_delta_steps)
        return true;
    else
        return false;
}
double CNC::get_vector_speed()
{

}
double CNC::get_speed()
{

}

//被定时器中断调用更新位置
void CNC::update_position(uint8_t ctr_bists)
{
    if(ctr_bists&X_STEP_BIT_MASK)
    {
        if(ctr_bists&X_DIR_BIT_MASK)
        {
            position[X_AXIS] +=  MM_PER_STEP; 
            position_step[X_AXIS] += 1;
        }
        else
        {
            position[X_AXIS] -=  MM_PER_STEP; 
            position_step[X_AXIS] -= 1;
        }
    }
    
    if(ctr_bists&Y_STEP_BIT_MASK)
    {
        if(ctr_bists&Y_DIR_BIT_MASK)
        {
            position[Y_AXIS] +=  MM_PER_STEP; 
            position_step[Y_AXIS] += 1;
        }
        else
        {
            position[Y_AXIS] -=  MM_PER_STEP; 
            position_step[Y_AXIS] -= 1;
        }
    }
    steps_completed++;
    
}


void CNC::print_position()
{
    
    uart1.printf("position:x:%3.3f(%d)\t y:%3.3f(%d)\tcompleted:%d\r\n",position[X_AXIS],position_step[X_AXIS],position[Y_AXIS],position_step[Y_AXIS],steps_completed);
}
void CNC::print_info()
{
    uart1.printf("\r\n===========Welcome Free CNC(V1.0)==============\r\n");
    
    uart1.printf("Steper SPR:       %d(steps per round)\r\n",STEPER_SPR);
    uart1.printf("Steper SPR*DIV:   %d(steps per round)\r\n",STEPER_SPR*STEPER_DIV);
    uart1.printf("Steper Radian:    %0.5fr(%0.1f°)\r\n",RADIAN_PER_STEP,ANGLE_PER_STEP);
    uart1.printf("Steper MIN rpm:   %0.5frpm\r\n",MIN_RPM);
    uart1.printf("Steper MAX rpm:   %0.5frpm\r\n",MAX_RPM);
    uart1.printf("\r\n");
    
    
    uart1.printf("CNC mm per step:  %0.3fmm(%0.0fum)\r\n",MM_PER_STEP,MM_PER_STEP*1000);

    uart1.printf("Time unit:        %6.1fus\r\n",TIME_UNIT*1e6);
    
    uart1.printf("Min Acc:  %6.3fmm/s^2     Min Acc:   %6.3fmm/min^2   \r\n",MIN_ACC_S,MIN_ACC_MIN);
    uart1.printf("Max Acc:  %6.3fmm/s^2     Max Acc:   %6.3fmm/min^2 \r\n",MAX_ACC_S,MAX_ACC_MIN);
    uart1.printf("Min Speed:%6.3fmm/s       Max Speed: %6.3fmm/minute\r\n",MIN_SPEED_MM_S,MIN_SPEED_MM_MIN);
    uart1.printf("Max Speed:%6.3fmm/s       Max Speed: %6.3fmm/minute\r\n",MAX_SPEED_MM_S,MAX_SPEED_MM_MIN);
    print_position();
    uart1.printf("===================================================\r\n");

}