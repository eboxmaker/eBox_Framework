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
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 1
#endif

#if debug
#define  I2C_DEBUG(...) DBG("[SOFTI2C]  "),DBG(__VA_ARGS__)
#else
#define  I2C_DEBUG(...)
#endif


/**
  *@brief    softI2C���캯��
  *@param    scl_pin:  ʱ��Pin
  *          sda_pin:  ����Pin
  *@retval   None
  */
SoftI2c::SoftI2c(Gpio *scl, Gpio *sda)
{
    _scl = scl;
    _sda = sda;
    _busy = 0;
}

/**
  *@brief    ����i2cʱ�Ӻ���������speed����timing,Ĭ��80K
  *@param    speed:  ���� 10,100,200,300,400 �ֱ����10k��100k��200k,300k,400k
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
        _timing = 1;    // Լ200k
        break;
    case 100:
        _timing = 3;    // Լ100k
        break;
    default:
        _timing = 4;    // Լ80k
        break;
    }
}

/**
 * @brief ����i2cͨ���ٶ�.
 *
 * @param[in] speed i2c�ٶȣ�����ֵΪ400K��300K��200K��100Kʱ��i2cͨ��Ƶ��Ϊ��ֵ.
 *
 * @return 0.
 */
void SoftI2c::config(uint32_t speed)
{
    _timing = speed;;
}

/**
 * @brief ��ȡi2cƵ��.
 *
 * @param ��.
 *
 * @return i2cƵ��.
 */
uint32_t SoftI2c::readConfig()
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
  *@brief    I2C����д start->data....->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t *data:  Ҫд�������
  *          uint16_t nWrite  Ҫд������ݳ���
  *          uint16_t tOut:  ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
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
  *@brief    ��ָ���Ĵ�������д start->regAddr->data....->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t regAddr��Ҫд��ļĴ�����ַ
  *          uint8_t *data:  Ҫд�������
  *          uint16_t nWrite  Ҫд������ݳ���
  *          uint16_t tOut:  ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
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
  *@brief    I2C����һ���ֽ�. start->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint16_t tOut: ��ʱ
  *@retval   ��ȡ��������
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
  *@brief    ��ָ���Ĵ���. start->WslaveAddr->regAddr->RslaveAddr->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t regAddr��   Ҫ��ȡ�ļĴ���
  *          uint16_t tOut: ��ʱ
  *@retval   ��ȡ��������
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
  *@brief    ������ȡ. start->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t *data: ��ȡ��������
  *          uint16_t nRead��Ҫ��ȡ�����ݳ���
  *          uint16_t tOut: ��ʱ
  *@retval   EOK��EWAIT
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
  *@brief    ָ���Ĵ���������ȡ. start->WslaveAddr->regAddr->RSlaverAddr->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t regAddr: �Ĵ�����ַ
  *          uint8_t *data: ��ȡ��������
  *          uint16_t nRead��Ҫ��ȡ�����ݳ���
  *          uint16_t tOut: ��ʱ
  *@retval   EOK��EWAIT
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
 * @brief �ȴ��ӻ�����.
 *
 * @param[in] slave_address 7bit�ӻ���ַ.
 *
 * @return �ӻ�״̬.����0��ʾ�ӻ����У�����-1��ʾ�ӻ�æ.
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
    while(ret != 0); //�������ֵ����0�������ȴ�
    return EOK;
}

/**
  *@brief    ��ȡI2C����Ȩ
  *@param    timing:  ʱ��ʱ��ͨ��readConfig��ȡ
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
 *@brief    �ͷ�I2C����Ȩ
 *@param    none
 *@retval   none
*/
void SoftI2c::releaseRight(void)
{
  _busy = 0;
}

/**
  * @brief ����һ��START�ź�.SCLΪ�ߵ�ƽʱ��SDA�ɸߵ�ƽ��͵�ƽ���䣬��ʼ��������
  * @param ��.
  * @return ��.
  */
