#ifndef __ADS8866_H
#define __ADS8866_H

#include "ebox_core.h"
class Ads8866
{
public:
    Ads8866(Gpio *clk, Gpio *data, Gpio *convst);
    void begin();
    uint16_t read();

private:
    Gpio *clk;
    Gpio *data;
    Gpio *convst;


};

#endif
