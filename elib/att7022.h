#ifndef __ATT7022_H
#define __ATT7022_H

#include "ebox.h"
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
	unsigned char RmsVoltage[3][2];   //???????
	unsigned char RmsCurrent[3][2];   //???????
}STRVALUE;

typedef struct{
  unsigned char StandardVoltage[3][2];  //?????????
	unsigned char StandardCurrent[3][2];  //?????????
	unsigned char VoltageAdjustRegister[3][3];  //???????????
	unsigned char CurrentAdjustRegister[3][3];  //???????????
	unsigned char PowerAdjustRegister[3][3];    //?????????
	unsigned char AdcGainRegister[3];    //ADC?????
	unsigned char EnUAngleRegister[3];   //???????????
	
	unsigned char IStartupAdjust[1][3];   //???????????
	
}STRPARA;

typedef struct{
    unsigned char DataTime ;

}STRVARIA;
extern STRVALUE ValueDlg ;
extern STRPARA  ParaDlg ;

#define ATT_RST_PIN 16
#define ATT_CS_PIN 4
class ATT7022
{

	public:
		void begin()
		{
			pinMode(ATT_RST_PIN,OUTPUT);
			pinMode(ATT_CS_PIN,OUTPUT);
			SPI.begin();
			ResetATT7022();
			ParaInit();
			WriteAdjustRegister(0x3f,ParaDlg.AdcGainRegister); 	
			ConfigAdjustRegister(); 
		
		};
		void ResetATT7022()
		{
		   digitalWrite(16,0) ;
			delay_ms(20);
		   digitalWrite(16,1) ;
			delay_ms(20);
		}
		//===================================================
		void ClearAdjustRegister(void)
		{
			unsigned char cmdbuf[3] = {0,0,0};
//			SPIWrite(0xC3,cmdbuf); 
			digitalWrite(ATT_CS_PIN,0);
			SPI.transfer(0XC3);
			SPI.transfer(cmdbuf[0]);
			SPI.transfer(cmdbuf[1]);
			SPI.transfer(cmdbuf[2]);		
			digitalWrite(ATT_CS_PIN,1);
			delay_ms(200);
		}
		//===================================================
		// ????????
		unsigned char ConfigAdjustRegister(void)
		{
				//??????
			WriteAdjustRegister(0x1B+APhase,ParaDlg.VoltageAdjustRegister[APhase]);	//??????? 
			WriteAdjustRegister(0x1B+BPhase,ParaDlg.VoltageAdjustRegister[BPhase]);	//??????? 
			WriteAdjustRegister(0x1B+CPhase,ParaDlg.VoltageAdjustRegister[CPhase]);	//??????? 
			//??????
			WriteAdjustRegister(0x26+APhase,ParaDlg.CurrentAdjustRegister[APhase]);	//??????? 
			WriteAdjustRegister(0x26+BPhase,ParaDlg.CurrentAdjustRegister[BPhase]);	//??????? 
			WriteAdjustRegister(0x26+CPhase,ParaDlg.CurrentAdjustRegister[CPhase]);	//???????
			//??????
			WriteAdjustRegister(0x06+APhase,ParaDlg.PowerAdjustRegister[APhase]);	//??????? 
			WriteAdjustRegister(0x06+BPhase,ParaDlg.PowerAdjustRegister[BPhase]);	//??????? 
			WriteAdjustRegister(0x06+CPhase,ParaDlg.PowerAdjustRegister[CPhase]);	//???????
			
			WriteAdjustRegister(0x09+APhase,ParaDlg.PowerAdjustRegister[APhase]);	//??????? 
			WriteAdjustRegister(0x09+BPhase,ParaDlg.PowerAdjustRegister[BPhase]);	//??????? 
			WriteAdjustRegister(0x09+CPhase,ParaDlg.PowerAdjustRegister[CPhase]);	//???????


			return 0;
		}
		//===================================================
//????????
long ReadSampleRegister(unsigned char cmd)
{
    long datatemp;
	unsigned char cmdbuf[3]={0,0,0xaa};
//    SPIWrite(0xC6,cmdbuf); 
//    Delay_US(50);
			digitalWrite(ATT_CS_PIN,0);
			SPI.transfer(0xC6);
			SPI.transfer(cmdbuf[0]);
			SPI.transfer(cmdbuf[1]);
			SPI.transfer(cmdbuf[2]);		
			digitalWrite(ATT_CS_PIN,1);

	
	cmdbuf[2] = 0x00 ;
//    SPIRead(0x0D+APhase,cmdbuf);
//    datatemp=ArrayTogether(cmdbuf,3);
			digitalWrite(ATT_CS_PIN,0);
			SPI.transfer(0x0D+APhase);
			cmdbuf[0] = SPI.transfer(0XFF);
			cmdbuf[1] = SPI.transfer(0XFF);
			cmdbuf[2] = SPI.transfer(0XFF);
			digitalWrite(ATT_CS_PIN,1);
			datatemp=ArrayTogether(cmdbuf,3);
			
    return datatemp;
}
//====================================================================
//	 ??????
void WriteAdjustRegister(unsigned char addr,unsigned char *tmpbuf)
{
	unsigned char cmdbuf[3] = {0,0,0} ;
	unsigned char soubuf[3] ;
	
	soubuf[0] = tmpbuf[0];
	soubuf[1] = tmpbuf[1];
	soubuf[2] = tmpbuf[2];
	
//	SPIWrite(0xC9,cmdbuf);  //??????????
			digitalWrite(ATT_CS_PIN,0);
			SPI.transfer(0xC9);
			SPI.transfer(cmdbuf[0]);
			SPI.transfer(cmdbuf[1]);
			SPI.transfer(cmdbuf[2]);		
			digitalWrite(ATT_CS_PIN,1);

	delay_ms(5);
//  	SPIWrite(addr,soubuf);  //???
			digitalWrite(ATT_CS_PIN,0);
			SPI.transfer(addr);
			SPI.transfer(soubuf[0]);
			SPI.transfer(soubuf[1]);
			SPI.transfer(soubuf[2]);		
			digitalWrite(ATT_CS_PIN,1);

	delay_ms(5);
	cmdbuf[2] = 1 ;
//  	SPIWrite(0xC9,cmdbuf);  //??????????,?????
			digitalWrite(ATT_CS_PIN,0);
			SPI.transfer(0xC9);
			SPI.transfer(cmdbuf[0]);
			SPI.transfer(cmdbuf[1]);
			SPI.transfer(cmdbuf[2]);		
			digitalWrite(ATT_CS_PIN,1);

	delay_ms(20);
}
//  ??????
void ReadAdjustRegister(unsigned char addr,unsigned char *tmpbuf)
{
    unsigned char cmdbuf[3] = {0,0,0x5a} ;
//	SPIWrite(0xC6,cmdbuf);  //??????????
			digitalWrite(ATT_CS_PIN,0);
			SPI.transfer(0xC6);
			SPI.transfer(cmdbuf[0]);
			SPI.transfer(cmdbuf[1]);
			SPI.transfer(cmdbuf[2]);		
			digitalWrite(ATT_CS_PIN,1);

  	delay_ms(10);
//	SPIRead(addr,tmpbuf);   //???
			digitalWrite(ATT_CS_PIN,0);
			SPI.transfer(0x0D+APhase);
			tmpbuf[0] = SPI.transfer(0XFF);
			tmpbuf[1] = SPI.transfer(0XFF);
			tmpbuf[2] = SPI.transfer(0XFF);
			digitalWrite(ATT_CS_PIN,1);

	delay_ms(100);
}
void AdjustVoltageFrun(float StandValue,unsigned char PhaseType)
{
  unsigned char    tmpBuff[3]={0x00,0x00,0x00};
//	unsigned char    MesureAddr;
	unsigned char    AdjustBuff[3];
	float RealVol ;  //?????
    float Ugain;
    long  Uadjust;

	RealVol = ReadSampleRegister(0x0D+PhaseType)/8192.00;  //?????????
	Ugain=(StandValue/RealVol)-1; 
	if(Ugain>0)
       	Ugain=Ugain*8388608;
    else
    	Ugain=Ugain*8388608+16777216; 
	
	Uadjust=(long)Ugain;          //????????
	DToBm(Uadjust,AdjustBuff,3);  //?????
    WriteAdjustRegister(0x1b+PhaseType,tmpBuff);  //???????
	WriteAdjustRegister(0x1B+PhaseType,AdjustBuff);	//??????? 
	ReadAdjustRegister(0x1B+PhaseType,ParaDlg.VoltageAdjustRegister[PhaseType]);
}

void AdjustCurrentFrun(float StandValue,unsigned char PhaseType)
{
	unsigned char    tmpBuff[3]={0x00,0x00,0x00};
//	unsigned char    MesureAddr;
	unsigned char    AdjustBuff[3];
	float RealCur ;  //?????
    float Ugain;
    long  Uadjust;

	RealCur = (ReadSampleRegister(0x10+PhaseType)&0x7fffff)/8192.00;  //?????????
	Ugain=(StandValue/RealCur)-1; 
	if(Ugain>=0)
       	Ugain=Ugain*8388608;
    else
    	Ugain=Ugain*8388608+16777216; 
	
	Uadjust=(long)Ugain;          //????????
	DToBm(Uadjust,AdjustBuff,3);  //?????
	WriteAdjustRegister(0x26+PhaseType,tmpBuff);  //???????
    WriteAdjustRegister(0x26+PhaseType,AdjustBuff);	//???????
	ReadAdjustRegister(0x26+PhaseType,ParaDlg.CurrentAdjustRegister[PhaseType]);
}
void AdjustPowerGain(float StandValue , unsigned char PhaseType)
{
    unsigned char    tmpBuff[3]={0x00,0x00,0x00};
	float RmsMesurePower ;  //?????
	float ERR ,PgainX,Pgain;
	unsigned char    AdjustBuff[3];
	RmsMesurePower=(float)(BmToYm(ReadSampleRegister(0X01+PhaseType)))/256.0; 
	
	ERR=(RmsMesurePower-StandValue)/StandValue;   //??????
    PgainX=(0-ERR)/(1+ERR);
        
    if(PgainX>=0)
        Pgain=PgainX*8388608;                    //?????????
    else
        Pgain=16777216+PgainX*8388608;
		
	DToBm(Pgain,AdjustBuff,3);            //????????3??????
    WriteAdjustRegister(0x06+PhaseType,tmpBuff);  //???????
	WriteAdjustRegister(0X06+PhaseType,AdjustBuff); //???????
	WriteAdjustRegister(0X09+PhaseType,AdjustBuff); //???????
	ReadAdjustRegister(0x06+PhaseType,ParaDlg.PowerAdjustRegister[PhaseType]);
}
float ReadURms(u8 Phase)//?????
{
  float value ;
	
  value = ReadSampleRegister(0x0D+Phase)/8192.0;  	   
	return value;
}

	private:
		long ArrayTogether(unsigned char *temp,unsigned char len)
		{
			long retData;
			unsigned char i;

			retData=0;
			for(i=0;i<len;i++){
				retData=retData<<8;
				retData=retData+(unsigned long)temp[i];
			}
			return retData;
		}
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
		long BmToYm(long value)
		{
				if((value|0xff7fffff)==0xff7fffff);
				else if((value&0x00800000)==0x00800000)
						value=(~value+1)&0x007fffff;
				
			return value;   
		}
		void ParaInit(void)
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
		uint8_t _att_rst_pin;
};
extern ATT7022 ATT;
#endif
