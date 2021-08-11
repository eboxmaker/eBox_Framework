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

#include "stm32f3xx_ll_bus.h"
/**
  *@brief    根据i2c时钟和设置速率speed计算timing,默认80K
  *@param    speed:  速率 10,100,200,300,400 分别代表10k，100k，200k,300k,400k
  *@retval   None
  */
void mcuI2c::begin(uint8_t address)
{    
  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB8   ------> I2C1_SCL
  PB9   ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  /** I2C Initialization
  */
  LL_I2C_EnableAutoEndMode(I2C1);
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_EnableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x2000090E;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
//    LL_I2C_SetTiming(_i2cx,timing);

//    setClock(K100);
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
//    LL_I2C_SetTiming(_i2cx,_speed);
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
    i2c_err_t ret = I2C_ERROR_OK;
    uint32_t counter = 0;
    uint8_t send_cnt = 0;
//    quantity =1;
    char buf[10] ;
    for(int i = 0; i < 10; i++)
    buf[i] = i;
    LL_I2C_ClearFlag_NACK(I2C1);

    while(LL_I2C_IsActiveFlag_BUSY(_i2cx) == SET){
        counter++;
        if( counter == 25000 ) {//
            LL_I2C_ClearFlag_BERR(_i2cx);
            return I2C_ERROR_BUS;
        }
    }
//    if(sendStop)
//        LL_I2C_HandleTransfer(_i2cx, address,LL_I2C_ADDRSLAVE_7BIT, 1,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE ); //LL_I2C_GENERATE_START_READ
//    else
//        LL_I2C_HandleTransfer(_i2cx, address,LL_I2C_ADDRSLAVE_7BIT, 1,LL_I2C_MODE_RELOAD,LL_I2C_GENERATE_START_WRITE ); //LL_I2C_GENERATE_START_READ

   /* 发送器件地址 */
    LL_I2C_HandleTransfer(I2C1, address, LL_I2C_ADDRSLAVE_7BIT, (quantity), LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
    if(quantity == 0) /*如果数据为空，则检测设备响应位 */
    {
        while(!LL_I2C_IsActiveFlag_STOP(I2C1))
        {
            ;
        }//等待发送完成
        LL_I2C_ClearFlag_STOP(I2C1);
        if(LL_I2C_IsActiveFlag_NACK(I2C1))//如果没有响应则表示没有找到设备
        {
            ret =  I2C_ERROR_ADDR_NACK_NO_RECV;
            LL_I2C_ClearFlag_NACK(I2C1);
        }
    }
    else
    {
        while(send_cnt < quantity)
        {
            counter = 0;
            while(!LL_I2C_IsActiveFlag_TXE(I2C1))
            {
                counter++;
                if( counter == 25000 ) {//发送数据超时
                    ret =  I2C_ERROR_BUS;
                    break;
                }
            }
            if(ret)
            {
                break;//如果出现错误停止发送;
            }
            else
            {
                LL_I2C_TransmitData8(I2C1, data[send_cnt++]);
            }
        }
        while(!LL_I2C_IsActiveFlag_STOP(I2C1))
        {
            ;
        }//等待发送完成
        LL_I2C_ClearFlag_STOP(I2C1);
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
    
    LL_I2C_ClearFlag_NACK(I2C1);

    while(LL_I2C_IsActiveFlag_BUSY(_i2cx) == SET){
        counter++;
        if( counter == 25000 ) {//
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
            *data++ = LL_I2C_ReceiveData8(_i2cx);
        counter++;
        if( counter == 25000 ) {//
            return I2C_ERROR_TIMEOUT;
        }
	}
    LL_I2C_ClearFlag_STOP(I2C1);
    return ret;
}
