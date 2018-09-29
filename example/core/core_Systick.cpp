/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : cat_li
  * @version: V2.0
  * @date   : 2017/12/17
  * @brief   ebox application example .
  *	core API中关于systick中断回调应用，。
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include "bsp.h"

// 采用那种调用方式
// use1 = 1, 中断回调只处理单任务
// use1 = 0, 中断回调处理多任务，但每次进入中断只处理一个任务
#define use1	0

#define N     100		//调用周期,systick为1ms,所以调用周期为10ms
#define M			10		//任务数量,多任务处理中每调用一次处理程序，执行一个任务，单个任务循环周期为N*M,两个任务间的间隔为N
#define M1		100		//长周期任务,周期为N*M1


//E_GPIO led(PA_5);
//E_GPIO led1(PB_1);


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F0 core_systick example"
#define EXAMPLE_DATE	"2017-12-31"
#define DEMO_VER			"1.0"

// 串口，led
//E_UART usart(USART1,UART1_TX,UART1_RX);
cpu_t	cpu;

/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	usart.printf("\n\r");
	usart.printf("*************************************************************\n\r");
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("* 例程名称   : %s%s\r\n",CPU_SERIES,EXAMPLE_NAME);	/* 打印例程名称 */
	usart.printf("* 例程版本   : %s\r\n", DEMO_VER);			/* 打印例程版本 */
	usart.printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	/* 打印ST固件库版本，这3个定义宏在stm32f0xx.h文件中 */
	usart.printf("* CMSIS版本  : V%d(STM32 HAL lib)\r\n", CMSIS_VERSION);
	usart.printf("* EBOX库版本 : %s (ebox)\r\n", EBOX_VERSION);
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("*                     CPU 信息\r\n");	/* 打印一行空格 */
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("* CPUID      : %08X %08X %08X\n\r"
			, cpu.chip_id[2], cpu.chip_id[1]);
	usart.printf("* flash size : %d KB \r\n",cpu.flash_size);
	usart.printf("* core       : %d\r\n",clock.core);
  usart.printf("* hclk       : %d\r\n",clock.hclk);
  usart.printf("* pclk1      : %d\r\n",clock.pclk1);
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("*************************************************************\n\r");
}

#if		 use1  		//中断回调只处理单任务

//    回调函数，调用周期取决于systick中断回调注册函数。如果systick中断只处理一个任务，使用
void SystickCallBack(void)
{
    LED.toggle();
}

#else 					//中断回调处理多任务，但每次进入中断只处理一个任务

// 任务1
void led2On(){
    LED.set();
}
// 任务2
void led2Off(){
    LED.reset();
}

void led100(){
		usart.printf("长周期任务LED1反转,周期：%d ms \r\n",N*M1);
		LED1.toggle();
}

// 任务列表
// 可以在systick里调用多个函数。需要调用的函数保存在数组里，每次只调用一个或2个
// 函数指针数组，保存需要调用的函数指针
fun_noPara_t funTable[M] = {
    led2Off,nullFun,nullFun,nullFun,nullFun,
    led2On,nullFun,nullFun,nullFun,nullFun
};



/**
 *@brief    回调函数
 *@param    NONE
 *@retval   NONE
*/
void SystickCallBackN(void){
    static uint8_t i;
		// i取值0-199。0和100对应100倍周期。0,1,2,3,4
    i = (i>=199)?0:i+1;
    // 每10倍周期调用一个函数（预先保存在数组里）如果SystickCallBack1每1ms被调用一次，
    // 则下面的函数每10ms被调用一次，修改10可以修改倍数。好处在于每10ms间隔的时候只
    // 调用一个函数，减少每次中断中执行的代码数量
		usart.printf("多任务,周期 %dms,相邻任务间隔%dms\r\n",N*M,N);
	  funTable[i%M]();		// 10为数组长度,如果只有5个任务可以将10改为5则每5个调用周期执行一次
//    switch (i%10)
//    {
//    case 0:
//        funTable[0]();		// 该函数每10x调用一次
//        break;
//    case 1:
//        funTable[1]();		// 该函数每10x调用一次，但和相邻函数之间间隔1x
//        break;
//    case 2:
//        funTable[2]();
//        break;
//    case 3:
//        funTable[3]();
//        break;
//		case 4:
//				funTable[4]();
//				break;
//    default:
//        break;
//    }

    // 类似于上面，每100个周期调用一次
		if(i%M1 == 0)
		{
			led100();
		}

}
#endif



void setup()
{
    ebox_init();
		usart.begin(115200);
		get_chip_info(&cpu);
		PrintfLogo();
    LED.mode(OUTPUT_PP);
		LED1.mode(OUTPUT_PP);

	#if	use1
	  // 默认周期回调。1ms
    //SystickCallBackRegister(SystickCallBack);
    // 指定回调周期，回调周期是系统默认周期的N倍,500ms
		usart.printf("单任务处理,每500ms执行一次(led 反转)");
    SystickCallBackMultipleReg(SystickCallBack,500);
	#else
    // 默认周期回调。1ms
    //SystickCallBackRegister(SystickCallBack1);
    // 指定回调周期，回调周期是系统默认周期的N倍
		usart.printf("多任务处理,每10ms调用一次多任务处理程序,任务实际调用间隔为10N,N为10 or 100");
    SystickCallBackMultipleReg(SystickCallBackN,N);
		#endif
}

int main(void){
    setup();
    while (1)
    {
    }
}
