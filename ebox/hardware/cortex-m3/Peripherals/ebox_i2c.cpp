/**
  ******************************************************************************
  * @file    ebox_i2c.cpp
  * @author  cat_li
  * @brief   ����������ģʽ
		1  2017/5/30  ���ӳ�ʱ����ֹ������������д�������ӷ���״̬
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
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 1
#endif

#if debug
#define  I2C_DEBUG(...) DBG("[I2C]  "),DBG(__VA_ARGS__)
#else
#define  I2C_DEBUG(...)
#endif

#define GetEndTime(timeOut)					(millis_seconds + timeOut)
// ��ʱ,����1 ���򷵻�0   �������millis()��ȡmillis_seconds,���Է�ֹ��Ϊ�ر��жϵ��³���������ʱ������
#define IsTimeOut(endTime,delay)		((uint32_t)(endTime - millis())>delay)

/**
 *@name     I2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin)
 *@brief      I2C���캯��
 *@param    I2Cx:  I2C1,I2C2
 *          scl_pin:  ʱ��Pin
 *          sda_pin:  ����Pin
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
 *@name     begin(uint16_t speed)
 *@brief    ����i2cʱ�Ӻ���������speed����timing��Ĭ��400k @8M
 *@param    speed:  ���� 10,100,400 �ֱ����10k��100k��400k
 *@retval   None
*/
void  mcuI2c::begin(uint16_t speed)
{
  rcc_clock_cmd((uint32_t)_i2cx,ENABLE);

  _sda->mode(AF_OD);
  _scl->mode(AF_OD);
  switch (speed)
  {
  case 10:
    config(10000);
    break;
  case 100:
    config(100000);
    break;
  case 400:
    config(400000);
    break;
  default:
    config(200000);
  }
}

void mcuI2c::config(uint32_t speed)
{
  _timing = speed;
  I2C_InitTypeDef  I2C_InitStructure;

  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//  I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = _timing;

  /* I2C Peripheral Enable */
  I2C_Cmd(_i2cx, ENABLE);
  /* Apply I2C configuration after enabling it */
  I2C_Init(_i2cx, &I2C_InitStructure);
}

uint32_t mcuI2c::readConfig()
{
  return _timing;
}

/**
  *@brief    I2Cд��һ���ֽ�. start->data->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t data:  Ҫд�������
  *          uint16_t tOut: ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
  */
uint8_t mcuI2c::write(uint8_t slaveAddr, uint8_t data,uint16_t tOut)
{
  uint8_t err = EOK;
  I2C_DEBUG("I2C state sr2 = %d, sr1 = %d \r\n",_i2cx->SR2,_i2cx->SR1);
  err += _start(tOut);
  err +=_send7bitsAddress(slaveAddr,WRITE,tOut);
  err +=_sendByte(data,tOut);
  _stop();
  return EOK;
}

/**
  *@brief    I2C����д start->data....->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t *data:  Ҫд�������
  *          uint16_t nWrite  Ҫд������ݳ���
  *          uint16_t tOut:  ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
  */
uint8_t mcuI2c::writeBuf(uint8_t slaveAddr, uint8_t *data, uint16_t nWrite,uint16_t tOut)
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
  *@brief    ��ָ���Ĵ�������д start->regAddr->data....->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t regAddr��Ҫд��ļĴ�����ַ
  *          uint8_t *data:  Ҫд�������
  *          uint16_t nWrite  Ҫд������ݳ���
  *          uint16_t tOut:  ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
  */
uint8_t mcuI2c::writeBuf(uint8_t slaveAddr,uint8_t regAddr,uint8_t *data, uint16_t nWrite,uint16_t tOut)
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
  *@brief    I2C����һ���ֽ�. start->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint16_t tOut: ��ʱ
  *@retval   ��ȡ��������
  */
