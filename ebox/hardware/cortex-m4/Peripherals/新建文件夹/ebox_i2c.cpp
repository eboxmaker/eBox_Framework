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


#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 0
#endif

#if debug
#define  I2C_DEBUG(...) DBG("[I2C]  "),DBG(__VA_ARGS__)
#else
#define  I2C_DEBUG(...)
#endif

/**
 *@name     I2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin)
 *@brief      I2C���캯��
 *@param    I2Cx:  I2C1,I2C2
 *          scl_pin:  ʱ��Pin
 *          sda_pin:  ����Pin
 *@retval   None
*/
mcuI2c::mcuI2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin)
{
    _busy = 0;
    _i2cx = I2Cx;
    _scl = scl_pin;
    _sda = sda_pin;
}
/**
 *@name     begin(uint16_t speed)
 *@brief    ����i2cʱ�Ӻ���������speed����timing��Ĭ��200k
 *@param    speed:  ���� 10,100��200,400 �ֱ����10k��100k,200k��400k
 *@retval   None
*/
void  mcuI2c::begin(Config_t *newConfig)
{
    rcc_clock_cmd((uint32_t)_i2cx, ENABLE);

    _sda->mode(AF_OD);
    _scl->mode(AF_OD);
    config(newConfig);
}

void mcuI2c::config(Config_t *newConfig)
{
    this->cfg = newConfig;
    switch (cfg->speed)
    {
    case K10:
        _timing = (10000);
        break;
    case K100:
        _timing = (100000);
        break;
    case K200:
        _timing = (200000);
        break;
    case K400:
        _timing = (400000);
        break;
    default:
        _timing = (200000);
    }

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

/**
  *@brief    I2Cд��һ���ֽ�. start->data->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t data:  Ҫд�������
  *          uint16_t tOut: ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
  */
uint8_t mcuI2c::write(uint16_t slaveAddr, uint8_t data)
{
    uint8_t err = EOK;
    I2C_DEBUG("I2C state sr2 = %d, sr1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
    err += _start();
    err += _send7bitsAddress(slaveAddr, WRITE);
    err += _sendByte(data);
    _stop();
    return err;
}

/**
  *@brief    ָ��λ��д��һ���ֽ�. start->data->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t data:  Ҫд�������
  *          uint16_t tOut: ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
  */
uint8_t mcuI2c::write(uint16_t slaveAddr, uint16_t regAddr, uint8_t data)
{
    uint8_t err = EOK;
    I2C_DEBUG("I2C state sr2 = %d, sr1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
    err += _start();
    err += _send7bitsAddress(slaveAddr, WRITE);
    err += _sendByte(regAddr);
    err += _sendByte(data);
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
uint8_t mcuI2c::write_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nWrite)
{
    uint8_t err = 0;
    err += _start();
    err += _send7bitsAddress(slaveAddr, WRITE);
    while (nWrite--)
    {
        err += _sendByte(*data);
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
uint8_t mcuI2c::write_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nWrite)
{
    uint8_t err = 0;
    err += _start();
    err += _send7bitsAddress(slaveAddr, WRITE);
    err += _sendByte(regAddr);
    while (nWrite--)
    {
        err += _sendByte(*data);
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
uint8_t mcuI2c::read(uint16_t slaveAddr)
{
    uint8_t data ;
    _start();
    _send7bitsAddress(slaveAddr, READ);
    _sendNack();
    _stop();
    _receiveByte(&data);
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
uint8_t mcuI2c::read(uint16_t slaveAddr, uint16_t regAddr)
{
    uint8_t data ;
    _start();
    _send7bitsAddress(slaveAddr, WRITE);
    _sendByte(regAddr);
    _start();
    _send7bitsAddress(slaveAddr, READ);
    _sendNack();
    _stop();
    _receiveByte(&data);
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
uint8_t mcuI2c::read_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nRead)
{
    uint8_t err = 0;
    err += _start();
    err += _send7bitsAddress(slaveAddr, READ);
    while (nRead--)
    {
        if (nRead == 0)
        {
            _sendNack();
            _stop();
        }
        err += _receiveByte(data);
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
uint8_t mcuI2c::read_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nRead)
{
    uint8_t err = 0;
    err += _start();
    err += _send7bitsAddress(slaveAddr, WRITE);
    err += _sendByte(regAddr);
    err += _start();
    err += _send7bitsAddress(slaveAddr, READ);
    while (nRead--)
    {
        if (nRead == 0)
        {
            _sendNack();
            _stop();
        }
        err += _receiveByte(data);
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
uint8_t mcuI2c:: check_busy(uint16_t slaveAddr)
{
    uint32_t end = GetEndTime(timeout);

    do
    {
        I2C_ClearFlag(_i2cx, I2C_FLAG_AF);
        I2C_GenerateSTART(_i2cx, ENABLE);

        I2C_Send7bitAddress(_i2cx, slaveAddr, I2C_Direction_Transmitter);
        if (IsTimeOut(end, timeout))
        {
            I2C_ClearFlag(_i2cx, I2C_FLAG_AF | I2C_FLAG_ADDR | I2C_FLAG_SB);
            //      I2C_SendData(_i2cx, slaveAddr);
            I2C_GenerateSTOP(_i2cx, ENABLE);
            delay_ms(1);
            //            I2C_DEBUG("I2C state sr2 = %d, sr1 = %d \r\n",_i2cx->SR2,_i2cx->SR1);
            return 1;
        }
    }
    while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    /* STOP condition */
    I2C_GenerateSTOP(_i2cx, ENABLE);
    delay_ms(1);
    return EOK;
}

/**
  *@brief    ��ȡI2C����Ȩ
  *@param    timing:  ʱ��ʱ��ͨ��readConfig��ȡ
  *@retval   uint8_t: EOK,E_BUSY
  */
uint8_t mcuI2c::take(Config_t *newConfig)
{
    uint32_t end = GetEndTime(timeout);

    while (_busy == 1)
    {
        delay_ms(1);
        if (IsTimeOut(end, timeout)) return EWAIT;
    }
    cfg->regAddrBits = newConfig->regAddrBits;
    if (cfg->speed !=  newConfig->speed) config(newConfig);
    
    _busy = 1;
    return EOK;
}
/**
 *@brief    �ͷ�I2C����Ȩ
 *@param    none
 *@retval   none
*/
void mcuI2c::release(void)
{
    _busy = 0;
}

int8_t mcuI2c::_start()
{
    uint32_t end = GetEndTime(timeout);
    /* Send STRAT condition */
    I2C_GenerateSTART(_i2cx, ENABLE);
    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if (IsTimeOut(end, timeout))
        {
            I2C_DEBUG("start fail, state reg SR2 = %d��SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
            return EWAIT;
        }

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


int8_t mcuI2c::_sendByte(uint8_t data)
{
    uint32_t end = GetEndTime(timeout);

    /* Send the byte to be written */
    I2C_SendData(_i2cx, data);
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {

        if (IsTimeOut(end, timeout))
        {
            I2C_DEBUG("send data fail, state reg SR2 = %d��SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
            return EWAIT;
        }

    }
    return EOK;
}
int8_t mcuI2c::_send7bitsAddress(uint8_t slaveAddr, uint8_t WR)
{
    uint32_t end = GetEndTime(timeout);

    if (WR) /* Send address for read */
    {
        //    I2C_DEBUG("send read address is %d \r\n",slaveAddr);
        I2C_Send7bitAddress(_i2cx, slaveAddr, I2C_Direction_Receiver);
        /* Test on EV6 and clear it */
        while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        {
            if (IsTimeOut(end, timeout))
            {
                I2C_DEBUG("send read address fail, state reg SR2 = %d��SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
                return EWAIT;
            }

        }
    }
    else   /* Send address for write */
    {
        //    I2C_DEBUG("send write address is %d \r\n",slaveAddr);
        I2C_Send7bitAddress(_i2cx, slaveAddr, I2C_Direction_Transmitter);
        /* Test on EV6 and clear it */
        while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        {
            if (IsTimeOut(end, timeout))
            {
                I2C_DEBUG("send write address fail, state reg SR2 = %d��SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
                return EWAIT;
            }

        }
    }
    return EOK;
}
int8_t mcuI2c::_receiveByte(uint8_t *data)
{
    uint32_t end = GetEndTime(timeout);

    while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
        if (IsTimeOut(end, timeout))
        {
            I2C_DEBUG("read data fail, state reg SR2 = %d��SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
            return EWAIT;
        }
    }
    *data = I2C_ReceiveData(_i2cx);//�����Ĵ�������
    return EOK;
}
