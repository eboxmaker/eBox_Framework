#ifndef __IMU_H
#define __IMU_H

extern float Yaw,Pitch,Rool;  //ƫ���ǣ������ǣ�������
//extern float gx,gy,gz,ax,ay,az;          //ȫ�ֱ���  

void IMUupdate(float gxi, float gyi, float gzi, float axi, float ayi, float azi) ;

#endif
