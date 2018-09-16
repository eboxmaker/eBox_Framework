/**
  ******************************************************************************
  * @file    i2c.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "ebox_i2c.h"

// 分频，建立，保持，高电平，低电平
#define	C16M10K	 	__LL_I2C_CONVERT_TIMINGS(0x3, 0xc7, 0xc3, 0x02, 0x04)		//10k@16M
#define	C16M100K	__LL_I2C_CONVERT_TIMINGS(0x3, 0x13, 0xf, 0x02, 0x04)		//100k@16M
#define	C16M400K	__LL_I2C_CONVERT_TIMINGS(0x1, 0x9, 0x3, 0x2, 0x3)			//400k@16M

#define	C48M10K	 	__LL_I2C_CONVERT_TIMINGS(0xb, 0xc7, 0xc3, 0x02, 0x04)		//10k@48M
#define	C48M100K	__LL_I2C_CONVERT_TIMINGS(0xb, 0x13, 0xf, 0x02, 0x04)		//100k@48M
#define	C48M400K	__LL_I2C_CONVERT_TIMINGS(0x5, 0x9, 0x3, 0x3, 0x3)			//400k@48M

#define	C8M10K	 	__LL_I2C_CONVERT_TIMINGS(0x1, 0xc7, 0xc3, 0x02, 0x04)		//10k@48M
#define	C8M100K	 	__LL_I2C_CONVERT_TIMINGS(0x1, 0x13, 0xf, 0x02, 0x04)		//100k@48M
#define	C8M400K	 	__LL_I2C_CONVERT_TIMINGS(0x0, 0x9, 0x3, 0x1, 0x3)			//400k@48M

mcuI2c::mcuI2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin)
{
  busy = 0;
  this->I2Cx = I2Cx;
  this->scl_pin = scl_pin;
  this->sda_pin = sda_pin;

}
void  mcuI2c::begin(uint32_t _speed)
{
  switch (cpu.clock.pclk1/1000000){
  case 16:
    switch (_speed){
    case 10:
      this->speed = C16M10K;	// 10k 	@16M
      break;
    case 100:
      this->speed = C16M100K;		// 100k @16M
      break;
    case 400:
    default:
      this->speed = C16M400K;			// 400k @16M
    }
    break;
  case 48:
    switch (speed){
    case 10:
      this->speed = C48M10K;	// 10k 	@48M
      break;
    case 100:
      this->speed = C48M100K;		// 100k @48M
      break;
    case 400:
    default:
      this->speed = C48M400K;			// 400k @48M
    }
    break;
  case 8:
  default:	// 默认系统时钟为8M
    switch (speed){
    case 10:
      this->speed = C8M10K;	// 10k 	@8M
      break;
    case 100:
      this->speed = C8M100K;		// 100k @8M
      break;
    case 400:
    default:
      this->speed = C8M400K;			// 400k @8M
    }
  }
  this->speed = _speed;
//    I2C_InitTypeDef I2C_InitStructure;


  rcc_clock_cmd((uint32_t)I2Cx,ENABLE);
  sda_pin->mode(AF_PP_PU,LL_GPIO_AF_1);
  scl_pin->mode(AF_PP_PU,LL_GPIO_AF_1);

  config(this->speed);

  /* I2c 配置 */
//    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ;
//    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//    //I2C_InitStructure.I2C_OwnAddress1 = SlaveAddress;
//    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//    I2C_InitStructure.I2C_ClockSpeed = this->speed;

//    /* I2C1 初始化 */
//    I2C_DeInit(I2Cx);
//    I2C_Init(I2Cx, &I2C_InitStructure);

//    /* 使能 I2C1 */
//    I2C_Cmd  (I2Cx, ENABLE);

}

void mcuI2c::config(uint32_t speed)
{
  this->speed = speed;
//    I2C_InitTypeDef I2C_InitStructure;

//    /* I2c 配置 */
//    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ;
//    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//    //I2C_InitStructure.I2C_OwnAddress1 = SlaveAddress;
//    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

//    I2C_InitStructure.I2C_ClockSpeed = this->speed;
//    I2C_Init(I2Cx, &I2C_InitStructure);
//    /* 使能 I2C1 */
//    I2C_Cmd  (I2Cx, ENABLE);
//    /*允许应答模式*/
//    I2C_AcknowledgeConfig(I2Cx, ENABLE);
  (I2Cx == I2C1)?(LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_SYSCLK)):(void());

  LL_I2C_Disable(I2Cx);
  LL_I2C_SetTiming(I2Cx,speed);
  LL_I2C_Enable(I2Cx);

}
uint32_t mcuI2c::read_config()
{
  return this->speed;
}
int8_t mcuI2c::start()
{
//    uint16_t times = 1000;
  int8_t err = 0;
//    I2C_GenerateSTART(I2Cx, ENABLE);

//    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
//    {
//        times--;
//        if(times == 0)
//        {
//            err = -1;
//            break;
//        }
//    }
  return err;
}
int8_t mcuI2c::stop()
{
  int8_t err = 0;
//    I2C_GenerateSTOP(I2Cx, ENABLE);
  return err;
}
int8_t mcuI2c::send_no_ack()
{
  int8_t err = 0;
//    I2C_AcknowledgeConfig(I2Cx, DISABLE);
  return err;
}
int8_t mcuI2c::send_ack()
{
  int8_t err = 0;
//    I2C_AcknowledgeConfig(I2Cx, ENABLE);
  return err;
}


