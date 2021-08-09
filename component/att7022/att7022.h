/**
  ******************************************************************************
  * @file    w25xxx.h
  * @author  shentq
  * @version V2.2
  * @date    2019/08/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ATT7022_H
#define __ATT7022_H

#include "ebox_core.h"
#include "spi.h"

#define APhase  0x00
#define BPhase  0x01
#define CPhase  0x02

#define URmsA	0x0D
#define URmsB	0x0E
#define URmsC	0x0F
#define IRmsA	0x10
#define IRmsB	0x11
#define IRmsC	0x12

typedef struct{
	unsigned char RmsVoltage[3][2];   //�����ѹ��Чֵ
	unsigned char RmsCurrent[3][2];   //���������Чֵ
}StrValue_t;

typedef struct{
  unsigned char StandardVoltage[3][2];  //�����׼��ѹ����ֵ
	unsigned char StandardCurrent[3][2];  //�����׼��������ֵ
	unsigned char VoltageAdjustRegister[3][3];  //�����ѹУ��Ĵ�������
	unsigned char CurrentAdjustRegister[3][3];  //�������У��Ĵ�������
	unsigned char PowerAdjustRegister[3][3];    //��������У��Ĵ���
	unsigned char AdcGainRegister[3];    //ADC����Ĵ���
	unsigned char EnUAngleRegister[3];   //��ѹ�нǲ���ʹ�ܼĴ���
	
	unsigned char IStartupAdjust[1][3];   //��ѹ�нǲ���ʹ�ܼĴ���
	
}StrPara_t;

typedef struct
{
	DataFloat_t urms;
	DataFloat_t irms;
	DataFloat_t p;
	DataFloat_t pf;//�๦������
	DataFloat_t pg;//���ѹ������λ��
	DataFloat_t pq;//�޹�����
	DataFloat_t ps;//���ڹ���
	DataFloat_t ep;//
	DataFloat_t epKwh;
}PhaseData_t;

class Att7022
{
public:
    Att7022(Gpio *_rst,Gpio *_cs, Spi *_spi)
    {
        this->rst  = _rst;
        this->cs  = _cs;
        this->spi = _spi;
    }
    virtual int begin();

    void        reset();
    uint32_t    read_id();

    void para_init(void);
    // ���У��Ĵ�������
    void ClearAdjustRegister(void);
    // ��ʼ��У��Ĵ���
    uint8_t ConfigAdjustRegister(void);
    //�������Ĵ�������
    long ReadSampleRegister(unsigned char cmd);
    //	 дУ��Ĵ���
    void WriteAdjustRegister(unsigned char addr,unsigned char *tmpbuf);
    //  ��У��Ĵ���
    void ReadAdjustRegister(unsigned char addr,unsigned char *tmpbuf);
    // ��ѹУ������
    void AdjustVoltageFrun(float StandValue,unsigned char PhaseType);
    // ����У������
    void AdjustCurrentFrun(float StandValue,unsigned char PhaseType);
    // У����������
    void AdjustPowerGain(float StandValue , unsigned char PhaseType);
    //��ȡA/B/C���ѹ
    //Phase:��
    //0-->A;1->B;2->C
    float ReadURms(uint8_t Phase);//��ѹ��Чֵ
    float ReadIRms(uint8_t Phase);//������Чֵ
    float ReadPF(uint8_t Phase);//�๦������
    float ReadPG(uint8_t Phase);//���ѹ������λ��
    float ReadP(uint8_t Phase);//�й�����
    float ReadQ(uint8_t Phase);//�޹�����
    float ReadS(uint8_t Phase);//���ڹ���
    float ReadEP(uint8_t Phase);//����
    float ReadEPKwh(uint8_t Phase);//����
private:
    Spi::Config_t cfg;
    Gpio        *rst;
    Gpio        *cs;
    Spi         *spi;
    bool        initialized;

    StrValue_t valueDlg ;
    StrPara_t  ParaDlg ;
    
    void    read(uint8_t cmd,uint8_t *buf);
    void    write(uint8_t cmd,uint8_t *buf);

};


//===============ԭ��ת���ɲ���==============================
void DToBm(long value,unsigned char *buff,unsigned char num);
//==================����ת����ԭ��========================
long BmToYm(long value);

#endif
