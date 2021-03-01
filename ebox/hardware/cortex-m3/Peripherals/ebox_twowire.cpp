#include "ebox_TwoWire.h"
#include "ebox.h"

#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define mcuTwoWireDebug 1
#endif

#if mcuTwoWireDebug
#define  I2C_DEBUG(...) DBG("[mcuTwoWire]:"),DBG(__VA_ARGS__)
#else
#define  I2C_DEBUG(...)
#endif

//#define _bitDelay 5

mcuTwoWire::mcuTwoWire(I2C_TypeDef *I2Cx,Gpio *sclPin, Gpio *sdaPin)
{
    _scl = sclPin;
    _sda = sdaPin;
    
    _i2cx = I2Cx;
    setTimeout(100);
}


/**
  *@brief    ����i2cʱ�Ӻ���������speed����timing,Ĭ��80K
  *@param    speed:  ���� 10,100,200,300,400 �ֱ����10k��100k��200k,300k,400k
  *@retval   None
  */
void mcuTwoWire::begin(uint8_t address)
{
    rcc_clock_cmd((uint32_t)_i2cx, ENABLE);
    /* I2C configuration */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x48;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    setClock();

    
    _sda->mode(AF_OD);
    _scl->mode(AF_OD);    
    I2C_DEBUG("scl_pin: 0x%x ; sda_pin: 0x%x\n",_scl->id, _sda->id);
}
void mcuTwoWire::begin(int address)
{
    begin((uint8_t)address);
}
void mcuTwoWire::begin()
{
    begin((uint8_t)0);
}
/**
 * @brief ����i2cͨ���ٶ�.
 *
 * @param[in] speed i2c�ٶȣ�����ֵΪ400K��300K��200K��100Kʱ��i2cͨ��Ƶ��Ϊ��ֵ.
 *
 * @return 0.
 */
void mcuTwoWire::setClock(Speed_t speed)
{
   switch (speed)
    {
    case K10:
        _speed = (10000);
        break;
    case K100:
        _speed = (100000);
        break;
    case K200:
        _speed = (200000);
        break;
    case K400:
        _speed = (400000);
        break;
    default:
        _speed = (200000);
    }
    I2C_InitStructure.I2C_ClockSpeed = 10000;

    /* I2C Peripheral Enable */
    I2C_Cmd(_i2cx, ENABLE);
    /* Apply I2C configuration after enabling it */
    I2C_Init(_i2cx, &I2C_InitStructure);
    
    
    I2C_DEBUG("speed:%dKhz;\n",_speed/1000);
}

i2c_err_t mcuTwoWire::_write(const uint8_t *data, size_t quantity)
{
    i2c_err_t ret;
    for(int i = 0; i < quantity; i++) {
        ret = _sendByte(data[i]);
        if(ret != I2C_ERROR_OK)
        {
            return ret;
        }
    }
    return ret;
}

/*
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 *          5 .. timeout
 */