int8_t mcuI2c::send_byte(uint8_t data)
{
//    uint16_t times = 1000;
  int8_t err = 0;
//    I2C_SendData(I2Cx, data);
//    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//    {
//        times--;
//        if(times == 0)
//        {
//            err = -2;
//            break;
//        }
//    }
  return err;
}
int8_t mcuI2c::send_7bits_address(uint8_t slave_address)
{
//    uint16_t times = 5000;
  int8_t err = 0;
//    if(slave_address & 0x01)
//    {
//        I2C_Send7bitAddress(I2Cx, slave_address, I2C_Direction_Receiver);
//        while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
//        {
//            times--;
//            if(times == 0)
//            {
//                err = -3;
//                break;
//            }
//        }
//    }
//    else
//    {
//        I2C_Send7bitAddress(I2Cx, slave_address, I2C_Direction_Transmitter);
//        while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//        {
//            times--;
//            if(times == 0)
//            {
//                err = -4;
//                break;
//            }
//        }
//    }
  return err;

}
int8_t mcuI2c::receive_byte(uint8_t *data)
{
//    uint16_t times = 1000;
  int8_t err = 0;
//    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
//    {
//        times--;
//        if(times == 0)
//        {
//            err = -5;
//            break;
//        }
//    }
//    *data = I2C_ReceiveData(I2Cx);//读出寄存器数据
  return err;
}


int8_t mcuI2c::write_byte(uint8_t slave_address, uint8_t data)
{
  uint16_t err = 0;

#if	(TIMEOUT != 0)
  uint32_t end = GetEndTime(timeout_us);
#endif
  LL_I2C_HandleTransfer(I2Cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,1,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);

  while (!LL_I2C_IsActiveFlag_STOP(I2Cx))
  {
    if (LL_I2C_IsActiveFlag_TXIS(I2Cx))
    {
      LL_I2C_TransmitData8(I2Cx,data);
    }
#if	(TIMEOUT != 0)
    if (IsTimeOut(end,timeout_us)) return E_TIMEOUT;
#endif
  }
  LL_I2C_ClearFlag_STOP(I2Cx);

//	return E_OK;

  return err;
}

int8_t mcuI2c::write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t data){
    return 0;
}

int8_t mcuI2c::write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t len)
{
  uint16_t err = 0;

#if	(TIMEOUT != 0)
  uint64_t end = GetEndTime(timeout_us);
#endif
  if (len >255){
    // num_to_writ>255: RELOAD,NBYTE=0xFF,START
    LL_I2C_HandleTransfer(I2Cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,0xFF,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_START_WRITE);

    while (!LL_I2C_IsActiveFlag_TCR(I2Cx))
    {
      if (LL_I2C_IsActiveFlag_TXIS(I2Cx))
      {
        LL_I2C_TransmitData8(I2Cx,*data++);
      }
#if	(TIMEOUT != 0)
      if (IsTimeOut(end,timeout_us))	return E_TIMEOUT;
#endif
    }
    len = len - 255;
    // num_to_writ>255: RELOAD,NBYTE=0xFF,NOSTART

    while (len > 255){
      // 发送地址寄存器
      LL_I2C_HandleTransfer(I2Cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,0xFF,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_NOSTARTSTOP);
#if	(TIMEOUT != 0)
      end = GetEndTime(timeout_us);
#endif
      while (!LL_I2C_IsActiveFlag_TCR(I2Cx))
      {
        if (LL_I2C_IsActiveFlag_TXIS(I2Cx))
        {
          LL_I2C_TransmitData8(I2Cx,*data++);
        }
#if	(TIMEOUT != 0)
        if (IsTimeOut(end,timeout_us))	return E_TIMEOUT;
#endif
      }
      len = len - 255;
    }
    // num_to_writ<=255: AUTOEND,NBYTE=num_to_writ,NOSTART
    LL_I2C_HandleTransfer(I2Cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,len,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_NOSTARTSTOP);
#if	(TIMEOUT != 0)
    end = GetEndTime(timeout_us);
#endif
    while (!LL_I2C_IsActiveFlag_STOP(I2Cx))
    {
      if (LL_I2C_IsActiveFlag_TXIS(I2Cx))
      {
        LL_I2C_TransmitData8(I2Cx,*data++);
      }
#if	(TIMEOUT != 0)
      if (IsTimeOut(end,timeout_us))	return E_TIMEOUT;
#endif
    }
    LL_I2C_ClearFlag_STOP(I2Cx);
  }else{
    // 写入地址寄存器和数据
    LL_I2C_HandleTransfer(I2Cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,len,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);
#if	(TIMEOUT != 0)
    end = GetEndTime(timeout_us);
#endif
    while (!LL_I2C_IsActiveFlag_STOP(I2Cx))
    {
      if (LL_I2C_IsActiveFlag_TXIS(I2Cx))
      {
        LL_I2C_TransmitData8(I2Cx,*data++);
      }
#if	(TIMEOUT != 0)
      if (IsTimeOut(end,timeout_us))	return E_TIMEOUT;
#endif
    }
    LL_I2C_ClearFlag_STOP(I2Cx);
  }

  return err;

}
int8_t mcuI2c::read_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data)
{
//    start();
//    send_7bits_address(slave_address);
//    I2C_Cmd(I2Cx, ENABLE);
//    send_byte(reg_address);
//    start();
//    send_7bits_address(slave_address + 1);
//    send_no_ack();
//    stop();
//    receive_byte(data);
//    send_ack();
  read_byte(slave_address,reg_address,data,1);
  return 0;
}

