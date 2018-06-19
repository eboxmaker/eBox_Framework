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

SoftI2c::SoftI2c(Gpio *scl, Gpio *sda)
{
    this->scl_pin = scl;
    this->sda_pin = sda;
}

/**
 * @brief �������i2c����ʼ�����i2cͨ���ٶ�.
 *
 * @param[in] speed i2c�ٶȣ�����ֵΪ400K��300K��200K��100Kʱ��i2cͨ��Ƶ��Ϊ��ֵ.
 *
 * @return ��.
 */
void SoftI2c::begin(uint32_t speed)
{
    this->speed = speed;
    config(this->speed);
    sda_pin->mode(OUTPUT_PP);
    scl_pin->mode(OUTPUT_PP);
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
    this->speed = speed;
    switch(this->speed)
    {
    case 400000:
        delay_times = 0;
        break;
    case 300000:
        delay_times = 4;
        break;
    case 200000:
        delay_times = 8;
        break;
    case 100000:
        delay_times = 16;
        break;
    default:
        delay_times = this->speed;
        break;
    }
}

/**
 * @brief ��ȡi2cƵ��.
 *
 * @param ��.
 *
 * @return i2cƵ��.
 */
uint32_t SoftI2c::read_config()
{
    return this->speed;
}

/**
 * @brief ����һ��START�ź�.
 *
 * @param ��.
 *
 * @return ��.
 */
int8_t SoftI2c::start()
{
    sda_pin->mode(OUTPUT_PP);
    sda_pin->set();
    scl_pin->set();
    delay_us(delay_times);
    sda_pin->reset();
    delay_us(delay_times);
    scl_pin->reset();
    return 0;
}

/**
 * @brief ����һ��STOP�ź�.
 *
 * @param ��.
 *
 * @return ��.
 */
int8_t SoftI2c::stop()
{
    sda_pin->mode(OUTPUT_PP);
    scl_pin->reset();
    sda_pin->reset();
    delay_us(delay_times);
    scl_pin->set();
    delay_us(delay_times);
    sda_pin->set();
    return 0;
}

/**
 * @brief �ȴ�һ��ACKӦ��.
 *
 * @param ��.
 *
 * @return �ȴ����.����0��ʾ�ȴ��ɹ�������-1��ʾ�ȴ�ʧ��.
 */
int8_t SoftI2c::wait_ack()
{
    uint8_t cErrTime = 5;
    sda_pin->mode(INPUT_PU);
    scl_pin->set();
    delay_us(delay_times);
    while(sda_pin->read())
    {
        cErrTime--;
        delay_us(delay_times);
        if(cErrTime == 0)
        {
            sda_pin->mode(OUTPUT_PP);
            stop();
            return -1;

        }

    }
    scl_pin->reset();
    delay_us(delay_times);
    return 0;
}

/**
 * @brief ����һ��ACKӦ��.
 *
 * @param ��.
 *
 * @return 0.
 */
int8_t SoftI2c::send_ack()
{
    sda_pin->mode(OUTPUT_PP);
    sda_pin->reset();
    delay_us(delay_times);
    scl_pin->set();
    delay_us(delay_times);
    scl_pin->reset();
    delay_us(delay_times);

    return 0;
}

/**
 * @brief ����һ��NACKӦ��.
 *
 * @param ��.
 *
 * @return 0.
 */
int8_t SoftI2c::send_no_ack()
{
    sda_pin->mode(OUTPUT_PP);
    sda_pin->set();
    delay_us(delay_times);
    scl_pin->set();
    delay_us(delay_times);
    scl_pin->reset();
    delay_us(delay_times);
    return 0;
}

/**
 * @brief ��������.
 *
 * @param[in] byte �������͵�����.
 *
 * @return ���ͽ��.����0��ʾ���ͳɹ�������-1��ʾ����ʧ��.
 */
int8_t SoftI2c::send_byte(uint8_t byte)
{
    int8_t ret = 0;

    uint8_t ii = 8;
    sda_pin->mode(OUTPUT_PP);
    while( ii-- )
    {
        scl_pin->reset();
        sda_pin->write(byte & 0x80);
        delay_us(delay_times);
        byte += byte;//<<1
        scl_pin->set();
        delay_us(delay_times);
        scl_pin->reset();
        delay_us(delay_times);
    }
    ret = wait_ack();
    return ret;
}

/**
 * @brief ����7bit�ӻ���ַλ.
 *
 * @param[in] slave_address 7bit�ӻ���ַλ.
 *
 * @return ���ͽ��.����0��ʾ���ͳɹ�������-1��ʾ����ʧ��.
 */
int8_t	SoftI2c::send_7bits_address(uint8_t slave_address)
{
    int8_t ret = 0;
    ret = send_byte(slave_address);
    return ret;
}

/**
 * @brief ��������.
 *
 * @param ��.    

 *
 * @return ���յ�������.
 */
int8_t SoftI2c::receive_byte(uint8_t *data)
{
    uint8_t i = 8;
    uint8_t byte = 0;
    int8_t ret = 0;
    sda_pin->mode(INPUT_PU);
    while(i--)
    {
        byte += byte;
        scl_pin->reset();
        delay_us(delay_times);
        scl_pin->set();
        delay_us(delay_times);
        byte |= sda_pin->read();
    }
    scl_pin->reset();
    delay_us(delay_times);
    *data = byte;
    return ret;
}

