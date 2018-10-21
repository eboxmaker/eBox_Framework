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
#include "stm32f072_define.h"
#include "ebox_config.h"

#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 0
#endif

#if debug
#define  I2C_DEBUG(...) DBG("[I2C]  "),DBG(__VA_ARGS__)
#else
#define  I2C_DEBUG(...)
#endif

// ��Ƶ�����������֣��ߵ�ƽ���͵�ƽ
#define	C16M10K	 	__LL_I2C_CONVERT_TIMINGS(0x3, 0xc7, 0xc3, 0x02, 0x04)		//10k@16M
#define	C16M100K	__LL_I2C_CONVERT_TIMINGS(0x3, 0x13, 0xf, 0x02, 0x04)		//100k@16M
#define	C16M400K	__LL_I2C_CONVERT_TIMINGS(0x1, 0x9, 0x3, 0x2, 0x3)			//400k@16M

#define	C48M10K	 	0xA010B2FF		//10k@48M,stand mode
#define	C48M100K	0x10805E89		//100k@48M,stand mode
#define C48M200K    0x00905E82      //200K@48M,Fast mode
#define	C48M400K	0x00901850   	//400k@48M,Fast mode


#define	C8M10K	 	0x101098F3		//10k@8M,stand mode
#define	C8M100K	 	0x00201D2B		//100k@8M,stand mode
#define C8M200K     0x0010021E      //200K@8M,Fast mode     �����ʸߣ�������ʹ��
#define	C8M400K	 	0x0010020A		//400k@8M,Fast mode     �����ʸߣ�������ʹ��

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
  *@brief    ����i2cʱ�Ӻ���������speed����timing��8M��48M��ƵĬ��100k������Ƶ��Ĭ��10k @8M
  *@param    speed:  ���� 10,100��200,400 �ֱ����10k��100k��200k��400k
  *@retval   None
  */
void  mcuI2c::begin(uint16_t speed)
{
  uint8_t index = 0;
  switch (cpu.clock.pclk1/1000000){
  case 48:
    switch (speed){
    case 10:
      _timing = C48M10K;	// 10k 	@48M
      break;
    case 400:
      _timing = C48M400K;		// 400k @48M
      break;
    case 200:
      _timing = C48M200K;		// 200k @48M
      break;
    case 100:
    default:
      _timing = C48M100K;			// 100k @48M
    }
    break;
  case 8:
    switch (speed){
    case 10:
      _timing = C8M10K;	        // 10k 	@8M
      break;
    case 400:
      _timing = C8M400K;		// 100k @8M
      break;
    case 200:
      _timing = C8M200K;		// 100k @8M
      break;
    case 100:
    default:
      _timing = C8M100K;		// 100k @8M
    }
    break;
  default:
    _timing = C8M10K;	        // 10k 	@8M
    I2C_DEBUG("PCLKƵ��Ϊ%Mhz,û�и���Ƶ��I2C timing���ã�Ĭ��Ϊ8M10Ktiming������ͨ��configֱ�Ӹ�timing����",cpu.clock.pclk1/1000000);
  }
  
  index = getIndex(_scl->id,I2C_MAP);
  _scl->mode(I2C_MAP[index]._pinMode ,I2C_MAP[index]._pinAf);
  index = getIndex(_sda->id,I2C_MAP);
  _sda->mode(I2C_MAP[index]._pinMode ,I2C_MAP[index]._pinAf);

  config(_timing);
}

void mcuI2c::config(uint32_t speed)
{
  _timing = speed;
  rcc_clock_cmd((uint32_t)_i2cx,ENABLE);
  // I2C1 ��Ҫѡ���ض���ʱ��
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
  *@brief    I2Cд��һ���ֽ�. start->data->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t data:  Ҫд�������
  *          uint16_t tOut: ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
  */
uint8_t mcuI2c::write(uint8_t slaveAddr, uint8_t data)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(200);
#endif
  LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,1,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);

  while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
  {
    if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
    {
      LL_I2C_TransmitData8(_i2cx,data);
    }
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,200)) return EWAIT;
#endif
  }
  LL_I2C_ClearFlag_STOP(_i2cx);

  return EOK;
}

/**
  *@brief    ָ��λ��д��һ���ֽ�. start->data->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t data:  Ҫд�������
  *          uint16_t tOut: ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
  */
