#ifndef __EVENT_MANAGER_H
#define __EVENT_MANAGER_H

#include "ebox.h"
#include "EventGpio.h"
#include "EventIo.h"


class EventManager
{
    public:
        void add(EventGpio *fun)
        {   
            list.insert_tail(fun);
        }
        void process()
        {
            int i=0;
            while(i < list.size())
            {
                te = (EventGpio *)(list.data(i));
                te->process();
                i++;
            }
        }

    private:
       List list;
       EventGpio *te;

};


#endif
