#include "common.h"
SYSTEM sys;
void SYSTEM::get_chip_info()
{
    chip_id[2] = *(__IO u32*)(0X1FFFF7E8);  //���ֽ�
    chip_id[1] = *(__IO u32 *)(0X1FFFF7EC); // 
    chip_id[0] = *(__IO u32 *)(0X1FFFF7F0); //���ֽ�
    
    flash_size = *(u16*)(0x1FFFF7E0);    //оƬflash����
}
extern u16 AD_value[];
float SYSTEM::get_cpu_temperature()
{
//    float temp;
//    temp = (1.43-(float)AD_value[15])/0.0043+25;
//    return (float)AD_value[16];
}