uint8_t mcuI2c::read(uint8_t slaveAddr,uint16_t tOut){
  uint8_t data ;
  _start(tOut);
  _send7bitsAddress(slaveAddr,READ,tOut);
  _sendNack();
  _stop();
  _receiveByte(&data,tOut);
  _sendAck();
  return data;
}

/**
  *@brief    ��ָ���Ĵ���. start->WslaveAddr->regAddr->RslaveAddr->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t regAddr��   Ҫ��ȡ�ļĴ���
  *          uint16_t tOut: ��ʱ
  *@retval   ��ȡ��������
  */
uint8_t mcuI2c::read(uint8_t slaveAddr,uint8_t regAddr,uint16_t tOut)
{
  uint8_t data ;
  _start(tOut);
  _send7bitsAddress(slaveAddr,WRITE,tOut);
  _sendByte(regAddr,tOut);
  _start(tOut);
  _send7bitsAddress(slaveAddr,READ,tOut);
  _sendNack();
  _stop();
  _receiveByte(&data,tOut);
  _sendAck();
  return data;
}

/**
  *@brief    ������ȡ. start->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t *data: ��ȡ��������
  *          uint16_t nRead��Ҫ��ȡ�����ݳ���
  *          uint16_t tOut: ��ʱ
  *@retval   EOK��EWAIT
  */
uint8_t mcuI2c::readBuf(uint8_t slaveAddr,uint8_t *data,uint16_t nRead,uint16_t tOut)
{
  uint8_t err = 0;
  err += _start(tOut);
  err +=_send7bitsAddress(slaveAddr,READ,tOut);
  while (nRead--)
  {
    if (nRead == 1)
    {
      _sendNack();
      _stop();
    }
    err +=_receiveByte(data,tOut);
    data++;
  }
  _sendAck();
  return err;
}

/**
  *@brief    ָ���Ĵ���������ȡ. start->WslaveAddr->regAddr->RSlaverAddr->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t regAddr: �Ĵ�����ַ
  *          uint8_t *data: ��ȡ��������
  *          uint16_t nRead��Ҫ��ȡ�����ݳ���
  *          uint16_t tOut: ��ʱ
  *@retval   EOK��EWAIT
  */
uint8_t mcuI2c::readBuf(uint8_t slaveAddr,uint8_t regAddr,uint8_t *data, uint16_t nRead,uint16_t tOut)
{
  uint8_t err = 0;
  err += _start(tOut);
  err += _send7bitsAddress(slaveAddr,WRITE,tOut);
  err += _sendByte(regAddr,tOut);
  err += _start(tOut);
  err +=_send7bitsAddress(slaveAddr,READ,tOut);
  while (nRead--)
  {
    if (nRead == 1)
    {
      _sendNack();
      _stop();
    }
    err +=_receiveByte(data,tOut);
    data++;
  }
  _sendAck();
  return err;
}

/**
  *@brief    �ȴ��豸��Ӧ����ָ���豸����startָ�����豸æ���򷵻�NACK,���򷵻�ACK,���豸����stopָ��
  *@param    slaveAddr:  �豸��ַ
  *@retval   uint8_t: EOK,EWAIT
  */
uint8_t mcuI2c:: waitAck(uint8_t slaveAddr,uint16_t tOut)
{
  uint32_t end = GetEndTime(tOut);
  __IO uint16_t SR1_Tmp = 0;

  do
  {
    I2C_ClearFlag(_i2cx, I2C_FLAG_AF);
//        I2C_ClearFlag(_i2cx, I2C_FLAG_AF |I2C_FLAG_STOPF);
    /* Send START condition */
    I2C_GenerateSTART(_i2cx, ENABLE);

    /* Read I2C_EE SR1 register to clear pending flags */
    SR1_Tmp = I2C_ReadRegister(_i2cx, I2C_Register_SR1);

    I2C_Send7bitAddress(_i2cx, slaveAddr, I2C_Direction_Transmitter);
    if (IsTimeOut(end,tOut))
    {
      I2C_ClearFlag(_i2cx, I2C_FLAG_AF|I2C_FLAG_ADDR|I2C_FLAG_SB);
      I2C_SendData(_i2cx, slaveAddr);
      I2C_GenerateSTOP(_i2cx, ENABLE);
//            I2C_DEBUG("I2C state sr2 = %d, sr1 = %d \r\n",_i2cx->SR2,_i2cx->SR1);
      return 1;
    }

  }while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

//  /* Clear AF flag */
  I2C_ClearFlag(_i2cx, I2C_FLAG_AF|I2C_FLAG_ADDR|I2C_FLAG_SB);
  /* STOP condition */
  I2C_GenerateSTOP(_i2cx, ENABLE);
//  I2C_ClearFlag(_i2cx, I2C_FLAG_AF|I2C_FLAG_ADDR|I2C_FLAG_SB);
  return EOK;
}

