/**
  ******************************************************************************
  * @file    w25x16.cpp
  * @author  shentq
  * @version V2.2
  * @date    2016/08/14
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


/* Includes ------------------------------------------------------------------*/
#include "ATT7022.h"
int Att7022::begin()
{
    uint16_t temp;
    int ret = 0;
    if(initialized == 0)
    {
        cfg.dev_num = cs->id;
        cfg.mode = Spi::MODE1;
        cfg.bit_order = Spi::MSB;
        cfg.prescaler = Spi::DIV4;
        cs->mode(OUTPUT_PP);
        cs->set();
        rst->mode(OUTPUT_PP);
        rst->set();
        spi->begin(&cfg); 
        
        //芯片初始化
        reset();
        para_init();
        ClearAdjustRegister();
        WriteAdjustRegister(0x3f,ParaDlg.AdcGainRegister); 	
        ConfigAdjustRegister(); 
        
        initialized = 1;
    }
    return ret;
}
void Att7022::reset()
{
    rst->reset();
    delay_ms(1);
    rst->set();
    delay_ms(1);

}

uint32_t Att7022::read_id()
{
    uint8_t buf[3];
    read(0,buf);
    return (buf[0]<<16) + (buf[1]<<8) + (buf[2]<<0);
}
void Att7022::para_init(void)
{
    ParaDlg.AdcGainRegister[0] = 0x46 ;
    ParaDlg.AdcGainRegister[1] = 0x55 ;
    ParaDlg.AdcGainRegister[2] = 0x00 ;
        
    ParaDlg.IStartupAdjust[0][0] = 0x0 ;
    ParaDlg.IStartupAdjust[0][1] = 0x00 ;
    ParaDlg.IStartupAdjust[0][2] = 0x0 ;

        
    ParaDlg.VoltageAdjustRegister[0][0] = 0x6a ;
    ParaDlg.VoltageAdjustRegister[0][1] = 0x26 ;
    ParaDlg.VoltageAdjustRegister[0][2] = 0x19 ;
        

        
    ParaDlg.VoltageAdjustRegister[1][0] = 0x67 ;
    ParaDlg.VoltageAdjustRegister[1][1] = 0X5A ;
    ParaDlg.VoltageAdjustRegister[1][2] = 0X9C ;
        
    ParaDlg.VoltageAdjustRegister[2][0] = 0x64 ;
    ParaDlg.VoltageAdjustRegister[2][1] = 0X85 ;
    ParaDlg.VoltageAdjustRegister[2][2] = 0XE7 ;
        
    ParaDlg.CurrentAdjustRegister[0][0] = 0x84 ;
    ParaDlg.CurrentAdjustRegister[0][1] = 0x68 ;
    ParaDlg.CurrentAdjustRegister[0][2] = 0xdb ;

        
    ParaDlg.CurrentAdjustRegister[1][0] = 0x0B ;
    ParaDlg.CurrentAdjustRegister[1][1] = 0XFD ;
    ParaDlg.CurrentAdjustRegister[1][2] = 0XC7 ;
        
    ParaDlg.CurrentAdjustRegister[2][0] = 0x0C ;
    ParaDlg.CurrentAdjustRegister[2][1] = 0X8A ;
    ParaDlg.CurrentAdjustRegister[2][2] = 0X6A ;

    ParaDlg.PowerAdjustRegister[0][0] = 0xCB ;
    ParaDlg.PowerAdjustRegister[0][1] = 0X10 ;
    ParaDlg.PowerAdjustRegister[0][2] = 0X06 ;

    ParaDlg.PowerAdjustRegister[1][0] = 0x1D ;
    ParaDlg.PowerAdjustRegister[1][1] = 0X2A ;
    ParaDlg.PowerAdjustRegister[1][2] = 0X2E ;

    ParaDlg.PowerAdjustRegister[2][0] = 0x1B ;
    ParaDlg.PowerAdjustRegister[2][1] = 0XDE ;
    ParaDlg.PowerAdjustRegister[2][2] = 0X38 ;

}

