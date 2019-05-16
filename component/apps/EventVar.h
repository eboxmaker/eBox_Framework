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
    EventVar(T *var)
    {
        this->var = var;
        state = *var;
        event_pos_edge = NULL;
        event_nag_edge = NULL;
        event_changed = NULL;
    }

    virtual void loop(void) ;
    
public:
    void (*event_pos_edge)();
    void (*event_nag_edge)();
    void (*event_changed)();
private:
    T *var;
    T state;


};


#endif
