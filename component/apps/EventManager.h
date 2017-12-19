#ifndef __EVENT_MANAGER_H
#define __EVENT_MANAGER_H

#include "ebox.h"


#include "EventIo.h"
#include "EventGpio.h"

class EventManager
{
    public:
        void add(EventIo *handler)
        {   
            list.insert_tail(handler);
        }
        
        void process()
        {
            int i=0;
            while(i < list.size())
            {
                te = (EventIo *)list.data(i);
                te->process();                
                i++;
            }
        }

    private:
       List list;
       EventIo *te;

};


#endif
