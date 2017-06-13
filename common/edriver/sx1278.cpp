#include "sx1278.h"
#define LORA_DEBUG 1
#if LORA_DEBUG
#define LORA_DBG(...) DBG(__VA_ARGS__)
#else
#define  LORA_DBG(...)
#endif

#define XTAL_FREQ               26000000
#define FREQ_STEP               49.59106

void Lora::begin(uint8_t dev_num,uint8_t bw, uint8_t cr, uint8_t sf)
{
    spi_config.dev_num = dev_num;
    spi_config.mode = SPI_MODE0;
    spi_config.prescaler = SPI_CLOCK_DIV16;
    spi_config.bit_order = SPI_BITODER_MSB;
    
    cs->mode(OUTPUT_PP);
    rst_pin->mode(OUTPUT_PP);

    cs->set();
    rst_pin->reset();
    delay_ms(100);
    rst_pin->set();
    spi->begin(&spi_config);
    delay_ms(100);
    config(bw,cr,sf);
    delay_ms(100);
}
void Lora::config(uint8_t bw, uint8_t cr, uint8_t sf) {
    //1,复位引脚复位
    //2.延时100ms
    //3.进入sleepmode
    //4.启动外部晶振
    //5.进入lora模式
    //6.设置载波频率
    setMode(SX1278_SLEEP);
    LORA_DBG("SX1278_REG_OP_MODE:0x%x\n",readRegister(SX1278_REG_OP_MODE));
    setRegValue(SX1278_REG_OP_MODE, SX1278_LORA, 7, 7);//进入lora模式
    LORA_DBG("SX1278_REG_OP_MODE:0x%x\n",readRegister(SX1278_REG_OP_MODE));

    //6.设置载波频率
	setRFFrequency(433000000);
    LORA_DBG("SX1278_REG_FRF_MSB:0x%x\n",readRegister(SX1278_REG_FRF_MSB));
    LORA_DBG("SX1278_REG_FRF_MID:0x%x\n",readRegister(SX1278_REG_FRF_MID));
    LORA_DBG("SX1278_REG_FRF_LSB:0x%x\n",readRegister(SX1278_REG_FRF_LSB));

    //7。输出功率设置    
    setRegValue(SX1278_REG_PA_CONFIG, SX1278_PA_SELECT_BOOST | SX1278_MAX_POWER | SX1278_OUTPUT_POWER);
    LORA_DBG("SX1278_REG_PA_CONFIG:0x%x\n",readRegister(SX1278_REG_PA_CONFIG));

    //8。设置PA的过流保护（关闭），电流微调默认0x0b
    setRegValue(SX1278_REG_OCP, SX1278_OCP_OFF | SX1278_OCP_TRIM, 5, 0);
    LORA_DBG("SX1278_REG_OCP:0x%x\n",readRegister(SX1278_REG_OCP));


    //9.LNA 增益设置001，最大增益
    setRegValue(SX1278_REG_LNA, SX1278_LNA_GAIN_1 | SX1278_LNA_BOOST_HF_ON);

    if(sf == SX1278_SF_6) {
        setRegValue(SX1278_REG_MODEM_CONFIG_1, bw | cr | SX1278_HEADER_IMPL_MODE);//隐式报头模式
        setRegValue(SX1278_REG_MODEM_CONFIG_2, SX1278_SF_6 | SX1278_TX_MODE_SINGLE | SX1278_RX_CRC_MODE_ON | SX1278_RX_TIMEOUT_MSB);
        //lora模式下无效
//        setRegValue(SX1278_REG_DETECT_OPTIMIZE, SX1278_DETECT_OPTIMIZE_SF_6, 2, 0);
//        setRegValue(SX1278_REG_DETECTION_THRESHOLD, SX1278_DETECTION_THRESHOLD_SF_6);
    } else {
        setRegValue(SX1278_REG_MODEM_CONFIG_1, bw | cr | SX1278_HEADER_EXPL_MODE);//隐式报头模式
        setRegValue(SX1278_REG_MODEM_CONFIG_2, sf | SX1278_TX_MODE_SINGLE | SX1278_RX_CRC_MODE_ON | SX1278_RX_TIMEOUT_MSB);
        //lora模式下无效
//        setRegValue(SX1278_REG_DETECT_OPTIMIZE, SX1278_DETECT_OPTIMIZE_SF_7_12, 2, 0);
//        setRegValue(SX1278_REG_DETECTION_THRESHOLD, SX1278_DETECTION_THRESHOLD_SF_7_12);
    }

    setRegValue(SX1278_REG_SYMB_TIMEOUT_LSB, 0x0f);
    
    setRegValue(SX1278_REG_PREAMBLE_MSB, SX1278_PREAMBLE_LENGTH_MSB);//前导码长度高位
    setRegValue(SX1278_REG_PREAMBLE_LSB, SX1278_PREAMBLE_LENGTH_LSB);//前导码长度低位

    setMode(SX1278_STANDBY);
}

