#ifndef __NRF24L01_H
#define __NRF24L01_H
#include "ebox.h"

class NRF24L01
{
public:
    NRF24L01(Gpio *irq, Gpio *ce, Gpio *cs, Spi *spi);
    void    begin(uint8_t dev_num);
    int     self_check();

    void    set_tx_mode();
    void    set_rx_mode();

    int     send(uint8_t *buf, uint8_t length);
    int     recv(uint8_t *buf);
    int     status(void);
    int     read_test(void);


private:
    Gpio *irq;
    Gpio *ce;
    Gpio *cs;
    Spi *spi;
    SPI_CONFIG_TYPE spi_dev_nrf24l01;

    uint8_t rx_aw;

    uint8_t write_reg(uint8_t reg, uint8_t value);
    uint8_t read_reg(uint8_t reg);
    uint8_t write_buf(uint8_t reg, uint8_t *buf, uint8_t length);
    uint8_t read_buf (uint8_t reg, uint8_t *buf, uint8_t length);

    void    set_rf_frq(uint8_t frq);

    void    set_chanal(uint8_t ch, uint8_t enable);
    void    set_chanal_ack(uint8_t ch, uint8_t enable);

    void    set_addr_width(uint8_t aw);
    void    set_local_addr(uint8_t ch, uint8_t *addr);
    void    set_destination_addr(uint8_t *addr);

    void    set_pload_width(uint8_t ch, uint8_t width);

    void    set_retry_gap(uint8_t gap);
    void    set_retry(uint8_t times);

    void    set_gain(uint8_t rf_gain);
    void    set_baudrate(bool baudrate);

    void    set_power(bool pwr);
    void    set_tx_rx_mode(bool mode);

    void    set_crc(bool crco, bool enable);

};
//enum{
//    N18 = 0,N12,N6,N0
//}RF_GAIN;
#define RF_N_18DB 0
#define RF_N_12DB 1
#define RF_N_6DB  2
#define RF_N_0DB  3

#define _1MBPS 0
#define _2MBPS 1

#define TX_MODE 0
#define RX_MODE 1

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//NRF24L01�Ĵ�����������
#define NRF_READ_REG    0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define NRF_WRITE_REG   0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define NOP             0xFF  //�ղ���,����������״̬�Ĵ���	 
//SPI(NRF24L01)�Ĵ�����ַ
#define CONFIG          0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
//bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define SETUP_AW        0x03  //���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define SETUP_RETR      0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define RF_SETUP        0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define STATUS          0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
//bit5:���ݷ�������ж�;bit6:���������ж�;
#define MAX_TX  	0x10  //�ﵽ����ʹ����ж�
#define TX_OK   	0x20  //TX��������ж�
#define RX_OK   	0x40  //���յ������ж�

#define OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define CD              0x09  //�ز����Ĵ���,bit0,�ز����;
#define RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define FIFO_STATUS     0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
//bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//config�Ĵ�����λ����
//#define RESERVED        0x80
//#define MASK_RX_DR      0x40
//#define MASK_TX_DS      0x20
//#define MASK_MAX_RT     0x10
//#define EN_CRC          0x08
//#define CRCO            0x04
//#define PWR_UP          0x02
//#define PRIM_RX         0x01
//interrupt flag
//#define RX_DR_IT_FLAG   0X40
//#define TX_DS_IT_FLAG   0X40
//#define MAX_RT_IT_FLAG   0X40
//#define RX_DR_IT_FLAG   0X40
//#define RX_DR_IT_FLAG   0X40
//#define RX_DR_IT_FLAG   0X40
//#define TX_FULL_IT_FLAG   0X40

#define TX_ADR_WIDTH    5   //5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5   //5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  4  //20�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  4  //20�ֽڵ��û����ݿ��

#endif
