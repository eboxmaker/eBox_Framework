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
  
    spi_dev_nrf24l01.dev_num  = dev_num;
    spi_dev_nrf24l01.mode     = SPI_MODE0;
    spi_dev_nrf24l01.bit_order = MSB_FIRST;
    
    spi->take_spi_right(&spi_dev_nrf24l01);
    spi->begin(&spi_dev_nrf24l01);
    spi->release_spi_right();
    
    ce->reset();
    cs->set();
    
    EXTIx ex(irq,EXTI_Trigger_Falling);
    ex.begin();
    ex.attach_interrupt(exti_callback);
    ex.interrupt(ENABLE);
    
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
	u8 i;
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
    ce->set();
    write_buf(NRF_WRITE_REG + TX_ADDR,(uint8_t *)tx_addr,TX_ADR_WIDTH);
    write_buf(NRF_WRITE_REG + RX_ADDR_P0,(uint8_t *)rx_addr,RX_ADR_WIDTH);
    

    
    
  	write_reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
  	write_reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  	write_reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  	write_reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	write_reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
  	write_reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	ce->set();//CE为高,10us后启动发送

}

int NRF24L01::send(uint8_t *buf,uint8_t length)
{
   uint8_t status;
    write_buf(WR_TX_PLOAD,buf,TX_PLOAD_WIDTH);
    ce->reset();
    while(irq->read() != 0 );
    status = read_reg(STATUS);
    write_reg(NRF_WRITE_REG + STATUS,status);
    if(status&MAX_TX)
    {
        write_reg(FLUSH_TX,0Xff);
        return MAX_TX;
    }
    if(status & TX_OK)
    {
        return TX_OK;
    }
    return 0xff;
    
}

void NRF24L01::set_rx_mode()
{
}
int  NRF24L01::recv(uint8_t *buf)
{
}

union {
    struct {
        uint8_t PRIM_RX     :1;
        uint8_t PWR_UP      :1;
        uint8_t CRCO        :1;
        uint8_t EN_CRC      :1;
        uint8_t MASK_MAX_RT :1;
        uint8_t MASK_TX_DS  :1;
        uint8_t MASK_RX_DR  :1;
        uint8_t :1;
    }bit;
    uint8_t     byte; 
}cfg;

void    NRF24L01::set_config(uint8_t data)
{
  	write_reg(NRF_WRITE_REG+CONFIG,data);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
}
