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
#include "bsp_ebox.h"

// 采用那种调用方式
// use1 = 1, 中断回调只处理单任务
// use1 = 0, 中断回调处理多任务，但每次进入中断只处理一个任务
#define use1	1

#define N     100		//调用周期,systick为1ms,所以调用周期为10ms
#define M			10		//任务数量,多任务处理中每调用一次处理程序，执行一个任务，单个任务循环周期为N*M,两个任务间的间隔为N
#define M1		100		//长周期任务,周期为N*M1



/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F0 core_systick example"
#define EXAMPLE_DATE	"2017-12-31"

#if		 use1  		//中断回调只处理单任务

//    回调函数，调用周期取决于systick中断回调注册函数。如果systick中断只处理一个任务，使用
void SystickCallBack(void)
{
    LED1.toggle();
}

#else 					//中断回调处理多任务，但每次进入中断只处理一个任务

// 任务1
void led2On(){
    LED1.set();
}
// 任务2
void led2Off(){
    LED1.reset();
}

void led100(){
		UART.printf("长周期任务LED1反转,周期：%d ms \r\n",N*M1);
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
		UART.printf("多任务,周期 %dms,相邻任务间隔%dms\r\n",N*M,N);
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
		UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
        LED1.mode(OUTPUT_PP);
		LED1.mode(OUTPUT_PP);

	#if	use1
	  // 默认周期回调。1ms
    //SystickCallBackRegister(SystickCallBack);
    // 指定回调周期，回调周期是系统默认周期的N倍,500ms
		UART.printf("单任务处理,每500ms执行一次(led 反转)");
    attachSystickCallBack(SystickCallBack,500);
	#else
    // 默认周期回调。1ms
    //SystickCallBackRegister(SystickCallBack1);
    // 指定回调周期，回调周期是系统默认周期的N倍
		UART.printf("多任务处理,每10ms调用一次多任务处理程序,任务实际调用间隔为10N,N为10 or 100");
        SystickCallBackMultipleReg(SystickCallBackN,N);
		#endif
}

int main(void){
    setup();
    while (1)
    {
    }
}
