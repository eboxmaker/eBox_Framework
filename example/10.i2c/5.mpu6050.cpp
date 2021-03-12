/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "bsp_ebox.h"

#include "mpu6050.h"

/**
	*	1	此例程需要调用eDrive目录下的mpu6050驱动
	*	2	此例程演示了mpu6050的基本操作
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"mpu6050 example"
#define EXAMPLE_DATE	"2018-08-11"



MPU6050 mpu(&sI2c1);

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    mpu.begin();



}
int16_t tmp[7];
int16_t x, y, z;
uint8_t id;
int main(void)
{
    setup();

    while(1)
    {
        mpu.get_id(&id);
        mpu.get_data(ACCEL_XOUT_H, tmp, 7);
        x = mpu.get_data(ACCEL_XOUT_H);
        y = mpu.get_data(ACCEL_YOUT_H);
        z = mpu.get_data(ACCEL_ZOUT_H);
        delay_ms(10);
        uart1.printf("\r\nid = %d", id);
        uart1.printf("\r\naccx = %d", tmp[0]);
        uart1.printf("\r\naccy = %d", tmp[1]);
        uart1.printf("\r\naccz = %d", tmp[2]);
        uart1.printf("\r\ntemp = %d", tmp[3]);
        uart1.printf("\r\ngyrox = %d", tmp[4]);
        uart1.printf("\r\ngyroy = %d", tmp[5]);
        uart1.printf("\r\ngyroz = %d", tmp[6]);
        uart1.printf("\r\n==========");
        uart1.printf("\r\nX = %d", x);
        uart1.printf("\r\nY = %d", y);
        uart1.printf("\r\nZ = %d", z);
        uart1.printf("\r\n==========");
        delay_ms(1000);
    }


}




