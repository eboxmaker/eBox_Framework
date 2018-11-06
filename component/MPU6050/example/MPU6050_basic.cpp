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

#include "kalman.h"
#include "filters.h"

#include "../mpu6050/mpu6050.h"
#include "../mpu6050/fourAxle.h"
#include "../mpu6050/imu.h"
/**
	*	1	此例程需要调用eDrive目录下的mpu6050驱动
	*	2	此例程演示了mpu6050的基本操作
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"mpu6050 example"
#define EXAMPLE_DATE	"2018-08-11"


//串口协议相关
NiMingSenser_t senser;
NiMingStatus_t status;
UartStream uartStream(&uart1);
FourAxle sender;

//传感器
MPU6050 mpu(&si2c1);

Vector na, ng;
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    mpu.begin(400000);
    sender.begin(uartStream);

    // Read the WHO_AM_I register, this is a good test of communication
    uint8_t c = mpu.readByte(MPU6050_ADDRESS, WHO_AM_I_MPU6050);  // Read WHO_AM_I register for MPU-6050
    if (c == 0x68)
    {
        if(mpu.MPU6050SelfTest()) // Start by performing self test and reporting values
        {

            Serial.println("Pass Selftest!");
            Serial.print("x-axis self test: acceleration trim within : ");
            Serial.print(mpu.SelfTest[0], 1);
            Serial.println("% of factory value");
            Serial.print("y-axis self test: acceleration trim within : ");
            Serial.print(mpu.SelfTest[1], 1);
            Serial.println("% of factory value");
            Serial.print("z-axis self test: acceleration trim within : ");
            Serial.print(mpu.SelfTest[2], 1);
            Serial.println("% of factory value");
            Serial.print("x-axis self test: gyration trim within : ");
            Serial.print(mpu.SelfTest[3], 1);
            Serial.println("% of factory value");
            Serial.print("y-axis self test: gyration trim within : ");
            Serial.print(mpu.SelfTest[4], 1);
            Serial.println("% of factory value");
            Serial.print("z-axis self test: gyration trim within : ");
            Serial.print(mpu.SelfTest[5], 1);
            Serial.println("% of factory value");

            mpu.calibrateMPU6050(); // Calibrate gyro and accelerometers, load biases in bias registers
            mpu.initMPU6050();
            Serial.println("MPU6050 initialized for active data mode...."); // Initialize device for active mode read of acclerometer, gyroscope, and temperature
        }

    }
    else
    {
        Serial.print("Could not connect to MPU6050: 0x");
        Serial.println(c, HEX);
        while (1) ; // Loop forever if communication doesn't happen
    }
}
uint32_t lastUpdate = 0;         // used to calculate integration interval
int main(void)
{
    setup();

    while(1)
    {

        if(millis() - lastUpdate > 50)
        {
            lastUpdate = millis();

            na = mpu.readNormalizeAccel();
            ng = mpu.readAngleGyro();


            //扩大100倍用于显示

            senser.ax = na.XAxis * 100;
            senser.ay = na.YAxis * 100;
            senser.az = na.ZAxis * 100;

            senser.gx = ng.XAxis * 100;
            senser.gy = ng.YAxis * 100;
            senser.gz = ng.ZAxis * 100;


            sender.sendNMSenser(senser);
            sender.sendNMStatues(status);
        }
    }
}

