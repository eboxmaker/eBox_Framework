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
#include "ADS8866.h"
#include "bsp_ebox.h"

Ads8866 ad(&PA5,&PA6,&PA4);


#define MAJOR           '2' // 
#define MINOR          '3' // 
#define BUGFIX         '1' // 

#define VERSION (MAJOR##'.'##MINOR##'.'##BUGFIX)

char A_param=0;
 
char B_pramm=0;
 
//添加宏定义
 
#define OBJECT     A 
#define DEFINE_(X)  X##_param    //一次定义
#define DEFINE(X)   DEFINE_(X) //再次定义
#define PARAM  DEFINE(OBJECT)



void setup()
{
    int ret;
    ebox_init();
    uart1.begin(115200);
    print_log();
    PB8.mode(OUTPUT_PP);

    ad.begin();

}
int main(void)
{
    float temper;
    setup();
    uint32_t last = millis();
    uint16_t hex;
    float vol;
    
    
     uart1.printf(VERSION);


    uart1.printf("A_param:%d\n",A_param);
    PARAM=100;//这样也是可以的
    uart1.printf("A_param:%d\n",A_param);
    DEFINE(A)=200; //这个操作就神了，先替换再拼接，我们只是做了二次定义，解决一切烦恼
    uart1.printf("A_param:%d\n",A_param);

    while(1)
    {
        hex = ad.read();
        vol = hex * 2.5 / 65535.0 ;
        //uart1.printf("adc:0X%04x,%0.6fV\r\n",hex,vol);

        delay_ms(1000);
    }

}


