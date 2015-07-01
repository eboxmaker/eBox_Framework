#include "i2c.h"

/* MPU6050 Register Address ------------------------------------------------------------*/
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG				0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据

void  I2C::i2cBegin()
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
		/* 使能与 I2C1 有关的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  

	pinMode(_SDAPin,AF_OD);
	pinMode(_SCLPin,AF_OD);


	  /* I2C 配置 */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ; 
	//I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 
	//I2C_InitStructure.I2C_OwnAddress1 = SlaveAddress; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
	I2C_InitStructure.I2C_ClockSpeed = 200000; 

	/* I2C1 初始化 */
	I2C_Init(I2C1, &I2C_InitStructure);	   
	
	/* 使能 I2C1 */
	I2C_Cmd  (I2C1,ENABLE); 
	/*允许应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);   

}

void I2C::setSpeed(uint32_t speed)
{
	I2C_InitStructure.I2C_ClockSpeed = speed; 
	I2C_Init(I2C1, &I2C_InitStructure);	   
	/* 使能 I2C1 */
	I2C_Cmd  (I2C1,ENABLE); 
	/*允许应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);   

}
void I2C::start()
{
	I2C_GenerateSTART(I2C1,ENABLE);

	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

}
void I2C::stop()
{
	I2C_GenerateSTOP(I2C1,ENABLE);
}
void I2C::sendNoAck()
{
	I2C_AcknowledgeConfig(I2C1,DISABLE);

}
void I2C::sendAck()
{
	I2C_AcknowledgeConfig(I2C1,ENABLE);

}


void I2C::sendByte(uint8_t Data)
{
	I2C_SendData(I2C1,Data);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}
void I2C::send7BitsAddress(uint8_t address)
{
	if(address&0x01)
	{
		I2C_Send7bitAddress(I2C1,address,I2C_Direction_Receiver);
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));		
	}
	else
	{
		I2C_Send7bitAddress(I2C1,address,I2C_Direction_Transmitter);
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}

}
uint8_t I2C::receiveByte()
{
	while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)));
	return I2C_ReceiveData(I2C1);//读出寄存器数据
}


void I2C::writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t regData)
{
	

	start();
	send7BitsAddress(slaveAddress);
	sendByte(regAddress);
	sendByte(regData);
	stop();

	
}
int8_t I2C::readByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data)
{
	start();
	send7BitsAddress(slaveAddress);
	I2C_Cmd(I2C1,ENABLE);
	sendByte(regAddress);
	start();
	send7BitsAddress(SlaveAddress + 1);
	sendNoAck();
	stop();
	*data = receiveByte();
	sendAck();
	return 0;
}

int8_t I2C::readByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint8_t numToRead)
{
	uint8_t i;
	start();
	send7BitsAddress(slaveAddress);
	I2C_Cmd(I2C1,ENABLE);
	sendByte(regAddress);
	start();
	send7BitsAddress(SlaveAddress + 1);
	
		while(numToRead)
		{
			if(numToRead == 1)
			{
				sendNoAck();
				stop();
			}
			*data = receiveByte();
			data++;
			numToRead--;
			i++;
		}
		sendAck();

	return i;
}

