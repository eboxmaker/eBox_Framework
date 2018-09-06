#include "EventGpio.h"

void EventGpio::begin(uint8_t first_state)
{
    io->mode(INPUT_PU);
    this->first_state = first_state;
    click_pushed = 0;
    click_released = 0;
    long_pressed = 0;
    state = first_state;
}
void EventGpio::process()
{
    if( io->read() != state)		// 如果状态变化
    {
        if(changed == 0)
        {
            last_time = millis();
            changed = 1;
        }    
        else
        {
//            if(( millis() - last_time ) > IO_EDGE_FILTER_COUNTS)
            {   
                if(state == 1)
                {
                    state = 0;
                    if(event_neg_edge != NULL)
                        event_neg_edge();
                }
                else
                {
                    state = 1;
                    if(event_pos_edge != NULL)
                        event_pos_edge();
                }
                changed = 0;
            }

        }
    }
    if(state == first_state )		
    {
        if(click_pushed && (long_pressed == 0))		// 单击
        {
            click_released = 1;
            if(event_release !=  NULL)
                event_release();
        }
        click_pushed = 0;
        
        if(event_high != NULL)
            event_high();
    }
    else
    {
         if(millis() - last_time > IO_EDGE_FILTER_COUNTS)
         {
             if(event_long_press !=  NULL)//使用long_press，禁用click，如果使用long_press则触发long_press之后不会再触发release
            {
                if(millis()  - last_time < 2000)
                {   
                    click_pushed = 1;
                    long_pressed = 0;
                }
                else if( long_pressed == 0)
                {
                    if(click_pushed)
                    {
                        long_pressed = 1;
                        if(event_long_press !=  NULL)
                            event_long_press();
                    }
                    click_released = 0;
                }
            }
            else//不使用long_press
            {
                if(click_pushed == 0)
                {
                    click_pushed = 1;
                    if(event_click !=  NULL)
                        event_click();
                }
                
            }
            if(NULL != event_low)
            {
                event_low();
            }
        }

    }
}
