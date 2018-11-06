#include "enc28j60.h"


static uint8_t enc28j60_bank; // �洢��Bank���, ENC28J60����Bank0~3��4���洢��, ��Ҫͨ��ECON1�Ĵ���ѡ��
static uint16_t enc28j60_next_pkt; // ��һ�����ݰ�ָ��, ��������ֲ�P43ͼ7-3


Enc28j60::Enc28j60(Spi *spi, Gpio *cs, Gpio *rst)
{
    this->cs = cs;
    this->rst = rst;
    this->spi = spi;

}
void Enc28j60::begin(uint8_t dev_num)
{
    config.dev_num = dev_num;
    config.mode = SPI_MODE0;
    config.prescaler = SPI_CLOCK_DIV2;
    config.bit_order = MSB_FIRST;

    spi->begin(&config);
    cs->mode(OUTPUT_PP);
    cs->set();
    rst->mode(OUTPUT_PP);
    rst->reset();
    delay_ms(10);
    rst->set();
    uint8_t mac[6] = {11, 22, 33, 44, 55, 66};
    init(mac);

}


void Enc28j60::begin_send(void)
{
    write_op(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
    if (read(EIR) & EIR_TXERIF)
    {
        set_bank(ECON1);
        write_op(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRTS);
    }

}
void Enc28j60::end_receive(void)
{
    // �ƶ����ջ�������ָ��
    write(ERXRDPTL, enc28j60_next_pkt);
    write(ERXRDPTH, enc28j60_next_pkt >> 8);

    // ���ݰ��ݼ�
    write_op(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
}
uint16_t Enc28j60::get_packet_length(void)
{
    uint16_t len = 0;
    uint16_t rxstat;

    // ���ý��ջ�������ָ��
    write(ERDPTL, enc28j60_next_pkt);
    write(ERDPTH, enc28j60_next_pkt >> 8);

    // ����һ������ָ��
    enc28j60_next_pkt = read_op(ENC28J60_READ_BUF_MEM, 0);
    enc28j60_next_pkt |= read_op(ENC28J60_READ_BUF_MEM, 0) << 8;

    // �����ĳ���
    len = read_op(ENC28J60_READ_BUF_MEM, 0);
    len |= read_op(ENC28J60_READ_BUF_MEM, 0) << 8;
    len -= 4; // ɾ��CRC����

    // ��ȡ����״̬
    rxstat = read_op(ENC28J60_READ_BUF_MEM, 0);
    rxstat |= read_op(ENC28J60_READ_BUF_MEM, 0) << 8;
    return len;
}
uint8_t Enc28j60::get_packet_num(void)
{
    return read(EPKTCNT);
}
void Enc28j60::init(uint8_t *mac_addr)
{
    write_op(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET); // ENC28J60�����λ
    while ((read(ESTAT) & ESTAT_CLKRDY) == 0);
    enc28j60_next_pkt = RXSTART_INIT; // ���ý��ջ�������ʼ��ַ, �ñ�������ÿ�ζ�ȡ������ʱ������һ�������׵�ַ

    // ���ý��ջ�������ʼָ��
    write(ERXSTL, RXSTART_INIT & 0xff);
    write(ERXSTH, RXSTART_INIT >> 8);

    // ���ý��ջ�������ָ��
    write(ERXRDPTL, RXSTART_INIT & 0xff);
    write(ERXRDPTH, RXSTART_INIT >> 8);

    // ���ý��ջ���������ָ��
    write(ERXNDL, RXSTOP_INIT & 0xff);
    write(ERXNDH, RXSTOP_INIT >> 8);

    // ���÷��ͻ�������ʼָ��
    write(ETXSTL, TXSTART_INIT & 0xff);
    write(ETXSTH, TXSTART_INIT >> 8);

    // ���÷��ͻ���������ָ��
    write(ETXNDL, TXSTOP_INIT & 0xff);
    write(ETXNDH, TXSTOP_INIT >> 8);

    // ʹ�ܵ�������, CRCУ��, ��ʽƥ���Զ�����
    write(ERXFCON, ERXFCON_UCEN | ERXFCON_CRCEN | ERXFCON_PMEN);
    write(EPMM0, 0x3f);
    write(EPMM1, 0x30);
    write(EPMCSL, 0xf9);
    write(EPMCSH, 0xf7);

    write(MACON1, MACON1_MARXEN | MACON1_TXPAUS | MACON1_RXPAUS); // ʹ��MAC����, ����MAC������ͣ����֡, �����յ���ͣ����֡ʱֹͣ����
    write(MACON2, 0x00); // �˳���λ״̬

    // ��0������ж�֡��60�ֽڳ�, ��׷��һ��CRC, ����CRCʹ��, ֡����У��ʹ��, MACȫ˫��ʹ��
    // ����ENC28J60��֧��802.3���Զ�Э�̻���, ���ԶԶ˵����翨��Ҫǿ������Ϊȫ˫��
    write_op(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN | MACON3_FULDPX);
    write(MAIPGL, 0x12);
    write(MAIPGH, 0x0c);
    write(MABBIPG, 0x15);

    // ���֡����
    write(MAMXFLL, MAX_FRAMELEN & 0xff);
    write(MAMXFLH, MAX_FRAMELEN >> 8);

    // д��MAC��ַ
    write(MAADR5, mac_addr[0]);
    write(MAADR4, mac_addr[1]);
    write(MAADR3, mac_addr[2]);
    write(MAADR2, mac_addr[3]);
    write(MAADR1, mac_addr[4]);
    write(MAADR0, mac_addr[5]);

    write_phy(PHCON1, PHCON1_PDPXMD); // ����PHYΪȫ˫��, LEDBΪ������
    write_phy(PHLCON, 0x0476); // LED״̬
    write_phy(PHCON2, PHCON2_HDLDIS); // ��˫���ػ���ֹ
    set_bank(ECON1); // ����BANK0
    write_op(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE | EIE_PKTIE | EIE_RXERIE); // ʹ���ж�: ȫ���ж�, �����ж�, ���մ����ж�
    write_op(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN); // �������
}
void Enc28j60::init_send(uint16_t len)
{
    while ((read(ECON1) & ECON1_TXRTS) != 0); // ��ѯ�����߼���λλ

    // ���÷��ͻ�������ʼ��ַ
    write(EWRPTL, TXSTART_INIT & 0xff);
    write(EWRPTH, TXSTART_INIT >> 8);

    // ���÷��ͻ�����������ַ ��ֵ��Ӧ�������ݰ�����
    write(ETXNDL, (TXSTART_INIT + len) & 0xff);
    write(ETXNDH, (TXSTART_INIT + len) >> 8);

    // ���Ϳ����ֽ�: �����ֽ�Ϊ0x00, ��ʾʹ��macon3����
    write_op(ENC28J60_WRITE_BUF_MEM, 0, 0x00);
}
uint8_t Enc28j60::read(uint8_t addr)
{
    set_bank(addr);
    return read_op(ENC28J60_READ_CTRL_REG, addr);
}
void Enc28j60::read_buf(uint8_t *p, uint16_t len)
{
    //    uint8_t first = 1;
    if (len == 0)
        return;
    cs->reset();
    spi->take_spi_right(&config);
    spi->transfer(ENC28J60_READ_BUF_MEM);
    while(len--)
    {
        *p++ = spi->transfer(0xff);
    }
    *p = '\0';
    cs->set();
    spi->release_spi_right();

    //    SPI1->CR1 |= SPI_CR1_SPE;
    //    SPI1->DR = ENC28J60_READ_BUF_MEM; // ͨ��SPI���Ͷ�ȡ����������
    //    while ((SPI1->SR & SPI_SR_TXE) == 0);
    //
    //    // ѭ����ȡ
    //    while (len--)
    //    {
    //        SPI1->DR = 0xff; // ������һ��Ҫ���͵�����
    //        while ((SPI1->SR & SPI_SR_RXNE) == 0); // �ȴ��������ݷ������
    //        data = SPI1->DR; // ���ձ�������
    //        if (first)
    //            first = 0; // ���Ե�һ������
    //        else
    //            *p++ = data;
    //        while ((SPI1->SR & SPI_SR_TXE) == 0);
    //    }
    //    while ((SPI1->SR & SPI_SR_RXNE) == 0);
    //    *p = SPI1->DR; // �������һ������
    //    while (SPI1->SR & SPI_SR_BSY);
    //    SPI1->CR1 &= ~SPI_CR1_SPE;
}
uint8_t Enc28j60::read_op(uint8_t op, uint8_t addr)
{
    uint8_t data;

    cs->reset();
    spi->take_spi_right(&config);
    spi->transfer(op | (addr & 0x1f));
    data = spi->transfer(0xff);
    if(addr & 0x80)// �����MAC��MII�Ĵ���, ��һ����ȡ���ֽ���Ч, ����Ϣ�����ڵ�ַ�����λ
        data = spi->transfer(0xff);
    cs->set();
    spi->release_spi_right();
    return data;


    //    SPI1->CR1 |= SPI_CR1_SPE; // ����SPI����, ͬʱ�Զ�����Ƭѡ�ź�CS
    //    SPI1->DR = op | (addr & 0x1f); // ������͵�ַ (1)
    //    while ((SPI1->SR & SPI_SR_TXE) == 0); // TXE��λʱ����(1)�պ÷���1λ, ������DR�з����´�Ҫ���͵�����
    //    SPI1->DR = 0xff; // �������� (2)
    //    while ((SPI1->SR & SPI_SR_RXNE) == 0); // �ȴ�����(1)�������
    //    data = SPI1->DR; // ���� (1)
    //
    //    while ((SPI1->SR & SPI_SR_TXE) == 0); // TXE��λʱ����(2)�պ÷���1λ
    //    if (addr & 0x80) // �����MAC��MII�Ĵ���, ��һ����ȡ���ֽ���Ч, ����Ϣ�����ڵ�ַ�����λ
    //        SPI1->DR = 0xff; // �������� (3), �ٴ�ͨ��SPI��ȡ����
    //    while ((SPI1->SR & SPI_SR_RXNE) == 0); // �ȴ�����(2)�������
    //    data = SPI1->DR; // ���� (2)
    //
    //    if (addr & 0x80)
    //    {
    //        while ((SPI1->SR & SPI_SR_RXNE) == 0); // �ȴ�����(3)�������
    //        data = SPI1->DR; // ���� (3)
    //    }
    //    while (SPI1->SR & SPI_SR_BSY);
    //    SPI1->CR1 &= ~SPI_CR1_SPE; // �ر�SPI����, ͬʱ�Զ�����Ƭѡ�ź�CS
    //    return data;
}
void Enc28j60::set_bank(uint8_t addr)
{
    addr &= 0x60;
    if (addr != enc28j60_bank)
    {
        write_op(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_BSEL1 | ECON1_BSEL0);
        write_op(ENC28J60_BIT_FIELD_SET, ECON1, addr >> 5);
        enc28j60_bank = addr;
    }
}
void Enc28j60::write(uint8_t addr, uint8_t data)
{

    set_bank(addr);
    write_op(ENC28J60_WRITE_CTRL_REG, addr, data);
}
void Enc28j60::write_buf(uint8_t *p, uint16_t len)
{
    cs->reset();
    spi->take_spi_right(&config);
    spi->transfer(ENC28J60_WRITE_BUF_MEM);

    while(len--)
    {
        spi->transfer(*p++);
    }
    cs->set();
    spi->release_spi_right();
}
void Enc28j60::write_op(uint8_t op, uint8_t addr, uint8_t data)
{
    uint8_t temp;
    cs->reset();
    spi->take_spi_right(&config);
    temp = op | (addr & ADDR_MASK);
    spi->transfer(temp);
    temp = data;
    spi->transfer(temp);
    cs->set();
    spi->release_spi_right();
}
void Enc28j60::write_phy(uint8_t addr, uint16_t data)
{
    write(MIREGADR, addr);
    write(MIWRL, data);
    write(MIWRH, data >> 8);
    while (read(MISTAT) & MISTAT_BUSY);
}




