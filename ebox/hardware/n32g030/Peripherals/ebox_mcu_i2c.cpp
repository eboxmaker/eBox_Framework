#include "ebox_mcu_i2c.h"

#if 1
// 是否打印调试信息, 1打印,0不打印
#define EBOX_DEBUG_MCUI2C_ENABLE       true
#define EBOX_DEBUG_MCUI2C_ENABLE_ERR   true
#endif


#if EBOX_DEBUG_MCUI2C_ENABLE
#define mcuI2cDebug(...)  ebox_printf("[mcuI2C DBG]:%d: ",__LINE__),ebox_printf(__VA_ARGS__ )
#else
#define mcuI2cDebug(...)
#endif

#if EBOX_DEBUG_MCUI2C_ENABLE_ERR
#define mcuI2cDebugErr(fmt, ...)  ebox_printf("[mcuI2C err]:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define mcuI2cDebugErr(fmt, ...)
#endif

static __IO uint32_t I2CTimeout = 0;

//#define _bitDelay 5

mcuI2c::mcuI2c(I2C_Module *I2Cx,Gpio *sclPin, Gpio *sdaPin)
{
    _scl = sclPin;
    _sda = sdaPin;
    _i2cx = I2Cx;
    setTimeout(100);
}


/**
  *@brief    根据i2c时钟和设置速率speed计算timing,默认80K
  *@param    speed:  速率 10,100,200,300,400 分别代表10k，100k，200k,300k,400k
  *@retval   None
  */
void mcuI2c::begin(uint8_t address)
{    
    uint32_t rcc_src;

    _sda->mode(AF_OD_PU,GPIO_AF6_I2C1);
    _scl->mode(AF_OD_PU,GPIO_AF6_I2C1);  

    rcc_clock_cmd((uint32_t)_i2cx, ENABLE);
    /* I2C configuration */
    I2C_DeInit(_i2cx);
    I2C_InitStructure.BusMode = I2C_BUSMODE_I2C;
    I2C_InitStructure.FmDutyCycle = I2C_FMDUTYCYCLE_2;
    I2C_InitStructure.OwnAddr1 = address;
    I2C_InitStructure.AckEnable = I2C_ACKEN;
    I2C_InitStructure.AddrMode = I2C_ADDR_MODE_7BIT;
    setClock();
    mcuI2cDebug("scl_pin: 0x%x ; sda_pin: 0x%x\n",_scl->id, _sda->id);
}
void mcuI2c::begin(int address)
{
    begin((uint8_t)address);
}
void mcuI2c::begin()
{
    begin((uint8_t)0);
}
/**
 * @brief 设置i2c通信速度.
 *
 * @param[in] speed i2c速度，当其值为400K，300K，200K，100K时，i2c通信频率为该值.
 *
 * @return 0.
 */
void mcuI2c::setClock(Speed_t speed)
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
    case K1000:
        _speed = (1000000);
        break;
    default:
        _speed = (100000);
    }
    I2C_InitStructure.ClkSpeed = _speed;

    /* Apply I2C configuration after enabling it */
    I2C_Init(_i2cx, &I2C_InitStructure);
    /* I2C Peripheral Enable */
    I2C_Enable(_i2cx, ENABLE);
    
    mcuI2cDebug("speed:%dKhz;\n",_speed/1000);
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
    uint8_t* sendBufferPtr = (uint8_t*)data;
    I2CTimeout             = I2CT_LONG_TIMEOUT;
    while (I2C_GetFlag(I2C1, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0)
        {
            mcuI2cDebug("_write(): %d",I2C_ERROR_BUS);
            return I2C_ERROR_BUS;
        }
    };

    I2C_ConfigAck(I2C1, ENABLE);

    I2C_GenerateStart(I2C1, ENABLE);
    I2CTimeout = I2C_FLAG_TIMOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVT_MASTER_MODE_FLAG)) // EV5
    {
        if ((I2CTimeout--) == 0)
        {
            mcuI2cDebug("_write(): %d",I2C_ERROR_ADDR_NACK_NO_RECV);
            return I2C_ERROR_ADDR_NACK_NO_RECV;
        }
    };

    I2C_SendAddr7bit(I2C1, address, I2C_DIRECTION_SEND);
    I2CTimeout = I2C_FLAG_TIMOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVT_MASTER_TXMODE_FLAG)) // EV6
    {
        if ((I2CTimeout--) == 0)
        {
            mcuI2cDebug("_write(): %d",I2C_ERROR_ADDR_NACK_NO_RECV);
            return I2C_ERROR_ADDR_NACK_NO_RECV;
        }
    };

    // send data
    while (quantity-- > 0)
    {
        I2C_SendData(I2C1, *sendBufferPtr++);
        I2CTimeout = I2C_FLAG_TIMOUT;
        while (!I2C_CheckEvent(I2C1, I2C_EVT_MASTER_DATA_SENDING)) // EV8
        {
            if ((I2CTimeout--) == 0)
            {
                mcuI2cDebug("_write(): %d",I2C_ERROR_DATA_NACK_NO_RECV);
                return I2C_ERROR_DATA_NACK_NO_RECV;
            }
        };
    };

    I2CTimeout = I2C_FLAG_TIMOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVT_MASTER_DATA_SENDED)) // EV8-2
    {
        if ((I2CTimeout--) == 0)
            {
                mcuI2cDebug("_write(): %d",I2C_ERROR_BUS);
                return I2C_ERROR_BUS;
            }
    };
    // Delay_us(8);
    I2C_GenerateStop(I2C1, ENABLE);
    return I2C_ERROR_OK;

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
size_t mcuI2c::_read(uint8_t address,uint8_t *data, uint16_t length,uint8_t sendStop)
{
    uint8_t* recvBufferPtr = data;
    I2CTimeout             = I2CT_LONG_TIMEOUT;
    while (I2C_GetFlag(I2C1, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0)
            return 9;
    };

    I2C_ConfigAck(I2C1, ENABLE);

    // send start
    I2C_GenerateStart(I2C1, ENABLE);
    I2CTimeout = I2C_FLAG_TIMOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVT_MASTER_MODE_FLAG)) // EV5
    {
        if ((I2CTimeout--) == 0)
            return 10;
    };

    // send addr
    I2C_SendAddr7bit(I2C1, address, I2C_DIRECTION_RECV);
    I2CTimeout = I2C_FLAG_TIMOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVT_MASTER_RXMODE_FLAG)) // EV6
    {
        if ((I2CTimeout--) == 0)
            return 6;
    };

    // recv data
    while (length-- > 0)
    {
        I2CTimeout = I2CT_LONG_TIMEOUT;
        while (!I2C_CheckEvent(I2C1, I2C_EVT_MASTER_DATA_RECVD_FLAG)) // EV7
        {
           if ((I2CTimeout--) == 0)
             return 14;
        };
        if (length == 1)
        {
           I2C_ConfigAck(I2C1, DISABLE);
        }
        if (length == 0)
        {
           I2C_GenerateStop(I2C1, ENABLE);
        }
        *recvBufferPtr++ = I2C_RecvData(I2C1);

    };
    return 0;

    
}
