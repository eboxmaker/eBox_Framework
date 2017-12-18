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
    if( io->read() != state)
    {
        if(changed == 0)
        {
            last_time = millis();
            changed = 1;
        }    
        else
        {
            if(( millis() - last_time ) > 5)
            {   
                if(state == 1)
                {
                    state = 0;
                    if(event_neg_edge != (void*)(0))
                        event_neg_edge();
                }
                else
                {
                    state = 1;
                    if(event_pos_edge != (void*)(0))
                        event_pos_edge();
                }
                changed = 0;
            }

        }
    }
    if(state == first_state )
    {
        if(click_pushed && (long_pressed == 0))
        {
            click_released = 1;
            if(event_release !=  (void*)0)
                event_release();
        }
        click_pushed = 0;
        
        if(event_high != (void *)0)
            event_high();
    }
    else
    {
         if(millis() - last_time > 5)
         {
             if(event_long_press !=  (void*)0)//ʹ��long_press������click�����ʹ��long_press�򴥷�long_press֮�󲻻��ٴ���release
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
                        if(event_long_press !=  (void*)0)
                            event_long_press();
                    }
                    click_released = 0;
                }
            }
            else//��ʹ��long_press
            {
                if(click_pushed == 0)
                {
                    click_pushed = 1;
                    if(event_click !=  (void*)0)
                        event_click();
                }
                
            }
            if((void*)0 != event_low)
            {
                event_low();
            }
        }

    }
}
