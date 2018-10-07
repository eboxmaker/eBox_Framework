/**
  ******************************************************************************
  * @file    ebox_i2c.cpp
  * @author  cat_li
  * @brief   仅工作在主模式
		1  2017/5/30  增加超时，防止程序死掉。读写函数增加返回状态
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
#include "ebox_core.h"
#include "ebox_gpio.h"

#include "ebox_config.h"

#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define debug 1
#endif

#if debug
#define  I2C_DEBUG(...) DBG("[I2C]  "),DBG(__VA_ARGS__)
#else
#define  I2C_DEBUG(...)
#endif

/**
 *@name     I2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin)
 *@brief      I2C构造函数
 *@param    I2Cx:  I2C1,I2C2
 *          scl_pin:  时钟Pin
 *          sda_pin:  数据Pin
 *@retval   None
*/
mcuI2c::mcuI2c(I2C_TypeDef *I2Cx,Gpio *scl_pin, Gpio *sda_pin)
{
  _busy = 0;
  _i2cx = I2Cx;
  _scl = scl_pin;
  _sda = sda_pin;
}

/**
  *@brief    根据i2c时钟和设置速率speed计算timing。默认400k @8M
  *@param    speed:  速率 10,100,400 分别代表10k，100k，400k
  *@retval   None
  */
void  mcuI2c::begin(uint16_t speed)
{
  switch (cpu.clock.pclk1/1000000){
  case 16:
    switch (speed){
    case 10:
      _timing = C16M10K;	// 10k 	@16M
      break;
    case 100:
      _timing = C16M100K;		// 100k @16M
      break;
    case 400:
    default:
      _timing = C16M400K;			// 400k @16M
    }
    break;
  case 48:
    switch (speed){
    case 10:
      _timing = C48M10K;	// 10k 	@48M
      break;
    case 100:
      _timing = C48M100K;		// 100k @48M
      break;
    case 400:
    default:
      _timing = C48M400K;			// 400k @48M
    }
    break;
  case 8:
  default:	// 默认系统时钟为8M
    switch (speed){
    case 10:
      _timing = C8M10K;	// 10k 	@8M
      break;
    case 100:
      _timing = C8M100K;		// 100k @8M
      break;
    case 400:
    default:
      _timing = C8M400K;			// 400k @8M
    }
  }
  _scl->mode(AF_PP_PU,LL_GPIO_AF_1);
  _sda->mode(AF_PP_PU,LL_GPIO_AF_1);
  config(_timing);
}

void mcuI2c::config(uint32_t speed)
{
  _timing = speed;
  rcc_clock_cmd((uint32_t)_i2cx,ENABLE);
  // I2C1 需要选择特定的时钟
  (_i2cx == I2C1)?(LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_SYSCLK)):(void());

  LL_I2C_Disable(_i2cx);
  LL_I2C_SetTiming(_i2cx,_timing);
  LL_I2C_Enable(_i2cx);
}

uint32_t mcuI2c::readConfig()
{
  return _timing;
}