i2c_err_t mcuTwoWire::_write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop)
{
    uint32_t cnt = 0;
    i2c_err_t ret;
    _start();
    _send7bitsAddress(address, 0);
    while (quantity--)
    {
        _sendByte(*data);
        data++;
    }
    _stop();
    return I2C_ERROR_OK;
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
size_t mcuTwoWire::_read(uint8_t address,uint8_t *data, uint16_t length,uint8_t sendStop)
{
    uint32_t cnt = 0;
    int ret = 0;
    ret = length;
     _start();
     _send7bitsAddress(address, 1);
    while (length--)
    {
        if (length == 0)
        {
            _sendNack();
            _stop();
        }
         _receiveByte(data);
        data++;
    }
    _sendAck();
        
    return ret;
    
}
//---------------------------------------------------------------------------


/**
  * @brief ����һ��START�ź�.SCLΪ�ߵ�ƽʱ��SDA�ɸߵ�ƽ��͵�ƽ���䣬��ʼ��������
  * @param ��.
  * @return ��.
  */
void mcuTwoWire::_start(void)
{   
    uint32_t cnt = 0;


    /* Send STRAT condition */
    I2C_GenerateSTART(_i2cx, ENABLE);
    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        cnt++;
        if (cnt > 0xfffe)
        {
            I2C_DEBUG("start fail, state reg SR2 = %d��SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
            return ;
        }
    }
    _err_at = 0;
    return ;
}

/**
  * @brief ����һ��STOP�ź�. SCLΪ�ߵ�ƽʱ��SDA�ɵ͵�ƽ��ߵ�ƽ���䣬������������
  * @param ��.
  * @return ��.
  */
void mcuTwoWire::_stop(void)
{   
    I2C_GenerateSTOP(_i2cx, ENABLE);
}



/**
 * @brief �ȴ�һ��ACKӦ��. ��9��clock������IC��ACK��SDA�ᱻ����
 * @param ��.
 * @return 
 *          0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 *          5 .. timeout
 */
i2c_err_t mcuTwoWire::_waitAck()
{

}
/*
  * @brief ��������.
  * @param[in] byte �������͵�����.
  * @return 
 *          0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 *          5 .. timeout
 */
i2c_err_t mcuTwoWire::_sendByte( uint8_t data )
{
    uint32_t cnt = 0;
    /* Send the byte to be written */
    I2C_SendData(_i2cx, data);
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        cnt++;
        if (cnt > 0xfffe)
        {
            I2C_DEBUG("send data fail, state reg SR2 = %d��SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
            return I2C_ERROR_TIMEOUT;
        }
    }

    return I2C_ERROR_OK;
}

/**
 * @brief ����7bit�ӻ���ַλ.
 *
 * @param[in] slave_address 7bit�ӻ���ַλ.
 *
 * @return ���ͽ��.����0��ʾ���ͳɹ�������-1��ʾ����ʧ��.
 */
i2c_err_t	mcuTwoWire::_send7bitsAddress(uint8_t slaveAddr, uint8_t WR)
{
    uint32_t cnt = 0;

    if (WR) /* Send address for read */
    {
        //    I2C_DEBUG("send read address is %d \r\n",slaveAddr);
        I2C_Send7bitAddress(_i2cx, slaveAddr, I2C_Direction_Receiver);
        /* Test on EV6 and clear it */
        while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        {
        cnt++;
        if (cnt > 0xfffe)
            {
                I2C_DEBUG("send read address fail, state reg SR2 = %d��SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
                return I2C_ERROR_TIMEOUT;
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
            cnt++;
            if (cnt > 0xfffe)
            {
                I2C_DEBUG("send write address fail, state reg SR2 = %d��SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
                return I2C_ERROR_TIMEOUT;
            }
        }
    }
    return I2C_ERROR_OK;
}
/**
  * @brief ��������.
  * @param ��.
  * @return ���յ�������.
  */
uint8_t mcuTwoWire::_receiveByte(uint8_t *data)
{
    uint32_t cnt = 0;
    while (!I2C_CheckEvent(_i2cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
        cnt++;
        if (cnt > 0xfffe)
        {
            I2C_DEBUG("read data fail, state reg SR2 = %d��SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
            return EWAIT;
        }

    }
    *data = I2C_ReceiveData(_i2cx);//�����Ĵ�������
    return EOK;
    return I2C_ERROR_OK;
}

/**
 * @brief ����һ��ACKӦ��. ��9��clock�ڼ䣬����SDA
 * @param ��.
 * @return 0.
 */
int8_t mcuTwoWire::_sendAck()
{
    int8_t err = 0;
    I2C_AcknowledgeConfig(_i2cx, ENABLE);
    return err;
}

/**
  * @brief ����һ��NACKӦ��,��9��clock�ڼ䣬����SDA
  * @param ��
  * @return 0.
  */
int8_t mcuTwoWire::_sendNack()
{
    int8_t err = 0;
    I2C_AcknowledgeConfig(_i2cx, DISABLE);
    return err;
}


