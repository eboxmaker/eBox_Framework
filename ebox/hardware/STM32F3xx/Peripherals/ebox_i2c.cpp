#include "ebox_i2c.h"
#include "ebox.h"

#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define mcuTwoWireDebug 1
#endif

#if mcuTwoWireDebug
#define  mcuI2C_DEBUG(...) DBG("[mcuI2c]:"),DBG(__VA_ARGS__)
#else
#define  mcuI2C_DEBUG(...)
#endif

//#define _bitDelay 5

mcuI2c::mcuI2c(I2C_TypeDef *I2Cx,Gpio *sclPin, Gpio *sdaPin)
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

    _sda->mode(AF_OD);
    _scl->mode(AF_OD);  
    
    rcc_clock_cmd((uint32_t)_i2cx, ENABLE);

    /* I2C configuration */
    I2C_InitStructure.PeripheralMode = LL_I2C_MODE_I2C;
//    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//    I2C_InitStructure.I2C_OwnAddress1 = address;
//    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.OwnAddress1 = address;
    I2C_InitStructure.TypeAcknowledge = LL_I2C_ACK;
    I2C_InitStructure.OwnAddrSize = LL_I2C_ADDRESSING_MODE_7BIT;
    I2C_InitStructure.DigitalFilter = 0;
    I2C_InitStructure.AnalogFilter = 0;
    I2C_InitStructure.Timing = 100*1000;  
    LL_I2C_Enable(_i2cx);
    LL_I2C_Init(_i2cx, &I2C_InitStructure);
    
    mcuI2C_DEBUG("scl_pin: 0x%x ; sda_pin: 0x%x\n",_scl->id, _sda->id);
    mcuI2C_DEBUG("speed:%dKhz;\n",_speed/1000);

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
    default:
        _speed = (200000);
    }
    LL_I2C_SetTiming(_i2cx,_speed);
    mcuI2C_DEBUG("speed:%dKhz;\n",_speed/1000);
}

i2c_err_t mcuI2c::_write(const uint8_t *data, size_t quantity)
{
    i2c_err_t ret = I2C_ERROR_OK;

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
i2c_err_t mcuI2c::_write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop)
{
    i2c_err_t ret;
    uint32_t counter = 0;

    while(LL_I2C_IsActiveFlag_BUSY(_i2cx) == SET){
        counter++;
        if( counter == 25000 ) {//
            return I2C_ERROR_BUS;
        }
    }
    if(sendStop)
        LL_I2C_HandleTransfer(_i2cx, address,LL_I2C_ADDRSLAVE_7BIT, quantity,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE ); //LL_I2C_GENERATE_START_READ
    else
        LL_I2C_HandleTransfer(_i2cx, address,LL_I2C_ADDRSLAVE_7BIT, quantity,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_START_WRITE ); //LL_I2C_GENERATE_START_READ

    /* Loop until STOP flag is raised  */
    while(!LL_I2C_IsActiveFlag_STOP(_i2cx))
    {
        if(LL_I2C_IsActiveFlag_TXE(_i2cx))
        {
            /* Write data in Transmit Data register.
            TXIS flag is cleared by writing data in TXDR register */
            LL_I2C_TransmitData8(_i2cx, *data++);
        }
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
size_t mcuI2c::_read(uint8_t address,uint8_t *data, uint16_t length,uint8_t sendStop)
{
    uint32_t counter = 0;
    i2c_err_t err = I2C_ERROR_OK;
    int ret = 0;
    ret = length;
    
    if(sendStop)
        LL_I2C_HandleTransfer(_i2cx, address, LL_I2C_ADDRSLAVE_7BIT, length, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
    else
        LL_I2C_HandleTransfer(_i2cx, address, LL_I2C_ADDRSLAVE_7BIT, length, LL_I2C_MODE_RELOAD, LL_I2C_GENERATE_START_READ);

      /* Loop until STOP flag is raised  */
	while(!LL_I2C_IsActiveFlag_STOP(_i2cx))
	{
        if(LL_I2C_IsActiveFlag_RXNE(_i2cx))
            *data++ = LL_I2C_ReceiveData8(_i2cx);
        counter++;
        if( counter == 25000 ) {//
            return I2C_ERROR_TIMEOUT;
        }
	}
    return ret;
}
