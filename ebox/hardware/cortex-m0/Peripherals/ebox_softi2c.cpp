/**
  ******************************************************************************
  * @file    softi2c.cpp
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

#include "ebox_config.h"

#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define debug 1
#endif

#if debug
#define  I2C_DEBUG(...) DBG("[SOFTI2C]  "),DBG(__VA_ARGS__)
#else
#define  I2C_DEBUG(...)
#endif


/**
  *@brief    softI2C构造函数
  *@param    scl_pin:  时钟Pin
  *          sda_pin:  数据Pin
  *@retval   None
  */
SoftI2c::SoftI2c(Gpio *scl, Gpio *sda)
{
    _scl = scl;
    _sda = sda;
    _busy = 0;
}

/**
  *@brief    根据i2c时钟和设置速率speed计算timing,默认80K
  *@param    speed:  速率 10,100,200,300,400 分别代表10k，100k，200k,300k,400k
  *@retval   None
  */
void SoftI2c::begin(uint16_t speed)
{
    _sda->mode(OUTPUT_PP);
    _scl->mode(OUTPUT_PP);
    
    switch(speed)
    {
    case 400:
    case 300:
    case 200:
        _timing = 1;    // 约200k
        break;
    case 100:
        _timing = 3;    // 约100k
        break;
    default:
        _timing = 4;    // 约80k
        break;
    }
}

/**
 * @brief 设置i2c通信速度.
 *
 * @param[in] speed i2c速度，当其值为400K，300K，200K，100K时，i2c通信频率为该值.
 *
 * @return 0.
 */
void SoftI2c::config(uint32_t speed)
{
    _timing = speed;;
}

/**
 * @brief 读取i2c频率.
 *
 * @param 无.
 *
 * @return i2c频率.
 */
uint32_t SoftI2c::readConfig()
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
uint8_t SoftI2c::write(uint8_t slaveAddr, uint8_t data,uint16_t tOut)
{
  uint8_t err = EOK;
  I2C_DEBUG("I2C Bus state,SCL is %d, SDA is %d \r\n",_scl->read(),_sda->read());
  err += _start(tOut);
  err +=_send7bitsAddress(slaveAddr,WRITE,tOut);
  err +=_sendByte(data,tOut);
  _stop();
  return err;
}

/**
  *@brief    I2C连续写 start->data....->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t *data:  要写入的数据
  *          uint16_t nWrite  要写入的数据长度
  *          uint16_t tOut:  超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t SoftI2c::writeBuf(uint8_t slaveAddr, uint8_t *data, uint16_t nWrite,uint16_t tOut)
{
  uint8_t err = 0;
  err += _start(tOut);
  err +=_send7bitsAddress(slaveAddr,WRITE,tOut);
  while (nWrite--)
  {
    err +=_sendByte(*data,tOut);
    data++;
  }
  _stop();
  return err;
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
uint8_t SoftI2c::writeBuf(uint8_t slaveAddr,uint8_t regAddr,uint8_t *data, uint16_t nWrite,uint16_t tOut)
{
  uint8_t err = 0;
  err += _start(tOut);
  err +=_send7bitsAddress(slaveAddr,WRITE,tOut);
  err+= _sendByte(regAddr,tOut);
  while (nWrite--)
  {
    err +=_sendByte(*data,tOut);
    data++;
  }
  _stop();
  return err;
}


/**
  *@brief    I2C读入一个字节. start->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint16_t tOut: 超时
  *@retval   读取到的数据
  */
uint8_t SoftI2c::read(uint8_t slaveAddr,uint16_t tOut){
  uint8_t data ;
  _start(tOut);
  _send7bitsAddress(slaveAddr,READ,tOut);
  _receiveByte(&data,tOut);
  _sendNack();
  _stop();
  return data;
}

/**
  *@brief    读指定寄存器. start->WslaveAddr->regAddr->RslaveAddr->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t regAddr：   要读取的寄存器
  *          uint16_t tOut: 超时
  *@retval   读取到的数据
  */
