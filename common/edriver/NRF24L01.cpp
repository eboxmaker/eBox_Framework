#include "NRF24L01.h"
#include "ebox_exti.h"
uint8_t tx_addr[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //���͵�ַ
uint8_t rx_addr_0[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //���͵�ַ
uint8_t rx_addr_1[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //���͵�ַ

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
    write_reg(NRF_WRITE_REG + RF_CH, frq & 0xef); //����RFͨ��Ϊ40
}
/*
����chͨ����״̬��0���رո�ͨ������������ͨ��
����nrf����ͨ��״̬�����ch>5�򷵻�0xff��
*/
void NRF24L01::set_chanal(uint8_t ch, uint8_t enable) //ʹ��ͨ��ch�Ľ��յ�ַ
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
����chͨ����״̬��0���رո�ͨ������������ͨ���Զ�Ӧ��
����nrf����ͨ��״̬�����ch>5�򷵻�0xff��
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
    write_reg(NRF_WRITE_REG + EN_AA, sta); //ʹ��ͨ��0���Զ�Ӧ��
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
        write_buf(NRF_WRITE_REG + RX_ADDR_P0 + ch, addr, this->rx_aw);//дRX�ڵ��ַ
    else if(2 <= ch && ch < 6)
    {
        tmp_addr[0] = rx_addr_1[0];
        tmp_addr[1] = rx_addr_1[1];
        tmp_addr[2] = rx_addr_1[2];
        tmp_addr[3] = rx_addr_1[3];
        tmp_addr[4] = addr[4];
        write_buf(NRF_WRITE_REG + RX_ADDR_P0 + ch, tmp_addr, this->rx_aw);//дRX�ڵ��ַ
    }

}
void NRF24L01::set_destination_addr(uint8_t *addr)
{
    write_buf(NRF_WRITE_REG + TX_ADDR, addr, 5);//дRX�ڵ��ַ
}
void NRF24L01::set_pload_width(uint8_t ch, uint8_t width)
{
    if(ch < 6 && width < 33)
        write_reg(NRF_WRITE_REG + RX_PW_P0 + ch, width); //ѡ��ͨ��0����Ч���ݿ��
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
    write_reg(NRF_WRITE_REG + RF_SETUP, sta); //����TX�������,0db����,2Mbps,���������濪��
}
void NRF24L01::set_baudrate(bool baudrate)
{
    uint8_t sta;
    sta = read_reg(RF_SETUP);
    sta &= 0xf7;
    sta |=  baudrate << 3;
    write_reg(NRF_WRITE_REG + RF_SETUP, sta); //����TX�������,0db����,2Mbps,���������濪��
}

void NRF24L01::set_tx_rx_mode(bool mode)
{
    uint8_t sta;
    sta = read_reg(CONFIG);
    sta &= 0xfe;
    sta |= mode << 0;
    write_reg(NRF_WRITE_REG + CONFIG, sta); //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
}

void NRF24L01::set_power(bool pwr)
{
    uint8_t sta;
    sta = read_reg(CONFIG);
    sta &= 0xfd;
    sta |= pwr << 1;
    write_reg(NRF_WRITE_REG + CONFIG, sta); //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ

}

void NRF24L01::set_crc(bool crco, bool enable)
{
    uint8_t sta;
    sta = read_reg(CONFIG);
    sta &= 0xf3;
    sta |= crco   << 2;
    sta |= enable << 3;
    write_reg(NRF_WRITE_REG + CONFIG, sta); //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
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
    // write_buf(NRF_WRITE_REG+TX_ADDR,tx_addr,TX_ADR_WIDTH);    //дTX�ڵ��ַ



    //    set_addr_width(5);
    //    set_destination_addr(tx_addr);
    //    set_local_addr(0,rx_addr_0);//дRX�ڵ��ַ
    //   //write_reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
    //    set_retry(10);
    //    set_retry_gap(1);
    //   	set_chanal_ack(0,ENABLE);    //ʹ��ͨ��0���Զ�Ӧ��
    //  	set_chanal(0,ENABLE);//ʹ��ͨ��0�Ľ��յ�ַ
    //
    //  	set_rf_frq(40);	     //����RFͨ��Ƶ��
    //
    //  	set_pload_width(0,4);//ѡ��ͨ��0����Ч���ݿ��


    //  	set_gain(RF_N_0DB);//����TX�������,0db����,���������濪��
    //    set_baudrate(_2MBPS);//2Mbps
    //
    //    set_power(1);//PWR_UP
    //    set_tx_rx_mode(0);//����ģʽ
    //    set_crc(1,1);//EN_CRC,16BIT_CRC

    write_buf(NRF_WRITE_REG + TX_ADDR, tx_addr, TX_ADR_WIDTH); //дTX�ڵ��ַ

    write_buf(NRF_WRITE_REG + RX_ADDR_P0, tx_addr, RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK

    write_reg(NRF_WRITE_REG + EN_AA, 0x01);  //ʹ��ͨ��0���Զ�Ӧ��

    write_reg(NRF_WRITE_REG + EN_RXADDR, 0x01); //ʹ��ͨ��0�Ľ��յ�ַ

    write_reg(NRF_WRITE_REG + SETUP_RETR, 0x1a); //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��

    write_reg(NRF_WRITE_REG + RF_CH, 40);    //����RFͨ��ΪCHANAL

    write_reg(NRF_WRITE_REG + RF_SETUP, 0x0f); //����TX�������,0db����,2Mbps,���������濪��

    write_reg(NRF_WRITE_REG + CONFIG, 0x0e); //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�

    ce->set();//CEΪ��,10us����������
    delay_us(10);
}

void NRF24L01::set_rx_mode()
{
    ce->reset();

    set_addr_width(5);
    set_local_addr(0, rx_addr_0); //дRX�ڵ��ַ

    set_chanal_ack(0, ENABLE);   //ʹ��ͨ��0���Զ�Ӧ��
    set_chanal(0, ENABLE); //ʹ��ͨ��0�Ľ��յ�ַ

    set_rf_frq(40);	     //����RFͨ��Ƶ��

    set_pload_width(0, 4); //ѡ��ͨ��0����Ч���ݿ��

    set_gain(RF_N_0DB);//����TX�������,0db����,���������濪��
    set_baudrate(_2MBPS);//2Mbps

    set_power(1);//PWR_UP
    set_tx_rx_mode(RX_MODE);//����ģʽ
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

