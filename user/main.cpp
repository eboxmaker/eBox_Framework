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
#include "ads7816.h"
#include "USR_GM3.H"

UsrGm3 gm3(&uart2,&PA4);

char buf[100];

void test1()
{
    uart2.write(uart1.read());
}

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.attach(test1,RxIrq);
    gm3.begin(115200);
    gm3.at_cmd_go();
    gm3.q_version();
    gm3.q_e();
    gm3.set_wkmod(NET);
    gm3.set_sockaen(1);
    gm3.set_socka("TCP","test.usr.cn","2317");
    gm3.set_hearten(1);
    gm3.set_heartdt("www.eboxmaker.com");
    gm3.set_hearttp(H_NET);
    gm3.set_hearttm(4);
    gm3.q_hearten();
    gm3.q_heartdt(buf);
    gm3.q_hearttp();
    gm3.q_hearttm();
    gm3.save();
}
uint64_t last_updata_time;
int main(void)
{
    setup();
    last_updata_time = millis();
    while(1)
    {
        if(millis() - last_updata_time >= 500)
        {
            last_updata_time = millis();      

        }
        if( gm3.pNetBuf.available() > 0 )
            {
                uart1.write(gm3.pNetBuf.read());
            
            }
    }

}


