#ifndef __IO_EVENT_H
#define __IO_EVENT_H

#include "ebox_core.h"
#include "LinkedList.h"
#include "cjson.h"

typedef struct
{
    Gpio *pin;
    String name;
}IoNode;




typedef void (*myCallbackFunction)(char *root);

class IoChang
{

    public:
        IoChang();
        void begin();
        void add(IoNode *node);
        void loop();
        void attach(myCallbackFunction fun);
    public:
        LinkedList<IoNode *> _list;
        uint32_t lastState;
        myCallbackFunction _clickFunc;
        cJSON *root;

};

#endif
