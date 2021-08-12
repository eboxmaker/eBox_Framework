#include "soft_i2c.h"


#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define EBOX_DEBUG_SOFTI2C_ENABLE       true
#define EBOX_DEBUG_SOFTI2C_ENABLE_ERR   true
#endif


#if EBOX_DEBUG_SOFTI2C_ENABLE
#define softI2cDebug(...)  ebox_printf("[sI2C DBG]:%d: ",__LINE__),ebox_printf(__VA_ARGS__ )
#else
#define softI2cDebug(...)
#endif

#if EBOX_DEBUG_SOFTI2C_ENABLE_ERR
#define softI2cDebugErr(fmt, ...)  ebox_printf("[sI2C err]:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define softI2cDebugErr(fmt, ...)
#endif



SoftI2c::SoftI2c(Gpio *sclPin, Gpio *sdaPin)
{
    _scl = sclPin;
    _sda = sdaPin;
    setTimeout(100);
}


/**
  *@brief    根据i2c时钟和设置速率speed计算timing,默认80K
  *@param    speed:  速率 10,100,200,300,400 分别代表10k，100k，200k,300k,400k
  *@retval   None
  */
void SoftI2c::begin(uint8_t address)
{
    _sda->mode(OUTPUT_OD_PU);
    _scl->mode(OUTPUT_OD_PU);
    _sda->set();
    _scl->set();
    softI2cDebug("scl_pin: 0x%x ; sda_pin: 0x%x\n",_scl->id, _sda->id);
    setClock();
}
void SoftI2c::begin(int address)
{
    _sda->mode(OUTPUT_OD_PU);
    _scl->mode(OUTPUT_OD_PU);
    _sda->set();
    _scl->set();
    softI2cDebug(" scl_pin: 0x%x ; sda_pin: 0x%x\n",_scl->id, _sda->id);
    setClock();
}
void SoftI2c::begin()
{
    _sda->mode(OUTPUT_OD_PU);
    _scl->mode(OUTPUT_OD_PU);
    _sda->set();
    _scl->set();
    softI2cDebug(" scl_pin: 0x%x ; sda_pin: 0x%x\n",_scl->id, _sda->id);
    setClock();
}
/**
 * @brief 设置i2c通信速度.
 *
 * @param[in] speed i2c速度，当其值为400K，300K，200K，100K时，i2c通信频率为该值.
 *
 * @return 0.
 */
void SoftI2c::setClock(Speed_t speed)
{
    switch(speed)
    {
    case K1000:
    case K400:
    case K300:
        _bitDelay = 1;    // 约200k
        break;
    case K200:
        _bitDelay = 2;    // 约250k
        break;
    case K100:
        _bitDelay = 3;    // 约100k
        break;
    default:
        _bitDelay = 10;    // 约80k
        break;
    }
    softI2cDebug("bit delay:%d;\n",speed);
}

i2c_err_t SoftI2c::_write(const uint8_t *data, size_t quantity)
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
i2c_err_t SoftI2c::_write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop)
{
    i2c_err_t ret;
    _start();
    ret = _send7bitsAddress(address ,0); // clr read bit
    if(ret != I2C_ERROR_OK)
    {
        ret =  I2C_ERROR_ADDR_NACK_NO_RECV;
        softI2cDebugErr("I2C_ERROR_ADDR_NACK_NO_RECV:%d\n",ret);
    }
    if(quantity > 1)
    {
        ret = _sendByte_first(data[0]);
        if(ret != I2C_ERROR_OK)
        {
            ret = I2C_ERROR_DATA_NACK_NO_RECV;
            softI2cDebugErr("I2C_ERROR_DATA_NACK_NO_RECV:%d\n",ret);
        }
        ret = _write(&data[1], quantity - 1);                
        if(ret != I2C_ERROR_OK)
        {
            ret = I2C_ERROR_DATA_NACK_NO_RECV;
            softI2cDebugErr("I2C_ERROR_DATA_NACK_NO_RECV:%d\n",ret);
        }
    }
    else if(quantity == 1)
    {
        ret = _sendByte_first(data[0]);
        if(ret != I2C_ERROR_OK)
        {
            ret = I2C_ERROR_DATA_NACK_NO_RECV;
            softI2cDebugErr("I2C_ERROR_DATA_NACK_NO_RECV:%d\n",ret);
        }
    }
    if(sendStop)
    {
        _stop();
    }
    return ret;
}
/**
  *@brief    指定寄存器连续读取. start->WslaveAddr->regAddr->RSlaverAddr->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint16_t regAddr: 寄存器地址
  *          uint8_t *data: 读取到的数据
  *          uint16_t nRead：要读取的数据长度
  *          : 超时
  *@retval   EOK，EWAIT
  */
size_t SoftI2c::_read(uint8_t address,uint8_t *data, uint16_t length,uint8_t sendStop)
{
    i2c_err_t ret;
    int len = length;

    _start();
    ret = _send7bitsAddress(address , 1); // clr read bit
    if(ret) 
    {
        softI2cDebugErr("Address No Ack:%d\n",ret);
        return 0;
    }
    while (length--)
    {
        ret = _receiveByte(data);
        data++;
        if (length == 0)
        {
            break;
        }
        _sendAck();
    }
    if(sendStop)
    {
        _sendNack();
        _stop();
    }
    return len;
}
//---------------------------------------------------------------------------


/**
  * @brief 发送一个START信号.SCL为高电平时，SDA由高电平向低电平跳变，开始传送数据
  * @param 无.
  * @return 无.
  */
