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
//#include "stm32f072_define.h"
#include "mcu_define.h"

#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define EBOX_DEBUG_MCUI2C_ENABLE       true
#define EBOX_DEBUG_MCUI2C_ENABLE_ERR   true
#endif


#if EBOX_DEBUG_MCUI2C_ENABLE
#define mcuI2cDebug(...)  ebox_printf("[I2C DBG]:%d: ",__LINE__),ebox_printf(__VA_ARGS__ )
#else
#define mcuI2cDebug(...)
#endif

#if EBOX_DEBUG_MCUI2C_ENABLE_ERR
#define mcuI2cDebugErr(fmt, ...)  ebox_printf("[I2C err]:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define mcuI2cDebugErr(fmt, ...)
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
 *@param    I2Cx:  _i2cx,I2C2
 *          scl_pin:  ʱ��Pin
 *          sda_pin:  ����Pin
 *@retval   None
*/
mcuI2c::mcuI2c(I2C_TypeDef *I2Cx,Gpio *sclPin, Gpio *sdaPin)
{
    _scl = sclPin;
    _sda = sdaPin;
    
    _i2cx = I2Cx;
    setTimeout(100);
}

/**
  *@brief    ����i2cʱ�Ӻ���������speed����timing��8M��48M��ƵĬ��100k������Ƶ��Ĭ��10k @8M
  *@param    speed:  ���� 10,100��200,400 �ֱ����10k��100k��200k��400k
  *@retval   None
  */
void  mcuI2c::begin(uint8_t address)
{
    uint8_t index = 0;

    index = getIndex(_scl->id, I2C_MAP);
    _scl->mode(I2C_MAP[index]._pinMode, I2C_MAP[index]._pinAf);
    index = getIndex(_sda->id, I2C_MAP);
    _sda->mode(I2C_MAP[index]._pinMode, I2C_MAP[index]._pinAf);
    setClock(K100);
}
void mcuI2c::begin(int address)
{
    begin((uint8_t)address);
}
void mcuI2c::begin()
{
    begin((uint8_t)0);
}
void mcuI2c::setClock(Speed_t speed)
{
    uint32_t _timing = 0;
    switch (cpu.clock.pclk1 / 1000000)
    {
    case 48:
        switch (speed)
        {
        case K10:
            _timing = C48M10K;	// 10k 	@48M
            break;
        case K400:
            _timing = C48M400K;		// 400k @48M
            break;
        case K200:
            _timing = C48M200K;		// 200k @48M
            break;
        case K100:
        default:
            _timing = C48M100K;			// 100k @48M
        }
        break;
    case 8:
        switch (speed)
        {
        case K10:
            _timing = C8M10K;	        // 10k 	@8M
            break;
        case K400:
            _timing = C8M400K;		// 100k @8M
            break;
        case K200:
            _timing = C8M200K;		// 100k @8M
            break;
        case K100:
        default:
            _timing = C8M100K;		// 100k @8M
        }
        break;
    default:
        _timing = C8M10K;	        // 10k 	@8M
        mcuI2cDebug("PCLKƵ��Ϊ%Mhz,û�и���Ƶ��I2C timing���ã�Ĭ��Ϊ8M10Ktiming������ͨ��configֱ�Ӹ�timing����", cpu.clock.pclk1 / 1000000);
    }
		
    rcc_clock_cmd((uint32_t)_i2cx, ENABLE);
    // _i2cx ��Ҫѡ���ض���ʱ��
    (_i2cx == _i2cx) ? (LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_SYSCLK)) : (void());

    LL_I2C_Disable(_i2cx);
    LL_I2C_SetTiming(_i2cx, _timing);
    LL_I2C_Enable(_i2cx);
}

/*
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 *          5 .. timeout
 */