//===================================================
// 清除校表寄存器参数
void Att7022::ClearAdjustRegister(void)
{
    unsigned char cmdbuf[3] = {0,0,0};
	write(0xC3,cmdbuf); 
	delay_ms(200);
}
//===================================================
// 初始化校表寄存器
uint8_t  Att7022::ConfigAdjustRegister(void)
{
    //电压校表参数
	WriteAdjustRegister(0x1B+APhase,ParaDlg.VoltageAdjustRegister[APhase]);	//写到校表寄存器 
	WriteAdjustRegister(0x1B+BPhase,ParaDlg.VoltageAdjustRegister[BPhase]);	//写到校表寄存器 
	WriteAdjustRegister(0x1B+CPhase,ParaDlg.VoltageAdjustRegister[CPhase]);	//写到校表寄存器 
	//电流校表参数
	WriteAdjustRegister(0x26+APhase,ParaDlg.CurrentAdjustRegister[APhase]);	//写到校表寄存器 
	WriteAdjustRegister(0x26+BPhase,ParaDlg.CurrentAdjustRegister[BPhase]);	//写到校表寄存器 
	WriteAdjustRegister(0x26+CPhase,ParaDlg.CurrentAdjustRegister[CPhase]);	//写到校表寄存器
	//功率校表参数
	WriteAdjustRegister(0x06+APhase,ParaDlg.PowerAdjustRegister[APhase]);	//写到校表寄存器 
	WriteAdjustRegister(0x06+BPhase,ParaDlg.PowerAdjustRegister[BPhase]);	//写到校表寄存器 
	WriteAdjustRegister(0x06+CPhase,ParaDlg.PowerAdjustRegister[CPhase]);	//写到校表寄存器
	
	WriteAdjustRegister(0x09+APhase,ParaDlg.PowerAdjustRegister[APhase]);	//写到校表寄存器 
	WriteAdjustRegister(0x09+BPhase,ParaDlg.PowerAdjustRegister[BPhase]);	//写到校表寄存器 
	WriteAdjustRegister(0x09+CPhase,ParaDlg.PowerAdjustRegister[CPhase]);	//写到校表寄存器


	return 0;
}


//===================================================
//读计量寄存器参数
long Att7022::ReadSampleRegister(unsigned char cmd)
{
    long datatemp;
	unsigned char cmdbuf[3]={0,0,0xaa};
    write(0xC6,cmdbuf); 
    delay_us(50);
	
	cmdbuf[2] = 0x00 ;
    read(cmd,cmdbuf);
    datatemp= (cmdbuf[0]<<16) + (cmdbuf[1]<<8) + (cmdbuf[2]<<0);
    return datatemp;
}
//====================================================================
//	 写校表寄存器
void Att7022::WriteAdjustRegister(unsigned char addr,unsigned char *tmpbuf)
{
	unsigned char cmdbuf[3] = {0,0,0} ;
	unsigned char soubuf[3] ;
	
	soubuf[0] = tmpbuf[0];
	soubuf[1] = tmpbuf[1];
	soubuf[2] = tmpbuf[2];
	
	write(0xC9,cmdbuf);  //使能较表寄存器写操作
	delay_ms(5);
  	write(addr,soubuf);  //写数据
	delay_ms(5);
	cmdbuf[2] = 1 ;
  	write(0xC9,cmdbuf);  //关闭较表寄存器写操作，防止误修改
	delay_ms(20);
}
//  读校表寄存器
void Att7022::ReadAdjustRegister(unsigned char addr,unsigned char *tmpbuf)
{
    unsigned char cmdbuf[3] = {0,0,0x5a} ;
	write(0xC6,cmdbuf);  //使能较表寄存器读操作
  	delay_ms(10);
	read(addr,tmpbuf);   //读数据
	delay_ms(100);
}
//===============================================================
// 电压校正函数
// PhaseType : 相电压类型
// StandValue: 标准电压值
// AdjustBuff: 保存校表数据
//===============================================================
void Att7022::AdjustVoltageFrun(float StandValue,unsigned char PhaseType)
{
    unsigned char    tmpBuff[3]={0x00,0x00,0x00};
	unsigned char    MesureAddr;
	unsigned char    AdjustBuff[3];
	float RealVol ;  //真实电压值
    float Ugain;
    long  Uadjust;

	RealVol = ReadSampleRegister(0x0D+PhaseType)/8192.00;  //测量输入电压有效值
	Ugain=(StandValue/RealVol)-1; 
	if(Ugain>0)
       	Ugain=Ugain*8388608;
    else
    	Ugain=Ugain*8388608+16777216; 
	
	Uadjust=(long)Ugain;          //得到校表寄存器值
	DToBm(Uadjust,AdjustBuff,3);  //转换为数组
    WriteAdjustRegister(0x1b+PhaseType,tmpBuff);  //清空校表存储器
	WriteAdjustRegister(0x1B+PhaseType,AdjustBuff);	//写到校表寄存器 
	ReadAdjustRegister(0x1B+PhaseType,ParaDlg.VoltageAdjustRegister[PhaseType]);
}
//===============================================================
// 电流校正函数
// PhaseType : 相电流类型
// StandValue: 标准电流值
// AdjustBuff: 保存校表数据
//===============================================================
void Att7022::AdjustCurrentFrun(float StandValue,unsigned char PhaseType)
{
	unsigned char    tmpBuff[3]={0x00,0x00,0x00};
	unsigned char    MesureAddr;
	unsigned char    AdjustBuff[3];
	float RealCur ;  //真实电流值
    float Ugain;
    long  Uadjust;

	RealCur = (ReadSampleRegister(0x10+PhaseType)&0x7fffff)/8192.00;  //测量输入电压有效值
	Ugain=(StandValue/RealCur)-1; 
	if(Ugain>=0)
       	Ugain=Ugain*8388608;
    else
    	Ugain=Ugain*8388608+16777216; 
	
	Uadjust=(long)Ugain;          //得到校表寄存器值
	DToBm(Uadjust,AdjustBuff,3);  //转换为数组
	WriteAdjustRegister(0x26+PhaseType,tmpBuff);  //清空校表存储器
    WriteAdjustRegister(0x26+PhaseType,AdjustBuff);	//写到校表寄存器
	ReadAdjustRegister(0x26+PhaseType,ParaDlg.CurrentAdjustRegister[PhaseType]);
}
// 校正功率增益
void Att7022::AdjustPowerGain(float StandValue , unsigned char PhaseType)
{
    unsigned char    tmpBuff[3]={0x00,0x00,0x00};
	float RmsMesurePower ;  //真实测量值
	float ERR ,PgainX,Pgain;
	unsigned char    AdjustBuff[3];
	RmsMesurePower=(float)(BmToYm(ReadSampleRegister(0X01+PhaseType)))/256.0; 
	
	ERR=(RmsMesurePower-StandValue)/StandValue;   //计算测量误差
    PgainX=(0-ERR)/(1+ERR);
        
    if(PgainX>=0)
        Pgain=PgainX*8388608;                    //计算较表寄存器参数
    else
        Pgain=16777216+PgainX*8388608;
		
	DToBm(Pgain,AdjustBuff,3);            //将较表数据转换成3字节补码形式
    WriteAdjustRegister(0x06+PhaseType,tmpBuff);  //清空校表存储器
	WriteAdjustRegister(0X06+PhaseType,AdjustBuff); //写到校表寄存器
	WriteAdjustRegister(0X09+PhaseType,AdjustBuff); //写到校表寄存器
	ReadAdjustRegister(0x06+PhaseType,ParaDlg.PowerAdjustRegister[PhaseType]);
}
//读取A/B/C相电压
//Phase:相
//0-->A;1->B;2->C
float Att7022::ReadURms(uint8_t Phase)//电压有效值
{
  float value ;
	
  value = ReadSampleRegister(0x0D+Phase)/8192.0;  	   
	return value;
}
float Att7022::ReadIRms(uint8_t Phase)//电流有效值
{
  float value ;
	value = ((ReadSampleRegister(0x10+Phase)&0x7fffff)/8192.0);  
	return value;
}
float Att7022::ReadPF(uint8_t Phase)//相功率因数
{
	float value ;
	value = (float)(BmToYm(ReadSampleRegister(0x14+Phase)))/8388608;   
  return value;
}
float Att7022::ReadPG(uint8_t Phase)//相电压电流相位角
{
	float value;
	value = ReadSampleRegister(0x18+Phase)/8388608.0*360/3.1416;
	return value;
}
float Att7022::ReadP(uint8_t Phase)//有功功率
{
	float value;
	value = (float)(BmToYm(ReadSampleRegister(0x01+Phase)))/256;
	return value;

}
float Att7022::ReadQ(uint8_t Phase)//无功功率
{
	float value;
	value = (float)(BmToYm(ReadSampleRegister(0x05+Phase)))/256;
	return value;
}
float Att7022::ReadS(uint8_t Phase)//视在功率
{
	float value;
	value = (float)(BmToYm(ReadSampleRegister(0x09+Phase)))/256;
	return value;
}
float Att7022::ReadEP(uint8_t Phase)//电能
{
	float value;
	value = (float)(BmToYm(ReadSampleRegister(0x1E+Phase)));
	return value;
}
float Att7022::ReadEPKwh(uint8_t Phase)//电能
{
	float value;
	value = (double)(BmToYm(ReadSampleRegister(0x1E+Phase)))/(double)3200.0;
	return value;
}
































