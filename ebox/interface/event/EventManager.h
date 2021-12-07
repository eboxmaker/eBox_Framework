#ifndef __EVENT_MANAGER_H
#define __EVENT_MANAGER_H

#include "ebox_core.h"
#include "stream.h"
#include "LinkedList/LinkedList.h"

class Event:public Object
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
        list.add(object);
    }
    void print_list(Stream &uart)
    {
//        uart.printf("list size:%d\r\n",list.size());
        for(int i = 0; i < list.size(); i++)
        {
            node = list.get(i);
            uart.println(node->name);
        }
    }

    void loop()
    {
        int i = 0;
        while(i < list.size())
        {
            list.get(i)->loop();
            i++;
        }
    }

private:
    LinkedList<Event*> list;
    Event *node;

};


#endif
