#include "ebox.h"
#include "mb.h"
#include "mbutils.h"

#ifndef __FERRMODBUS_H
#define __FERRMODBUS_H


//输入寄存器起始地址
#define REG_INPUT_START       0x0000
//输入寄存器数量
#define REG_INPUT_NREGS       8
//保持寄存器起始地址
#define REG_HOLDING_START     0x0000
//保持寄存器数量
#define REG_HOLDING_NREGS     8

//线圈起始地址
#define REG_COILS_START       0x0000
//线圈数量
#define REG_COILS_SIZE        16

//开关寄存器起始地址
#define REG_DISCRETE_START    0x0000
//开关寄存器数量
#define REG_DISCRETE_SIZE     16

/* Private variables ---------------------------------------------------------*/
//输入寄存器内容
extern uint16_t usRegInputBuf[REG_INPUT_NREGS];
//寄存器起始地址
extern uint16_t usRegInputStart ;

//保持寄存器内容
extern uint16_t usRegHoldingBuf[REG_HOLDING_NREGS];
//保持寄存器起始地址
extern uint16_t usRegHoldingStart ;
//线圈状态															
extern uint8_t ucRegCoilsBuf[REG_COILS_SIZE / 8] ;
//开关输入状态
extern uint8_t ucRegDiscreteBuf[REG_DISCRETE_SIZE / 8];



void FreemodbusConfig(void)	;
void FreemodbusPoll(void)	;

class MODCFG
{
  public:
		//IO接口
	Uart *uart;
	Timer *timer;
	Gpio *Mode4851;
	Gpio *Mode4852;
	  //初始化参数
	eMBMode Mode;	
	UCHAR SlaveAdress;
	UCHAR PortId;
	ULONG BaudRate;
	eMBParity Pariby;
	//增补
	uint8_t DataBit;
	float StopBit;
  };
extern  MODCFG modbus ;

#endif
