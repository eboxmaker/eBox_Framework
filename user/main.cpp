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
#include "kalman.h"
#include "../component/fouraxle/fourAxle.h"
/**
	*	1	此例程需要调用eDrive目录下的mpu6050驱动
	*	2	此例程演示了mpu6050的基本操作
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"mpu6050 example"
#define EXAMPLE_DATE	"2018-08-11"



float aRes, gRes; // scale resolutions per LSB for the sensors
// Pin definitions
int intPin = 12;  // These can be changed, 2 and 3 are the Arduinos ext int pins
#define blinkPin 13  // Blink LED on Teensy or Pro Mini when updating
boolean blinkOn = false;
int16_t accelCount[3];  // Stores the 16-bit signed accelerometer sensor output
float ax, ay, az;       // Stores the real accel value in g's
int16_t gyroCount[3];   // Stores the 16-bit signed gyro sensor output
float gx, gy, gz;       // Stores the real gyro value in degrees per seconds
float gyroBias[3] = {0, 0, 0}, accelBias[3] = {0, 0, 0}; // Bias corrections for gyro and accelerometer
int16_t tempCount;   // Stores the real internal chip temperature in degrees Celsius
float temperature;
float SelfTest[6];
static double q[4] = {1.0f, 0.0f, 0.0f, 0.0f};            // vector to hold quaternion
uint32_t delt_t = 0; // used to control display output rate
uint32_t count = 0;  // used to control display output rate
float pitch, yaw, roll;
// parameters for 6 DoF sensor fusion calculations
float GyroMeasError = PI * (40.0f / 180.0f);     // gyroscope measurement error in rads/s (start at 60 deg/s), then reduce after ~10 s to 3
float beta = sqrt(3.0f / 4.0f) * GyroMeasError;  // compute beta
float GyroMeasDrift = PI * (2.0f / 180.0f);      // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)
float zeta = sqrt(3.0f / 4.0f) * GyroMeasDrift;  // compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value
float deltat = 0.0f;                              // integration interval for both filter schemes
uint32_t lastUpdate = 0, firstUpdate = 0;         // used to calculate integration interval
uint32_t Now = 0;                                 // used to calculate integration interval


MPU6050 mpu(&si2c1);

UartStream uartStream(&uart1);
int16_t data[9];
NiMingStatus_t status;
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    mpu.begin(0);
    test.begin(uartStream);

    data[0] = 12;
    data[8] = 34;

    
}
int main(void)
{
    setup();

    while(1)
    {
    status.roll += 10;
    status.pitch += 10;
    status.yaw += 10;
        status.alt += 10;
//        test.sendNChannel(1,data,9);
        test.sendNMStatues(status);
        delay_ms(5);
    }
}

