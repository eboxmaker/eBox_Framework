#include "TwoWire.h"


#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define debug 1
#endif

#if debug
#define  I2C_DEBUG(...) DBG("[SOFTI2C]  "),DBG(__VA_ARGS__)
#else
#define  I2C_DEBUG(...)
#endif

//#define _bitDelay 5

TwoWire::TwoWire(Gpio *sclPin, Gpio *sdaPin)
{
    _scl = sclPin;
    _sda = sdaPin;
    setTimeout(100);
}


void TwoWire::begin()
{
    begin(K100);
}


/**
  *@brief    根据i2c时钟和设置速率speed计算timing,默认80K
  *@param    speed:  速率 10,100,200,300,400 分别代表10k，100k，200k,300k,400k
  *@retval   None
  */
void TwoWire::begin(Speed_t speed)
{
    _sda->mode(OUTPUT_OD_PU);
    _scl->mode(OUTPUT_OD_PU);
    _sda->set();
    _scl->set();

    I2C_DEBUG("scl pin id 0x%x state is %d , sda pin id 0x%x  state is %d \r\n", _scl->id, _scl->read(), _sda->id, _sda->read());
    setClock(speed);
}
/**
 * @brief 设置i2c通信速度.
 *
 * @param[in] speed i2c速度，当其值为400K，300K，200K，100K时，i2c通信频率为该值.
 *
 * @return 0.
 */
void TwoWire::setClock(Speed_t speed)
{
    switch(speed)
    {
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
        _bitDelay = 4;    // 约80k
        break;
    }
    ebox_printf("speed:%d\n_bit:%d\nbit:%d\n",speed,_bitDelay,_bitDelay);
}

//
//
//
void TwoWire::beginTransmission(uint8_t address)
{
    transmitting = 1;
    txAddress = address;
    txIndex = 0;
    txLength = 0;
    _err_at = 0;
}


//
void TwoWire::beginTransmission(int address)
{
    beginTransmission((uint8_t)address);
}

uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);
}
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{   
    uint8_t ret = _write(txAddress, txBuffer, txLength, sendStop);
    txIndex = 0;
    txLength = 0;
    transmitting = 0;
    return ret;
}


uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
    if (isize > 0) 
    {
        // send internal address; this mode allows sending a repeated start to access
        // some devices' internal registers. This function is executed by the hardware
        // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

        beginTransmission(address);

        // the maximum size of internal address is 3 bytes
        if (isize > 3)
        {
            isize = 3;
        }

        // write internal register address - most significant byte first
        while (isize-- > 0)
            write((uint8_t)(iaddress >> (isize*8)));
        endTransmission(false);
    }

    // clamp to buffer length
    if(quantity > I2C_BUFFER_LENGTH)
    {
        quantity = I2C_BUFFER_LENGTH;
    }
    // perform blocking read into buffer
    size_t read = _read(address, rxBuffer, quantity, sendStop);
    // set rx buffer iterator vars
    rxIndex = 0;
    rxLength = read;

    return read;
}
uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
	return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}    
uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);

}
uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

size_t TwoWire::write(uint8_t data)
{       
    if(txLength >= I2C_BUFFER_LENGTH) {
        return 0;
    }
    txBuffer[txIndex] = data;
    ++txIndex;
    txLength = txIndex;
    return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t size)
{
    for(size_t i = 0; i < size; ++i) {
        if(!write(data[i])) {
            return i;
        }
    }
    return size;
}

int TwoWire::available(void)
{
    int result = rxLength - rxIndex;
    return result;
}

int TwoWire::read(void)
{
    int value = -1;
    if(rxIndex < rxLength) {
        value = rxBuffer[rxIndex];
        ++rxIndex;
    }
    return value;
}

int TwoWire::peek(void)
{
    int value = -1;
    if(rxIndex < rxLength) {
        value = rxBuffer[rxIndex];
    }
    return value;
}

void TwoWire::flush(void)
{
    rxIndex = 0;
    rxLength = 0;
}















//--------------------------------------------------------------------


