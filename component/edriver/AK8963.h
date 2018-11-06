#ifndef __AK8963_H
#define __AK8963_H

#include "ebox.h"
//#include "my_math.h"

#define AK8963_ADDRESS				0x18	//AK8963的I2C地址 0X0C<<1
#define AK8963_ID				0X48	//AK8963的器件ID

//AK8963的内部寄存器
#define AK8963_WHO_AM_I		0x00	//AK8963的器件ID寄存器地址 should return 0x48
#define AK8963_INFO      0x01
#define AK8963_ST1       0x02  // data ready status bit 0
#define AK8963_XOUT_L    0x03  // data
#define AK8963_XOUT_H    0x04
#define AK8963_YOUT_L    0x05
#define AK8963_YOUT_H    0x06
#define AK8963_ZOUT_L    0x07
#define AK8963_ZOUT_H    0x08
#define AK8963_ST2       0x09  // Data overflow bit 3 and data read error status bit 2
#define AK8963_CNTL          	  	0X0A    // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define AK8963_ASTC      0x0C  // Self test control
#define AK8963_I2CDIS    0x0F  // I2C disable
#define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value


class AK8963: public I2c
{
protected:
    uint8_t address;
    float sensAdjustVal[3];
    TicToc timer;
    float magBiasSub[3], magSens[3];
public:
    AK8963(I2c *i2c, uint8_t address = AK8963_ADDRESS);

    //初始化为16位采样、连续测量
    void begin(uint32_t speed = 400000);


    //得到磁力计值(原始值)
    //mx,my,mz:磁力计x,y,z轴的原始读数(带符号)
    //如果mx、my、mz均为0，表明磁力计异常
    void getMag(short *mx, short *my, short *mz);
    //mx,my,mz:磁力计x, y, z轴的标准单位读数
    void getMag(float *mx, float *my, float *mz);

    //设置磁力计校准参数，椭球到正球的变换
    void setMagBiasSens(
        float bx, float by, float bz,
        float sx, float sy, float sz);
};


#endif

