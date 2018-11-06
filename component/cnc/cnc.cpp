#include "cnc.h"
#include "bsp_ebox.h"
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


    steps_completed_evnets = 0;
    block.acc = acc;
    block.speed = speed;
    block.position[0] = new_position[0];
    block.position[1] = new_position[1];
    block.position[2] = new_position[2];
    calculate_block(&block);
    steps_total_events = block.max_delta_steps;
    draw_line(&block);

}
void CNC::move_signal_to(uint8_t Axis, double new_value)
{


    block.acc = acc;
    block.speed = speed;
    for(int i = 0; i < 3; i++)
    {
        if(i == Axis) block.position[i] = new_value;
        else block.position[i] = this->position[i];
    }



    bit_set(block.ctr_bits, Axis);
    steps_completed_evnets = 0;
    calculate_block(&block);
    steps_total_events = block.max_delta_steps;



    steper.timer_start();
    for(; block.step_planed < block.max_delta_steps; block.step_planed++)
    {

        uint32_t cn = calculate_cn(&block);
        while(steper.bitsRing.isfull())
        {

            //do run time
        }
        steper.write_buffer(cn, block.ctr_bits);
        //        uart1.printf("steps:0X%X\r\n", block.ctr_bits);
    }
}

uint8_t  CNC::judge_quadrant(double x_center, double y_center, double x, double y)
{
    unsigned short nDir;
    if(x - x_center >= 0)
    {
        if(y -  y_center >= 0)
        {
            nDir = 1;
            return 1;
        }
        else
        {
            nDir = 4;
            return 4;
        }
    }
    else
    {
        if(y -  y_center >= 0)
        {
            nDir = 2;
            return 2;
        }
        else
        {
            nDir = 3;
            return 3;
        }
    }
}




void  CNC::dda_circle( double XEnd, double YEnd, double i,  double j, int blsCW)
{
    double radius = sqrt( i * i + j * j);
    double x_center = position_step[X_AXIS] - i;
    double y_center = position_step[Y_AXIS] - j;
    dda_circle(x_center, y_center, position_step[X_AXIS], position_step[Y_AXIS], XEnd, YEnd, radius, blsCW);
}

