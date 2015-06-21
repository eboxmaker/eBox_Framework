#include "lcd1602.h"
#include "stdio.h"
#include "lcd1602.h"

#define DATAOUT  GPIOB->ODR             //PD[0..7]--(LCD)D0~D7

LCD1602::LCD1602(uint8_t LEDPin,uint8_t ENPin,uint8_t RWPin,uint8_t RSPin,uint8_t DB0,uint8_t DB1,uint8_t DB2,uint8_t DB3,uint8_t DB4,uint8_t DB5,uint8_t DB6,uint8_t DB7)
{
		 _LEDPin = LEDPin;
		 _ENPin	= ENPin;
		 _RWPin	= RWPin;
		 _RSPin	= RSPin;
		 _DB0		= DB0;
		 _DB1		= DB1;
		 _DB2		= DB2;
		 _DB3		= DB3;
		 _DB4		= DB4;
		 _DB5		= DB5;
		 _DB6		= DB6;
		 _DB7		= DB7;
	pinMode(_LEDPin,OUTPUT);
	pinMode(_ENPin,OUTPUT);
	pinMode(_RWPin,OUTPUT);
	pinMode(_RSPin,OUTPUT);
	pinMode(_DB0,OUTPUT);
	pinMode(_DB1,OUTPUT);
	pinMode(_DB2,OUTPUT);
	pinMode(_DB3,OUTPUT);
	pinMode(_DB4,OUTPUT);
	pinMode(_DB5,OUTPUT);
	pinMode(_DB6,OUTPUT);
	pinMode(_DB7,OUTPUT);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	init();
	nDelay = 10;
}
LCD1602::LCD1602(uint8_t LEDPin,uint8_t ENPin,uint8_t RWPin,uint8_t RSPin,uint8_t DB0,uint8_t DB1,uint8_t DB2,uint8_t DB3)
{
		 _LEDPin = LEDPin;
		 _ENPin	= ENPin;
		 _RWPin	= RWPin;
		 _RSPin	= RSPin;
		 _DB0		= DB0;
		 _DB1		= DB1;
		 _DB2		= DB2;
		 _DB3		= DB3;
	pinMode(_LEDPin,OUTPUT);
	pinMode(_ENPin,OUTPUT);
	pinMode(_RWPin,OUTPUT);
	pinMode(_RSPin,OUTPUT);
	pinMode(_DB0,OUTPUT);
	pinMode(_DB1,OUTPUT);
	pinMode(_DB2,OUTPUT);
	pinMode(_DB3,OUTPUT);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	nDelay = 10;
}

inline void LCD1602::LCDdelay(uint32_t nCount)
{
   for(; nCount != 0; nCount--);
}
inline uint8_t LCD1602::bz(void)
{                         
	uint8_t result;


	pinMode(_DB7,INPUT);
	digitalWrite(_RSPin,0);
	digitalWrite(_RWPin,1);
	digitalWrite(_ENPin,1);
	LCDdelay(nDelay);
	result = digitalRead(_DB7);
	digitalWrite(_ENPin,0);

	return result; 

}
/******************************写命令*************************************************/	
/*********************************************************************
*名    称：Writecmd()
*功    能：写指令到LCD1602,指令数据占一个字节
*入口参数：cmd:待写入的指令
*出口参数：无
*********************************************************************/
void LCD1602::wcmd(uint8_t cmd)
{                          
	while(bz())
	{
		;
	}

	pinMode(_DB7,OUTPUT);
	digitalWrite(_RSPin,0);	   //对同一个寄存器的两次写入，中间延时一会
	digitalWrite(_RWPin,0);
	digitalWrite(_ENPin,0);
	
//	digitalWrite(_DB0,cmd&0x01);
//	digitalWrite(_DB1,cmd&0x02);
//	digitalWrite(_DB2,cmd&0x04);
//	digitalWrite(_DB3,cmd&0x08);
//	digitalWrite(_DB4,cmd&0x10);
//	digitalWrite(_DB5,cmd&0x20);
//	digitalWrite(_DB6,cmd&0x40);
//	digitalWrite(_DB7,cmd&0x80);
	
	DATAOUT &= 0XFF00;
	DATAOUT |= cmd;


	LCDdelay(nDelay);
	digitalWrite(_ENPin,1);
	LCDdelay(nDelay);
	digitalWrite(_ENPin,0);
	LCDdelay(nDelay);
}	
/*********************************************************************
*名    称：Writedata()
*功    能：写一字节数据到LCD1602
*入口参数：dat：无符号字节类型，0~255  包括各个ASCII码字符
*出口参数：无
*********************************************************************/
void LCD1602::wdat(uint8_t dat) 
{                         
	while(bz())//等待LCD1602空闲
	{
		;
	}

	pinMode(_DB7,OUTPUT);
	digitalWrite(_RSPin,1);
	digitalWrite(_RWPin,0);
	digitalWrite(_ENPin,0);
//	digitalWrite(_DB0,dat&0x01);
//	digitalWrite(_DB1,dat&0x02);
//	digitalWrite(_DB2,dat&0x04);
//	digitalWrite(_DB3,dat&0x08);
//	digitalWrite(_DB4,dat&0x10);
//	digitalWrite(_DB5,dat&0x20);
//	digitalWrite(_DB6,dat&0x40);
//	digitalWrite(_DB7,dat&0x80);
	
	DATAOUT &= 0XFF00;
	DATAOUT |= dat;

	LCDdelay(nDelay);
	digitalWrite(_ENPin,1);
	LCDdelay(nDelay);
	digitalWrite(_ENPin,0);
}
	
