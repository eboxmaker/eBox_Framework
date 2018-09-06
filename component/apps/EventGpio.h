#ifndef __IOEVENT_H
#define __IOEVENT_H
#include "ebox_core.h"
#include "eventio.h"
#define IO_EDGE_FILTER_COUNTS 0

class EventGpio : public EventIo
{
    
    public:
        EventGpio(Gpio *io,void (*high)(),void (*low)(),void (*pos_edge)(),void (*neg_edge)(),void (*click)(),void(*release)(),void (*long_press)())
        {
            this->io = io;
            event_high = high;
            event_low = low;
            event_pos_edge = pos_edge;
            event_neg_edge = neg_edge;
            event_click = click;
            event_release = release;
            event_long_press = long_press;
        }
        void begin(uint8_t first_state);
        virtual void process(void);
        void (*event_high)();
        void (*event_low)();
        void (*event_pos_edge)();
        void (*event_neg_edge)(); 
        void (*event_click)();
        void (*event_release)();
        void (*event_long_press)();
        

    private:
        Gpio *io;
        uint8_t state;
        uint32_t last_time;
        uint16_t long_pressed;
        uint8_t click_pushed;
        uint8_t click_released;
        uint8_t first_state;
        uint8_t changed;
} ;


#endif