int8_t mcuI2c::read_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t len)
{
  uint8_t i = 0;
//    start();
//    send_7bits_address(slave_address);
//    I2C_Cmd(I2Cx, ENABLE);
//    send_byte(reg_address);
//    start();
//    send_7bits_address(slave_address + 1);

//    while(len)
//    {
//        if(len == 1)
//        {
//            send_no_ack();
//            stop();
//        }
//        receive_byte(data);
//        data++;
//        len--;
//        i++;
//    }
//    send_ack();

#if	(TIMEOUT != 0)
  uint64_t end = GetEndTime(timeout_us);
#endif
  // 发送地址寄存器
  LL_I2C_HandleTransfer(I2Cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,1,LL_I2C_MODE_SOFTEND,LL_I2C_GENERATE_START_WRITE);
  while (!LL_I2C_IsActiveFlag_TC(I2Cx))
  {
    if (LL_I2C_IsActiveFlag_TXIS(I2Cx))
    {
      LL_I2C_TransmitData8(I2Cx,reg_address);
    }
//		if (IsTimeOut(end,timeout_us))
//		{
//			return E_TIMEOUT;
//		}
  }
  // 发送读指令，从当前地址开始读取数据
  LL_I2C_HandleTransfer(I2Cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,len,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_READ);
#if	(TIMEOUT != 0)
  end = GetEndTime(timeout_us);
#endif
  while (!LL_I2C_IsActiveFlag_STOP(I2Cx))
  {
    if (LL_I2C_IsActiveFlag_RXNE(I2Cx))
    {
      *data++ = LL_I2C_ReceiveData8(I2Cx);
    }
#if	(TIMEOUT != 0)
    if (IsTimeOut(end,timeout_us)) return E_TIMEOUT;
#endif
  }
  LL_I2C_ClearFlag_STOP(I2Cx);
//	return E_OK;

  return i;
}

int8_t mcuI2c::wait_dev_busy(uint8_t slave_address){
    return wait_dev_busy1(slave_address,300);
}

#define GetEndTime(timeOut)					(millis_seconds + timeOut)
// 超时,返回1 否则返回0   这里采用millis()获取millis_seconds,可以防止因为关闭中断导致程序死在延时函数里
#define IsTimeOut(endTime,delay)		((uint32_t)(endTime - millis())>delay)

int8_t mcuI2c::wait_dev_busy1(uint8_t slave_address,uint16_t timeout_us)
{
	uint64_t end = GetEndTime(timeout_us);
	do
	{
		// clear STOP & NACK flag
		SET_BIT(I2Cx->ICR,LL_I2C_ICR_NACKCF | LL_I2C_ICR_STOPCF);

		LL_I2C_HandleTransfer(I2Cx,slave_address,LL_I2C_ADDRESSING_MODE_7BIT,0,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);
		delay_us(100);
		if (IsTimeOut(end,timeout_us))
		{
			return 1;
		}
	}while (LL_I2C_IsActiveFlag_NACK(I2Cx) == 1); //如果无响应，则继续等待

	LL_I2C_ClearFlag_STOP(I2Cx);

	return 0;
}
int8_t mcuI2c::take_i2c_right(uint32_t speed)
{
  while (busy == 1)
  {
    delay_ms(1);
  }
  if (this->speed != speed)
    config(this->speed);
  busy = 1;
  return 0;
}
int8_t mcuI2c::release_i2c_right(void)
{
  busy = 0;
  return 0;
}
