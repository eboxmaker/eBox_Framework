
#include "ebox.h"
#include "uartx.h"
#include "interrupts.h"

#include "mpu6050.h"


MPU6050 mpu(0x17,0x16);

void setup()
{
	eBoxInit();
	uart3.begin(115200);

	
	mpu.begin();
	mpu.setSpeed(100000);
	pinMode(7,OUTPUT);
}

int16_t tmp[7];
int16_t x,y,z;
int main(void)
{
	setup();
	
	while(1)
	{
		mpu.getData(ACCEL_XOUT_H,tmp,3);
//		x = mpu.getData(ACCEL_XOUT_H);
//		y = mpu.getData(ACCEL_YOUT_H);
//		z = mpu.getData(ACCEL_ZOUT_H);
		uart3.printf("\r\nx = %d",tmp[0]);
		uart3.printf("\r\nx = %d",tmp[1]);
		uart3.printf("\r\nx = %d",tmp[2]);
		uart3.printf("\r\n==========");
//		uart3.printf("\r\nX = %d",x);
//		uart3.printf("\r\nY = %d",y);
//		uart3.printf("\r\nZ = %d",z);
//		uart3.printf("\r\n==========");
		delay_ms(500);
	}


}