void Lora::enttry_tx() {
    LORA_DBG("MODE:0x%x\n",readRegister(SX1278_REG_OP_MODE));
  setMode(SX1278_STANDBY);
    LORA_DBG("MODE:0x%x\n",readRegister(SX1278_REG_OP_MODE));
  setRegValue(SX1278_REG_PA_DAC, SX1278_PA_BOOST_ON, 2, 0);
  setRegValue(SX1278_REG_HOP_PERIOD, SX1278_HOP_PERIOD_OFF);
  setRegValue(SX1278_REG_DIO_MAPPING_1, SX1278_DIO0_TX_DONE, 7, 6);
  clearIRQFlags();
  setRegValue(SX1278_REG_IRQ_FLAGS_MASK, SX1278_MASK_IRQ_FLAG_TX_DONE);
  state = TXREADY;
}

void Lora::write(uint8_t* pBuffer,uint8_t len)
{
  if(len > 255) {
    return ;
  }
  setRegValue(SX1278_REG_PAYLOAD_LENGTH, len);
  setRegValue(SX1278_REG_FIFO_TX_BASE_ADDR, SX1278_FIFO_TX_BASE_ADDR_MAX);
  setRegValue(SX1278_REG_FIFO_ADDR_PTR, SX1278_FIFO_TX_BASE_ADDR_MAX);

  writeRegisterBurst(SX1278_REG_FIFO, pBuffer, len);
  
  setMode(SX1278_TX);
  state = TXING;
}

void Lora::evnet_tc()
{
    state=TXREADY;
  clearIRQFlags();  
}

void Lora::enttry_rx() {
    u8 addr;
    setMode(SX1278_STANDBY);
    setRegValue(SX1278_REG_PA_DAC, SX1278_PA_BOOST_OFF);
    setRegValue(SX1278_REG_HOP_PERIOD, SX1278_HOP_PERIOD_MAX);
    setRegValue(SX1278_REG_DIO_MAPPING_1, SX1278_DIO0_RX_DONE | SX1278_DIO1_RX_TIMEOUT, 7, 5);
    setRegValue(SX1278_REG_IRQ_FLAGS_MASK, (SX1278_MASK_IRQ_FLAG_RX_TIMEOUT & SX1278_MASK_IRQ_FLAG_RX_DONE));
    clearIRQFlags();
    
    addr = getRegValue(SX1278_REG_FIFO_RX_BASE_ADDR,7,0);
    //setRegValue(SX1278_REG_FIFO_RX_BASE_ADDR, SX1278_FIFO_RX_BASE_ADDR_MAX);
    setRegValue(SX1278_REG_FIFO_ADDR_PTR, addr);
//    setRegValue(SX1278_REG_FIFO_RX_BASE_ADDR, SX1278_FIFO_RX_BASE_ADDR_MAX);
//    setRegValue(SX1278_REG_FIFO_ADDR_PTR, SX1278_FIFO_RX_BASE_ADDR_MAX);

    setMode(SX1278_RXCONTINUOUS); 
}
void Lora::read(uint8_t* p,uint8_t *len)
{
    uint8_t addr;
    uint8_t sf = getRegValue(SX1278_REG_MODEM_CONFIG_2, 7, 4) & B11110000;
    if(sf == SX1278_SF_6) {
        *len = 21;
    }
    else
    {
      *len = getRegValue(SX1278_REG_RX_NB_BYTES);
    }
//    LORA_DBG("sf:%x\n",sf);
    addr = getRegValue(SX1278_REG_FIFO_RX_CURRENT_ADDR,7,0);
    setRegValue(SX1278_REG_FIFO_ADDR_PTR,addr,7,0);
//    LORA_DBG("SX1278_REG_FIFO_RX_CURRENT_ADDR:0x%x\n",readRegister(SX1278_REG_FIFO_ADDR_PTR));

    
//    for(uint8_t i = 0; i < 8; i++) {
//      p->source[i] = readRegister(SX1278_REG_FIFO);
//    }
//    for(uint8_t i = 0; i < 8; i++) {
//      p->destination[i] = readRegister(SX1278_REG_FIFO);
//    }
    readRegisterBurst(SX1278_REG_FIFO, p,*len);


    //重新初始化FIFO地址
//    setRegValue(SX1278_REG_FIFO_RX_BASE_ADDR, SX1278_FIFO_RX_BASE_ADDR_MAX);
//    setRegValue(SX1278_REG_FIFO_ADDR_PTR, SX1278_FIFO_RX_BASE_ADDR_MAX);
    //清控中断标志位
    clearIRQFlags();
}

