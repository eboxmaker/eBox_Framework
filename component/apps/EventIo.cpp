#include "eventio.h"

template <class T>
void EventVar<T>::process(void)
{
    if(*io != state)
    {
        if(*io <= state)
        {
            if(event_nag_edge != NULL)
                event_nag_edge();
        }
        else
        {
            if(event_pos_edge != NULL)
                event_pos_edge();
        }
        if(event_changed != NULL)
            event_changed();
        state = *io;
    }
}
