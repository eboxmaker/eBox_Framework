#include "TwoWire.h"


#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 0
#endif

#if debug
#define  I2C_DEBUG(...) DBG("[SOFTI2C]  "),DBG(__VA_ARGS__)
#else
#define  I2C_DEBUG(...)
#endif


TwoWire::TwoWire(Gpio *sclPin, Gpio *sdaPin)
{
    _scl = sclPin;
    _sda = sdaPin;
    setTimeout(100);
    _bitTimeout = 10;
}


void TwoWire::begin()
{
    _sda->mode(OUTPUT_OD_PU);
    _scl->mode(OUTPUT_OD_PU);
    _sda->set();
    _scl->set();
    
    setClock(K100);

    I2C_DEBUG("scl pin id 0x%x state is %d , sda pin id 0x%x  state is %d \r\n", _scl->id, _scl->read(), _sda->id, _sda->read());
}


/**
  *@brief    ����i2cʱ�Ӻ���������speed����timing,Ĭ��80K
  *@param    speed:  ���� 10,100,200,300,400 �ֱ����10k��100k��200k,300k,400k
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
 * @brief ����i2cͨ���ٶ�.
 *
 * @param[in] speed i2c�ٶȣ�����ֵΪ400K��300K��200K��100Kʱ��i2cͨ��Ƶ��Ϊ��ֵ.
 *
 * @return 0.
 */
void TwoWire::setClock(Speed_t speed)
{
    switch(speed)
    {
    case K400:
    case K300:
    case K200:
        _bitDelay = 1;    // Լ200k
        break;
    case K100:
        _bitDelay = 3;    // Լ100k
        break;
    default:
        _bitDelay = 4;    // Լ80k
        break;
    }
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
    uint8_t ret = i2c_write(txAddress, txBuffer, txLength, sendStop);
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
    size_t read = i2c_read(address, rxBuffer, quantity, sendStop);
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




// Send a START Condition
//
int TwoWire::i2c_start(void)
{
    _sda->set();          // SDA��
    _scl->set();          // SCL��
    delay_us(_bitDelay);
    _sda->reset();        // SDA����
    delay_us(_bitDelay);
    _scl->reset();        //SCL����
    delay_us(_bitDelay);
    return EOK;
}

void TwoWire::i2c_stop(void)
{   
    _sda->reset();      // SDA�͵�ƽ
    _scl->set();        // SCL���߲�����
    delay_us(_bitDelay);
    _sda->set();        // SDA����
    delay_us(_bitDelay);
}



/**
 * @brief �ȴ�һ��ACKӦ��. ��9��clock������IC��ACK��SDA�ᱻ����
 * @param ��.
 * @return EOK,EWAIT.
 */
i2c_err_t TwoWire::i2c_waitAck()
{
    i2c_err_t ret;
    _sda->set();
    delay_us(_bitDelay);
    _scl->set();
    delay_us(_bitDelay);
    uint32_t last = millis();
    while(1)
    {
        if (!_sda->read())	//SDAΪ������豸����Ack������û����
        {
            ret = I2C_ERROR_OK;
            break;
        }
        else if(millis() - last > _bitTimeout)
        {
            ret = I2C_ERROR_TIMEOUT;
            I2C_DEBUG("I2C_ERROR_TIMEOUT\n");
            break;
        }
        delay_us(_bitDelay);
    }
    _scl->reset();
    delay_us(_bitDelay);
    
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
i2c_err_t TwoWire::i2c_write( uint8_t c )
{
    i2c_err_t ret;
    uint8_t ii = 8;
    _scl->reset(); //��ʱSCL�Ѿ��ǵ�
    while( ii-- )
    {
        _sda->write(c & 0x80);   // SCL�͵�ƽʱ�������͵�SDA
        delay_us(_bitDelay);
        _scl->set();                // ����һ��ʱ������
        delay_us(_bitDelay);
        c = c << 1;
        _scl->reset();
    }
    ret = i2c_waitAck();
    
    return ret;
}
i2c_err_t TwoWire::i2c_write(const uint8_t *data, size_t quantity)
{
    int i = 0;
    for( i = 0; i < quantity; i++) {
        i2c_write(data[i]);
        
    }
    return I2C_ERROR_OK;
}

/*
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 *          5 .. timeout
 */
i2c_err_t TwoWire::i2c_write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop)
{
    i2c_start();
    i2c_err_t ret = i2c_write(address & 0xfe); // clr read bit
    if(ret != I2C_ERROR_OK)
        ret = I2C_ERROR_ADDR_NACK_NO_RECV;
    if(quantity != 0)
    {
        ret = i2c_write(data, quantity);                
        if(ret != I2C_ERROR_OK)
            ret = I2C_ERROR_DATA_NACK_NO_RECV;
    }
    if(sendStop)
    {
        i2c_stop();
    }
    return ret;
}

// read a byte from the I2C slave device
//
uint8_t TwoWire::i2c_read()
{
    uint8_t i = 8;
    uint8_t byte = 0;
    _sda->set();
    while(i--)
    {
        byte += byte;
        _scl->reset();      // ʱ�Ӹߵ�ƽ�ڼ��SDA
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
  *@brief    ָ���Ĵ���������ȡ. start->WslaveAddr->regAddr->RSlaverAddr->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint16_t regAddr: �Ĵ�����ַ
  *          uint8_t *data: ��ȡ��������
  *          uint16_t nRead��Ҫ��ȡ�����ݳ���
  *          : ��ʱ
  *@retval   EOK��EWAIT
  */
size_t TwoWire::i2c_read(uint8_t address,uint8_t *data, uint16_t length,uint8_t sendStop)
{
    int ret = 0;
    ret = i2c_start();
    if(ret != I2C_ERROR_OK)
        ret = 0;

    ret = i2c_write(address | 0x01); // clr read bit
    if(ret != I2C_ERROR_OK)
        ret = 0;
    
    ret = length;
    while (length--)
    {
        *data= i2c_read();
        data++;
        if (length == 0)
        {
            i2c_sendNack();
            i2c_stop();
            break;
        }
        i2c_sendAck();
    }
    return ret;
}
/**
 * @brief ����һ��ACKӦ��. ��9��clock�ڼ䣬����SDA
 * @param ��.
 * @return 0.
 */
int8_t TwoWire::i2c_sendAck()
{
    //    _sda->mode(OUTPUT_PP);
    _sda->reset();          // ����SDA��
    delay_us(_bitDelay);
    _scl->set();            // ����һ��ʱ��
    delay_us(_bitDelay);
    _scl->reset();
    delay_us(_bitDelay);
    return EOK;
}

/**
  * @brief ����һ��NACKӦ��,��9��clock�ڼ䣬����SDA
  * @param ��
  * @return 0.
  */
int8_t TwoWire::i2c_sendNack()
{
    _sda->set();
    delay_us(_bitDelay);
    _scl->set();
    delay_us(_bitDelay);
    _scl->reset();
    delay_us(_bitDelay);
    return EOK;
}