i2c_err_t SoftI2c::_start(void)
{   
    _sda->set();
    _scl->set();          // SCL高
    delay_us(_bitDelay);
    _sda->reset();        // SDA拉低
    delay_us(_bitDelay);
    _err_at = 0;
    return I2C_ERROR_OK;
}

/**
  * @brief 发送一个STOP信号. SCL为高电平时，SDA由低电平向高电平跳变，结束传送数据
  * @param 无.
  * @return 无.
  */
i2c_err_t SoftI2c::_stop(void)
{   
    _sda->reset();      // SDA低电平
    _scl->set();        // SCL拉高并保持
    delay_us(_bitDelay);
    _sda->set();        // SDA拉高
    delay_us(_bitDelay);
    return I2C_ERROR_OK;
}



/**
 * @brief 等待一个ACK应答. 第9个clock，若从IC发ACK，SDA会被拉低
 * @param 无.
 * @return 
 *          0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 *          5 .. timeout
 */
i2c_err_t SoftI2c::_waitAck()
{
    i2c_err_t ret;
    _err_at++;
    _sda->set();
    delay_us(_bitDelay);
    _scl->set();
    delay_us(_bitDelay);
    uint8_t cnt = 0;
    while(1)
    {
        if (!_sda->read())	//SDA为低则从设备返回Ack，否则没返回
        {
            ret = I2C_ERROR_OK;
            break;
        }
        else if(cnt >= 5)
        {
            ret = I2C_ERROR_TIMEOUT;
            softI2cDebugErr("I2C_ERROR_TIMEOUT(at:%d)\n",_err_at );
            break;
        }
        delay_us(_bitDelay);
        cnt++;
    }
    _scl->reset();
    delay_us(_bitDelay);
    
    return ret;
}
/*
  * @brief 发送数据.
  * @param[in] byte 将被发送的数据.
  * @return 
 *          0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 *          5 .. timeout
 */
i2c_err_t SoftI2c::_sendByte( uint8_t c )
{
    uint8_t ii = 8;
    _scl->reset();        //SCL拉低
    delay_us(_bitDelay);
    while( ii-- )
    {
        _sda->write(c & 0x80);   // SCL低电平时将数据送到SDA
        delay_us(_bitDelay);
        _scl->set();                // 产生一个时钟脉冲
        delay_us(_bitDelay);
        c = c << 1;
        _scl->reset();
    }
    delay_us(_bitDelay);
    i2c_err_t ret = _waitAck();
    
    return ret;
}
i2c_err_t SoftI2c::_sendByte_first( uint8_t c)
{
    uint16_t cnt = 0;
    _scl->reset();        //SCL拉低
    delay_us(_bitDelay);
    _sda->write(c & 0x80);   // SCL低电平时将数据送到SDA
    delay_us(_bitDelay);
    _scl->set();                // 产生一个时钟脉冲
    delay_us(_bitDelay);
    while(!_scl->read())//等待从机释放总线
    {
        cnt++;
        if(cnt > 0x2ff)
        {
            _err_at++;
            softI2cDebugErr("I2C_ERROR_TIMEOUT(at:%d)\n",_err_at );
            return I2C_ERROR_TIMEOUT;
        }
    }
    c = c << 1;
    _scl->reset();
    
    uint8_t ii = 7;
    while( ii-- )
    {
        _sda->write(c & 0x80);   // SCL低电平时将数据送到SDA
        delay_us(_bitDelay);
        _scl->set();                // 产生一个时钟脉冲
        delay_us(_bitDelay);
        c = c << 1;
        _scl->reset();
    }
    delay_us(_bitDelay);
    i2c_err_t ret = _waitAck();
    
    return ret;
}
/**
 * @brief 发送7bit从机地址位.
 *
 * @param[in] slave_address 7bit从机地址位.
 *
 * @return 发送结果.返回0表示发送成功，返回-1表示发送失败.
 */
i2c_err_t	SoftI2c::_send7bitsAddress(uint8_t slaveAddr, uint8_t WR)
{
    // 写，从地址最低位置0；读，从地址最低位置1；
    slaveAddr = (WR == 0) ? (slaveAddr & 0xfe) : (slaveAddr | 0x01);
    return _sendByte(slaveAddr);;
}
/**
  * @brief 接收数据.
  * @param 无.
  * @return 接收到的数据.
  */
i2c_err_t SoftI2c::_receiveByte(uint8_t *data)
{
    uint8_t i = 8;
    uint8_t byte = 0;
    _sda->set();
    while(i--)
    {
        byte += byte;
        _scl->reset();      // 时钟高电平期间读SDA
        delay_us(_bitDelay);
        _scl->set();
        delay_us(_bitDelay);
        byte |= _sda->read();
    }
    _scl->reset();
    delay_us(_bitDelay);
//    softI2cDebug("rx :0x%x\n",byte);

    *data = byte;
    return I2C_ERROR_OK;
}

/**
 * @brief 发送一个ACK应答. 第9个clock期间，拉低SDA
 * @param 无.
 * @return 0.
 */
i2c_err_t SoftI2c::_sendAck()
{
    _sda->reset();          // 拉低SDA，
    delay_us(_bitDelay);
    _scl->set();            // 给出一个时钟
    delay_us(_bitDelay);
    _scl->reset();
    delay_us(_bitDelay);
    return I2C_ERROR_OK;
}

/**
  * @brief 发送一个NACK应答,第9个clock期间，拉高SDA
  * @param 无
  * @return 0.
  */
i2c_err_t SoftI2c::_sendNack()
{
    _sda->set();
    delay_us(_bitDelay);
    _scl->set();
    delay_us(_bitDelay);
    _scl->reset();
    delay_us(_bitDelay);
    return I2C_ERROR_OK;
}