i2c_err_t TwoWire::_write(const uint8_t *data, size_t quantity)
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
i2c_err_t TwoWire::_write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop)
{
    i2c_err_t ret;
    _start();
    ret = _send7bitsAddress(address ,0); // clr read bit
    if(ret != I2C_ERROR_OK)
    {
        ret =  I2C_ERROR_ADDR_NACK_NO_RECV;
        I2C_DEBUG("I2C_ERROR_ADDR_NACK_NO_RECV\n");
    }
//    delay_us(200);
    if(quantity > 1)
    {
        ret = _sendByte_first(data[0]);
        ret = _write(&data[1], quantity - 1);                
        if(ret != I2C_ERROR_OK)
        {
            ret = I2C_ERROR_DATA_NACK_NO_RECV;
            I2C_DEBUG("I2C_ERROR_DATA_NACK_NO_RECV\n");
        }
    }
    else if(quantity == 1)
    {
        _sendByte_first(data[0]);
        if(ret != I2C_ERROR_OK)
        {
            ret = I2C_ERROR_DATA_NACK_NO_RECV;
            I2C_DEBUG("I2C_ERROR_DATA_NACK_NO_RECV\n");
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
size_t TwoWire::_read(uint8_t address,uint8_t *data, uint16_t length,uint8_t sendStop)
{
    int ret = 0;
    _start();
    ret = _sendByte(address | 0x01); // clr read bit
    if(ret != I2C_ERROR_OK)
        ret = 0;
    
    ret = length;
    while (length--)
    {
        *data= _receiveByte();
        data++;
        if (length == 0)
        {
            _sendNack();
            _stop();
            break;
        }
        _sendAck();
    }
    return ret;
}
//---------------------------------------------------------------------------


/**
  * @brief 发送一个START信号.SCL为高电平时，SDA由高电平向低电平跳变，开始传送数据
  * @param 无.
  * @return 无.
  */
void TwoWire::_start(void)
{   
    _sda->set();
    _scl->set();          // SCL高
    delay_us(_bitDelay);
    _sda->reset();        // SDA拉低
    delay_us(_bitDelay);
}

/**
  * @brief 发送一个STOP信号. SCL为高电平时，SDA由低电平向高电平跳变，结束传送数据
  * @param 无.
  * @return 无.
  */
void TwoWire::_stop(void)
{   
    _sda->reset();      // SDA低电平
    _scl->set();        // SCL拉高并保持
    delay_us(_bitDelay);
    _sda->set();        // SDA拉高
    delay_us(_bitDelay);
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
i2c_err_t TwoWire::_waitAck()
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
        else if(cnt >= 2)
        {
            ret = I2C_ERROR_TIMEOUT;
            I2C_DEBUG("I2C_ERROR_TIMEOUT(at:%d)\n",_err_at );
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
i2c_err_t TwoWire::_sendByte( uint8_t c )
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
i2c_err_t TwoWire::_sendByte_first( uint8_t c)
{
    uint8_t ii = 8;
    uint8_t cnt = 0;
    _scl->reset();        //SCL拉低
    delay_us(_bitDelay);
    tag:
    while( ii-- )
    {
        _sda->write(c & 0x80);   // SCL低电平时将数据送到SDA
        delay_us(_bitDelay);
        _scl->set();                // 产生一个时钟脉冲
        delay_us(_bitDelay);
        if((_scl->read() == 0))
        {
            cnt++;
            ii++;
            if(cnt < 254)
                goto tag;
            else
            {
                ii--;
                cnt = 0;
            }
        }
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
i2c_err_t	TwoWire::_send7bitsAddress(uint8_t slaveAddr, uint8_t WR)
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
uint8_t TwoWire::_receiveByte()
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

    return byte;
}

/**
 * @brief 发送一个ACK应答. 第9个clock期间，拉低SDA
 * @param 无.
 * @return 0.
 */
int8_t TwoWire::_sendAck()
{
    //    _sda->mode(OUTPUT_PP);
    _sda->reset();          // 拉低SDA，
    delay_us(_bitDelay);
    _scl->set();            // 给出一个时钟
    delay_us(_bitDelay);
    _scl->reset();
    delay_us(_bitDelay);
    return EOK;
}

/**
  * @brief 发送一个NACK应答,第9个clock期间，拉高SDA
  * @param 无
  * @return 0.
  */
int8_t TwoWire::_sendNack()
{
    _sda->set();
    delay_us(_bitDelay);
    _scl->set();
    delay_us(_bitDelay);
    _scl->reset();
    delay_us(_bitDelay);
    return EOK;
}


