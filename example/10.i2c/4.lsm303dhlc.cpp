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

#include "lsm303dhlc.h"

/**
	*	1	此例程需要调用eDrive目录下的lsm303dhlc驱动
	*	2	此例程演示了lsm303dhlc的基本操作
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"L3G4200D example"
#define EXAMPLE_DATE	"2018-08-11"



LSM303DLHC lsm(&sI2c2);

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    lsm.begin();



}
int main(void)
{
    setup();
    while(1)
    {
        lsm.read_acc();
        lsm.read_mag();
        uart1.printf("==========================\r\n");
        uart1.printf("x:%d;\r\n", lsm.acc_x);
        uart1.printf("y:%d;\r\n", lsm.acc_y);
        uart1.printf("z:%d;\r\n", lsm.acc_z);
        //lsm.read_reg(LSM303_MAG_ADDR,LSM303M_CRB_REG);
        uart1.printf("x:%d;\r\n", lsm.mag_x);
        uart1.printf("y:%d;\r\n", lsm.mag_y);
        uart1.printf("z:%d;\r\n", lsm.mag_z);
        delay_ms(1000);
    }


}