/*********************************************************************
*名    称：pos()
*功    能：设定显示位置
*入口参数：pos:显示位置,值的范围如下：
*    0x00----------------0x0f 0x10-------0x27 第一行（一次显示16个字符）
*    0x40----------------0x4f 0x50-------0x67 第二行
*出口参数：无
*********************************************************************/
void LCD1602::pos(uint8_t pos)
{                         
	wcmd(pos | 0x80);
}

/*********************************************************************
*名    称：Setpos()
*功    能：根据习惯设定显示位置
*入口参数：row:行，row=1表示第一行，row=2表示第二行
*          col:列，0~15，用于指定显示的列，范围可以是0~40
*出口参数：无
*********************************************************************/
void LCD1602::Setpos(uint8_t row,uint8_t col)
{
	if(row==1) wcmd(col | 0x80);
	else wcmd(col | 0xC0);
}
/*********************************************************************
*功  能：显示一个字符	  
*入  口：ch：待显示的字符
*********************************************************************/
void LCD1602::DispChar(char ch)
{
	wdat(ch);
}
/*********************************************************************
*功  能：在指定位置显示一个字符	  
*入  口：row:行 1或2        col:列，0~15
*        ch：待显示的字符
*********************************************************************/
void LCD1602::Setpos_DispChar(uint8_t row,uint8_t col,char ch)
{
	Setpos(row,col);
	wdat(ch);
}
/*********************************************************************
*名    称：DispString()
*功    能：使LCD1602显示一个字符串，显示位置需提前设定
*入口参数：str[]:待显示的字符串
*出口参数：无
*********************************************************************/
void LCD1602::DispString(char str[])
{
    uint8_t i=0;
	while(str[i] != '\0')
	{
	   wdat(str[i]);
	   ++i;
	}
}
/*********************************************************************
*名    称：Setpos_DispString()
*功    能：使LCD1602从指定位置开始显示一个字符串
*入口参数：row:显示行，1或2
*          col:显示列，0~15 （0~40） 
*          str[]:待显示的字符串
*出口参数：无
*说    明：指定位置是显示的初始位置，第一个字符显示的位置
*********************************************************************/
void LCD1602::Setpos_DispString(uint8_t row,uint8_t col,char str[])
{


	Setpos(row,col);
	DispString(str);
}
void LCD1602::Setpos_Dispnum(uint8_t row,uint8_t col,uint32_t num)
{
   Setpos(row,col);	
   Dispnum(num);
}
void LCD1602::Dispnum(uint32_t num)
{
	char s[3];
	sprintf(s,"%05d",num);
  DispString(s);//?????
}
/*********************************************************************
*名    称：DispDateTime()
*功    能：显示日历，显示日期与时间
*入口参数：year:年,可以是任意值
*          month:月，1~12
*          day:日，1~31
*          hour:时，1~24 或  1~12
*          min:分，0~59
*          sec:秒，0~59
*出口参数：无
*说    明：使用前必须先宏定义USE_DISP_DATE，在包含文件之前定义有效
*显示格式：Date:yyyy-mm-dd  *
*          Time:hh:mm:ss    *
*********************************************************************/