/**
  *@brief    I2C写入一个字节. start->data->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t data:  要写入的数据
  *          uint16_t tOut: 超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t mcuI2c::write(uint8_t slaveAddr, uint8_t data,uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,1,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);

  while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
  {
    if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
    {
      LL_I2C_TransmitData8(_i2cx,data);
    }
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,tOut)) return EWAIT;
#endif
  }
  LL_I2C_ClearFlag_STOP(_i2cx);

  return EOK;
}

/**
  *@brief    I2C连续写 start->data....->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t *data:  要写入的数据
  *          uint16_t nWrite  要写入的数据长度
  *          uint16_t tOut:  超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t mcuI2c::writeBuf(uint8_t slaveAddr, uint8_t *data, uint16_t nWrite,uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  if (nWrite >255){
    // num_to_writ>255: RELOAD,NBYTE=0xFF,START
    LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,0xFF,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_START_WRITE);

    while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
    {
      if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
      {
        LL_I2C_TransmitData8(_i2cx,*data++);
      }
#if	(USE_TIMEOUT != 0)
      if (IsTimeOut(end,tOut))	return EWAIT;
#endif
    }
    nWrite = nWrite - 255;
    // num_to_writ>255: RELOAD,NBYTE=0xFF,NOSTART

    while (nWrite > 255){
      // 发送地址寄存器
      LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,0xFF,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_NOSTARTSTOP);
#if	(USE_TIMEOUT != 0)
      end = GetEndTime(tOut);
#endif
      while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
      {
        if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
        {
          LL_I2C_TransmitData8(_i2cx,*data++);
        }
#if	(USE_TIMEOUT != 0)
        if (IsTimeOut(end,tOut))	return EWAIT;
#endif
      }
      nWrite = nWrite - 255;
    }
    // num_to_writ<=255: AUTOEND,NBYTE=num_to_writ,NOSTART
    LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,nWrite,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_NOSTARTSTOP);
#if	(USE_TIMEOUT != 0)
    end = GetEndTime(tOut);
#endif
    while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
    {
      if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
      {
        LL_I2C_TransmitData8(_i2cx,*data++);
      }
#if	(USE_TIMEOUT != 0)
      if (IsTimeOut(end,tOut))	return EWAIT;
#endif
    }
    LL_I2C_ClearFlag_STOP(_i2cx);
  }else{
    // 写入地址寄存器和数据
    LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,nWrite,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);
#if	(USE_TIMEOUT != 0)
    end = GetEndTime(tOut);
#endif
    while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
    {
      if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
      {
        LL_I2C_TransmitData8(_i2cx,*data++);
      }
#if	(USE_TIMEOUT != 0)
      if (IsTimeOut(end,tOut))	return EWAIT;
#endif
    }
    LL_I2C_ClearFlag_STOP(_i2cx);
  }

  return EOK;
}

/**
  *@brief    在指定寄存器连续写 start->regAddr->data....->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t regAddr：要写入的寄存器地址
  *          uint8_t *data:  要写入的数据
  *          uint16_t nWrite  要写入的数据长度
  *          uint16_t tOut:  超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t mcuI2c::writeBuf(uint8_t slaveAddr,uint8_t regAddr,uint8_t *data, uint16_t nWrite,uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  // 发送地址寄存器
  LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,1,LL_I2C_MODE_SOFTEND,LL_I2C_GENERATE_START_WRITE);

  while (!LL_I2C_IsActiveFlag_TC(_i2cx))
  {
    if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
    {
      LL_I2C_TransmitData8(_i2cx,regAddr);
    }
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,tOut)) return EWAIT;
#endif
  }
  // 发送数据
  if (nWrite >255){
    // num_to_writ>255: RELOAD,NBYTE=0xFF,NOSTART
    while (nWrite > 255){
      // 发送地址寄存器
      LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,0xFF,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_NOSTARTSTOP);
#if	(USE_TIMEOUT != 0)
      end = GetEndTime(tOut);
#endif
      while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
      {
        if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
        {
          LL_I2C_TransmitData8(_i2cx,*data++);
        }
#if	(USE_TIMEOUT != 0)
        if (IsTimeOut(end,tOut)) return EWAIT;
#endif
      }
      nWrite = nWrite - 255;
    }
    // num_to_writ<=255: AUTOEND,NBYTE=num_to_writ,NOSTART
    LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,nWrite,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_NOSTARTSTOP);
#if	(USE_TIMEOUT != 0)
    end = GetEndTime(tOut);
#endif
    while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
    {
      if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
      {
        LL_I2C_TransmitData8(_i2cx,*data++);
      }
#if	(USE_TIMEOUT != 0)
      if (IsTimeOut(end,tOut)) return EWAIT;
#endif
    }
    LL_I2C_ClearFlag_STOP(_i2cx);
  }else{
    // 写入地址寄存器和数据
    LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,nWrite,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_NOSTARTSTOP);
#if	(USE_TIMEOUT != 0)
    end = GetEndTime(tOut);
#endif
    while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
    {
      if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
      {
        LL_I2C_TransmitData8(_i2cx,*data++);
      }
#if	(USE_TIMEOUT != 0)
      if (IsTimeOut(end,tOut)) return EWAIT;
#endif
    }
    LL_I2C_ClearFlag_STOP(_i2cx);
  }
  return EOK;
}


/**
  *@brief    I2C读入一个字节. start->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint16_t tOut: 超时
  *@retval   读取到的数据
  */
uint8_t mcuI2c::read(uint8_t slaveAddr,uint16_t tOut){
  uint8_t ret = 0;
  readBuf(slaveAddr,&ret,1,tOut);
  return ret;
}

