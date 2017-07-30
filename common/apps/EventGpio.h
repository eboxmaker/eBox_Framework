#ifndef __IOEVENT_H
#define __IOEVENT_H
#include "ebox.h"

#define IO_EDGE_FILTER_COUNTS 5
class EventGpio
{
    
    public:
        EventGpio(Gpio *io,void (*high)(),void (*low)(),void (*pos_edge)(),void (*nag_edge)(),void (*click)(),void(*release)(),void (*long_press)())
        {
            this->io = io;
            event_high = high;
            event_low = low;
            event_pos_edge = pos_edge;
            event_nag_edge = nag_edge;
            event_click = click;
            event_release = release;
            event_long_press = long_press;
        }
        void begin(uint8_t first_state);
        void process();
        void (*event_high)();
        void (*event_low)();
        void (*event_pos_edge)();
        void (*event_nag_edge)(); 
        void (*event_click)();
        void (*event_release)();
        void (*event_long_press)();
        

    private:
        Gpio *io;
        u8 state;
        u8 pos_count;
        u8 nag_count;
        u16 pushed_count;
        u16 release_count;
        u16 long_pressed;
        u8 click_pushed;
        u8 click_released;
        u8 first_state;
} ;


#endif
