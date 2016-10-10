#include "NRF24L01.h"
#include "ebox_exti.h"
uint8_t tx_addr[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //发送地址
uint8_t rx_addr_0[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //发送地址
uint8_t rx_addr_1[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //发送地址

void exti_callback()
{
    uart1.printf("exti event");
}
NRF24L01::NRF24L01(Gpio *irq, Gpio *ce, Gpio *cs, Spi *spi)
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
    spi_dev_nrf24l01.bit_order = SPI_BITODER_MSB;
    spi_dev_nrf24l01.prescaler = SPI_CLOCK_DIV8;
    spi->take_spi_right(&spi_dev_nrf24l01);
    spi->begin(&spi_dev_nrf24l01);
    spi->release_spi_right();

    ce->reset();
    cs->set();


}
uint8_t NRF24L01::write_reg(uint8_t reg, uint8_t value)
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
uint8_t NRF24L01::write_buf(uint8_t reg, uint8_t *buf, uint8_t length)
{
    spi->take_spi_right(&spi_dev_nrf24l01);
    cs->reset();
    spi->write(reg);
    spi->write(buf, length);
    cs->set();
    spi->release_spi_right();
    return 1;
}
uint8_t NRF24L01::read_buf(uint8_t reg, uint8_t *buf, uint8_t length)
{
    spi->take_spi_right(&spi_dev_nrf24l01);
    cs->reset();
    spi->write(reg);
    spi->read(buf, length);
    cs->set();
    spi->release_spi_right();
    return 1;
}
void NRF24L01::set_rf_frq(uint8_t frq)
{
    write_reg(NRF_WRITE_REG + RF_CH, frq & 0xef); //设置RF通道为40
}
/*
设置ch通道的状态；0：关闭该通道，其他开启通道
返回nrf所有通道状态；如果ch>5则返回0xff；
*/
void NRF24L01::set_chanal(uint8_t ch, uint8_t enable) //使能通道ch的接收地址
{
    uint8_t sta;
    if(ch > 5)
        return ;
    sta = read_reg(EN_RXADDR);
    if(enable)
    {
        ch = 1 << ch;
        sta |= ch;
    }
    else
    {
        ch = 1 << ch;
        ch = ~ch;
        sta &= ch;
    }
    write_reg(NRF_WRITE_REG + EN_RXADDR, sta);
}
/*
设置ch通道的状态；0：关闭该通道，其他开启通道自动应答
返回nrf所有通道状态；如果ch>5则返回0xff；
*/

void NRF24L01::set_chanal_ack(uint8_t ch, uint8_t enable)
{
    uint8_t sta;
    if(ch > 5)
        return ;
    sta = read_reg(EN_RXADDR);
    if(enable)
    {
        ch = 1 << ch;
        sta |= ch;
    }
    else
    {
        ch = 1 << ch;
        ch = ~ch;
        sta &= ch;
    }
    write_reg(NRF_WRITE_REG + EN_AA, sta); //使能通道0的自动应答
}
void NRF24L01::set_addr_width(uint8_t aw)
{
    if( 3 < aw && aw < 6)
    {
        this->rx_aw  = aw;
        aw -= 2;
        write_reg(NRF_WRITE_REG + SETUP_AW, aw);
    }
}

void NRF24L01::set_local_addr(uint8_t ch, uint8_t *addr)
{
    uint8_t tmp_addr[5];
    if(0 < ch && ch < 2)
        write_buf(NRF_WRITE_REG + RX_ADDR_P0 + ch, addr, this->rx_aw);//写RX节点地址
    else if(2 <= ch && ch < 6)
    {
        tmp_addr[0] = rx_addr_1[0];
        tmp_addr[1] = rx_addr_1[1];
        tmp_addr[2] = rx_addr_1[2];
        tmp_addr[3] = rx_addr_1[3];
        tmp_addr[4] = addr[4];
        write_buf(NRF_WRITE_REG + RX_ADDR_P0 + ch, tmp_addr, this->rx_aw);//写RX节点地址
    }

}
void NRF24L01::set_destination_addr(uint8_t *addr)
{
    write_buf(NRF_WRITE_REG + TX_ADDR, addr, 5);//写RX节点地址
}
void NRF24L01::set_pload_width(uint8_t ch, uint8_t width)
{
    if(ch < 6 && width < 33)
        write_reg(NRF_WRITE_REG + RX_PW_P0 + ch, width); //选择通道0的有效数据宽度
}
void NRF24L01::set_retry(uint8_t times)
{
    uint8_t sta;
    sta = read_reg(SETUP_RETR);
    sta &= 0xf0;
    sta |= times & 0x0f;
    write_reg(NRF_WRITE_REG + SETUP_RETR, sta);
}

void NRF24L01::set_retry_gap(uint8_t gap)
{
    uint8_t sta;
    sta = read_reg(SETUP_RETR);
    sta &= 0x0f;
    sta |= gap & 0xf0;
    write_reg(NRF_WRITE_REG + SETUP_RETR, sta);
}

void NRF24L01::set_gain(uint8_t rf_gain)
{
    uint8_t sta;
    sta = read_reg(RF_SETUP);
    sta &= 0xf9;
    sta |=  (rf_gain << 1) + 1;
    write_reg(NRF_WRITE_REG + RF_SETUP, sta); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
}
void NRF24L01::set_baudrate(bool baudrate)
{
    uint8_t sta;
    sta = read_reg(RF_SETUP);
    sta &= 0xf7;
    sta |=  baudrate << 3;
    write_reg(NRF_WRITE_REG + RF_SETUP, sta); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
}

void NRF24L01::set_tx_rx_mode(bool mode)
{
    uint8_t sta;
    sta = read_reg(CONFIG);
    sta &= 0xfe;
    sta |= mode << 0;
    write_reg(NRF_WRITE_REG + CONFIG, sta); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
}

void NRF24L01::set_power(bool pwr)
{
    uint8_t sta;
    sta = read_reg(CONFIG);
    sta &= 0xfd;
    sta |= pwr << 1;
    write_reg(NRF_WRITE_REG + CONFIG, sta); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式

}

void NRF24L01::set_crc(bool crco, bool enable)
{
    uint8_t sta;
    sta = read_reg(CONFIG);
    sta &= 0xf3;
    sta |= crco   << 2;
    sta |= enable << 3;
    write_reg(NRF_WRITE_REG + CONFIG, sta); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
}

int NRF24L01::self_check()
{
    uint8_t buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
    int ret;
    spi->take_spi_right(&spi_dev_nrf24l01);
    cs->reset();
    this->write_buf(NRF_WRITE_REG + TX_ADDR, buf, 5);
    this->read_buf(TX_ADDR, buf, 5);
    for(int i = 0; i < 5; i++)
    {
        if(buf[i] != 0xa5)
        {
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
    // write_buf(NRF_WRITE_REG+TX_ADDR,tx_addr,TX_ADR_WIDTH);    //写TX节点地址



    //    set_addr_width(5);
    //    set_destination_addr(tx_addr);
    //    set_local_addr(0,rx_addr_0);//写RX节点地址
    //   //write_reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
    //    set_retry(10);
    //    set_retry_gap(1);
    //   	set_chanal_ack(0,ENABLE);    //使能通道0的自动应答
    //  	set_chanal(0,ENABLE);//使能通道0的接收地址
    //
    //  	set_rf_frq(40);	     //设置RF通信频率
    //
    //  	set_pload_width(0,4);//选择通道0的有效数据宽度


    //  	set_gain(RF_N_0DB);//设置TX发射参数,0db增益,低噪声增益开启
    //    set_baudrate(_2MBPS);//2Mbps
    //
    //    set_power(1);//PWR_UP
    //    set_tx_rx_mode(0);//接收模式
    //    set_crc(1,1);//EN_CRC,16BIT_CRC

    write_buf(NRF_WRITE_REG + TX_ADDR, tx_addr, TX_ADR_WIDTH); //写TX节点地址

    write_buf(NRF_WRITE_REG + RX_ADDR_P0, tx_addr, RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK

    write_reg(NRF_WRITE_REG + EN_AA, 0x01);  //使能通道0的自动应答

    write_reg(NRF_WRITE_REG + EN_RXADDR, 0x01); //使能通道0的接收地址

    write_reg(NRF_WRITE_REG + SETUP_RETR, 0x1a); //设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次

    write_reg(NRF_WRITE_REG + RF_CH, 40);    //设置RF通道为CHANAL

    write_reg(NRF_WRITE_REG + RF_SETUP, 0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启

    write_reg(NRF_WRITE_REG + CONFIG, 0x0e); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断

    ce->set();//CE为高,10us后启动发送
    delay_us(10);
}

void NRF24L01::set_rx_mode()
{
    ce->reset();

    set_addr_width(5);
    set_local_addr(0, rx_addr_0); //写RX节点地址

    set_chanal_ack(0, ENABLE);   //使能通道0的自动应答
    set_chanal(0, ENABLE); //使能通道0的接收地址

    set_rf_frq(40);	     //设置RF通信频率

    set_pload_width(0, 4); //选择通道0的有效数据宽度

    set_gain(RF_N_0DB);//设置TX发射参数,0db增益,低噪声增益开启
    set_baudrate(_2MBPS);//2Mbps

    set_power(1);//PWR_UP
    set_tx_rx_mode(RX_MODE);//接收模式
    set_crc(1, 1); //EN_CRC,16BIT_CRC

    ce->set();
}

int NRF24L01::send(uint8_t *buf, uint8_t length)
{
    uint8_t status;
    ce->reset();
    write_buf(WR_TX_PLOAD, buf, TX_PLOAD_WIDTH);
    ce->set();
    while(irq->read() != 0 )
    {
    };
    status = read_reg(STATUS);
    write_reg(NRF_WRITE_REG + STATUS, status);
    write_reg(FLUSH_TX, 0Xff);
    if(status & MAX_TX)
    {
        return MAX_TX;
    }
    else if(status & TX_OK)
    {
        return TX_OK;
    }
    else
        return ERROR;

}

int  NRF24L01::recv(uint8_t *buf)
{
    uint8_t state;
    ce->set();
    while(irq->read() != 0);
    ce->reset();
    state = read_reg(STATUS);
    write_reg(NRF_WRITE_REG + STATUS, state);
    if(state & RX_OK)
    {
        read_buf(RD_RX_PLOAD, buf, RX_PLOAD_WIDTH);
        write_reg(FLUSH_RX, 0XFF);
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