/**
 * @brief ��ӻ�ָ���Ĵ���д������.
 *
 * @param[in] slave_address 7bit�ӻ���ַ.
 * @param[in] reg_address �Ĵ�����ַ.
 * @param[in] data ��Ҫд�������.
 *
 * @return д����.����0��ʾ���ͳɹ�������-1��ʾ���ʹӻ���ַʧ�ܣ�����-2��ʾ���ʹӻ��Ĵ�����ַʧ�ܣ�����-3��ʾ��������ʧ��.
 */
int8_t SoftI2c::write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t data)
{
    int8_t ret = 0;
    start();

    if (send_7bits_address(slave_address&0xfe) == -1)
        ret = -1;

    if (send_byte(reg_address) == -1)
        ret = -2;

    if (send_byte(data) == -1)
        ret = -3;

    stop();

    delay_us(10);
    return ret;
}

/**
 * @brief ��ӻ�ָ���Ĵ���д������.
 *
 * @param[in] slave_address 7bit�ӻ���ַ.
 * @param[in] reg_address �Ĵ�����ַ.
 * @param[in] data ָ��Ҫд�����ݵ�ָ��.
 * @param[in] num_to_write ��Ҫд��������ֽ���.
 *
 * @return д����.����0��ʾ���ͳɹ�������-1��ʾ���ʹӻ���ַʧ�ܣ�����-2��ʾ���ʹӻ��Ĵ�����ַʧ�ܣ�����-3��ʾ��������ʧ��.
 */
int8_t SoftI2c::write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t len)
{
    int8_t ret = 0;
    start();

    if (send_7bits_address(slave_address&0xfe) == -1)
        ret = -1;

    if (send_byte(reg_address) == -1)
        ret = -2;

    while(len--)
    {
        send_byte(*data++);
        if (wait_ack() == -1)
            ret = -3;
    }

    stop();

    delay_us(10);
    return ret;
}

/**
 * @brief �Ӵӻ�ָ���Ĵ�����ȡ����.
 *
 * @param[in] slave_address 7bit�ӻ���ַ.
 * @param[in] reg_address �Ĵ�����ַ.
 * @param[out] data ָ��Ҫ�洢��ȡ�������ݵ�ָ��.
 *
 * @return ��ȡ���.����0��ʾ���ͳɹ�������-1��ʾ���ʹӻ���ַʧ�ܣ�����-2��ʾ���ʹӻ��Ĵ�����ַʧ�ܣ�����-3��ʾ���Ͷ�ָ��ʧ��.
 */
int8_t 	SoftI2c::read_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data)
{

    int8_t ret = 0;
    start();

    if (send_7bits_address(slave_address&0xfe) == -1)
        ret = -1;


    if (send_byte(reg_address) == -1)
        ret = -2;

    start();

    if (send_byte(slave_address | 0x01) == -1)
        ret = -3;

    receive_byte(data);
    send_no_ack();
    stop();

    return ret;
}

/**
 * @brief �Ӵӻ�ָ���Ĵ�����ȡ����.
 *
 * @param[in] slave_address 7bit�ӻ���ַ.
 * @param[in] reg_address �Ĵ�����ַ.
 * @param[out] data ָ��Ҫ�洢��ȡ�������ݵ�ָ��.
 * @param[in] num_to_read Ҫ��ȡ���ֽ���.
 *
 * @return ��ȡ���.����0��ʾ���ͳɹ�������-1��ʾ���ʹӻ���ַʧ�ܣ�����-2��ʾ���ʹӻ��Ĵ�����ַʧ�ܣ�����-3��ʾ���Ͷ�ָ��ʧ��.
 */
int8_t 	SoftI2c::read_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t len)
{
    int8_t ret = 0;
    int i = 0;

    start();

    if (send_7bits_address(slave_address&0xfe) == -1)
        ret = -1;


    if (send_byte(reg_address) == -1)
        ret = -2;

    start();
    if (send_7bits_address(slave_address | 0x01) == -1)
        ret = -3;

    while(len)
    {
        receive_byte(data++);
        len--;
        i++;
        if(len == 0)
        {
            send_no_ack();
            stop();
            ret = 0;
        }
        else
            send_ack();
    }

    return ret;
}

/**
 * @brief �ȴ��ӻ�����.
 *
 * @param[in] slave_address 7bit�ӻ���ַ.
 *
 * @return �ӻ�״̬.����0��ʾ�ӻ����У�����-1��ʾ�ӻ�æ.
 */
int8_t SoftI2c::wait_dev_busy(uint8_t slave_address)
{
    int8_t ret;
    uint8_t i = 0;
    do
    {
        start();
        ret = send_7bits_address(slave_address);
        send_ack();
        send_byte(slave_address);
        stop();
        if(i++ == 100)
        {
            return -1;
        }
    }
    while(ret != 0); //�������ֵ����0�������ȴ�
    return 0;
}

/**
 * @brief ��ȡi2cʹ��Ȩ������i2cͨ���ٶȣ���i2c��ʹ�ã���ȴ�ֱ�������.
 *
 * @param[in] speed i2c�ٶȣ�����ֵΪ400K��300K��200K��100Kʱ��i2cͨ��Ƶ��Ϊ��ֵ.
 *
 * @return 0.
 */
int8_t SoftI2c::take_i2c_right(uint32_t speed)
{

    while((busy == 1) && (1))
    {
        delay_ms(1);
    }
    this->speed = speed;
    config(this->speed);
    busy = 1;
    return 0;
}

/**
 * @brief �ͷ�i2cʹ��Ȩ��ʹ�����.
 *
 * @param ��.
 *
 * @return 0.
 */
int8_t SoftI2c::release_i2c_right(void)
{
    busy = 0;
    return 0;
}