int8_t SoftI2c::_start(uint16_t tOut)
{
#if	(USE_TIMEOUT != 0)
  uint32_t end = GetEndTime(tOut);
#endif
  // scl,sda ��Ϊ�ߵ�ƽ�����߿���
  while (!(_sda->read() && _scl->read())){
#if	(USE_TIMEOUT != 0)
    if (IsTimeOut(end,tOut)){
      I2C_DEBUG("start fail,bus busy SDA is %d��SCL is %d,time is %d us \r\n",_sda->read(),_scl->read(),_timing);
      return EWAIT;
    }
#endif
  }
  _sda->mode(OUTPUT_PP);
  _sda->set();
  _scl->set();        // SCL��
  delay_us(_timing);
  _sda->reset();      // SDA����
  delay_us(_timing);
  _scl->reset();
  return EOK;
}

/**
  * @brief ����һ��STOP�ź�. SCLΪ�ߵ�ƽʱ��SDA�ɵ͵�ƽ��ߵ�ƽ���䣬������������
  * @param ��.
  * @return ��.
  */
void SoftI2c::_stop()
{
    _sda->mode(OUTPUT_PP);
    _scl->reset();
    _sda->reset();      // SDA�͵�ƽ
    delay_us(_timing);
    _scl->set();        // SCL���߲�����
    delay_us(_timing);  
    _sda->set();        // SDA����
}

/**
 * @brief �ȴ�һ��ACKӦ��. ��9��clock������IC��ACK��SDA�ᱻ����
 * @param ��.
 * @return EOK,EWAIT.
 */
int8_t SoftI2c::_waitAck()
{
    uint8_t cErrTime = 10;
    _sda->mode(INPUT_PU);
    _scl->set();
    delay_us(_timing);
    while(_sda->read())       // �ȴ�SDA�����ͣ���ACK��Ӧ
    {
        cErrTime--;
        delay_us(_timing);
        if(cErrTime == 0)     // ����Ӧ������stop�ź�
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
 * @brief ����һ��ACKӦ��. ��9��clock�ڼ䣬����SDA
 * @param ��.
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
  * @brief ����һ��NACKӦ��,��9��clock�ڼ䣬����SDA
  * @param ��
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
  * @brief ��������.
  * @param[in] byte �������͵�����.
  * @return ���ͽ��.EOK,EWAIT.
  */
int8_t SoftI2c::_sendByte(uint8_t byte,uint16_t tOut)
{
    int8_t ret = EOK;

    uint8_t ii = 8;
    _sda->mode(OUTPUT_PP);
    _scl->reset();
    while( ii-- )
    {
        _sda->write(byte & 0x80);   // SCL�͵�ƽʱ�������͵�SDA
        delay_us(_timing);
        _scl->set();                // ����һ��ʱ������
        delay_us(_timing);
        byte = byte<<1;
        _scl->reset();
    }
    ret = _waitAck();
    return ret;
}

/**
 * @brief ����7bit�ӻ���ַλ.
 *
 * @param[in] slave_address 7bit�ӻ���ַλ.
 *
 * @return ���ͽ��.����0��ʾ���ͳɹ�������-1��ʾ����ʧ��.
 */
int8_t	SoftI2c::_send7bitsAddress(uint8_t slaveAddr,uint8_t WR,uint16_t tOut)
{
    int8_t ret = 0;
    // д���ӵ�ַ���λ��0�������ӵ�ַ���λ��1��
    slaveAddr = (WR == WRITE) ? (slaveAddr & 0xfe):(slaveAddr | 0x01);
    ret = _sendByte(slaveAddr,tOut);
    return ret;
}

/**
  * @brief ��������.
  * @param ��.  
  * @return ���յ�������.
  */
int8_t SoftI2c::_receiveByte(uint8_t *data,uint16_t tOut)
{
    uint8_t i = 8;
    uint8_t byte = 0;
    _sda->mode(INPUT_PU);
    while(i--)
    {
        byte += byte;
        _scl->reset();      // ʱ�Ӹߵ�ƽ�ڼ��SDA
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
