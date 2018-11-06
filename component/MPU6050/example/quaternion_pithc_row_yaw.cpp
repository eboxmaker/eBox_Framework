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

//姿态和算法
Vector na, ng;
float pitch, yaw, roll;

//窗口滤波器
#define NUM 50
FilterMoveWindow filterx(NUM);
FilterMoveWindow filtery(NUM);
FilterMoveWindow filterz(NUM);
FilterMoveWindow filtergx(NUM);
FilterMoveWindow filtergy(NUM);
FilterMoveWindow filtergz(NUM);




void update();

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
        update();

        if(millis() - lastUpdate > 50)
        {
            lastUpdate = millis();


            //扩大100倍用于显示
            status.roll = roll * 100;
            status.pitch = -pitch * 100;
            status.yaw = -yaw * 100;

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

uint32_t delta_t;
uint32_t count = 0;  // used to control display output rate
void update()
{
    if(millis() - count >= 10)
    {
        delta_t = millis() - count;
        double dt = delta_t / 1000.0;
        count = millis();
        if (mpu.readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01) // check if data ready interrupt
        {
            na = mpu.readNormalizeAccel();
            ng = mpu.readAngleGyro();



            //            na.XAxis = filterx.in(na.XAxis);
            //            na.YAxis = filtery.in(na.YAxis);
            //            na.ZAxis = filterz.in(na.ZAxis);
            //            ng.XAxis = filtergx.in(ng.XAxis);
            //            ng.YAxis = filtergy.in(ng.YAxis);
            //            ng.ZAxis = filtergz.in(ng.ZAxis);

            IMUupdate(ng.XAxis, ng.YAxis, ng.ZAxis, na.XAxis, na.YAxis, na.ZAxis, &roll, &pitch, &yaw);



        }
    }

}