void Lora::setMode(uint8_t mode) {
  setRegValue(SX1278_REG_OP_MODE, mode, 2, 0);
}


uint8_t Lora::setRegValue(uint8_t reg, uint8_t value, uint8_t msb, uint8_t lsb) 
{
    uint8_t currentValue;
    uint8_t newValue;
    if((msb > 7) || (lsb > 7)) {
        return 0xFF;
    }
    currentValue = readRegister(reg);
    newValue = currentValue & ((0xff << (msb + 1)) | (0xff >> (8 - lsb)));
    writeRegister(reg, newValue | value);
    return 0;
}

uint8_t Lora::getRegValue(uint8_t reg, uint8_t msb, uint8_t lsb) {
    uint8_t rawValue;
    uint8_t maskedValue;
    if((msb > 7) || (lsb > 7)) {
        return 0xFF;
    }
    rawValue = readRegister(reg);
    maskedValue = rawValue & ((0xff << lsb) & (0xff >> (7 - msb)));
    return(maskedValue);
}

void Lora::clearIRQFlags(void) {
  writeRegister(SX1278_REG_IRQ_FLAGS, B11111111);
}

void Lora::readRegisterBurst(uint8_t reg, uint8_t* data, uint8_t numBytes){
  spi->take_spi_right(&spi_config);
  cs->reset();
  spi->transfer(reg | SX1278_READ);
  for(uint8_t i = 0; i< numBytes; i++) {
    data[i] = spi->transfer(reg);
  }
  cs->set();
  spi->release_spi_right();
}

void Lora::writeRegisterBurst(uint8_t reg, uint8_t* data, uint8_t numBytes) {
  spi->take_spi_right(&spi_config);
  cs->reset();
  spi->transfer(reg | SX1278_WRITE);
  for(uint8_t i = 0; i< numBytes; i++) {
    spi->transfer(data[i]);
  }
  cs->set();
  spi->release_spi_right();
}


uint8_t Lora::readRegister(uint8_t reg) {
    uint8_t inByte;
    spi->take_spi_right(&spi_config);
    cs->reset();
    spi->transfer(reg | SX1278_READ);
    inByte = spi->transfer(0xff);
    cs->set();
    spi->release_spi_right();
    return inByte;
}
void Lora::writeRegister(uint8_t reg, uint8_t data) {
    spi->take_spi_right(&spi_config);
    cs->reset();
    spi->transfer(reg | SX1278_WRITE);
    spi->transfer(data);
    cs->set();
    spi->release_spi_right();    
}

void Lora::setRFFrequency(uint32_t freq)
{
    freq = ( uint32_t )( ( double )freq / ( double )FREQ_STEP );
    setRegValue(SX1278_REG_FRF_MSB, ( freq >> 16 ) & 0xFF );
    setRegValue(SX1278_REG_FRF_MID, ( freq >> 8 ) & 0xFF );
    setRegValue(SX1278_REG_FRF_LSB, freq & 0xFF);
}
uint32_t Lora::getRFFrequency( void )
{
    uint8_t tempMSB,tempMID,tempLSB;
    uint32_t tempFrq;
    tempMSB = getRegValue( SX1278_REG_FRF_MSB);
    tempMID = getRegValue( SX1278_REG_FRF_MID);
    tempLSB = getRegValue( SX1278_REG_FRF_LSB);
    tempFrq = ( ( uint32_t )tempMSB << 16 ) | ( ( uint32_t )tempMID << 8 ) | ( ( uint32_t )tempLSB );
    tempFrq = ( uint32_t )( ( double )tempFrq * ( double )FREQ_STEP );

    return tempFrq;
}
