#include "AK8963.h"


AK8963::AK8963(I2c* i2c, uint8_t address /*= AK8963_ADDRESS*/) :
	I2cIC(i2c),
	address(address)
{
	magBiasSub[0] = 0;
	magBiasSub[1] = 0;
	magBiasSub[2] = 0;
	magSens[0] = 1;
	magSens[1] = 1;
	magSens[2] = 1;
}

void AK8963::begin(uint32_t speed /*= 400000*/)
{
	I2cIC::begin(speed);
	uint8_t res = readByte(address, AK8963_WHO_AM_I);//读取AK8963 ID  
	if (res == AK8963_ID)
	{
		// First extract the factory calibration for each magnetometer axis
		uint8_t rawData[3];  // x/y/z gyro calibration data stored here
		writeByte(address, AK8963_CNTL, 0x00); // Power down magnetometer  
		delay_ms(10);
		writeByte(address, AK8963_CNTL, 0x0F); // Enter Fuse ROM access mode
		delay_ms(10);
		readBytes(address, AK8963_ASAX, 3, &rawData[0]);  // Read the x-, y-, and z-axis calibration values
		sensAdjustVal[0] = (float)(rawData[0] - 128) / 256.0f + 1.0f;   // Return x-axis sensitivity adjustment values, etc.
		sensAdjustVal[1] = (float)(rawData[1] - 128) / 256.0f + 1.0f;
		sensAdjustVal[2] = (float)(rawData[2] - 128) / 256.0f + 1.0f;
		writeByte(address, AK8963_CNTL, 0x00); // Power down magnetometer  
		delay_ms(10);
		// Configure the magnetometer for continuous read and highest resolution
		// set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
		// and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
		//writeByte(address, AK8963_CNTL, Mscale << 4 | Mmode); // Set magnetometer data resolution and sample ODR
		writeByte(address, AK8963_CNTL, 0X16);		//设置AK8963为连续测量模式，频率100HZ，,16bit
		delay_ms(10);
	}
	timer.tic();
}

void AK8963::getMag(float *mx, float *my, float *mz)
{
	short x, y, z;
	getMag(&x, &y, &z);
	//±4800uT 2^16/9600 = 6.83lsb/uT     1/6.83 = 0.1465
	//地磁强度为 5-6 x 10^(-5) T = 50 - 60 uT
	if ((x == 0.0f) && (y == 0.0f) && (z == 0.0f))
	{
		*mx = 0;
		*my = 0;
		*mz = 0;
		return;
	}
	*mx = ((float)x*0.1465 - magBiasSub[0])*magSens[0];
	*my = ((float)y*0.1465 - magBiasSub[1])*magSens[1];
	*mz = ((float)z*0.1465 - magBiasSub[2])*magSens[2];
}

void AK8963::getMag(short *mx, short *my, short *mz)
{
	//限制读取的频率<100Hz，避免AK8963死机？
	if (timer.toc() >= 10 && (readByte(address, AK8963_ST1) & 0x01)) // wait for magnetometer data ready bit to be set
	{
		timer.tic();
		u8 buf[7];
		readBytes(address, AK8963_XOUT_L, 7, buf);
		if (!(buf[6] & 0x08))// Check if magnetic sensor overflow set, if not then report data
		{
			*mx = ((u16)buf[1] << 8) | buf[0];
			*my = ((u16)buf[3] << 8) | buf[2];
			*mz = ((u16)buf[5] << 8) | buf[4];
			*mx *= sensAdjustVal[0];
			*my *= sensAdjustVal[1];
			*mz *= sensAdjustVal[2];
		}
		else
		{
			*mx = 0;
			*my = 0;
			*mz = 0;
		}
	}
	else
	{
		*mx = 0;
		*my = 0;
		*mz = 0;
	}

}

void AK8963::setMagBiasSens(float bx, float by, float bz, float sx, float sy, float sz)
{
	magBiasSub[0] = bx;
	magBiasSub[1] = by;
	magBiasSub[2] = bz;
	magSens[0] = sx;
	magSens[1] = sy;
	magSens[2] = sz;
}
