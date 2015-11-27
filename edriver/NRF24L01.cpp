#include "NRF24L01.h"
#include "exti.h"
const u8 tx_addr[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const u8 rx_addr[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址

void exti_callback()
{
    uart1.printf("exti event");
}
NRF24L01::NRF24L01(GPIO *irq,GPIO *ce,GPIO *cs,SPI *spi)
{
    this->irq = irq;
    this->ce = ce;
    this->cs = cs;
    this->spi = spi;
}
void NRF24L01::begin(uint8_t dev_num)
{
    ce->mode(OUTPUT_PP);
    cs->mode(OUTPUT_PP);
    irq->mode(INPUT_PU);
  
    spi_dev_nrf24l01.dev_num  = dev_num;
    spi_dev_nrf24l01.mode     = SPI_MODE0;
    spi_dev_nrf24l01.bit_order = MSB_FIRST;
    spi_dev_nrf24l01.prescaler = SPI_CLOCK_DIV8;
    spi->take_spi_right(&spi_dev_nrf24l01);
    spi->begin(&spi_dev_nrf24l01);
    spi->release_spi_right();
    
    ce->reset();
    cs->set();
    
    
}
uint8_t NRF24L01::write_reg(uint8_t reg,uint8_t value)
{
    spi->take_spi_right(&spi_dev_nrf24l01);
    cs->reset();
    spi->write(reg);
    spi->write(value);
    cs->set();
    spi->release_spi_right();
    return 1;
}
uint8_t NRF24L01::read_reg(uint8_t reg)
{
    uint8_t ret;
    spi->take_spi_right(&spi_dev_nrf24l01);
    cs->reset();
    spi->write(reg);
    ret = spi->read();
    cs->set();
    spi->release_spi_right();
    return ret;
}
uint8_t NRF24L01::write_buf(uint8_t reg,uint8_t *buf,uint8_t length)
{
    spi->take_spi_right(&spi_dev_nrf24l01);
    cs->reset();
    spi->write(reg);
    spi->write(buf,length);
    cs->set();
    spi->release_spi_right();
    return 1;
}
uint8_t NRF24L01::read_buf(uint8_t reg,uint8_t *buf,uint8_t length)
{
    spi->take_spi_right(&spi_dev_nrf24l01);
    cs->reset();
    spi->write(reg);
    spi->read(buf,length);
    cs->set();
    spi->release_spi_right();
    return 1;
}
int NRF24L01::self_check()
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
    int ret;
    spi->take_spi_right(&spi_dev_nrf24l01);
    cs->reset();
    this->write_buf(NRF_WRITE_REG+TX_ADDR,buf,5);
    this->read_buf(TX_ADDR,buf,5);
    for(int i = 0; i < 5; i++){
        if(buf[i] != 0xa5){
            ret = -1;
            break;
        }
        else
            ret = 1;
    }
    cs->set();
    spi->release_spi_right();
    return ret;
}



void NRF24L01::set_tx_mode()
{
  //  uint8_t cfg;
    ce->reset();
    write_buf(NRF_WRITE_REG + TX_ADDR,(uint8_t *)tx_addr,TX_ADR_WIDTH);
    write_buf(NRF_WRITE_REG + RX_ADDR_P0,(uint8_t *)rx_addr,RX_ADR_WIDTH);
    

    
    
  	write_reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  	write_reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  	write_reg(NRF_WRITE_REG+SETUP_RETR,0x1A);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	write_reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
  	write_reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	write_reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	ce->set();//CE为高,10us后启动发送
    delay_us(10);
}

int NRF24L01::send(uint8_t *buf,uint8_t length)
{
   uint8_t status;
    ce->reset();
    write_buf(WR_TX_PLOAD,buf,TX_PLOAD_WIDTH);
    ce->set();
    while(irq->read() != 0 ){
    };
    status = read_reg(STATUS);
    write_reg(NRF_WRITE_REG + STATUS,status);
    uart1.printf("out\r\n",status);
    if(status&MAX_TX)
    {
        write_reg(FLUSH_TX,0Xff);
        return MAX_TX;
    }
    else if(status & TX_OK)
    {
        return TX_OK;
    }
    else
        return ERROR;
    
}

void NRF24L01::set_rx_mode()
{
    ce->reset();
    write_buf(NRF_WRITE_REG + RX_ADDR_P0,(uint8_t *)rx_addr,RX_ADR_WIDTH);//写RX节点地址
   	write_reg(NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    
  	write_reg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  	 
  	write_reg(NRF_WRITE_REG+RF_CH,40);	     //设置RF通信频率		  
  	write_reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  	write_reg(NRF_WRITE_REG+RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	write_reg(NRF_WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
    ce->set();
}
int  NRF24L01::recv(uint8_t *buf)
{
    uint8_t state;
    ce->set();
    while(irq->read() != 0);
    ce->reset();
    state = read_reg(STATUS);
    write_reg(NRF_WRITE_REG+STATUS,state);
    if(state & RX_OK)
    {
        read_buf(RD_RX_PLOAD,buf,RX_PLOAD_WIDTH);
        write_reg(FLUSH_RX,0XFF);
        return RX_OK;
    }
    return ERROR;
}
int NRF24L01::status(void)
{
return read_reg(STATUS);
}
int NRF24L01::read_test(void)
{
return read_reg(SETUP_AW);
}