i2c_err_t mcuI2c::_write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop)
{
    i2c_err_t ret = I2C_ERROR_OK;
    uint32_t counter = 0;
    uint8_t send_cnt = 0;
    LL_I2C_ClearFlag_NACK(_i2cx);

    while(LL_I2C_IsActiveFlag_BUSY(_i2cx) == SET){
        counter++;
        if( counter == 25000 ) {//
            LL_I2C_ClearFlag_BERR(_i2cx);
            mcuI2cDebugErr("busy ISR:0X%08X\n",_i2cx->ISR);
            return I2C_ERROR_BUS;
        }
    }
//    if(sendStop)
//        LL_I2C_HandleTransfer(_i2cx, address,LL_I2C_ADDRSLAVE_7BIT, 1,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE ); //LL_I2C_GENERATE_START_READ
//    else
//        LL_I2C_HandleTransfer(_i2cx, address,LL_I2C_ADDRSLAVE_7BIT, 1,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_START_WRITE ); //LL_I2C_GENERATE_START_READ

   /* ����������ַ */
    LL_I2C_HandleTransfer(_i2cx, address, LL_I2C_ADDRSLAVE_7BIT, (quantity), LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
    if(quantity == 0) /*�������Ϊ�գ������豸��Ӧλ */
    {
        counter = 0;
        while(!LL_I2C_IsActiveFlag_STOP(_i2cx))
        {
            counter++;
            if( counter == 25000 ) {//�������ݳ�ʱ
                mcuI2cDebugErr("Addr stop. ISR:0X%08X\n",_i2cx->ISR);
                ret =  I2C_ERROR_BUS;
                break;
            }
        }//�ȴ��������
        LL_I2C_ClearFlag_STOP(_i2cx);
        if(LL_I2C_IsActiveFlag_NACK(_i2cx))//���û����Ӧ���ʾû���ҵ��豸
        {
            mcuI2cDebugErr("Addr no ack. ISR:0X%08X\n",_i2cx->ISR);
            ret =  I2C_ERROR_ADDR_NACK_NO_RECV;
            LL_I2C_ClearFlag_NACK(_i2cx);
        }
    }
    else
    {
        while(send_cnt < quantity)
        {
            counter = 0;
            while(!LL_I2C_IsActiveFlag_TXE(_i2cx))
            {
                counter++;
                if( counter == 25000 ) {//�������ݳ�ʱ
                    mcuI2cDebugErr("data %dBytes send cnt:%d ISR:0X%08X\n",quantity,send_cnt,_i2cx->ISR);
                    ret =  I2C_ERROR_BUS;
                    break;
                }
            }
            if(ret)
            {
                break;//������ִ���ֹͣ����;
            }
            else
            {
                LL_I2C_TransmitData8(_i2cx, data[send_cnt++]);
            }
        }
        counter = 0;
        while(!LL_I2C_IsActiveFlag_STOP(_i2cx))
        {
            counter++;
            if( counter == 25000 ) {//�������ݳ�ʱ
                mcuI2cDebugErr("data stop err ISR:0X%08X\n",_i2cx->ISR);
                ret =  I2C_ERROR_BUS;
                break;
            }
        }//�ȴ��������
        LL_I2C_ClearFlag_STOP(_i2cx);
    }
    return ret;
}
/**
  *@brief    ָ���Ĵ���������ȡ. start->WslaveAddr->regAddr->RSlaverAddr->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint16_t regAddr: �Ĵ�����ַ
  *          uint8_t *data: ��ȡ��������
  *          uint16_t nRead��Ҫ��ȡ�����ݳ���
  *          : ��ʱ
  *@retval   EOK��EWAIT
  */
size_t mcuI2c::_read(uint8_t address,uint8_t *data, uint16_t length,uint8_t sendStop)
{
    uint32_t counter = 0;
    i2c_err_t err = I2C_ERROR_OK;
    int read_cnt = 0;
    
    LL_I2C_ClearFlag_NACK(_i2cx);

    while(LL_I2C_IsActiveFlag_BUSY(_i2cx) == SET){
        counter++;
        if( counter == 25000 ) {//
            mcuI2cDebugErr("busy ISR:0X%08X\n",_i2cx->ISR);
            LL_I2C_ClearFlag_BERR(_i2cx);
            return I2C_ERROR_BUS;
        }
    }
    if(sendStop)
        LL_I2C_HandleTransfer(_i2cx, address, LL_I2C_ADDRSLAVE_7BIT, length, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
    else
        LL_I2C_HandleTransfer(_i2cx, address, LL_I2C_ADDRSLAVE_7BIT, length, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);

      /* Loop until STOP flag is raised  */
	while(!LL_I2C_IsActiveFlag_STOP(_i2cx))
	{
        if(LL_I2C_IsActiveFlag_RXNE(_i2cx))
        {
            data[read_cnt++] = LL_I2C_ReceiveData8(_i2cx);
        }
        counter++;
        if( counter == 25000 ) {//
            mcuI2cDebugErr("data recv:%d; ISR:0X%08X\n",read_cnt,_i2cx->ISR);
            return read_cnt;
        }
	}
    LL_I2C_ClearFlag_STOP(_i2cx);
    return read_cnt;
}
