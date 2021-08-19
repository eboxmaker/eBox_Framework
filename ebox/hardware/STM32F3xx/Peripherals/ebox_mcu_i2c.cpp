#include "ebox_mcu_i2c.h"

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



mcuI2c::mcuI2c(I2C_TypeDef *I2Cx,Gpio *sclPin, Gpio *sdaPin)
{
    _scl = sclPin;
    _sda = sdaPin;
    
    _i2cx = I2Cx;
    setTimeout(100);
}

#include "stm32f3xx_ll_bus.h"
/**
  *@brief    ����i2cʱ�Ӻ���������speed����timing,Ĭ��80K
  *@param    speed:  ���� 10,100,200,300,400 �ֱ����10k��100k��200k,300k,400k
  *@retval   None
  */
void mcuI2c::begin(uint8_t address)
{    
  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

    LL_I2C_InitTypeDef I2C_InitStruct = {0};

    _scl->mode(AF_OD_PU,LL_GPIO_AF_4);
    _sda->mode(AF_OD_PU,LL_GPIO_AF_4);
    
    switch((uint32_t)_i2cx)//ֻ��ʹ���ڲ�ʱ�ӣ��������ʼ���Ͳ���ȷ
    {
        case I2C1_BASE:
            LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);break;
        #if (USE_I2C2 )
        case I2C2_BASE:
            LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);break;
        #endif
        #if (USE_I2C3 )
        case I2C3_BASE:
            LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);break;
        #endif
        default:
            mcuI2cDebugErr("i2cx :0x%08x",_i2cx);
            break;
    }

    /* Peripheral clock enable */
    rcc_clock_cmd((uint32_t)_i2cx,ENABLE);

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    /** I2C Initialization
    */
    LL_I2C_EnableAutoEndMode(_i2cx);
    LL_I2C_DisableOwnAddress2(_i2cx);
    LL_I2C_DisableGeneralCall(_i2cx);
    LL_I2C_EnableClockStretching(_i2cx);
    I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
    I2C_InitStruct.Timing = 0X2000090E;_speed = 1000000;
    I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
    I2C_InitStruct.DigitalFilter = 0;
    I2C_InitStruct.OwnAddress1 = 0;
    I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
    I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
    LL_I2C_Init(_i2cx, &I2C_InitStruct);
    LL_I2C_SetOwnAddress2(_i2cx, 0, LL_I2C_OWNADDRESS2_NOMASK);
    mcuI2cDebug("scl_pin: 0x%x ; sda_pin: 0x%x\n",_scl->id, _sda->id);
    mcuI2cDebug("speed:%dKhz;\n",_speed/1000);

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
 * @brief ����i2cͨ���ٶ�.
 *
 * @param[in] speed i2c�ٶȣ�����ֵΪ400K��300K��200K��100Kʱ��i2cͨ��Ƶ��Ϊ��ֵ.
 *
 * @return 0.
 */
void mcuI2c::setClock(Speed_t speed)
{
    uint32_t timing = 0x2000090e;
   switch (speed)
    {
    case K10:
        _speed = (10000);
         timing = 0X200009FE;
        break;
    case K100:
        _speed = (100000);
        timing = 0X2000090E;
        break;
    case K200:
        _speed = (200000);
        timing = 0X00000A17;
        break;
    case K300:
        _speed = (400000);
        timing = 0X00000212;
    case K400:
        _speed = (400000);
        timing = 0X0000020B;
        break;
    case K1000:
        _speed = (1000000);
        timing = 0X00000001;
        break;
    default:
        _speed = (200000);
    }
    LL_I2C_Disable(_i2cx);
    LL_I2C_ConfigFilters(_i2cx,LL_I2C_ANALOGFILTER_ENABLE,0);
    LL_I2C_SetTiming(_i2cx,timing);
    LL_I2C_Enable(_i2cx);
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