/**
  *@brief    ��ȡI2C����Ȩ
  *@param    timing:  ʱ��ʱ��ͨ��readConfig��ȡ
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
 *@brief    �ͷ�I2C����Ȩ
 *@param    none
 *@retval   none
*/
void mcuI2c::releaseRight(void)
{
  _busy = 0;
}

int8_t mcuI2c::_start(uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  /* Send STRAT condition */
  I2C_GenerateSTART(_i2cx, ENABLE);
  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_MODE_SELECT))
  {
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,tOut)){
      I2C_DEBUG("start fail, state reg SR2 = %d��SR1 = %d \r\n",_i2cx->SR2,_i2cx->SR1);
      return EWAIT;
    }
#endif
  }
  return EOK;
}

void mcuI2c::_stop()
{
  I2C_GenerateSTOP(_i2cx, ENABLE);
}
int8_t mcuI2c::_sendNack()
{
  int8_t err = 0;
  I2C_AcknowledgeConfig(_i2cx, DISABLE);
  return err;
}
int8_t mcuI2c::_sendAck()
{
  int8_t err = 0;
  I2C_AcknowledgeConfig(_i2cx, ENABLE);
  return err;
}


int8_t mcuI2c::_sendByte(uint8_t data,uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  /* Send the byte to be written */
  I2C_SendData(_i2cx, data);
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,tOut)){
      I2C_DEBUG("send data fail, state reg SR2 = %d��SR1 = %d \r\n",_i2cx->SR2,_i2cx->SR1);
      return EWAIT;
    }
#endif
  }
}
int8_t mcuI2c::_send7bitsAddress(uint8_t slaveAddr,uint8_t WR,uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  if (WR) /* Send address for read */
  {
//    I2C_DEBUG("send read address is %d \r\n",slaveAddr);
    I2C_Send7bitAddress(_i2cx, slaveAddr, I2C_Direction_Receiver);
    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
#if	(USE_TIMEOUT != 0)
      if (IsTimeOut(end,tOut)){
        I2C_DEBUG("send read address fail, state reg SR2 = %d��SR1 = %d \r\n",_i2cx->SR2,_i2cx->SR1);
        return EWAIT;
      }
#endif
    }
  }
  else   /* Send address for write */
  {
//    I2C_DEBUG("send write address is %d \r\n",slaveAddr);
    I2C_Send7bitAddress(_i2cx, slaveAddr, I2C_Direction_Transmitter);
    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
#if	(USE_TIMEOUT != 0)
      if (IsTimeOut(end,tOut)){
        I2C_DEBUG("send write address fail, state reg SR2 = %d��SR1 = %d \r\n",_i2cx->SR2,_i2cx->SR1);
        return EWAIT;
      }
#endif
    }
  }
  return EOK;
}
int8_t mcuI2c::_receiveByte(uint8_t *data,uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,tOut)){
      I2C_DEBUG("read data fail, state reg SR2 = %d��SR1 = %d \r\n",_i2cx->SR2,_i2cx->SR1);
      return EWAIT;
    }
#endif
  }
  *data = I2C_ReceiveData(_i2cx);//�����Ĵ�������
  return EOK;
}
