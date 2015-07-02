
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

}

int16_t tmp[7];
int16_t x,y,z;
uint8_t id;
int main(void)
{
	setup();
	
	while(1)
	{
	  mpu.getID(&id);
		mpu.getData(ACCEL_XOUT_H,tmp,7);
		x = mpu.getData(ACCEL_XOUT_H);
		y = mpu.getData(ACCEL_YOUT_H);
		z = mpu.getData(ACCEL_ZOUT_H);
		delay_ms(10);
		uart3.printf("\r\nid = %d",id);
		uart3.printf("\r\naccx = %d",tmp[0]);
		uart3.printf("\r\naccy = %d",tmp[1]);
		uart3.printf("\r\naccz = %d",tmp[2]);
		uart3.printf("\r\ntemp = %d",tmp[3]);
		uart3.printf("\r\ngyrox = %d",tmp[4]);
		uart3.printf("\r\ngyroy = %d",tmp[5]);
		uart3.printf("\r\ngyroz = %d",tmp[6]);
		uart3.printf("\r\n==========");
		uart3.printf("\r\nX = %d",x);
		uart3.printf("\r\nY = %d",y);
		uart3.printf("\r\nZ = %d",z);
		uart3.printf("\r\n==========");
		delay_ms(1000);
	}


}




