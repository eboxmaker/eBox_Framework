#ifndef __RtcMillis_H
#define __RtcMillis_H

#include "BasicRtc.h"
#include "ebox_core.h"
class RtcMillis :public BasicRtc
{
public:
    RtcMillis(){};
    virtual void    begin(){};
    virtual void    begin(DateTime &dt){set(dt);};
    virtual void    loop(){update();};
    virtual void    update();
    virtual void    set(DateTime &dt);
    virtual DateTime   now();
protected:
    uint32_t lastMillis;
};


#endif