uint8_t SoftI2c::read(uint8_t slaveAddr,uint8_t regAddr,uint16_t tOut){
  uint8_t data ;
  _start(tOut);
  _send7bitsAddress(slaveAddr,WRITE,tOut);
  _sendByte(regAddr,tOut);
  _start(tOut);
  _send7bitsAddress(slaveAddr,READ,tOut);
  _receiveByte(&data,tOut);
  _sendNack();
  _stop();
  return data; 
}

/**
  *@brief    连续读取. start->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t *data: 读取到的数据
  *          uint16_t nRead：要读取的数据长度
  *          uint16_t tOut: 超时
  *@retval   EOK，EWAIT
  */
uint8_t SoftI2c::readBuf(uint8_t slaveAddr,uint8_t *data,uint16_t nRead,uint16_t tOut)
{
  uint8_t err = 0;
  err += _start(tOut);
  err +=_send7bitsAddress(slaveAddr,READ,tOut);
  while (nRead--)
  {
    err +=_receiveByte(data,tOut);
    data++;
    if (nRead == 0)
    {
      _sendNack();
      _stop();
      break;
    }
    _sendAck();
  }
  return err;
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
uint8_t SoftI2c::readBuf(uint8_t slaveAddr,uint8_t regAddr,uint8_t *data, uint16_t nRead,uint16_t tOut)
{
  uint8_t err = 0;
  err += _start(tOut);
  err += _send7bitsAddress(slaveAddr,WRITE,tOut);
  err += _sendByte(regAddr,tOut);
  err += _start(tOut);
  err +=_send7bitsAddress(slaveAddr,READ,tOut);
  while (nRead--)
  {
    err +=_receiveByte(data,tOut);
    data++;
    if (nRead == 0)
    {
      _sendNack();
      _stop();
      break;
    }
    _sendAck();
  }
  return err;
}

/**
 * @brief 等待从机空闲.
 *
 * @param[in] slave_address 7bit从机地址.
 *
 * @return 从机状态.返回0表示从机空闲，返回-1表示从机忙.
 */
uint8_t SoftI2c::waitAck(uint8_t slaveAddr,uint16_t tOut)
{
    int8_t ret;
    uint8_t i = 0;
    do
    {
        _start(tOut);
        ret = _send7bitsAddress(slaveAddr,WRITE,tOut);
        _sendAck();
        _sendByte(slaveAddr,tOut);
        _stop();
        if(i++ == 100)
        {
            return EBUSY;
        }
    }
    while(ret != 0); //如果返回值不是0，继续等待
    return EOK;
}

/**
  *@brief    获取I2C控制权
  *@param    timing:  时钟时序，通过readConfig获取
  *@retval   uint8_t: EOK,E_BUSY
  */
uint8_t SoftI2c::takeRight(uint32_t timing,uint16_t tOut)
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
void SoftI2c::releaseRight(void)
{
  _busy = 0;
}

/**
  * @brief 发送一个START信号.SCL为高电平时，SDA由高电平向低电平跳变，开始传送数据
  * @param 无.
  * @return 无.
  */
int8_t SoftI2c::_start(uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  // scl,sda 均为高电平，总线空闲
  while (!(_sda->read() && _scl->read())){
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,tOut)){
      I2C_DEBUG("start fail,bus busy SDA is %d，SCL is %d,time is %d us \r\n",_sda->read(),_scl->read(),_timing);
      return EWAIT;
    }
#endif
  }
  _sda->mode(OUTPUT_PP);
  _sda->set();
  _scl->set();        // SCL高
  delay_us(_timing);
  _sda->reset();      // SDA拉低
  delay_us(_timing);
  _scl->reset();
  return EOK;
}

/**
  * @brief 发送一个STOP信号. SCL为高电平时，SDA由低电平向高电平跳变，结束传送数据
  * @param 无.
  * @return 无.
  */
