#include "EventGpio.h"

void EventGpio::begin()
{
    if(this->first_state == 0)
        io->mode(INPUT_PD);
    else
        io->mode(INPUT_PU);

}
void EventGpio::loop()
{
    if( io->read() != state)    // 如果状态变化
    {
        if(changed == 0)//如果没有被标记这一次变化，就产生一个变化的标记
        {
            last_time = millis();
            changed = 1;
        }
        else//如果已经被标记。
        {
            //如果超过了滤波时间，
            if(( millis() - last_time ) > IO_EDGE_FILTER_COUNTS)
            {
                if(state == 1)//且初始态为1，则说明是下降沿
                {
                    state = 0;//将IO状态标记改为当前IO状态
                    if(event_neg_edge != NULL)
                        event_neg_edge(this);
                }
                else//初始态为0，则上升沿
                {
                    state = 1;//将IO状态标记改为当前IO状态
                    if(event_pos_edge != NULL)
                        event_pos_edge(this);
                }
                changed = 0;//执行完回调函数，将变化标记清空
            }

        }
    }
    //如果在滤波内发生了毛刺，要清除毛刺产生的状态
    if(( millis() - last_time ) > IO_EDGE_FILTER_COUNTS)
    {
        if(changed == 1 && io->read() == first_state)
            changed = 0;

    }
    

    //state为滤波后的IO状态值
    if(state == first_state )//如果和初始态一样
    {
        if(click_pushed && (long_pressed == 0))// 且检测到被按下，且没有触发long_press事件，则调用释放事件
        {
            click_released = 1;//标记释放事件发生过
            if(event_release !=  NULL)
                event_release(this);
        }
        click_pushed = 0;//按键已经被释放，清除按下的状态


    }
    else//state为滤波后的IO状态值，如果和初始态不一样
    {
        if(millis() - last_time > IO_EDGE_FILTER_COUNTS)//验证滤波时间
        {
            if(event_long_press !=  NULL)//使用long_press，禁用click，如果使用long_press则触发long_press之后不会再触发release
            {
                if(millis()  - last_time < LONG_PRESS_COUNTS)
                {
                    click_pushed = 1;
                    long_pressed = 0;
                    long_press_triger_time = millis();
                }
                else //state为滤波后的IO状态值，如果和初始态不一样，且超过了长按触发条件，且之前没执行过长按事件
                {
                    if( long_press_type == Single)
                    {
                        if(long_pressed == 0)
                        {
                            {
                                long_pressed = 1;
                                if(event_long_press !=  NULL)
                                    event_long_press(this);
                            }
                        }
                    }
                    else
                    {
                        if( millis()  - long_press_triger_time  > LONG_PRESS_INTERVAL)
                        {
                            long_press_triger_time = millis();
                            //且按键是按下的状态，则执行长按事件
                            {
                                long_pressed = 1;
                                if(event_long_press !=  NULL)
                                    event_long_press(this);
                            }
                        }
                    }

                    click_released = 0;//不再执行释放事件
                }
            }
            else//不使用long_press
            {
                if(click_pushed == 0)//state为滤波后的IO状态值，如果和初始态不一样，且之前没执行过单机事件，则执行单机事件
                {
                    click_pushed = 1;
                    if(event_click !=  NULL)
                        event_click(this);
                }

            }

        }

    }
    if(io->read() == 1){
        if(event_high != NULL)
            event_high(this);
    }
    else{
        if(NULL != event_low)
            event_low(this);
    }
}