void CNC::dda_circle(double x_center,   double y_center,
                     double XStart,     double YStart,
                     double XEnd,       double YEnd,
                     double radius,     int blsCW)
{
    double xCur = XStart, yCur = YStart;
    long xRes = 0, yRes = 0;
    int ndir, Q;
    int judge = 1;
    int full_circle = 0;
    int xEnable, yEnable;

    xEnable = yEnable = 0;
    Q = radius;
    uint8_t ctr_bits = 0;
    cnc.position_step[X_AXIS] = XStart;
    cnc.position_step[Y_AXIS] = YStart;

    steps_completed_evnets = 0;
    steps_total_events = 0;
    //insertPoint(xCre,yCur);
    uart1.printf("%f\t%f\r\n", xCur, yCur);
    if(XStart == XEnd & YStart == YEnd)
    {
        full_circle = 1;
    }
    while(judge == 1 || full_circle == 1)//终点检测
    {
        xRes += fabs(xCur - x_center);
        yRes += fabs(yCur - y_center);
        if(xRes >= Q & fabs(yCur - y_center) <= radius)
        {
            xRes = xRes % Q;
            yEnable = 1;
        }
        if(yRes >= Q & fabs(xCur - x_center) <= radius)
        {
            yRes = yRes % Q;
            xEnable = 1;
        }
        if(blsCW == 0)//逆圆插补
        {

            ndir = judge_quadrant(x_center, y_center, xCur, yCur);
            if(yEnable == 1)//xres 溢出，y轴进给
            {
                switch(ndir)
                {
                case 1:
                    if(fabs(yCur - y_center) < radius)
                    {
                        yCur += 1;
                        bit_set(ctr_bits, Y_STEP_BIT);
                        bit_set(ctr_bits, Y_DIR_BIT);
                    }
                    break;
                case 2:
                    yCur -= 1;
                    bit_set(ctr_bits, Y_STEP_BIT);
                    bit_clear(ctr_bits, Y_DIR_BIT);
                    break;
                case 3:
                    if(fabs(yCur - y_center) < radius)
                    {
                        yCur -= 1;
                        bit_set(ctr_bits, Y_STEP_BIT);
                        bit_clear(ctr_bits, Y_DIR_BIT);
                    }
                    break;
                case 4:
                    yCur += 1;
                    bit_set(ctr_bits, Y_STEP_BIT);
                    bit_set(ctr_bits, Y_DIR_BIT);
                    break;
                }
            }
            if(xEnable == 1)//xres 溢出，y轴进给
            {
                switch(ndir)
                {
                case 1:
                    xCur -= 1;
                    bit_set(ctr_bits, X_STEP_BIT);
                    bit_clear(ctr_bits, X_DIR_BIT);
                    break;
                case 2:
                    if(fabs(xCur - x_center) < radius)
                    {
                        xCur -= 1;
                        bit_set(ctr_bits, X_STEP_BIT);
                        bit_clear(ctr_bits, X_DIR_BIT);
                    }
                    break;
                case 3:
                    xCur += 1;
                    bit_set(ctr_bits, X_STEP_BIT);
                    bit_set(ctr_bits, X_DIR_BIT);
                    break;
                case 4:
                    if(fabs(xCur - x_center) < radius)
                    {
                        xCur += 1;
                        bit_set(ctr_bits, X_STEP_BIT);
                        bit_set(ctr_bits, X_DIR_BIT);
                    }
                    break;
                }
            }
        }
        else//顺圆插补
        {
            ndir = judge_quadrant(x_center, y_center, xCur, yCur);
            if(yEnable == 1)//xres 溢出，y轴进给
            {
                switch(ndir)
                {
                case 1:
                    yCur -= 1;
                    bit_set(ctr_bits, Y_STEP_BIT);
                    bit_clear(ctr_bits, Y_DIR_BIT);
                    break;
                case 2:
                    if(fabs(yCur - y_center) < radius)
                    {
                        yCur += 1;
                        bit_set(ctr_bits, Y_STEP_BIT);
                        bit_set(ctr_bits, Y_DIR_BIT);
                    }
                    break;
                case 3:
                    yCur += 1;
                    bit_set(ctr_bits, Y_STEP_BIT);
                    bit_set(ctr_bits, Y_DIR_BIT);
                    break;
                case 4:
                    if(fabs(yCur - y_center) < radius)
                    {
                        yCur -= 1;
                        bit_set(ctr_bits, Y_STEP_BIT);
                        bit_clear(ctr_bits, Y_DIR_BIT);
                    }
                    break;
                }
            }
            if(xEnable == 1)//xres 溢出，y轴进给
            {
                switch(ndir)
                {
                case 1:
                    if(fabs(xCur - x_center) < radius)
                    {
                        xCur += 1;
                        bit_set(ctr_bits, X_STEP_BIT);
                        bit_set(ctr_bits, X_DIR_BIT);
                    }
                    break;
                case 2:
                    xCur += 1;
                    bit_set(ctr_bits, X_STEP_BIT);
                    bit_set(ctr_bits, X_DIR_BIT);
                    break;
                case 3:
                    if(fabs(xCur - x_center) < radius)
                    {
                        xCur -= 1;
                        bit_set(ctr_bits, X_STEP_BIT);
                        bit_clear(ctr_bits, X_DIR_BIT);
                    }
                    break;
                case 4:
                    xCur -= 1;
                    bit_set(ctr_bits, X_STEP_BIT);
                    bit_clear(ctr_bits, X_DIR_BIT);
                    break;
                }
            }
        }
        judge = (fabs(xCur - XEnd) >= 1 || fabs(yCur - YEnd) >= 1);
        if(xEnable || yEnable)//判断运动给进
        {
            //insertPoint(xcur,ycur);

            while(steper.bitsRing.isfull())
            {
                //do run time
            }
            steper.write_buffer(15535, ctr_bits);
            steper.timer_start();
            ctr_bits = 0;
            lcd.draw_pixel(xCur, yCur, YELLOW);
            //            uart1.printf("%f\t%f\r\n",xCur,yCur);
            xEnable = yEnable = 0;
            full_circle = 0;
            steps_total_events++;
            // delay_ms(10);
        }
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

    double k;

    temp_ctr_bit = block->ctr_bits;
    counter = block->c0;
    // set_xy(x0, y0);
    dx = block->delta_steps[X_AXIS]; //计算x距离
    dy = block->delta_steps[Y_AXIS]; //计算y距离
    uart1.printf("dx :%5d;\t dy:%5d\r\n", dx, dy);

    if(block->ctr_bits & X_DIR_BIT_MASK)
    {
        x_inc = 1;
    }
    else
    {
        x_inc = -1;
    }
    if(block->ctr_bits & Y_DIR_BIT_MASK)
    {
        y_inc = 1;
    }
    else
    {
        y_inc = -1;
    }

    k = (double)dy / (double)dx;
    uart1.printf("k=%0.5f", k);
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
                bit_set(temp_ctr_bit, Y_STEP_BIT);

            } // end if error overflowed
            else
            {
                bit_clear(temp_ctr_bit, Y_STEP_BIT);
            }

            // adjust the error term
            error += dy2;

            // move to the next pixel
            x0 += x_inc; //x坐标值每次画点后都递增1
            bit_set(temp_ctr_bit, X_STEP_BIT);


            counter = calculate_cn(block);
            while(steper.bitsRing.isfull())
            {
                //do run time
            }
            steper.write_buffer(counter, temp_ctr_bit);
            block->vn[0] = MM_PER_STEP / (counter * TIME_UNIT);
            block->vn[1] = block->vn[0] * k;
            block->vn[0] = MM_PER_STEP / (counter * TIME_UNIT);
            //            uart1.printf("v%04d:[%0.3f][%0.3f][%0.3f]\r\n",block->step_planed, block->vn[0], block->vn[1], block->vn[2]);


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
                bit_set(temp_ctr_bit, X_STEP_BIT);

            } // end if error overflowed
            else
            {
                bit_clear(temp_ctr_bit, X_STEP_BIT);
            }

            // adjust the error term
            error += dx2;

            // move to the next pixel
            y0 += y_inc;
            bit_set(temp_ctr_bit, Y_STEP_BIT);


            counter = calculate_cn(block);

            while(steper.bitsRing.isfull())
            {
                //do run time
            }
            steper.write_buffer(counter, temp_ctr_bit);

            block->vn[0] = MM_PER_STEP / (counter * TIME_UNIT);
            block->vn[1] = block->vn[0] / k;
            block->vn[0] = MM_PER_STEP / (counter * TIME_UNIT);
            //            uart1.printf("v%04d:[%0.3f][%0.3f][%0.3f]\r\n",block->step_planed, block->vn[0], block->vn[1], block->vn[2]);

            block->step_planed++;
        } // end for
    } // end else |slope| > 1
    uart1.printf("steps planed:%5d\r\n", block->step_planed);

}

