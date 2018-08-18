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

class EventIo
{
    
    public:
        EventIo(){};
        virtual void process(void) = 0;

} ;

template <class T>
class EventVar :public EventIo
{
    public:
        EventVar(void (*pos_edge)(),void (*nag_edge)(),void (*changed)())
        {

            event_pos_edge = pos_edge;
            event_nag_edge = nag_edge;
            event_changed = changed;
        }
        void begin(T *io)
        {
            this->io = io;
            state = *io;
        }
        virtual void process(void) ;
        void (*event_pos_edge)();
        void (*event_nag_edge)(); 
        void (*event_changed)();
    private:
        T *io;
        T state;


};


#endif
