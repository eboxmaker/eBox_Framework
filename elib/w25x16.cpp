/*
file   : w25x16.c
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "w25x16.h"
#include "softspi.h"


SPICONFIG spiDevW25x16;

void W25X::begin()
{
	spiDevW25x16.devNum = 1;
	spiDevW25x16.mode = SPI_MODE0;
	spiDevW25x16.prescaler = 0;
	spiDevW25x16.bitOrder = SPI_BITODER_MSB;
	
	sspi.begin(&spiDevW25x16);
	pMode(cs,_OPP);
	dgWrite(cs,HIGH);
}
void W25X::readId(uint16_t* id)
{
	if(spiDevW25x16.devNum != sspi.readConfig())
		sspi.config(&spiDevW25x16);

	dgWrite(cs,LOW);
	sspi.transfer(0x90);
	sspi.transfer(0x00);
	sspi.transfer(0x00);
	sspi.transfer(0x00);
	*id |= sspi.transfer(0xff)<<8;
	*id |= sspi.transfer(0xff);
	dgWrite(cs,HIGH);


}
/***************************************************************
函数名称 :  void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
功能描述 : 读取SPI FLASH， 在指定地址开始读取指定长度的数据
输入参数 : 
			pBuffer:数据存储区
			ReadAddr:开始读取的地址(24bit)
			NumByteToRead:要读取的字节数(最大65535)
输出参数 : 无
返回值   : 无
***************************************************************/
void W25X::read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;    												    
	dgWrite(cs,LOW);
	sspi.transfer(W25X_ReadData);         //发送读取命令   
	sspi.transfer((u8)((ReadAddr)>>16));  //发送24bit地址    
	sspi.transfer((u8)((ReadAddr)>>8));   
	sspi.transfer((u8)ReadAddr);   
//	sspi.transfer(0xff,pBuffer,NumByteToRead);
	for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=sspi.transfer(0XFF);   //循环读数  
    }
	dgWrite(cs,HIGH);
}  
/***************************************************************
函数名称 :  void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
功能描述 : 读取SPI FLASH， 在指定地址开始读取指定长度的数据
输入参数 : 
			pBuffer:数据存储区
			ReadAddr:开始读取的地址(24bit)
			NumByteToRead:要读取的字节数(最大65535)
输出参数 : 无
返回值   : 无
***************************************************************/
void W25X::fastRead(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;    												    
	dgWrite(cs,LOW);
	sspi.transfer(W25X_FastReadData);         //发送读取命令   
	sspi.transfer((u8)((ReadAddr)>>16));  //发送24bit地址    
	sspi.transfer((u8)((ReadAddr)>>8));   
	sspi.transfer((u8)ReadAddr);   
	sspi.transfer(0xff);   //空字节
//	sspi.transfer(0xff,pBuffer,NumByteToRead);
	for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=sspi.transfer(0XFF);   //循环读数  
    }
	dgWrite(cs,HIGH);
}  
/***************************************************************
函数名称 : void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)     
功能描述 : 写SPI FLASH，在指定地址开始写入指定长度的数据，该函数带擦除操作!
输入参数 : 
			pBuffer:数据存储区
			WriteAddr:开始写入的地址(24bit)
			NumByteToWrite:要写入的字节数(最大65535)  		   
输出参数 : 无
返回值   : 无
***************************************************************/
void W25X::write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	
	secpos=WriteAddr/4096;//扇区地址 0~511 for w25x16
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		read(SPI_FLASH_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			eraseSector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			writeNoCheck(SPI_FLASH_BUF,secpos*4096,4096);//写入整个扇区  

		}else writeNoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};	 	 
}


