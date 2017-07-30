#include "EventGpio.h"

void EventGpio::begin(uint8_t first_state)
{
    io->mode(INPUT_PU);
    this->first_state = first_state;
    state = first_state;
}
void EventGpio::process()
{
    
    if(io->read() != state)
    {
        if(state == 1)
        {
            pos_count = 0;
            nag_count ++;
            if(nag_count > IO_EDGE_FILTER_COUNTS)
            {   
                state = 0;
                if(event_nag_edge != (void*)0)
                    event_nag_edge();
            }
        }
        else
        {
            nag_count = 0;
            pos_count ++;
            if(pos_count > IO_EDGE_FILTER_COUNTS)
            {
                state = 1;
                if(event_pos_edge != (void*)0)
                    event_pos_edge();
            }
        }
    }
    else
    {
        nag_count = 0;
        pos_count = 0;
    }
    
    if(state == first_state )
    {
        if(release_count > IO_EDGE_FILTER_COUNTS)
		{
			click_released = 1;
            if(click_pushed && (long_pressed == 0))
            {
                if(event_release !=  (void*)0)
                    event_release();
            }
            click_pushed = 0;
		}
		else
		{
			release_count++;
		}
		pushed_count  = 0;
        
        if(event_high != (void *)0)
            event_high();
    }
    else
    {
        if(event_long_press !=  (void*)0)//使用long_press，禁用click，如果使用long_press则触发long_press之后不会再触发release
        {
            if(pushed_count > IO_EDGE_FILTER_COUNTS && pushed_count < 2000)
            {   
                click_pushed = 1;
            }
            if(pushed_count > 2000)
            {
                long_pressed = 1;
                if(click_released)
                {
                    event_long_press();
                }
                click_released = 0;
            }
            else
            {
                pushed_count++;
                long_pressed = 0;
            }
        }
        else//不使用long_press
        {
            if(pushed_count > IO_EDGE_FILTER_COUNTS)
            {
                click_pushed = 1;
                if(click_released)
                {
                    if(event_click !=  (void*)0)
                        event_click();
                }
                click_released = 0;
            }
            else
            {
                pushed_count++;
            }
            release_count = 0;        
        }
		if((void*)0 != event_low)
		{
			event_low();
		}
    }
}

