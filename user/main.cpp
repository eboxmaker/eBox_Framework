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



UartStream uartStream(&uart1);
MPU6050 mpu(&i2c1);

NiMingSenser_t senser;
NiMingStatus_t status;

Kalman kalmanX,kalmanY,kalmanZ;


#define NUM 50
FilterMoveWindow filterx(NUM);
FilterMoveWindow filtery(NUM);
FilterMoveWindow filterz(NUM);
FilterMoveWindow filtergx(NUM);
FilterMoveWindow filtergy(NUM);
FilterMoveWindow filtergz(NUM);


float SelfTest[6];
int16_t accelCount[3];  // Stores the 16-bit signed accelerometer sensor output
int16_t gyroCount[3];   // Stores the 16-bit signed gyro sensor output
float gyroBias[3] = {0, 0, 0}, accelBias[3] = {0, 0, 0}; // Bias corrections for gyro and accelerometer

float aRes, gRes; // scale resolutions per LSB for the sensors
float ax, ay, az;       // Stores the real accel value in g's
float gx, gy, gz;       // Stores the real gyro value in degrees per seconds
float pitch, yaw, roll;






double kalAngleX, kalAngleY, kalAngleZ; // Calculated angle using a Kalman filter
void update();

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    mpu.begin(400000);
    test.begin(uartStream);

    // Read the WHO_AM_I register, this is a good test of communication
    uint8_t c = mpu.readByte(MPU6050_ADDRESS, WHO_AM_I_MPU6050);  // Read WHO_AM_I register for MPU-6050
    if (c == 0x68) 
    {
        mpu.MPU6050SelfTest(SelfTest); // Start by performing self test and reporting values

        if (SelfTest[0] < 1.0f && SelfTest[1] < 1.0f && SelfTest[2] < 1.0f && SelfTest[3] < 1.0f && SelfTest[4] < 1.0f && SelfTest[5] < 1.0f)
        {
            Serial.println("Pass Selftest!");

            mpu.calibrateMPU6050(gyroBias, accelBias); // Calibrate gyro and accelerometers, load biases in bias registers
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

            
            status.roll = roll*100;
            status.pitch = -pitch*100;
            status.yaw = -yaw*100;
            
            
            senser.ax = ax;
            senser.ay = ay;
            senser.az = az;
            
            senser.gx = gx*100;
            senser.gy = gy*100;
            senser.gz = gz*100;
            
            
                
            test.sendNMSenser(senser);
            test.sendNMStatues(status);
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
        double dt = delta_t/1000.0;
        count = millis();
        if (mpu.readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01) // check if data ready interrupt
        { 
            mpu.readAccelData(accelCount);  // Read the x/y/z adc values
            aRes = mpu.getAres();

            // Now we'll calculate the accleration value into actual g's
            ax = (float)accelCount[0] * aRes; // get actual g value, this depends on scale being set
            ay = (float)accelCount[1] * aRes;
            az = (float)accelCount[2] * aRes;
            
    //        ax = filterx.in(ax);
    //        ay = filtery.in(ay);
    //        az = filterz.in(az);
            
            mpu.readGyroData(gyroCount);  // Read the x/y/z adc values
            gRes = mpu.getGres();

            // Calculate the gyro value into actual degrees per second
            gx = (float)gyroCount[0] * gRes; // get actual gyro value, this depends on scale being set
            gy = (float)gyroCount[1] * gRes;
            gz = (float)gyroCount[2] * gRes;
            
    //        gx = filtergx.in(gx);
    //        gy = filtergy.in(gy);
    //        gz = filtergz.in(gz);
            IMUupdate(gx * PI / 180.0f, gy * PI / 180.0f, gz * PI / 180.0f,ax, ay, az);
            roll = Rool;
            pitch = Pitch;
            yaw = Yaw;


          
        }
    }

}
