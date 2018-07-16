#include "cnc.h"


int32_t counter = 22100;
uint32_t i;
uint16_t acc_untill;
uint16_t acc_delta=200;

void draw_line( uint16_t x1, uint16_t y1)
{
    uint8_t temp_ctr_bit = 0;
    uint16_t x0 = 0;
    uint16_t y0 = 0;
    int dx,             // difference in x's
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

    if (dx >= 0)
    {
        x_inc = 1;
    }
    else
    {
        x_inc = -1;
        dx    = -dx;
    }

    if (dy >= 0)
    {
        y_inc = 1;
    }
    else
    {
        y_inc = -1;
        dy    = -dy;
    }

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
                bit_set(temp_ctr_bit,Y_STEMP_BIT);

            } // end if error overflowed
            else
            {
                bit_clear(temp_ctr_bit,Y_STEMP_BIT);
            }

            // adjust the error term
            error += dy2;

            // move to the next pixel
            x0 += x_inc; //x坐标值每次画点后都递增1
            bit_set(temp_ctr_bit,X_STEMP_BIT);
            while(steper.bitsRing.isfull())
            {
                //do run time
            }
            steper.write_buffer(counter,temp_ctr_bit);
            if(index < acc_untill)
                counter -= acc_delta;
            else
                counter += acc_delta;

            uart1.printf("X>Y:%05d:counter:%05d(%02d)\r\n",index,counter,temp_ctr_bit);
        } // end for
    } // end if |slope| <= 1
    else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
    {
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
                bit_set(temp_ctr_bit,X_STEMP_BIT);

            } // end if error overflowed
            else
            {
                bit_clear(temp_ctr_bit,X_STEMP_BIT);
            }

            // adjust the error term
            error += dx2;

            // move to the next pixel
            y0 += y_inc;
            bit_set(temp_ctr_bit,Y_STEMP_BIT);
            while(steper.bitsRing.isfull())
            {
                //do run time
            }
            steper.write_buffer(counter,temp_ctr_bit);
            if(index < acc_untill)
                counter -= acc_delta;
            else
                counter += acc_delta;
            uart1.printf("Y>X:%05d:counter:%05d(%02d)\r\n",index,counter,temp_ctr_bit);
        } // end for
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
    uart1.printf("Dx:%f;Dy:%f;\r\n", delta_mm[X_AXIS],delta_mm[Y_AXIS]);
    uart1.printf("Dx:%05d;Dy:%05d;\r\n", delta_steps[X_AXIS],delta_steps[Y_AXIS]);

    draw_line(delta_steps[X_AXIS],delta_steps[Y_AXIS]);
    
}
void CNC::move_x_to(float new_x)
{

}
void CNC::move_y_to(float new_y)
{

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
    if(speed > MAX_SPEED)
        this->speed = MAX_SPEED;
    else
        this->speed = speed;

}