//计算位置运动block参数

// Calculates the distance (not time) it takes to accelerate from initial_rate to target_rate using the
// given acceleration:
double CNC::estimate_acceleration_distance(double initial_rate, double target_rate, double acceleration)
{
    return( (target_rate * target_rate - initial_rate * initial_rate) / (2 * acceleration) );
}
double CNC::intersection_distance(double initial_rate, double final_rate, double acceleration, double distance)
{
    return( (2 * acceleration * distance - initial_rate * initial_rate + final_rate * final_rate) / (4 * acceleration) );
}
void CNC::calculate_block(CncBlock_t *block)
{
    double max_mm;

    //检查参数
    if(block->speed > MAX_SPEED_MM_MIN) block->speed = MAX_SPEED_MM_MIN;
    else if(block->speed < MIN_SPEED_MM_MIN) block->speed = MIN_SPEED_MM_MIN;
    if(block->acc > MAX_ACC_MIN) block->acc = MAX_ACC_MIN;
    else if(block->acc > MIN_ACC_MIN) block->acc = MIN_ACC_MIN;

    block->delta_mm[X_AXIS] = (block->position[X_AXIS] - position[X_AXIS]);
    block->delta_mm[Y_AXIS] = block->position[Y_AXIS] - position[Y_AXIS];
    block->delta_mm[Z_AXIS] = block->position[Z_AXIS] - position[Z_AXIS];

    if(block->delta_mm[X_AXIS] < 0 )
    {
        bit_clear( block->ctr_bits, X_DIR_BIT);
        block->delta_mm[X_AXIS] = -block->delta_mm[X_AXIS];
    }
    else
    {
        bit_set( block->ctr_bits, X_DIR_BIT);
    }
    if(block->delta_mm[Y_AXIS] < 0 )
    {
        bit_clear( block->ctr_bits, Y_DIR_BIT);
        block->delta_mm[Y_AXIS] = -block->delta_mm[Y_AXIS];
    }
    else
    {
        bit_set( block->ctr_bits, Y_DIR_BIT);
    }
    if(block->delta_mm[Z_AXIS] < 0 )
    {
        bit_clear( block->ctr_bits, Z_DIR_BIT);
        block->delta_mm[Z_AXIS] = -block->delta_mm[Z_AXIS];
    }
    else
    {
        bit_set( block->ctr_bits, Z_DIR_BIT);
    }

    max_mm = max3v(block->delta_mm[X_AXIS], block->delta_mm[Y_AXIS], block->delta_mm[Z_AXIS]);
    block->delta_steps[X_AXIS] = mm_to_steps(block->delta_mm[X_AXIS]);
    block->delta_steps[Y_AXIS] = mm_to_steps(block->delta_mm[Y_AXIS]);
    block->delta_steps[Z_AXIS] = mm_to_steps(block->delta_mm[Z_AXIS]);
    block->max_delta_steps = max3v(block->delta_steps[X_AXIS], block->delta_steps[Y_AXIS], block->delta_steps[Z_AXIS]);
    int32_t accelerate_steps =
        mm_to_steps(estimate_acceleration_distance(0, block->speed, block->acc));
    int32_t decelerate_steps =
        mm_to_steps(estimate_acceleration_distance(block->speed, 0, -block->acc));
    // Calculate the size of Plateau of Nominal Rate.
    int32_t plateau_steps = block->max_delta_steps - accelerate_steps - decelerate_steps;
    if (plateau_steps < 0)
    {
        accelerate_steps = ceil(
                               intersection_distance(0, 0, block->acc, block->max_delta_steps));
        accelerate_steps = max(accelerate_steps, 0); // Check limits due to numerical round-off
        accelerate_steps = min(accelerate_steps, block->max_delta_steps);
        plateau_steps = 0;
    }
    block->accelerate_until = accelerate_steps;
    block->decelerate_after = accelerate_steps + plateau_steps;
    //计算c0
    double acc = block->acc / 3600.0;
    block->c0 = sqrt(2 * MM_PER_STEP / (acc * TIME_UNIT_POW));
    block->step_planed = 0;


    uart1.printf("\r\n===block===\r\n");
    uart1.printf("block positon:%0.5f,%0.5f\r\n", block->position[X_AXIS], block->position[Y_AXIS]);
    uart1.printf("all:%05d(%0.5f)\r\n", block->max_delta_steps, max_mm);
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
        cn = block->c0 * (sqrt(double(block->step_planed + 1)) - sqrt(double(block->step_planed)));
        //             uart1.printf("steps:%3d\t + \t%04d;\r\n", block->step_planed,cn);
    }
    else if(block->step_planed < block->decelerate_after)
    {
        cn = cn;
        //             uart1.printf("steps:%3d\t = \t%04d;\r\n", block->step_planed,cn);
    }
    else if(block->step_planed < block->max_delta_steps)
    {
        cn = block->c0 * (sqrt(double(block->max_delta_steps - block->step_planed + 1)) - sqrt(double(block->max_delta_steps - block->step_planed)));
        //             uart1.printf("steps:%3d\t - \t%04d;\r\n", block->step_planed,cn);
    }
    else
        cn = cn;
    return cn;
}
uint32_t CNC::mm_to_steps(double mm)
{

    return round(mm / MM_PER_STEP);
}
double CNC::step_to_mm(uint32_t steps)
{
    return (steps * MM_PER_STEP);
}







