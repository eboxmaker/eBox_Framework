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
#include "pid_v1.h"
#include "PID_AutoTune_v0.h"

void AutoTuneHelper(bool start);
void changeAutoTune();
void SerialSend();
void SerialReceive();
void DoModel();


uint8_t ATuneModeRemember=2;


//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=20, Ki= 1200, Kd=0.01;


double kpmodel=1.5, taup=100, theta[50];
double outputStart=5;
double aTuneStep=50, aTuneNoise=1, aTuneStartValue=100;
unsigned int aTuneLookBack=20;

//set to false to connect to the real world
bool useSimulation = true;

bool tuning = false;
unsigned long  modelTime, serialTime;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd,P_ON_E, DIRECT);
PID_ATune aTune(&Input, &Output);
Encoder encoder(TIM4,&PB6,&PB7);
float x;
uint16_t y;
Pwm pwm1(&PA0);
void setup()
{
    ebox_init();
    uart1.begin(115200);
    encoder.begin(3);
    pwm1.begin(2000, 0000);
    pwm1.set_oc_polarity(1);//set output polarity after compare
    uart1.printf("max frq = %dKhz\r\n",pwm1.get_max_frq()/1000);
    uart1.printf("max frq = %f\r\n",pwm1.get_accuracy());
    
    if(useSimulation)
    {
        for(uint8_t i=0;i<50;i++)
        {
        theta[i]=outputStart;
        }
        modelTime = 0;
    }
    //initialize the variables we're linked to
    Input = 0;
    Setpoint = 35.5;

    //turn the PID on
    myPID.SetMode(AUTOMATIC);
    
    if(tuning)
    {
        tuning=false;
        changeAutoTune();
        tuning=true;
    }
  
  serialTime = 0;
}
int main(void)
{
    static uint64_t last_time = millis();
    static uint64_t last_time1 = millis();
    static uint64_t now = millis();
    setup();
    uint16_t temp;
    float speed;
    while(1)
    {
          if(!useSimulation)
          { //pull the input in from the real world
            Input = encoder.read_speed()/4000;

          }
        if(tuning)
        {
            uint8_t val = (aTune.Runtime());
            if (val!=0)
            {
                tuning = false;
            }
            if(!tuning)
            { //we're done, set the tuning parameters
                Kp = aTune.GetKp();
                Ki = aTune.GetKi();
                Kd = aTune.GetKd();
                myPID.SetTunings(Kp,Ki,Kd);
                AutoTuneHelper(false);
            }

        }
        else 
            myPID.Compute();
        if(useSimulation)
        {
             uart1.printf("%0.2f\t%0.2f\r\n",Input,Output);
           theta[30]=Output;
            if(now>=modelTime)
            {
                modelTime +=100; 
                DoModel();
            }
        }
        else
        {
            pwm1.set_duty(0); 
        }

        //send-receive with processing if it's time
        if(millis()>serialTime)
        {
//            SerialReceive();
//            SerialSend();
            serialTime+=500;
        }
//    if(millis() - last_time > 5)
//    {
//        last_time = millis();
//        Input  = encoder.read_speed()/4000;
//        pwm1.set_duty(Output);
//        uart1.printf("%0.2f\t%0.2f\r\n",Input,Output);
//    }
//        myPID.Compute();
//        if(millis() - last_time1 > 10)
//        {
//            last_time1 = millis();
//            uart1.printf("%0.2f\t%0.2f\r\n",Input,Output);
//        }       
        
    }
}
void changeAutoTune()
{
 if(!tuning)
  {
    //Set the output to the desired starting frequency.
    Output=aTuneStartValue;
    aTune.SetNoiseBand(aTuneNoise);
    aTune.SetOutputStep(aTuneStep);
    aTune.SetLookbackSec((int)aTuneLookBack);
    AutoTuneHelper(true);
    tuning = true;
  }
  else
  { //cancel autotune
    aTune.Cancel();
    tuning = false;
    AutoTuneHelper(false);
  }
}

void AutoTuneHelper(bool start)
{
  if(start)
    ATuneModeRemember = myPID.GetMode();
  else
    myPID.SetMode(ATuneModeRemember);
}
void SerialSend()
{
  uart1.print("setpoint: ");uart1.print(Setpoint); uart1.print(" ");
  uart1.print("input: ");uart1.print(Input); uart1.print(" ");
  uart1.print("output: ");uart1.print(Output); uart1.print(" ");
  if(tuning){
    uart1.println("tuning mode");
  } else {
    uart1.print("kp: ");uart1.print(myPID.GetKp());uart1.print(" ");
    uart1.print("ki: ");uart1.print(myPID.GetKi());uart1.print(" ");
    uart1.print("kd: ");uart1.print(myPID.GetKd());uart1.println();
  }
}

void SerialReceive()
{

   char b = uart1.read(); 
   if((b=='1' && !tuning) || (b!='1' && tuning))
       changeAutoTune();
}
void DoModel()
{
  //cycle the dead time
  for(uint8_t i=0;i<49;i++)
  {
    theta[i] = theta[i+1];
  }
  //compute the input
  Input = (kpmodel / taup) *(theta[0]-outputStart) + Input*(1-1/taup) + ((float)random(-10,10))/100;

}
