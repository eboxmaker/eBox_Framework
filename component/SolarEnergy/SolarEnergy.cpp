/**
  ******************************************************************************
  * @file    pwm.cpp
  * @author  shentq
  * @version V2.0
  * @date    2016/08/14
  * @brief   ebox application example .
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/


#include "ebox.h"
#include "math.h"
#include "ebox_encoder.h"
#include "oled_ssd1306_128x32.h"

//SoftI2c si2cx(&PB10, &PB11);
OledSSD1306_128x32 oled(&mcuI2c2);

class Fun
{
    public:
        double x1;
        double x2;
        Fun(double a,double b,double c)
        {
            if (b*b - 4 * a*c < 0)//判断方程是否有实根
            {
                uart1.printf("方程无实根！");
            }
            else//如果有解就打印出方程的根
            {
                x1 = (-b + sqrt(b * b - 4 * a*c)) / (2 * a);
                x2 = (-b - sqrt(b * b - 4 * a*c)) / (2 * a);
                uart1.printf("输出为：x1=%0.2f,x2=%0.2f\n", x1, x2);
            }
        }
};

class SolarEnergy
{
    public:
        float res;
        float e;
    
        SolarEnergy()
        {
            res = 10;
            e = 50;
        }
        void setPowerOut(float p)
        {
            power_out = p;
            power_inner = e*e/res - p;
            cur = sqrt(power_inner/res);
            vout = cur*res;
        }
        void loop()
        {
            
        }
    public:
        float power_out;
        float cur;
        float vout;
        float power_inner;

};
class DCDC
{
    public:
        float vin;
        float vout;
        float iout;
        float load;
        float power_load;
        float power_in;
        float efficiency;
        float percent;
        DCDC(SolarEnergy *s)
        {
            se = s;
            load = 10;
        };
        
        void setPercent(float percent)
        {
            double a = (se->res/load)*percent*percent + 1;
            double b = -se->e;
            double c = 0;
            uart1.printf("------percent:%0.2f---------\n",percent);
            Fun f1(a,b,c);
            vin = f1.x1;
            vout = vin*percent;
            power_load = vout *vout / load;
            power_in = (se->e - vin)*(se->e - vin)/se->res;
            se->setPowerOut(power_in);
            
            uart1.printf("vout:%0.1f\t vin:%0.1f\t",vout,vin);
            uart1.printf("pout\t%0.2f\t pin\t%0.2f\t pAll\t%0.2f\t n\t%0.3f\n",power_load,power_in,power_load + power_in,power_load / (power_load + power_in));

        }
        
        
        void setPayload(float _load)
        {
            load = _load;
        }
        void loop()
        {
//            vol = e * load / ( load + res) * load / ( load + res);
//            cur = vol / (load );
//            power = vol * cur;
        }
    
    private:
        SolarEnergy *se;
};

class Load
{
    public:
        float e;
        float res;
        
};

SolarEnergy se;
DCDC dc(&se);
void setup()
{
    ebox_init();
    uart1.begin(115200);
    dc.setPayload(0.1);
}
int x = 0;
int y = 0;
uint32_t last = 0;

int main(void)
{

    setup();
    double percent = 0.1;
    for(int i = 0; i <= 100; i++)
    {
        percent = i*0.01;
        dc.setPercent(percent);
    }
    while(1)
    {
        if(millis() - last > 1000)
        {
            last = millis();
//            percent += 0.1;
//            if(percent > 10) percent = 0.1;
//            dc.setPercent(percent);

//            uart1.printf("vol:%0.1f\tcur:%0.1f\tp:%0.1f\n",se.vol,se.cur,se.power);
        }
        se.loop();
        delay_ms(1);
    }
}




