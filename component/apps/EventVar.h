/**
  ******************************************************************************
  * @file    incapture.h
  * @author  shentq
  * @version V2.0
  * @date    2017/07/23
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/


#ifndef __EVENTIO_H
#define __EVENTIO_H

#include "ebox_core.h"
#include "eventManager.h"


template <class T>
class EventVar : public Event
{
public:
    EventVar(T *var,String name = "null")
    {
        this->var = var;
        state = *var;
        event_pos_edge = NULL;
        event_nag_edge = NULL;
        event_changed = NULL;
        this->name = name;
    }

    virtual void loop(void)
    {
        if(*var != state)
        {
            if(event_changed != NULL)
                event_changed(this);
            
            if(*var <= state){
                if(event_nag_edge != NULL)
                    event_nag_edge(this);
            }
            else{
                if(event_pos_edge != NULL)
                    event_pos_edge(this);
            }
            state = *var;
        }
    }
    
public:
    void (*event_pos_edge)(Object *sender);
    void (*event_nag_edge)(Object *sender);
    void (*event_changed)(Object *sender);
    T *var;
private:
    T state;


};
typedef EventVar<uint8_t>   EventVarUint8;


#endif
