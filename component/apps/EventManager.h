#ifndef __EVENT_MANAGER_H
#define __EVENT_MANAGER_H

#include "ebox.h"
#include "list.h"

class Event
{

public:
    Event() {};
    virtual void loop(void) = 0;

} ;

class EventManager
{
public:
    void add(Event *object)
    {
        list.insert_tail(object);
    }

    void loop()
    {
        int i = 0;
        while(i < list.size())
        {
            node = (Event *)list.data(i);
            node->loop();
            i++;
        }
    }

private:
    List list;
    Event *node;

};


#endif
