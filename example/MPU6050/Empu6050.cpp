
#include "ebox.h"




#include "mpu6050.h"


MPU6050 mpu(&i2c1);

void setup()
{
	eBoxInit();
	uart1.begin(9600);

	mpu.begin(400000);

	
}

int16_t tmp[7];
int16_t x,y,z;
uint8_t id;
int main(void)
{
	setup();
	
	while(1)
	{
	  mpu.get_id(&id);
		mpu.get_data(ACCEL_XOUT_H,tmp,7);
		x = mpu.get_data(ACCEL_XOUT_H);
		y = mpu.get_data(ACCEL_YOUT_H);
		z = mpu.get_data(ACCEL_ZOUT_H);
		delay_ms(10);
		uart1.printf("\r\nid = %d",id);
		uart1.printf("\r\naccx = %d",tmp[0]);
		uart1.printf("\r\naccy = %d",tmp[1]);
		uart1.printf("\r\naccz = %d",tmp[2]);
		uart1.printf("\r\ntemp = %d",tmp[3]);
		uart1.printf("\r\ngyrox = %d",tmp[4]);
		uart1.printf("\r\ngyroy = %d",tmp[5]);
		uart1.printf("\r\ngyroz = %d",tmp[6]);
		uart1.printf("\r\n==========");
		uart1.printf("\r\nX = %d",x);
		uart1.printf("\r\nY = %d",y);
		uart1.printf("\r\nZ = %d",z);
		uart1.printf("\r\n==========");
		delay_ms(1000);
	}


}