/**
  *@brief    读指定寄存器. start->WslaveAddr->regAddr->RslaveAddr->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t regAddr：   要读取的寄存器
  *          uint16_t tOut: 超时
  *@retval   读取到的数据
  */
uint8_t mcuI2c::read(uint8_t slaveAddr,uint8_t regAddr,uint16_t tOut){
  uint8_t ret = 0;
  readBuf(slaveAddr,regAddr,&ret,1,tOut);
  return ret;  
}

/**
  *@brief    连续读取. start->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t *data: 读取到的数据
  *          uint16_t nRead：要读取的数据长度
  *          uint16_t tOut: 超时
  *@retval   EOK，EWAIT
  */
uint8_t mcuI2c::readBuf(uint8_t slaveAddr,uint8_t *data,uint16_t nRead,uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  // 发送读指令，从当前地址开始读取数据
  LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,nRead,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_READ);
  while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
  {
    if (LL_I2C_IsActiveFlag_RXNE(_i2cx))
    {
//      *data++ = LL_I2C_ReceiveData8(_i2cx);
      I2C_DEBUG("n %d read is %d \r\n",nRead,LL_I2C_ReceiveData8(_i2cx));
    }
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,tOut)) return EWAIT;
#endif
  }
  LL_I2C_ClearFlag_STOP(_i2cx);
  return EOK;
}

/**
  *@brief    指定寄存器连续读取. start->WslaveAddr->regAddr->RSlaverAddr->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t regAddr: 寄存器地址
  *          uint8_t *data: 读取到的数据
  *          uint16_t nRead：要读取的数据长度
  *          uint16_t tOut: 超时
  *@retval   EOK，EWAIT
  */
uint8_t mcuI2c::readBuf(uint8_t slaveAddr,uint8_t regAddr,uint8_t *data, uint16_t nRead,uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  // 发送地址寄存器
  LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,1,LL_I2C_MODE_SOFTEND,LL_I2C_GENERATE_START_WRITE);
  while (!LL_I2C_IsActiveFlag_TC(_i2cx))
  {
    if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
    {
      LL_I2C_TransmitData8(_i2cx,regAddr);
    }
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,tOut))
    {
      return EWAIT;
    }
#endif
  }
  // 发送读指令，从当前地址开始读取数据
  LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,nRead,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_READ);
#if	(USE_TIMEOUT != 0)
  end = GetEndTime(tOut);
#endif
  while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
  {
    if (LL_I2C_IsActiveFlag_RXNE(_i2cx))
    {
      *data++ = LL_I2C_ReceiveData8(_i2cx);
    }
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,tOut)) return EWAIT;
#endif
  }
  LL_I2C_ClearFlag_STOP(_i2cx);
  return EOK;
}







/**
  *@brief    等待设备响应。向指定设备发送start指令，如果设备忙，则返回NACK,否则返回ACK,主设备发送stop指令
  *@param    slaveAddr:  设备地址
  *@retval   uint8_t: EOK,EWAIT
  */
uint8_t mcuI2c:: waitAck(uint8_t slaveAddr,uint16_t tOut)
{
  uint32_t end = GetEndTime(tOut);
  do
  {
    // clear STOP & NACK flag
    SET_BIT(_i2cx->ICR,LL_I2C_ICR_NACKCF | LL_I2C_ICR_STOPCF);

    LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,0,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);
    delay_us(100);
    if (IsTimeOut(end,tOut))
    {
      I2C_DEBUG("fail,ISR reg is %d \r\n",_i2cx->ISR);  
      return EWAIT;
    }
  }while (LL_I2C_IsActiveFlag_NACK(_i2cx) == 1); //如果无响应，则继续等待

  LL_I2C_ClearFlag_STOP(_i2cx);

  return EOK;
}

/**
  *@brief    获取I2C控制权
  *@param    timing:  时钟时序，通过readConfig获取
  *@retval   uint8_t: EOK,E_BUSY
  */
uint8_t mcuI2c::takeRight(uint32_t timing,uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  while (_busy == 1)
  {
    delay_ms(1);
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,tOut)) return EWAIT;
#endif
  }
  if (_timing != timing) config(timing);
    _busy = 1;
  return EOK;
}
/**
 *@brief    释放I2C控制权
 *@param    none
 *@retval   none
*/
void mcuI2c::releaseRight(void)
{
  _busy = 0;
}