//状态
bool CNC::is_motion_over()
{
    if(steps_completed_evnets == steps_total_events)
        return true;
    else
        return false;
}

bool CNC::is_block_over(CncBlock_t *block)
{
    if(steps_completed_evnets == block->max_delta_steps)
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
    if(ctr_bists & X_STEP_BIT_MASK)
    {
        if(ctr_bists & X_DIR_BIT_MASK)
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

    if(ctr_bists & Y_STEP_BIT_MASK)
    {
        if(ctr_bists & Y_DIR_BIT_MASK)
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
    steps_completed_evnets++;
    uart1.printf("%d(%d):(%f,%f)\r\n", steps_completed_evnets, steps_total_events, position[X_AXIS], position[Y_AXIS]);
    lcd.draw_pixel(cnc.position_step[X_AXIS], cnc.position_step[Y_AXIS], WHITE);
    if(steps_completed_evnets >= steps_total_events)
    {
        uart1.printf("motion over\r\n");
        steper.timer_stop();
        return ;
    }

}

int CNC::guaxiang(double x, double y)
{
    unsigned short nDir;
    //    uart1.printf("(%0.1f,%0.1f):",x,y);
    if(x > 0)
    {
        if(y > 0)
        {
            if(x > y)
            {
                nDir = 0;
            }
            else if(x == y)
            {
                nDir = 1;
            }
            else if(x < y)
            {
                nDir = 1;
            }
        }
        else if(y == 0)
        {
            nDir = 0;
        }
        else if(y <  0)
        {
            if(x > -y)
            {
                nDir = 7;
            }
            else if(x == -y)
            {
                nDir = 7;
            }
            else if(x < -y)
            {
                nDir = 6;
            }
        }

    }
    else if(x == 0)
    {
        if(y > 0)
        {
            nDir = 2;
        }
        else if(y < 0)
        {
            nDir = 6;
        }
    }
    else if(x < 0)
    {
        if(y > 0)
        {
            if(-x > y)
            {
                nDir = 3;
            }
            else if(-x == y)
            {
                nDir = 3;
            }
            else if(-x < y)
            {
                nDir = 2;;
            }
        }
        else if(y == 0)
        {
            nDir = 4;
        }
        else if(y <  0)
        {
            if(-x > -y)
            {
                nDir = 4;
            }
            else if(-x == -y)
            {
                nDir = 5;;
            }
            else if(-x < -y)
            {
                nDir = 5;
            }
        }

    }

    //    uart1.printf("\r\n");
    return nDir;
}
int CNC::_guaxiang(double x, double y)
{
    unsigned short nDir;
    //    uart1.printf("(%0.1f,%0.1f):",x,y);
    if(x > 0)
    {
        if(y > 0)
        {
            if(x > y)
            {
                nDir = 0;
            }
            else if(x == y)
            {
                nDir = 8;
            }
            else if(x < y)
            {
                nDir = 1;
            }
        }
        else if(y == 0)
        {
            nDir = 8;
        }
        else if(y <  0)
        {
            if(x > -y)
            {
                nDir = 7;
            }
            else if(x == -y)
            {
                nDir = 8;
            }
            else if(x < -y)
            {
                nDir = 6;
            }
        }

    }
    else if(x == 0)
    {
        if(y > 0)
        {
            nDir = 8;
        }
        else if(y < 0)
        {
            nDir = 8;
        }
    }
    else if(x < 0)
    {
        if(y > 0)
        {
            if(-x > y)
            {
                nDir = 3;
            }
            else if(-x == y)
            {
                nDir = 8;
            }
            else if(-x < y)
            {
                nDir = 2;
            }
        }
        else if(y == 0)
        {
            nDir = 8;
        }
        else if(y <  0)
        {
            if(-x > -y)
            {
                nDir = 4;
            }
            else if(-x == -y)
            {
                nDir = 8;;
            }
            else if(-x < -y)
            {
                nDir = 5;
            }
        }

    }

    //    uart1.printf("\r\n");
    return nDir;
}

void CNC::print_position()
{

    uart1.printf("position:x:%3.3f(%d)\t y:%3.3f(%d)\tcompleted:%d\r\n", position[X_AXIS], position_step[X_AXIS], position[Y_AXIS], position_step[Y_AXIS], steps_completed_evnets);
}
void CNC::print_info()
{
    uart1.printf("\r\n===========Welcome Free CNC(V1.0)==============\r\n");

    uart1.printf("Steper SPR:       %d(steps per round)\r\n", STEPER_SPR);
    uart1.printf("Steper SPR*DIV:   %d(steps per round)\r\n", STEPER_SPR * STEPER_DIV);
    uart1.printf("Steper Radian:    %0.5fr(%0.1f°)\r\n", RADIAN_PER_STEP, ANGLE_PER_STEP);
    uart1.printf("Steper MIN rpm:   %0.5frpm\r\n", MIN_RPM);
    uart1.printf("Steper MAX rpm:   %0.5frpm\r\n", MAX_RPM);
    uart1.printf("\r\n");


    uart1.printf("CNC mm per step:  %0.3fmm(%0.0fum)\r\n", MM_PER_STEP, MM_PER_STEP * 1000);

    uart1.printf("Time unit:        %6.1fus\r\n", TIME_UNIT * 1e6);

    uart1.printf("Min Acc:  %6.3fmm/s^2     Min Acc:   %6.3fmm/min^2   \r\n", MIN_ACC_S, MIN_ACC_MIN);
    uart1.printf("Max Acc:  %6.3fmm/s^2     Max Acc:   %6.3fmm/min^2 \r\n", MAX_ACC_S, MAX_ACC_MIN);
    uart1.printf("Min Speed:%6.3fmm/s       Max Speed: %6.3fmm/minute\r\n", MIN_SPEED_MM_S, MIN_SPEED_MM_MIN);
    uart1.printf("Max Speed:%6.3fmm/s       Max Speed: %6.3fmm/minute\r\n", MAX_SPEED_MM_S, MAX_SPEED_MM_MIN);
    print_position();
    uart1.printf("===================================================\r\n");

}

double delta_t;
double a, v, s, t, sc, tc;
double delta_t2;
double a2, v2, s2, t2, sc2, tc2;
#define TIME_UNITx 1e-4
void test2()
{
    if(a != 0)
        delta_t  = (-v  + sqrt(v * v   + 2 * a * 0.01 )) / a;
    else
        delta_t  = 0.01 / v;

    if(a2 != 0)
        delta_t2 = (-v2 + sqrt(v2 * v2 + 2 * a2 * 0.01)) / a2;
    else
        delta_t2  = 0.01 / v2;

    //    uart1.printf("adelta_t:%0.6f(%0.6f)\n",delta_t,delta_t2);
}
void test3()
{
    int i = 0, k = 0;
    double t1mark = 0, t2mark = 0, _t1, _t2;
    while(1 )
    {
        if(tc < 0.15)
        {

            a = tc * 8000;
            a2 = tc * 8000;
        }
        else
        {
            a = tc * 1000;
            a2 = tc * 8000;
            v = v2 / 8;
            //v2=0;

        }
        s += v * TIME_UNITx + 0.5 * a * TIME_UNITx * TIME_UNITx;
        v += a * TIME_UNITx;

        s2 += v2 * TIME_UNITx + 0.5 * a2 * TIME_UNITx * TIME_UNITx;
        v2 += a2 * TIME_UNITx;

        //        if(v > 50)v=50;
        tc += TIME_UNITx;
        if(s > 0.01)
        {
            _t1 = tc - t1mark;
            s -= 0.01;
            i++;
            uart1.printf("1.%d:tc:%0.6fs\t\t_t1c:%0.6fs\t\tAx:%0.6f\t\tVx:%0.6f\t\tSx:%0.2f\t\n", i, tc, _t1, a, v, s + i * 0.01);
            t1mark = tc;
        }
        else if(s < -0.01)
        {
            _t1 = tc - t1mark;
            s = s + 0.01;
            i--;
            uart1.printf("1.%d:tc:%0.6fs\t\t_t1c:%0.6fs\t\tAx:%0.6f\t\tVx:%0.6f\t\tSx:%0.2f\t\n", i, tc, _t1, a, v, s + i * 0.01);
            t1mark = tc;

        }

        //        if(v2 > 100)v2=100;
        //        tc2+=TIME_UNITx;
        if(s2 > 0.01)
        {
            _t2 = tc2 - t2mark;
            s2 = s2 - 0.01;
            k++;
            uart1.printf("2.%d:tc:%0.6fs\t\t_t2c:%0.6fs\t\tAy:%0.6f\t\tVy:%0.6f\t\tSy:%0.2f\t\n", k, tc2, _t2, a2, v2, s2 + k * 0.01);
            t2mark = tc2;
        }
        else if(s2 < -0.01)
        {
            _t2 = tc2 - t2mark;
            s2 = s2 + 0.01;
            k--;
            uart1.printf("2.%d:tc:%0.6fs\t\t_t2c:%0.6fs\t\tAy:%0.6f\t\tVy:%0.6f\t\tSy:%0.2f\t\n", k, tc2, _t2, a2, v2, s2 + k * 0.01);
            t2mark = tc2;

        }
        lcd.draw_pixel(64 + s + i * 0.01, +s2 + k * 0.01, GREEN);
        //        lcd.draw_pixel(64+v,80+v2,WHITE);
        //        lcd.draw_pixel(64+a,80+a2,BLUE);

    }

}

void test()
{

    test3();
    while(1)
    {
        test2();
        tc += delta_t;
        s += v * delta_t + 0.5 * a * delta_t *delta_t;
        v += a * delta_t;
        sc = 0.5 * a * tc * tc;


        tc2 += delta_t2;
        s2 += v2 * delta_t2 + 0.5 * a2 * delta_t2 * delta_t2;
        v2 += a2 * delta_t2;
        sc2 = 0.5 * a2 * tc2 * tc2;
        a  = (tc * 40000);
        a2 = (tc2 * 80000);
        //        if(((int(round(s*100)))&(int(round(s*100)) - 1)) == 0)
        {
            uart1.printf("tc:%0.6fs\t\ta:%0.3f\t\tv:%0.3f\t\ts:%0.3f(%0.3f)\t\n", tc, a, v, s, sc);
        }
        //        if(((int(round(s2*100)))&(int(round(s2*100)) - 1)) == 0)
        {
            uart1.printf("tc:%0.6fs\t\ta:%0.3f\t\tv:%0.3f\t\ts:%0.3f(%0.3f)\t\n", tc2, a2, v2, s2, sc2);
        }
        //        delay_ms(10);
    }


}