/***************************************************************
函数名称 : void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
功能描述 : SPI在一页(0~65535)内写入少于256个字节的数据， 在指定地址开始写入最大256字节的数据
输入参数 : 
			pBuffer:数据存储区
			WriteAddr:开始写入的地址(24bit)
			NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
输出参数 : 无
返回值   : 无
***************************************************************/
void W25X::writePage(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
	writeEnable();                  //SET WEL 
	dgWrite(cs,LOW);
	sspi.transfer(W25X_PageProgram);      //发送写页命令   
	sspi.transfer((u8)((WriteAddr)>>16)); //发送24bit地址    
	sspi.transfer((u8)((WriteAddr)>>8));   
	sspi.transfer((u8)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++)sspi.transfer(pBuffer[i]);//循环写数  
//	sspi.transfer(pBuffer,NumByteToWrite);
	dgWrite(cs,HIGH);
	_waitBusy();					   //等待写入结束
} 
/***************************************************************
函数名称 : void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
功能描述 :  无检验写SPI FLASH，必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!具有自动换页功能，
			在指定地址开始写入指定长度的数据,但是要确保地址不越界!
输入参数 : 
			pBuffer:数据存储区
			WriteAddr:开始写入的地址(24bit)
			NumByteToWrite:要写入的字节数(最大65535)
输出参数 : 无
返回值   : 无
***************************************************************/
void W25X::writeNoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		writePage(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 
/***************************************************************
函数名称 : void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)     
功能描述 : 擦除一个扇区，擦除一个山区的最少时间:150ms
输入参数 : Dst_Addr:扇区地址 0~511 for w25x16
输出参数 : 无
返回值   : 无
***************************************************************/
void W25X::eraseSector(u32 Dst_Addr)   
{   
	Dst_Addr*=4096;
	writeEnable();                  //SET WEL 	 
	_waitBusy();   
	dgWrite(cs,LOW);
	sspi.transfer(W25X_SectorErase);      //发送扇区擦除指令 
	sspi.transfer((u8)((Dst_Addr)>>16));  //发送24bit地址    
	sspi.transfer((u8)((Dst_Addr)>>8));   
	sspi.transfer((u8)Dst_Addr);  
	dgWrite(cs,HIGH);
	_waitBusy();   				   //等待擦除完成

} 
/***************************************************************
函数名称 : void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)     
功能描述 :  擦除整个芯片
			整片擦除时间:
			W25X16:25s 
			W25X32:40s 
			W25X64:40s 
输入参数 : 无
输出参数 : 无
返回值   : 无
***************************************************************/
void W25X::eraseChip(void)   
{                                             
	writeEnable();                  //SET WEL 
	_waitBusy();   
	dgWrite(cs,LOW);
	sspi.transfer(W25X_ChipErase); 	//发送片擦除命令  
	dgWrite(cs,HIGH);
	_waitBusy();   				   				//等待芯片擦除结束
}   





//等待空闲
void W25X::_waitBusy(void)   
{   
	while ((readSR()&0x01)==0x01);   // 等待BUSY位清空
}  
//进入掉电模式
void W25X::powerDown(void)   
{ 
	volatile int i;	 	
	dgWrite(cs,LOW);
  sspi.transfer(W25X_PowerDown);        //发送掉电命令  
    //等待TPD  
	for (i=0;i<300;i++);
	dgWrite(cs,HIGH);

}   
//唤醒
void W25X::wakeUp(void)   
{  
 	volatile int i;	 	
 	dgWrite(cs,LOW);
 sspi.transfer(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
     //等待TRES1
   	for (i=0;i<300;i++);
	dgWrite(cs,HIGH);
}  
/***************************************************************
函数名称 :  u8 SPI_Flash_ReadSR(void)   
功能描述 : 读取W25X16的状态寄存器
				BIT7  6   5   4   3   2   1   0
				SPR   RV  TB BP2 BP1 BP0 WEL BUSY
				说明：
				SPR:默认0,状态寄存器保护位,配合WP使用
				TB,BP2,BP1,BP0:FLASH区域写保护设置
				WEL:写使能锁定
				BUSY:忙标记位(1,忙;0,空闲)
				默认:0x00
输入参数 : 无
输出参数 : 无
返回值   : byte	寄存器状态
***************************************************************/
u8 W25X::readSR(void)   
{  
	u8 byte=0;   
	dgWrite(cs,LOW);
	sspi.transfer(W25X_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=sspi.transfer(0Xff);               
	dgWrite(cs,HIGH);
	return byte;   
} 

/***************************************************************
函数名称 :  void SPI_FLASH_Write_SR(u8 sr)   
功能描述 : 写SPI_FLASH状态寄存器， 只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
输入参数 : sr:状态
输出参数 : 无
返回值   : 无
***************************************************************/
void W25X::writeSR(u8 sr)   
{   
	dgWrite(cs,LOW);
	sspi.transfer(W25X_WriteStatusReg);   //发送写取状态寄存器命令    
	sspi.transfer(sr);               //写入一个字节  
	dgWrite(cs,HIGH);
}

   
/***************************************************************
函数名称 :  void SPI_FLASH_Write_Enable(void)   
功能描述 : SPI_FLASH写使能，将WEL置位
输入参数 : 无
输出参数 : 无
返回值   : 无
***************************************************************/
void W25X::writeEnable(void)   
{
	dgWrite(cs,LOW);
    sspi.transfer(W25X_WriteEnable);      //发送写使能  
	dgWrite(cs,HIGH);
} 
/***************************************************************
函数名称 :  void SPI_FLASH_Write_Disable(void)   
功能描述 : SPI_FLASH写禁止， 将WEL清零 
输入参数 : 无
输出参数 : 无
返回值   : 无
***************************************************************/
void W25X::writeDisable(void)   
{  
 	dgWrite(cs,LOW);
   sspi.transfer(W25X_WriteDisable);     //发送写禁止指令    
	dgWrite(cs,HIGH);
} 			    
