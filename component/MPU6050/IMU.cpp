
#include<math.h>
#include "imu.h"
//---------------------------------------------------------------------------------------------------
// ��������

//#define f 1
#define Kp 10.0f            // ��������֧�������������ٶȼ�/��ǿ��
#define Ki 0.005f           // ��������֧���ʵ�������ƫ�����ν�
#define halfT 0.005f        // �������ڵ�һ��

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;          // ��Ԫ����Ԫ�أ�������Ʒ���
float exInt = 0, eyInt = 0, ezInt = 0;        // ��������С�������


////�����˲�
////ʱ�䳣��   t=a/(1-a)*dt    a=t/(t+dt)         t ����Ƶ��  dt������ʱ��
//#define        kfa   0.98
//#define        kfan  1.0-kfa
////ang= kfa*ang+kfgn*acc;

//#define        kfg   0.80
//#define        kfgn  1.0-kfg
//float Yaw,Pitch,Rool;  //ƫ���ǣ������ǣ�������

//ang= kfa*g+kfgn*acc;

//====================================================================================================
// Function
//====================================================================================================
//gx ����ʱ�仡������
//������Ϊ3��������ݣ�����һ��
//���Ϊ4Ԫ��

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az, float *roll, float *pitch, float *yaw)
{
    float norm;
    float vx, vy, vz;
    float ex, ey, ez;
    //���ӻ����˲�
//    float q0q0 = q0 * q0;
//    float q0q1 = q0 * q1;
//    float q0q2 = q0 * q2;
//    float q0q3 = q0 * q3;
//    float q1q1 = q1 * q1;
//    float q1q2 = q1 * q2;
//    float q1q3 = q1 * q3;
//    float q2q2 = q2 * q2;
//    float q2q3 = q2 * q3;
//    float q3q3 = q3 * q3;


    // ����������
    norm = sqrt(ax * ax + ay * ay + az * az);
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    // ���Ʒ��������
    vx = 2 * (q1 * q3 - q0 * q2);
    vy = 2 * (q0 * q1 + q2 * q3);
    vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

    // ���������ͷ��򴫸��������ο�����֮��Ľ���˻����ܺ�
    ex = (ay * vz - az * vy);
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);

    // ������������������
    exInt = exInt + ex * Ki;
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

    // ������������ǲ���
    gx = gx + Kp * ex + exInt;
    gy = gy + Kp * ey + eyInt;
    gz = gz + Kp * ez + ezInt;

    // ������Ԫ���ʺ�������
    q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
    q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
    q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
    q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;

    // ��������Ԫ
    norm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;
    *pitch   = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3; // pitch
    *roll    = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3; // rollv
    *yaw     = atan2(2.0f * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3;

}

