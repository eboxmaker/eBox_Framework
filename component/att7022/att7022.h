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
	unsigned char RmsVoltage[3][2];   //三相电压有效值
	unsigned char RmsCurrent[3][2];   //三相电流有效值
}StrValue_t;

typedef struct{
  unsigned char StandardVoltage[3][2];  //三相标准电压输入值
	unsigned char StandardCurrent[3][2];  //三相标准电流输入值
	unsigned char VoltageAdjustRegister[3][3];  //三相电压校表寄存器参数
	unsigned char CurrentAdjustRegister[3][3];  //三相电流校表寄存器参数
	unsigned char PowerAdjustRegister[3][3];    //功率增益校表寄存器
	unsigned char AdcGainRegister[3];    //ADC增益寄存器
	unsigned char EnUAngleRegister[3];   //电压夹角测量使能寄存器
	
	unsigned char IStartupAdjust[1][3];   //电压夹角测量使能寄存器
	
}StrPara_t;

typedef struct
{
	DataFloat_t urms;
	DataFloat_t irms;
	DataFloat_t p;
	DataFloat_t pf;//相功率因数
	DataFloat_t pg;//相电压电流相位角
	DataFloat_t pq;//无功功率
	DataFloat_t ps;//视在功率
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
    // 清除校表寄存器参数
    void ClearAdjustRegister(void);
    // 初始化校表寄存器
    uint8_t ConfigAdjustRegister(void);
    //读计量寄存器参数
    long ReadSampleRegister(unsigned char cmd);
    //	 写校表寄存器
    void WriteAdjustRegister(unsigned char addr,unsigned char *tmpbuf);
    //  读校表寄存器
    void ReadAdjustRegister(unsigned char addr,unsigned char *tmpbuf);
    // 电压校正函数
    void AdjustVoltageFrun(float StandValue,unsigned char PhaseType);
    // 电流校正函数
    void AdjustCurrentFrun(float StandValue,unsigned char PhaseType);
    // 校正功率增益
    void AdjustPowerGain(float StandValue , unsigned char PhaseType);
    //读取A/B/C相电压
    //Phase:相
    //0-->A;1->B;2->C
    float ReadURms(uint8_t Phase);//电压有效值
    float ReadIRms(uint8_t Phase);//电流有效值
    float ReadPF(uint8_t Phase);//相功率因数
    float ReadPG(uint8_t Phase);//相电压电流相位角
    float ReadP(uint8_t Phase);//有功功率
    float ReadQ(uint8_t Phase);//无功功率
    float ReadS(uint8_t Phase);//视在功率
    float ReadEP(uint8_t Phase);//电能
    float ReadEPKwh(uint8_t Phase);//电能
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


//===============原码转换成补码==============================
void DToBm(long value,unsigned char *buff,unsigned char num);
//==================补码转换成原码========================
long BmToYm(long value);

#endif
