/*
file   : mmc_sd.cpp
*/

#include "mmc_sd.h"


int SD::begin(uint8_t dev_num)
{
	int ret = 0;
	SPIDevSDCard.dev_num = dev_num;
	SPIDevSDCard.mode = SPI_MODE0;
	SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
	SPIDevSDCard.bit_order = SPI_BITODER_MSB;
		
	cs->mode(OUTPUT_PP);
	cs->set();
	SD_Type = 0;
	
	ret = init();
	return ret;
}
/*******************************************************************************
* Function Name  : SD_WaitReady
* Description    : 等待SD卡Ready
* Input          : None
* Output         : None
* Return         : uint8_t 
*                   0： 成功
*                   other：失败
*******************************************************************************/

uint8_t SD::_wait(void)
{
  uint8_t r1 = 0;
  uint32_t retry=0;
  do
  {
    r1 = spi->read();
    retry++;
    if(retry>=0xfffffffe)
      return 1; 
  }while(r1==0); 
	return 0;
}
/*******************************************************************************
* Function Name  : SD_SendCommand
* Description    : 向SD卡发送一个命令
* Input          : uint8_t cmd   命令 
*                  uint32_t arg  命令参数
*                  uint8_t crc   crc校验值
* Output         : None
* Return         : uint8_t r1 SD卡返回的响应
*******************************************************************************/
uint8_t SD::_send_command(uint8_t cmd, uint32_t arg,uint8_t crc)
{
  unsigned char r1 = 0;
  unsigned int Retry = 0;

 
	cs->set();
	spi->write(0xff);//提高兼容性，如果没有这里，有些SD卡可能不支持
	cs->reset();//片选端置低，选中SD卡
	
  /* 发送命令序列 */
  spi->write(cmd | 0x40);                      
  spi->write((uint8_t)(arg >> 24));//参数[31..24]
  spi->write((uint8_t)(arg >> 16));//参数[23..16]
  spi->write((uint8_t)(arg >> 8));//参数[15..8]
  spi->write((uint8_t)arg);    //参数[7..0]
  spi->write(crc);
    
  //等待响应，或超时退出
  while((r1 = spi->read())==0xFF)
  {
    Retry++;
    if(Retry > 800)break; //根据实验测得，最好重试次数多点
  }   
  //关闭片选
	cs->set();
  //在总线上额外增加8个时钟，让SD卡完成剩下的工作
  spi->write(0xFF);

  //返回状态值
  return r1;
}
/*******************************************************************************
* Function Name  : SD_SendCommand_NoDeassert
* Description    : 向SD卡发送一个命令(结束是不失能片选，还有后续数据传来）
* Input          : uint8_t cmd   命令 
*                  uint32_t arg  命令参数
*                  uint8_t crc   crc校验值
* Output         : None
* Return         : uint8_t r1 SD卡返回的响应
*******************************************************************************/
uint8_t SD::_send_command_no_deassert(uint8_t cmd, uint32_t arg,uint8_t crc)
{
  unsigned char r1 = 0;
  unsigned int Retry = 0;
	

	cs->set();
	spi->write(0xff);//提高兼容性，如果没有这里，有些SD卡可能不支持
	cs->reset();//片选端置低，选中SD卡
  
  /* 发送命令序列 */
  spi->write(cmd | 0x40);                      
  spi->write((uint8_t)(arg >> 24));//参数[31..24]
  spi->write((uint8_t)(arg >> 16));//参数[23..16]
  spi->write((uint8_t)(arg >> 8));//参数[15..8]
  spi->write((uint8_t)arg);    //参数[7..0]
  spi->write(crc);
  
  //等待响应，或超时退出
  while((r1 = spi->read())==0xFF)
  {
    Retry++;
    if(Retry > 600)break; //根据实验测得，最好重试次数多点  
  }
  //返回响应值
  return r1;
}
/*******************************************************************************
* Function Name  : SD_Init
* Description    : 初始化SD卡
* Input          : None
* Output         : None
* Return         : uint8_t 
*                  0：NO_ERR
*                  1：TIME_OUT
*                  99：NO_CARD
*******************************************************************************/
uint8_t SD::init()
{
  uint16_t i = 0;      // 用来循环计数
  uint8_t r1 = 0;      // 存放SD卡的返回值
  uint16_t retry = 0;  // 用来进行超时计数
  uint8_t buff[6];
	
	SPIDevSDCard.prescaler = SPI_CLOCK_DIV256;
	spi->begin(&SPIDevSDCard);
	cs->reset();
	  // 纯延时，等待SD卡上电完成
  for(i=0;i<0xf00;i++);

  //先产生至少74个脉冲，让SD卡自己初始化完成
  for(i=0;i<10;i++)
  {
    spi->write(0xFF);//80clks
  }
	  //-----------------SD卡复位到idle开始-----------------
  //循环连续发送CMD0，直到SD卡返回0x01,进入IDLE状态
  //超时则直接退出
  retry = 0;
  do
  {
    //发送CMD0，让SD卡进入IDLE状态
    r1 = _send_command(CMD0, 0,0x95);
    retry++;
  }while((r1 != 0x01) && (retry<200));
  //跳出循环后，检查原因：初始化成功？or 重试超时？
  if(retry==200) return 1;   //超时返回1	
	
	
  //-----------------SD卡复位到idle结束-----------------	 
  //获取卡片的SD版本信息
  r1 = _send_command_no_deassert(CMD8, 0x1aa,0x87);	     
    //如果卡片版本信息是v1.0版本的，即r1=0x05，则进行以下初始化
  if(r1 == 0x05)
  {
     //设置卡类型为SDV1.0，如果后面检测到为MMC卡，再修改为MMC
     SD_Type = SD_TYPE_V1;	   
     //如果是V1.0卡，CMD8指令后没有后续数据
     //片选置高，结束本次命令
     cs->set();
     //多发8个CLK，让SD结束后续操作
     spi->write(0xFF);	  
     //-----------------SD卡、MMC卡初始化开始-----------------	 
     //发卡初始化指令CMD55+ACMD41
     // 如果有应答，说明是SD卡，且初始化完成
     // 没有回应，说明是MMC卡，额外进行相应初始化
     retry = 0;
     do
     {
       //先发CMD55，应返回0x01；否则出错
       r1 = _send_command(CMD55, 0, 0);
       if(r1 != 0x01)
         return r1;	  
       //得到正确响应后，发ACMD41，应得到返回值0x00，否则重试400次
       r1 = _send_command(ACMD41, 0, 0);
       retry++;
     }while((r1!=0x00) && (retry<400));
     // 判断是超时还是得到正确回应
     // 若有回应：是SD卡；没有回应：是MMC卡
        
     //----------MMC卡额外初始化操作开始------------
     if(retry==400)
     {
        retry = 0;
        //发送MMC卡初始化命令（没有测试）
        do
        {
           r1 = _send_command(CMD1, 0, 0);
           retry++;
        }while((r1!=0x00)&& (retry<400));
        if(retry==400)return 1;   //MMC卡初始化超时		    
        //写入卡类型
        SD_Type = SD_TYPE_MMC;
     }
     //----------MMC卡额外初始化操作结束------------	    
     //设置SPI为高速模式
		 SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
		 spi->config(&SPIDevSDCard);

     spi->write(0xFF);
        
     //禁止CRC校验	   
     r1 = _send_command(CMD59, 0, 0x95);
     if(r1 != 0x00)return r1;  //命令错误，返回r1   	   
     //设置Sector Size
     r1 = _send_command(CMD16, 512, 0x95);
     if(r1 != 0x00)return r1;//命令错误，返回r1		 
     //-----------------SD卡、MMC卡初始化结束-----------------

  }//SD卡为V1.0版本的初始化结束	 
  //下面是V2.0卡的初始化
  //其中需要读取OCR数据，判断是SD2.0还是SD2.0HC卡
  else if(r1 == 0x01)
  {
    //V2.0的卡，CMD8命令后会传回4字节的数据，要跳过再结束本命令
    buff[0] = spi->read();  //should be 0x00
    buff[1] = spi->read();  //should be 0x00
    buff[2] = spi->read();  //should be 0x01
    buff[3] = spi->read();  //should be 0xAA	   
		cs->set();
		spi->write(0xFF);//the next 8 clocks			 
    //判断该卡是否支持2.7V-3.6V的电压范围
    //if(buff[2]==0x01 && buff[3]==0xAA) //如不判断，让其支持的卡更多
   // {	  
       retry = 0;
       //发卡初始化指令CMD55+ACMD41
       do
       {
    	  r1 = _send_command(CMD55, 0, 0);
    	  if(r1!=0x01)return r1;	   
    	  r1 = _send_command(ACMD41, 0x40000000, 1);
          if(retry>200)return r1;  //超时则返回r1状态  
       }while(r1!=0);		  
       //初始化指令发送完成，接下来获取OCR信息		   
       //-----------鉴别SD2.0卡版本开始-----------
       r1 = _send_command_no_deassert(CMD58, 0, 0);
       if(r1!=0x00)return r1;  //如果命令没有返回正确应答，直接退出，返回应答		 
       //读OCR指令发出后，紧接着是4字节的OCR信息
       buff[0] = spi->read();
       buff[1] = spi->read(); 
       buff[2] = spi->read();
       buff[3] = spi->read();

       //OCR接收完成，片选置高
			 cs->set();
       spi->write(0xFF);

       //检查接收到的OCR中的bit30位（CCS），确定其为SD2.0还是SDHC
       //如果CCS=1：SDHC   CCS=0：SD2.0
       if(buff[0]&0x40)SD_Type = SD_TYPE_V2HC;    //检查CCS	 
       else SD_Type = SD_TYPE_V2;	    
       //-----------鉴别SD2.0卡版本结束----------- 
       //设置SPI为高速模式
			 SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
			spi->config(&SPIDevSDCard);
			
   // }	    
  }
    return r1;
}
/*******************************************************************************
* Function Name  : SD_ReceiveData
* Description    : 从SD卡中读回指定长度的数据，放置在给定位置
* Input          : uint8_t *data(存放读回数据的内存>len)
*                  uint16_t len(数据长度）
*                  uint8_t release(传输完成后是否释放总线CS置高 0：不释放 1：释放）
* Output         : None
* Return         : uint8_t 
*                  0：NO_ERR
*                  other：错误信息
*******************************************************************************/
int SD::_receive_data(uint8_t *data, uint16_t len, uint8_t release)
{
  uint16_t retry;
  uint8_t r1;
  // 启动一次传输
  cs->reset();
  //等待SD卡发回数据起始令牌0xFE
  retry = 0;										   
  do
  {
    r1 = spi->read();
    retry++;
    if(retry>8000)  //8000次等待后没有应答，退出报错(根据实验测试，此处最好多试几次
    {
      cs->set();
        return 1;
    }
    }while(r1 != 0xFE);
		   
    //开始接收数据
//    while(len--)
//    {
//      *data = spi->read();
//      data++;
//    }
    //速度优化
    spi->read(data,len);
    //下面是2个伪CRC（dummy CRC）
    spi->write(0xFF);
    spi->write(0xFF);
    //按需释放总线，将CS置高
    if(release == RELEASE)
    {
      //传输结束
      cs->set();
      spi->write(0xFF);
    }											  					    
    return 0;
}
/*******************************************************************************
* Function Name  : SD_GetCID
* Description    : 获取SD卡的CID信息，包括制造商信息
* Input          : uint8_t *cid_data(存放CID的内存，至少16Byte）
* Output         : None
* Return         : uint8_t 
*                  0：NO_ERR
*                  1：TIME_OUT
*                  other：错误信息
*******************************************************************************/
int SD::get_CID(uint8_t *cid_data)
{
  uint8_t r1;
	
	spi->take_spi_right(&SPIDevSDCard);

  //发CMD10命令，读CID
  r1 = _send_command(CMD10, 0, 0xFF);
  if(r1 != 0x00)return r1;  //没返回正确应答，则退出，报错    
  //接收16个字节的数据
  _receive_data(cid_data, 16, RELEASE);	
	spi->release_spi_right();	
  return 0;
}
/*******************************************************************************
* Function Name  : SD_GetCSD
* Description    : 获取SD卡的CSD信息，包括容量和速度信息
* Input          : uint8_t *cid_data(存放CID的内存，至少16Byte）
* Output         : None
* Return         : uint8_t 
*                  0：NO_ERR
*                  1：TIME_OUT
*                  other：错误信息
*******************************************************************************/
int SD::get_CSD(uint8_t *csd_data)
{
  uint8_t r1;

	spi->take_spi_right(&SPIDevSDCard);
  //发CMD9命令，读CSD
  r1 = _send_command(CMD9, 0, 0xFF);
  if(r1 != 0x00)return r1;  //没返回正确应答，则退出，报错  
  //接收16个字节的数据
  _receive_data(csd_data, 16, RELEASE);
	spi->release_spi_right();	

  return 0;
}
/*******************************************************************************
* Function Name  : SD_GetCapacity
* Description    : 获取SD卡的容量（字节）
* Input          : None
* Output         : None
* Return         : uint32_t capacity 
*                   0： 取容量出错 
*******************************************************************************/
uint64_t SD::get_capacity(void)
{
  uint8_t csd[16];
  uint64_t Capacity;
  uint8_t r1;
  uint16_t i;
  uint16_t temp;
	spi->take_spi_right(&SPIDevSDCard);

  //取CSD信息，如果期间出错，返回0
  if(get_CSD(csd)!=0) return 0;	    
  //如果为SDHC卡，按照下面方式计算
  if((csd[0]&0xC0)==0x40)
  {									  
    Capacity=((uint32_t)csd[8])<<8;
    Capacity+=(uint32_t)csd[9]+1;	 
    Capacity = (Capacity)*1024;//得到扇区数
    Capacity*=512;//得到字节数			   
  }
  else
  {		    
    i = csd[6]&0x03;
    i<<=8;
    i += csd[7];
    i<<=2;
    i += ((csd[8]&0xc0)>>6);
    
    //C_SIZE_MULT
    r1 = csd[9]&0x03;
    r1<<=1;
    r1 += ((csd[10]&0x80)>>7);	 
    r1+=2;//BLOCKNR
    temp = 1;
    while(r1)
    {
      temp*=2;
      r1--;
    }
    Capacity = ((uint32_t)(i+1))*((uint32_t)temp);	 
    // READ_BL_LEN
    i = csd[5]&0x0f;
    //BLOCK_LEN
    temp = 1;
    while(i)
    {
      temp*=2;
      i--;
    }
    //The final result
    Capacity *= (uint32_t)temp;//字节为单位 	  
  }

		spi->release_spi_right();	

  return (uint64_t)Capacity;
}
/*******************************************************************************
* Function Name  : SD_ReadSingleBlock
* Description    : 读SD卡的一个block
* Input          : uint32_t sector 取地址（sector值，非物理地址） 
*                  uint8_t *buffer 数据存储地址（大小至少512byte） 
* Output         : None
* Return         : uint8_t r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
uint8_t SD::read_single_block(uint32_t sector, uint8_t *buffer)
{
  uint8_t r1;

  //设置为高速模式
  SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
	spi->take_spi_right(&SPIDevSDCard);
	
  if(SD_Type!=SD_TYPE_V2HC)
  //如果不是SDHC，将sector地址转成byte地址
  sector = sector<<9;

  r1 = _send_command(CMD17, sector, 1);//读命令
												    
  if(r1 != 0x00)return r1; 		   							  
  r1 = _receive_data(buffer, 512, RELEASE);
	
	spi->release_spi_right();	
	
  if(r1 != 0)
    return r1;   //读数据出错！
  else 
    return 0; 
}
/*******************************************************************************
* Function Name  : SD_WriteSingleBlock
* Description    : 写入SD卡的一个block
* Input          : uint32_t sector 扇区地址（sector值，非物理地址） 
*                  uint8_t *buffer 数据存储地址（大小至少512byte） 
* Output         : None
* Return         : uint8_t r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
uint8_t SD::write_single_block(uint32_t sector,const  uint8_t *data)
{
  uint8_t r1;
//  uint16_t i;
  uint32_t retry;
  //设置为高速模式
	SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
	spi->take_spi_right(&SPIDevSDCard);

  //如果不是SDHC，给定的是sector地址，将其转换成byte地址
  if(SD_Type!=SD_TYPE_V2HC)
  {
     sector = sector<<9;//512*sector即物理扇区的边界对齐地址
  }

  r1 = _send_command(CMD24, sector, 0x01);
  if(r1 != 0x00)
  {
    return r1;  //应答不正确，直接返回
  }
    
  //开始准备数据传输
  cs->reset();
  //先放3个空数据，等待SD卡准备好
  spi->write(0xff);
  spi->write(0xff);
  spi->write(0xff);
  //放起始令牌0xFE
  spi->write(0xFE);

  //发一个sector的数据
//  for(i=0;i<512;i++)
//  {
//     spi->write(*data++);
//  }
  //速度优化
  spi->write((uint8_t *)data,512);
  //发2个Byte的dummy CRC
  spi->write(0xff);
  spi->write(0xff);
    
  //等待SD卡应答
  r1 = spi->read();
  if((r1&0x1F)!=0x05)
  {
     cs->set();
     return r1;
  }
    
  //等待操作完成
  retry = 0;
  while(!spi->read())//卡自编程时，数据线被拉低
  {
     retry++;
     if(retry>0xfffffffe)        //如果长时间写入没有完成，报错退出
     {
        cs->set();
        return 1;           //写入超时返回1
     }
  }

  //写入完成，片选置1
  cs->set();
  spi->write(0xff);
	
	spi->release_spi_right();	
	

  return 0;
}
/*******************************************************************************
* Function Name  : SD_ReadMultiBlock
* Description    : 读SD卡的多个block
* Input          : uint32_t sector 取地址（sector值，非物理地址） 
*                  uint8_t *buffer 数据存储地址（大小至少512byte）
*                  uint8_t count 连续读count个block
* Output         : None
* Return         : uint8_t r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
uint8_t SD::read_multi_block(uint32_t sector, uint8_t *buffer, uint8_t count)
{
  uint8_t r1;	 			 
	
	SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
	spi->take_spi_right(&SPIDevSDCard);
	
	if(SD_Type != SD_TYPE_V2HC)
			sector = sector<<9;//如果不是SDHC，将sector地址转成byte地址
 //SD_WaitReady();
 //发读多块命令
  r1 = _send_command(CMD18, sector, 1);//读命令
  if(r1 != 0x00)return r1;	 
  do//开始接收数据
  {
    if(_receive_data(buffer, 512, NO_RELEASE) != 0x00)
    {
       break;
    }
    buffer += 512;
  } while(--count);		 
  //全部传输完毕，发送停止命令
  _send_command(CMD12, 0, 1);
  //释放总线
  cs->set();
  spi->write(0xFF);    
	
	spi->release_spi_right();	
	
  if(count != 0)
    return count;   //如果没有传完，返回剩余个数	 
  else 
    return 0;	 
}
/*******************************************************************************
* Function Name  : SD_WriteMultiBlock
* Description    : 写入SD卡的N个block
* Input          : uint32_t sector 扇区地址（sector值，非物理地址） 
*                  uint8_t *buffer 数据存储地址（大小至少512byte）
*                  uint8_t count 写入的block数目
* Output         : None
* Return         : uint8_t r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
uint8_t SD::write_multi_block(uint32_t sector,  const uint8_t *data, uint8_t count)
{
  uint8_t r1;
//  uint16_t i;	 		 
	
	SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
	spi->take_spi_right(&SPIDevSDCard);

	
  if(SD_Type != SD_TYPE_V2HC)
    sector = sector<<9;//如果不是SDHC，给定的是sector地址，将其转换成byte地址  
  if(SD_Type != SD_TYPE_MMC) 
    r1 = _send_command(ACMD23, count, 0x01);//如果目标卡不是MMC卡，启用ACMD23指令使能预擦除   
  r1 = _send_command(CMD25, sector, 0x01);//发多块写入指令
  if(r1 != 0x00)return r1;  //应答不正确，直接返回	 
  cs->reset();//开始准备数据传输   
  spi->write(0xff);//先放3个空数据，等待SD卡准备好
  spi->write(0xff);   
  //--------下面是N个sector写入的循环部分
  do
  {
    //放起始令牌0xFC 表明是多块写入
     spi->write(0xFC);	  
     //放一个sector的数据
//     for(i=0;i<512;i++)
//     {
//        spi->write(*data++);
//     }
      //速度优化
      spi->write((uint8_t *)data,512);
     //发2个Byte的dummy CRC
     spi->write(0xff);
     spi->write(0xff);
        
     //等待SD卡应答
     r1 = spi->read();
     if((r1&0x1F)!=0x05)
     {
        cs->set();    //如果应答为报错，则带错误代码直接退出
        return r1;
     }

     //等待SD卡写入完成
     if(_wait()==1)
     {
        cs->set();    //等待SD卡写入完成超时，直接退出报错
        return 1;
     }	   
    }while(--count);//本sector数据传输完成
    
    //发结束传输令牌0xFD
    spi->write(0xFD);
    r1 = spi->read();
    if(r1==0x00)
    {
       count =  0xfe;
    }		   
    if(_wait()) //等待准备好
    {
      cs->set();
      return 1;  
    }
    //写入完成，片选置1
    cs->set();
    spi->write(0xff);  
		
	spi->release_spi_right();	
	
    return count;   //返回count值，如果写完则count=0，否则count=1
}
/*******************************************************************************
* Function Name  : SD_Read_Bytes
* Description    : 在指定扇区,从offset开始读出bytes个字节
* Input          : uint32_t address 扇区地址（sector值，非物理地址） 
*                  uint8_t *buf     数据存储地址（大小<=512byte）
*                  uint16_t offset  在扇区里面的偏移量
                   uint16_t bytes   要读出的字节数
* Output         : None
* Return         : uint8_t r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
uint8_t SD::read_bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes)
{
  uint8_t r1;uint16_t i=0;  
	
	spi->take_spi_right(&SPIDevSDCard);

  r1=_send_command(CMD17,address<<9,1);//发送读扇区命令      
  if(r1!=0x00)return r1;  //应答不正确，直接返回
  cs->reset();//选中SD卡
  while (spi->read()!= 0xFE)//直到读取到了数据的开始头0XFE，才继续
  {
    i++;
    if(i>3000)
    {
      cs->set();//关闭SD卡
      return 1;//读取失败
    }
  }; 		 
  for(i=0;i<offset;i++)
    spi->read();//跳过offset位 
  for(;i<offset+bytes;i++)
    *buf++=spi->read();//读取有用数据	
  for(;i<512;i++) 
    spi->write(0xff); 	 //读出剩余字节
  spi->write(0xff);//发送伪CRC码
  spi->write(0xff);  
  cs->set();//关闭SD卡
	
	spi->release_spi_right();	
	

  return 0;
}