uint8_t mcuI2c::write(uint8_t slaveAddr,uint16_t regAddr,uint8_t data,uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(200);
#endif
  LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,2,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);

  while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
  {
    if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
    {
      LL_I2C_TransmitData8(_i2cx,regAddr);
    }
    if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
    {
      LL_I2C_TransmitData8(_i2cx,data);
    }
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,200)) return EWAIT;
#endif
  }
  LL_I2C_ClearFlag_STOP(_i2cx);

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
      // ���͵�ַ�Ĵ���
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
    // д���ַ�Ĵ���������
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
  *@brief    ��ָ���Ĵ�������д start->regAddr->data....->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t regAddr��Ҫд��ļĴ�����ַ
  *          uint8_t *data:  Ҫд�������
  *          uint16_t nWrite  Ҫд������ݳ���
  *          uint16_t tOut:  ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
  */
uint8_t mcuI2c::writeBuf(uint8_t slaveAddr,uint16_t regAddr,uint8_t *data, uint16_t nWrite,uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  // ���͵�ַ�Ĵ���
  LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,1,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_START_WRITE);

  while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
  {
    if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
    {
      LL_I2C_TransmitData8(_i2cx,regAddr);
    }
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,tOut)) return EWAIT;
#endif
  }
  // ��������
  if (nWrite >255){
    // num_to_writ>255: RELOAD,NBYTE=0xFF,NOSTART
    while (nWrite > 255){
      // ���͵�ַ�Ĵ���
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
    // д���ַ�Ĵ���������
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
  *@brief    I2C����һ���ֽ�. start->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint16_t tOut: ��ʱ
  *@retval   ��ȡ��������
  */
uint8_t mcuI2c::read(uint8_t slaveAddr){
  uint8_t ret = 0;
  readBuf(slaveAddr,&ret,1,200);
  return ret;
}

/**
  *@brief    ��ָ���Ĵ���. start->WslaveAddr->regAddr->RslaveAddr->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t regAddr��   Ҫ��ȡ�ļĴ���
  *          uint16_t tOut: ��ʱ
  *@retval   ��ȡ��������
  */
uint8_t mcuI2c::read(uint8_t slaveAddr,uint16_t regAddr,uint16_t tOut)
{
  uint8_t ret = 0;
  readBuf(slaveAddr,regAddr,&ret,1,tOut);
  return ret;
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
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  // ���Ͷ�ָ��ӵ�ǰ��ַ��ʼ��ȡ����
  LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,nRead,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_READ);
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
  *@brief    ָ���Ĵ���������ȡ. start->WslaveAddr->regAddr->RSlaverAddr->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t regAddr: �Ĵ�����ַ
  *          uint8_t *data: ��ȡ��������
  *          uint16_t nRead��Ҫ��ȡ�����ݳ���
  *          uint16_t tOut: ��ʱ
  *@retval   EOK��EWAIT
  */
uint8_t mcuI2c::readBuf(uint8_t slaveAddr,uint16_t regAddr,uint8_t *data, uint16_t nRead,uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
//   while(LL_I2C_IsActiveFlag_STOP(_i2cx)) I2C_DEBUG("ISR reg is %d \r\n",_i2cx->ISR);;
  // ���͵�ַ�Ĵ���
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
  // ���Ͷ�ָ��ӵ�ǰ��ַ��ʼ��ȡ����
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
  *@brief    �ȴ��豸��Ӧ����ָ���豸����startָ�����豸æ���򷵻�NACK,���򷵻�ACK,���豸����stopָ��
  *@param    slaveAddr:  �豸��ַ
  *@retval   uint8_t: EOK,EWAIT
  */
uint8_t mcuI2c:: checkBusy(uint8_t slaveAddr,uint16_t tOut)
{
  uint8_t tmp = 0;
  //uint32_t end = GetEndTime(tOut);
  do
  {
    // clear STOP & NACK flag
    SET_BIT(_i2cx->ICR,LL_I2C_ICR_NACKCF | LL_I2C_ICR_STOPCF);
    LL_I2C_HandleTransfer(_i2cx,slaveAddr,LL_I2C_ADDRESSING_MODE_7BIT,0,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE);
    // �ȴ�����ֹͣλ�������
    while (!LL_I2C_IsActiveFlag_STOP(_i2cx));
    LL_I2C_ClearFlag_STOP(_i2cx);
    if (tOut-- == 0)
    {
      I2C_DEBUG("fail,ISR reg is %d \r\n",_i2cx->ISR);
      return EWAIT;
    }
    tmp = LL_I2C_IsActiveFlag_NACK(_i2cx);
  }while (tmp == 1); //�������Ӧ��������ȴ�
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
