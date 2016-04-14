#ifndef __CSB_H
#define __CSB_H
#include "ebox.h"

class ULTRA
{
    public:
        float value;
    public:
        ULTRA(GPIO *triger);
        void begin();
        float detect();
        
        
    private:
        GPIO *triger;
        uint8_t flag;
    
    
    friend void mesure_high();
    

};
extern ULTRA ultra;

#endif