void LCD1602::DispDateTime(uint32_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec)
{
	char s[8];
	Setpos(1,0);
	DispString("Date:");
	
	sprintf(s,"%04d-%02d-%02d",year,month,day);
	DispString(s);
	

	Setpos(1,15);
	DispChar('*'); //第一行结束符显示
	Setpos(2,0);
	DispString("Time:  ");

	sprintf(s,"%02d:%02d:%02d",hour,min,sec);
	DispString(s);

	Setpos(2,15);
	DispChar('*'); //第二行结束符显示
}

void LCD1602::DispTimeHMS(uint8_t hour,uint8_t min,uint8_t sec)
{
	char s[8];

	Setpos(2,0);
	DispString("Time:  ");

	sprintf(s,"%02d:%02d:%02d",hour,min,sec);
	DispString(s);

	Setpos(2,15);
	DispChar('*'); //第二行结束符显示
}
//屏幕整体左移一格，用于滚动显示
void LCD1602::ShiftLeft()
{
	wcmd(0x18);
}
//屏幕整体右移一格，用于滚动显示
void LCD1602::ShiftRight(void)
{
	wcmd(0x1C);
}
//清屏，清除显示
void LCD1602::Clear(void)
{
	wcmd(0x01);
}
//使光标还回原点位置
void LCD1602::Return(void)
{
	wcmd(0x02);
}
//关显示
void LCD1602::Close(void)
{
	wcmd(0x08);
}
//开显示
void LCD1602::Open(void)
{
	wcmd(0x0C);
}
/*********************************************************************
*名    称：FlickerChar()
*功    能：使指定位置字符闪烁，不显示光标
*入口参数：row:行 1,2  
*          col:列 0~15
*出口参数：无
*说    明：写命令0x0D不显示光标的闪烁，写命令0x0F是显示光标的闪烁
*          一旦设定闪烁后，会根据位置变化闪烁，关闪烁写命令0x0C
*********************************************************************/
void LCD1602::Flicker(void)
{
	wcmd(0x0D);
}
void LCD1602::FlickerChar(uint8_t row,uint8_t col)
{
	wcmd(0x0D);
	Setpos(row,col);
}
//关闭字符闪烁
void LCD1602::CloseFlicker(void)
{
	wcmd(0x0C);
}
//屏幕秒闪烁一次
void LCD1602::FlickerScreen(void)
{
	wcmd(0x08);//关显示
	LCDdelay(500000);
	wcmd(0x0C);//开显示
	LCDdelay(500000);
}
void LCD1602::BackLight(u8 i)
{
	if(i == 1)
		digitalWrite(_LEDPin,1);
	else
		digitalWrite(_LEDPin,0);
}

/*********************************************************************
*名    称：Init()
*功    能：初始化LCD1602
*入口参数：无
*出口参数：无
*常用命令：0x38:16*2显示，5*&点阵显示字符，8位数据;指令执行时间40us
*          0x0C:开显示，关光标	  40us
*          0x08:关显示，关光标	  40us
*          0x0D:字符闪烁，关光标，接着设定位置，闪烁周期0.4ms左右
*          0x0F:字符闪烁，开光标
*          0x06:写完数据自动右移光标，普通情形，从左向右显示  40us
*          0x04:写完数据自动左移光标，可以从右向左显示	 40us
*          0x01:清除显示内容，即清屏  1.64ms
*          0x02:使光标还回起始位置	  1.64ms
*          0x18:屏幕上所有字符同时左移一格，适合滚动显示 40us
*          0x1C:屏幕上所有字符同时右移一格，适合滚动显示 40us
*********************************************************************/
void LCD1602::init(void)
{   
	wcmd(0x38);  //16*2显示，5*7点阵，8位数据
	LCDdelay(100);
	wcmd(0x38);  //16*2显示，5*7点阵，8位数据
	LCDdelay(100);
	wcmd(0x38);  //16*2显示，5*7点阵，8位数据
	LCDdelay(100);
	wcmd(0x08);  //先关显示，后开显示
	LCDdelay(100);
	wcmd(0x01);  //清除LCD的显示内容
	LCDdelay(100);
	wcmd(0x06);  //自动右移光标,0x04为左移光标
	LCDdelay(100);
	wcmd(0x0c);  //显示开，关光标;0x08为关显示
	LCDdelay(100);
	BackLight(1);
}
void LCD1602::TEST(void)
{
	char str[]={"SHENTQLF!"};
	Setpos_DispString(1,3,str);
//	DispFreq(123456);
	FlickerChar(2,3);
	FlickerScreen();
	FlickerScreen();
	FlickerScreen();
	LCDdelay(100);
	CloseFlicker();	          
}

