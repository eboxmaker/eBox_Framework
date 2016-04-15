#ifndef __CSB_H
#define __CSB_H
#include "ebox.h"

class ULTRA:public IN_CAPTURE
{
    public:
        float wave_time;
    public:
        ULTRA(GPIO *echo,GPIO *triger):IN_CAPTURE(echo)
        {
            this->triger = triger;
        };
        void begin();
        float detect();
        
        
    private:
        GPIO *triger;
        uint8_t flag;
    
    
    friend void mesure_high();
    

};
extern ULTRA ultra;

#endif