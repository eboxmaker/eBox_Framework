#ifndef __EBOX_DAC_H
#define __EBOX_DAC_H
#include "ebox_core.h"
#include "mcu.h"

#define DAC1  0
#define DAC2  1



#define FS_Val 4095

static int Sin_tab[40] =
{
    0.500 * FS_Val,
    0.598 * FS_Val,
    0.691 * FS_Val,
    0.778 * FS_Val,
    0.854 * FS_Val,
    0.916 * FS_Val,
    0.962 * FS_Val,
    0.990 * FS_Val,
    1.000 * FS_Val,
    0.990 * FS_Val,
    0.962 * FS_Val,
    0.916 * FS_Val,
    0.854 * FS_Val,
    0.778 * FS_Val,
    0.691 * FS_Val,
    0.598 * FS_Val,
    0.500 * FS_Val,
    0.402 * FS_Val,
    0.309 * FS_Val,
    0.222 * FS_Val,
    0.146 * FS_Val,
    0.084 * FS_Val,
    0.038 * FS_Val,
    0.010 * FS_Val,
    0.000 * FS_Val,
    0.010 * FS_Val,
    0.038 * FS_Val,
    0.084 * FS_Val,
    0.146 * FS_Val,
    0.222 * FS_Val,
    0.309 * FS_Val,
    0.402 * FS_Val,
};


class DACCLASS
{

public:
    DACCLASS(uint16_t dac_channel);
    void begin();
    void dac_out(int16_t value);
    void dac_sin_out_update(void);
private:
    uint16_t dac_channel;
};
#endif