void Att7022::read(uint8_t cmd,uint8_t *buf)
{
    spi->take(&cfg);
    cs->reset();
    spi->write(cmd);
    delay_us(5);
    spi->read_buf(buf,3);
    cs->set();
    spi->release();
}
void Att7022::write(uint8_t cmd,uint8_t *buf)
{
    spi->take(&cfg);
    cs->reset();
    spi->write(cmd);
    delay_us(5);
    spi->write_buf(buf,3);
    cs->set();
    spi->release();
}


//===============原码转换成补码==============================
void DToBm(long value,unsigned char *buff,unsigned char num)
{
    if(value>0)
  	{
  		if(num==3)
  			value=value&0x007fffff;
  		if(num==2)
  			value=value&0x00007fff;
  		if(num==1)
  			value=value&0x0000007f;
  	}
  	else if(value<=0)
  	{
   		if(num==3)
   			value=value|0x800000;
   		if(num==2)
   			value=value|0x8000;
   		if(num==1)
   			value=value|0x80;
  	}
  
  	*buff    =(unsigned char)(value>>16);
  	*(buff+1)=(unsigned char)(value>>8);
  	*(buff+2)=(unsigned char)value;
}
//==================补码转换成原码========================
long BmToYm(long value)
{
    if((value|0xff7fffff)==0xff7fffff);
    else if((value&0x00800000)==0x00800000)
        value=(~value+1)&0x007fffff;
    
	return value;   
}
