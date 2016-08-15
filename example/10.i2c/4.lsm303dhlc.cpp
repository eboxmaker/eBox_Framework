/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */


#include "ebox.h"

#include "lsm303dhlc.h"


LSM303DLHC lsm(&si2c2);

void setup()
{
    ebox_init();
    uart1.begin(9600);
    uart1.printf("buadrate = 9600");
    lsm.begin(400000);

}
u32 count;
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