#ifndef __AK8963_H
#define __AK8963_H

#include "ebox.h"
//#include "my_math.h"

#define AK8963_ADDRESS				0x18	//AK8963��I2C��ַ 0X0C<<1
#define AK8963_ID				0X48	//AK8963������ID

//AK8963���ڲ��Ĵ���
#define AK8963_WHO_AM_I		0x00	//AK8963������ID�Ĵ�����ַ should return 0x48
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

    //��ʼ��Ϊ16λ��������������
    void begin(uint32_t speed = 400000);


    //�õ�������ֵ(ԭʼֵ)
    //mx,my,mz:������x,y,z���ԭʼ����(������)
    //���mx��my��mz��Ϊ0�������������쳣
    void getMag(short *mx, short *my, short *mz);
    //mx,my,mz:������x, y, z��ı�׼��λ����
    void getMag(float *mx, float *my, float *mz);

    //���ô�����У׼��������������ı任
    void setMagBiasSens(
        float bx, float by, float bz,
        float sx, float sy, float sz);
};


#endif

