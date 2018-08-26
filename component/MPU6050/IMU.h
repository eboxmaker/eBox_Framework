#ifndef __IMU_H
#define __IMU_H

extern float Yaw,Pitch,Rool;  //Æ«º½½Ç£¬¸©Ñö½Ç£¬·­¹ö½Ç
//extern float gx,gy,gz,ax,ay,az;          //È«¾Ö±äÁ¿  

void IMUupdate(float gxi, float gyi, float gzi, float axi, float ayi, float azi) ;

#endif
