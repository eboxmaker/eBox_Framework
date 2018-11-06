#include "enc28j60.h"


static uint8_t enc28j60_bank; // 存储区Bank编号, ENC28J60具有Bank0~3共4个存储区, 需要通过ECON1寄存器选择
static uint16_t enc28j60_next_pkt; // 下一个数据包指针, 详见数据手册P43图7-3


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
    // 移动接收缓冲区读指针
    write(ERXRDPTL, enc28j60_next_pkt);
    write(ERXRDPTH, enc28j60_next_pkt >> 8);

    // 数据包递减
    write_op(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
}
uint16_t Enc28j60::get_packet_length(void)
{
    uint16_t len = 0;
    uint16_t rxstat;

    // 设置接收缓冲器读指针
    write(ERDPTL, enc28j60_next_pkt);
    write(ERDPTH, enc28j60_next_pkt >> 8);

    // 读下一个包的指针
    enc28j60_next_pkt = read_op(ENC28J60_READ_BUF_MEM, 0);
    enc28j60_next_pkt |= read_op(ENC28J60_READ_BUF_MEM, 0) << 8;

    // 读包的长度
    len = read_op(ENC28J60_READ_BUF_MEM, 0);
    len |= read_op(ENC28J60_READ_BUF_MEM, 0) << 8;
    len -= 4; // 删除CRC计数

    // 读取接收状态
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
    write_op(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET); // ENC28J60软件复位
    while ((read(ESTAT) & ESTAT_CLKRDY) == 0);
    enc28j60_next_pkt = RXSTART_INIT; // 设置接收缓冲区起始地址, 该变量用于每次读取缓冲区时保留下一个包的首地址

    // 设置接收缓冲区起始指针
    write(ERXSTL, RXSTART_INIT & 0xff);
    write(ERXSTH, RXSTART_INIT >> 8);

    // 设置接收缓冲区读指针
    write(ERXRDPTL, RXSTART_INIT & 0xff);
    write(ERXRDPTH, RXSTART_INIT >> 8);

    // 设置接收缓冲区结束指针
    write(ERXNDL, RXSTOP_INIT & 0xff);
    write(ERXNDH, RXSTOP_INIT >> 8);

    // 设置发送缓冲区起始指针
    write(ETXSTL, TXSTART_INIT & 0xff);
    write(ETXSTH, TXSTART_INIT >> 8);

    // 设置发送缓冲区结束指针
    write(ETXNDL, TXSTOP_INIT & 0xff);
    write(ETXNDH, TXSTOP_INIT >> 8);

    // 使能单播过滤, CRC校验, 格式匹配自动过滤
    write(ERXFCON, ERXFCON_UCEN | ERXFCON_CRCEN | ERXFCON_PMEN);
    write(EPMM0, 0x3f);
    write(EPMM1, 0x30);
    write(EPMCSL, 0xf9);
    write(EPMCSH, 0xf7);

    write(MACON1, MACON1_MARXEN | MACON1_TXPAUS | MACON1_RXPAUS); // 使能MAC接收, 允许MAC发送暂停控制帧, 当接收到暂停控制帧时停止发送
    write(MACON2, 0x00); // 退出复位状态

    // 用0填充所有短帧至60字节长, 并追加一个CRC, 发送CRC使能, 帧长度校验使能, MAC全双工使能
    // 由于ENC28J60不支持802.3的自动协商机制, 所以对端的网络卡需要强制设置为全双工
    write_op(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN | MACON3_FULDPX);
    write(MAIPGL, 0x12);
    write(MAIPGH, 0x0c);
    write(MABBIPG, 0x15);

    // 最大帧长度
    write(MAMXFLL, MAX_FRAMELEN & 0xff);
    write(MAMXFLH, MAX_FRAMELEN >> 8);

    // 写入MAC地址
    write(MAADR5, mac_addr[0]);
    write(MAADR4, mac_addr[1]);
    write(MAADR3, mac_addr[2]);
    write(MAADR2, mac_addr[3]);
    write(MAADR1, mac_addr[4]);
    write(MAADR0, mac_addr[5]);

    write_phy(PHCON1, PHCON1_PDPXMD); // 配置PHY为全双工, LEDB为拉电流
    write_phy(PHLCON, 0x0476); // LED状态
    write_phy(PHCON2, PHCON2_HDLDIS); // 半双工回环禁止
    set_bank(ECON1); // 返回BANK0
    write_op(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE | EIE_PKTIE | EIE_RXERIE); // 使能中断: 全局中断, 接收中断, 接收错误中断
    write_op(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN); // 允许接收
}
void Enc28j60::init_send(uint16_t len)
{
    while ((read(ECON1) & ECON1_TXRTS) != 0); // 查询发送逻辑复位位

    // 设置发送缓冲区起始地址
    write(EWRPTL, TXSTART_INIT & 0xff);
    write(EWRPTH, TXSTART_INIT >> 8);

    // 设置发送缓冲区结束地址 该值对应发送数据包长度
    write(ETXNDL, (TXSTART_INIT + len) & 0xff);
    write(ETXNDH, (TXSTART_INIT + len) >> 8);

    // 发送控制字节: 控制字节为0x00, 表示使用macon3设置
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
    //    SPI1->DR = ENC28J60_READ_BUF_MEM; // 通过SPI发送读取缓冲区命令
    //    while ((SPI1->SR & SPI_SR_TXE) == 0);
    //
    //    // 循环读取
    //    while (len--)
    //    {
    //        SPI1->DR = 0xff; // 送入下一次要发送的数据
    //        while ((SPI1->SR & SPI_SR_RXNE) == 0); // 等待本次数据发送完毕
    //        data = SPI1->DR; // 接收本次数据
    //        if (first)
    //            first = 0; // 忽略第一次数据
    //        else
    //            *p++ = data;
    //        while ((SPI1->SR & SPI_SR_TXE) == 0);
    //    }
    //    while ((SPI1->SR & SPI_SR_RXNE) == 0);
    //    *p = SPI1->DR; // 接收最后一次数据
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
    if(addr & 0x80)// 如果是MAC和MII寄存器, 第一个读取的字节无效, 该信息包含在地址的最高位
        data = spi->transfer(0xff);
    cs->set();
    spi->release_spi_right();
    return data;


    //    SPI1->CR1 |= SPI_CR1_SPE; // 启动SPI总线, 同时自动拉低片选信号CS
    //    SPI1->DR = op | (addr & 0x1f); // 操作码和地址 (1)
    //    while ((SPI1->SR & SPI_SR_TXE) == 0); // TXE置位时数据(1)刚好发完1位, 可以向DR中放入下次要发送的数据
    //    SPI1->DR = 0xff; // 送入数据 (2)
    //    while ((SPI1->SR & SPI_SR_RXNE) == 0); // 等待数据(1)发送完毕
    //    data = SPI1->DR; // 接收 (1)
    //
    //    while ((SPI1->SR & SPI_SR_TXE) == 0); // TXE置位时数据(2)刚好发完1位
    //    if (addr & 0x80) // 如果是MAC和MII寄存器, 第一个读取的字节无效, 该信息包含在地址的最高位
    //        SPI1->DR = 0xff; // 送入数据 (3), 再次通过SPI读取数据
    //    while ((SPI1->SR & SPI_SR_RXNE) == 0); // 等待数据(2)发送完毕
    //    data = SPI1->DR; // 接收 (2)
    //
    //    if (addr & 0x80)
    //    {
    //        while ((SPI1->SR & SPI_SR_RXNE) == 0); // 等待数据(3)发送完毕
    //        data = SPI1->DR; // 接收 (3)
    //    }
    //    while (SPI1->SR & SPI_SR_BSY);
    //    SPI1->CR1 &= ~SPI_CR1_SPE; // 关闭SPI总线, 同时自动拉高片选信号CS
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




