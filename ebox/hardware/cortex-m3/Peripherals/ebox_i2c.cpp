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


#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define debug 0
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
mcuI2c::mcuI2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin)
{
    _busy = 0;
    _i2cx = I2Cx;
    _scl = scl_pin;
    _sda = sda_pin;
}
/**
 *@name     begin(uint16_t speed)
 *@brief    根据i2c时钟和设置速率speed计算timing。默认200k
 *@param    speed:  速率 10,100，200,400 分别代表10k，100k,200k，400k
 *@retval   None
*/
void  mcuI2c::begin(I2c::Config_t *newConfig)
{
    rcc_clock_cmd((uint32_t)_i2cx, ENABLE);

    _sda->mode(AF_OD);
    _scl->mode(AF_OD);
    config(newConfig);
}

void mcuI2c::config(I2c::Config_t *newConfig)
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
  *@brief    I2C写入一个字节. start->data->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t data:  要写入的数据
  *          uint16_t timeout: 超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t mcuI2c::write(uint16_t slaveAddr,uint8_t data)
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
  *@brief    指定位置写入一个字节. start->data->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t data:  要写入的数据
  *          uint16_t timeout: 超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t mcuI2c::write(uint16_t slaveAddr,uint16_t regAddr, uint8_t data)
{
    uint8_t err = EOK;
    I2C_DEBUG("I2C state sr2 = %d, sr1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
    err += _start();
    err += _send7bitsAddress(slaveAddr, WRITE);
    if(cfg->regAddrBits == BIT16)
        err += _sendByte(regAddr>>8);
    err += _sendByte(regAddr);
    err += _sendByte(data);
    _stop();
    return err;
}

/**
  *@brief    I2C连续写 start->data....->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t *data:  要写入的数据
  *          uint16_t nWrite  要写入的数据长度
  *          uint16_t timeout:  超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t mcuI2c::write_buf(uint16_t slaveAddr,uint8_t *data, uint16_t nWrite)
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
  *@brief    在指定寄存器连续写 start->regAddr->data....->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t regAddr：要写入的寄存器地址
  *          uint8_t *data:  要写入的数据
  *          uint16_t nWrite  要写入的数据长度
  *          uint16_t timeout:  超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t mcuI2c::write_buf(uint16_t slaveAddr,uint16_t regAddr, uint8_t *data, uint16_t nWrite)
{
    uint8_t err = 0;
    err += _start();
    err += _send7bitsAddress(slaveAddr, WRITE);
    if(cfg->regAddrBits == BIT16)
        err += _sendByte(regAddr>>8);
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
  *@brief    I2C读入一个字节. start->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint16_t timeout: 超时
  *@retval   读取到的数据
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
  *@brief    读指定寄存器. start->WslaveAddr->regAddr->RslaveAddr->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t regAddr：   要读取的寄存器
  *          uint16_t timeout: 超时
  *@retval   读取到的数据
  */
uint8_t mcuI2c::read(uint16_t slaveAddr,uint16_t regAddr)
{
    uint8_t data ;
    _start();
    _send7bitsAddress(slaveAddr, WRITE);
    _sendByte(regAddr);
    _start();
    _send7bitsAddress(slaveAddr, READ);
    if(cfg->regAddrBits == BIT16)
        _sendByte(regAddr>>8);
    _sendByte(regAddr);
    _sendNack();
    _stop();
    _receiveByte(&data);
    _sendAck();
    return data;
}

/**
  *@brief    连续读取. start->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t *data: 读取到的数据
  *          uint16_t nRead：要读取的数据长度
  *          uint16_t timeout: 超时
  *@retval   EOK，EWAIT
  */
uint8_t mcuI2c::read_buf(uint16_t slaveAddr,uint8_t *data, uint16_t nRead)
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
  *@brief    指定寄存器连续读取. start->WslaveAddr->regAddr->RSlaverAddr->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t regAddr: 寄存器地址
  *          uint8_t *data: 读取到的数据
  *          uint16_t nRead：要读取的数据长度
  *          uint16_t timeout: 超时
  *@retval   EOK，EWAIT
  */
uint8_t mcuI2c::read_buf(uint16_t slaveAddr,uint16_t regAddr, uint8_t *data, uint16_t nRead)
{
    uint8_t err = 0;
    err += _start();
    err += _send7bitsAddress(slaveAddr, WRITE);
    if(cfg->regAddrBits == BIT16)
        err += _sendByte(regAddr>>8);
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
  *@brief    等待设备响应。向指定设备发送start指令，如果设备忙，则返回NACK,否则返回ACK,主设备发送stop指令
  *@param    slaveAddr:  设备地址
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
  *@brief    获取I2C控制权
  *@param    timing:  时钟时序，通过readConfig获取
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
 *@brief    释放I2C控制权
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
            I2C_DEBUG("start fail, state reg SR2 = %d，SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
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
            I2C_DEBUG("send data fail, state reg SR2 = %d，SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
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
                I2C_DEBUG("send read address fail, state reg SR2 = %d，SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
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
                I2C_DEBUG("send write address fail, state reg SR2 = %d，SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
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
            I2C_DEBUG("read data fail, state reg SR2 = %d，SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
            return EWAIT;
        }

    }
    *data = I2C_ReceiveData(_i2cx);//读出寄存器数据
    return EOK;
}
