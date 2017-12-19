#include "eventio.h"

template <class T>
void EventVar<T>::process(void)
{
    if(*io != state)
    {
        if(*io <= state)
        {
            if(event_nag_edge != (void*)0)
                event_nag_edge();
        }
        else
        {
            if(event_pos_edge != (void*)0)
                event_pos_edge();
        }
        if(event_changed != (void*)0)
            event_changed();
        state = *io;
    }
}
