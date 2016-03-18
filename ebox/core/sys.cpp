#include "common.h"
#include "stdlib.h"
SYSTEM sys;
void SYSTEM::get_chip_info()
{
    chip_id[2] = *(__IO u32 *)(0X1FFFF7E8); //低字节
    chip_id[1] = *(__IO u32 *)(0X1FFFF7EC); //
    chip_id[0] = *(__IO u32 *)(0X1FFFF7F0); //高字节

    flash_size = *(u16 *)(0x1FFFF7E0);   //芯片flash容量
}
extern u16 AD_value[];
float SYSTEM::get_cpu_temperature()
{
    float temp;
    //    temp = (1.43-(float)AD_value[15])/0.0043+25;
    return (float)temp;

}
void random_seed(uint16_t seed)
{
    srand(seed);
}
uint16_t random()
{
    return rand();
}
uint16_t random(uint16_t max)
{
    return (rand() % max);
}
uint16_t random(uint16_t min, uint16_t max)
{
    return (min + rand() % (max - min));
}