void SoftI2c::_stop()
{
    _sda->mode(OUTPUT_PP);
    _scl->reset();
    _sda->reset();      // SDA低电平
    delay_us(_timing);
    _scl->set();        // SCL拉高并保持
    delay_us(_timing);  
    _sda->set();        // SDA拉高
}

/**
 * @brief 等待一个ACK应答. 第9个clock，若从IC发ACK，SDA会被拉低
 * @param 无.
 * @return EOK,EWAIT.
 */
int8_t SoftI2c::_waitAck()
{
    uint8_t cErrTime = 10;
    _sda->mode(INPUT_PU);
    _scl->set();
    delay_us(_timing);
    while(_sda->read())       // 等待SDA被拉低，即ACK响应
    {
        cErrTime--;
        delay_us(_timing);
        if(cErrTime == 0)     // 无响应，发送stop信号
        {
            _sda->mode(OUTPUT_PP);
            _stop();
            //I2C_DEBUG(" slave device no ack \r\n");
            return EWAIT;
        }
    }
    _scl->reset();
    delay_us(_timing);
    return EOK;
}

/**
 * @brief 发送一个ACK应答. 第9个clock期间，拉低SDA
 * @param 无.
 * @return 0.
 */
int8_t SoftI2c::_sendAck()
{
    _sda->mode(OUTPUT_PP);
    _sda->reset();
    delay_us(_timing);
    _scl->set();
    delay_us(_timing);
    _scl->reset();
    delay_us(_timing);
    return EOK;
}

/**
  * @brief 发送一个NACK应答,第9个clock期间，拉高SDA
  * @param 无
  * @return 0.
  */
int8_t SoftI2c::_sendNack()
{
    _sda->mode(OUTPUT_PP);
    _sda->set();
    delay_us(_timing);
    _scl->set();
    delay_us(_timing);
    _scl->reset();
    delay_us(_timing);
    return EOK;
}

/**
  * @brief 发送数据.
  * @param[in] byte 将被发送的数据.
  * @return 发送结果.EOK,EWAIT.
  */
int8_t SoftI2c::_sendByte(uint8_t byte,uint16_t tOut)
{
    int8_t ret = EOK;

    uint8_t ii = 8;
    _sda->mode(OUTPUT_PP);
    _scl->reset();
    while( ii-- )
    {
        _sda->write(byte & 0x80);   // SCL低电平时将数据送到SDA
        delay_us(_timing);
        _scl->set();                // 产生一个时钟脉冲
        delay_us(_timing);
        byte = byte<<1;
        _scl->reset();
    }
    ret = _waitAck();
    return ret;
}

/**
 * @brief 发送7bit从机地址位.
 *
 * @param[in] slave_address 7bit从机地址位.
 *
 * @return 发送结果.返回0表示发送成功，返回-1表示发送失败.
 */
int8_t	SoftI2c::_send7bitsAddress(uint8_t slaveAddr,uint8_t WR,uint16_t tOut)
{
    int8_t ret = 0;
    // 写，从地址最低位置0；读，从地址最低位置1；
    slaveAddr = (WR == WRITE) ? (slaveAddr & 0xfe):(slaveAddr | 0x01);
    ret = _sendByte(slaveAddr,tOut);
    return ret;
}

/**
  * @brief 接收数据.
  * @param 无.  
  * @return 接收到的数据.
  */
int8_t SoftI2c::_receiveByte(uint8_t *data,uint16_t tOut)
{
    uint8_t i = 8;
    uint8_t byte = 0;
    _sda->mode(INPUT_PU);
    while(i--)
    {
        byte += byte;
        _scl->reset();      // 时钟高电平期间读SDA
        delay_us(_timing);
        _scl->set();
        delay_us(_timing);
        byte |= _sda->read();
    }
    _scl->reset();
    delay_us(_timing);
    *data = byte;
    return EOK;
}
