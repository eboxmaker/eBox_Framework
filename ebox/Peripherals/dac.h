#ifndef __DAC_H
#define __DAC_H
#include "common.h"
class DACCLASS
{

    public:
        DACCLASS(GPIO *pin);
        void begin(uint16_t *buf1,uint16_t *buf2,uint16_t buf_size);
    private:
        GPIO *pin;
};
#endif
