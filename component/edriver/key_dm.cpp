/*===========================================================================
 *  程序名称:  key.c                             	   
 *  源文件名:  key.c                                                       	   
 *  创建日期:  2016/3/20                                                                                                                      
 *  作    者:  施飞峰                                                             
 *  网	  站： http://www.cnblogs.com/xzwj/														         
 *  版本说明:  V4.0 														 
 *--------------------------------------------------------------------------- 
 *  硬件环境:                                                                                 
 *     		          			
 *--------------------------------------------------------------------------- 
 *  软件环境:                                                                                                  
 *     		 					
 *---------------------------------------------------------------------------                                                    
 *  内容描述:                                                                        
 *                                
 *                                                 							                                                                     
 *--------------------------------------------------------------------------- 
 * 【版权】  Copyright(C)             All Rights Reserved  				          
 * 【声明】  此程序用于测试，引用请注明版权和作者信息！                    
 *===========================================================================*/

/***********************************************************
 *                  头 文 件 配 置 区                     *
 ***********************************************************/
#include "ebox.h"
//#include "num.h"
#include "key_dm.h"
/*--------------------------------------------------------*
 *                  函 数 声 明 区                        *
 *--------------------------------------------------------*/

/*--------------------------------------------------------*
 *                  全局变量声明区                         *
 *--------------------------------------------------------*/
//KEY_M KEY_STATUS[2];
unsigned char g_key_num;
unsigned char g_key_state;
unsigned char g_key_value;
//extern NUM_FONT num_font;

/*---------------------------------------------------------
-   函数名称： void key_init(void)                   
-   功能描述：按键初始化函数                            
-   参数列表：                              
-   返回结果：  
-   备    注：                                            
----------------------------------------------------------*/
BUTTON::BUTTON(Gpio *pin)
{
		data_pin = pin;
}


void BUTTON::begin()
{
	data_pin->mode(INPUT_PU);
	g_key_num = 16;
	g_key_state = 0;
	g_key_value = 0;
}
/*---------------------------------------------------------
-   函数名称： void key_init(void)                   
-   功能描述：按键初始化函数                            
-   参数列表：                              
-   返回结果：  
-   备    注：                                            
----------------------------------------------------------*/
void BUTTON::initialize(void)
{

	g_key_num = 16;
	g_key_state = 0;
	g_key_value = 0;
}
/*---------------------------------------------------------
-   函数名称： void key_init(void)                   
-   功能描述：按键初始化函数                            
-   参数列表：                              
-   返回结果：  
-   备    注：                                            
----------------------------------------------------------*/
char BUTTON::mode(uint8_t key_num)
{
	if ((count1>0)&&(count1<600)&&(count2==0))
	{
		bmode = KEY_CLICK;    //单击
	}
	else if ((count1 > 600)&&(count2==0))
	{
		bmode = KEY_LONPR;    //长按
	}
	else if(count2)
	{
		bmode = KEY_DBLCK;     //双击
	}
//		num_font.printf(0,0,NUM_M1, bmode, COLOR_YELLOW,COLOR_RED);
	count1 = 0;
	count2 = 0;
	return 1;
}
/*---------------------------------------------------------
-   函数名称： uchar key_con_mul(void)
-   功能描述： 基于状态机调度器的按键操作可以实现多种按键状态的识别
-   参数列表：
-   返回结果：
-   备    注： Key_Control_multifunction(void)
----------------------------------------------------------*/
char BUTTON::control_mul(void)
{
//		if (data_pin->read())
//		{
//				g_key_value = 1;   //有按键按下
//		}
//		else
//		{
//				g_key_value = 0;   //没有按键按下 或按键已经释放
//		}
//		num_font.printf(0,0,NUM_M1, 0, COLOR_YELLOW,COLOR_RED);
		switch(g_key_state)
		{
				case KEY_STATE_0:
				{
						if(!data_pin->read())//g_key_value
						{
								g_key_state = KEY_STATE_1;   //有按键按下，切换到状态1
//								num_font.printf(32,0,NUM_M1, 1, COLOR_YELLOW,COLOR_RED);
						}
				}break;
				case KEY_STATE_1:
				{
						if(!data_pin->read())					//如果按键仍按下，识别出是哪个按键
						{							
								g_key_state = KEY_STATE_2;  //确认哪个按键按下后，切换到状态2

						}
						else
						{
								g_key_state = KEY_STATE_0;  // 如果按键已抬起，则识别为误操作，转换到按键初始态0
						}
				}break;
				case KEY_STATE_2:
				{
						if(!data_pin->read())                //按键处于连续按下状态，该按键对应的计数器1 加1
						{
								count1++;
								g_key_state = KEY_STATE_2; //按键未释放，继续执行 状态2 
//								num_font.printf(64,0,NUM_M1,2, COLOR_YELLOW,COLOR_RED);

						}
						else
						{
								g_key_state = KEY_STATE_3; //按键已释放，切换到状态3 
						}
				}break;
				case KEY_STATE_3:
				{
						if(data_pin->read())              //按键处于释放状态，该按键对应的计数器2 加1
						{
								count2++;
								if (count2 < 100)//按键对应的计数器2 累加值没有超过判定值，继续执行状态3
								{
										g_key_state = KEY_STATE_3; 
								} 
								else
								{
										g_key_state = KEY_STATE_0;  //按键对应的计数器2 累加值没有超过判定值 可以判断按键已释放，转换到按键初始态
										count2=0;
										mode(g_key_num);
								}		
						}
						else
						{
								g_key_state = KEY_STATE_4;  //当有第二次按键按下时，转换到按键状态4
								count2 = 0;
						}
				}break;
				case KEY_STATE_4:
				{
						if(!data_pin->read())
						{
//								num_font.printf(100,0,NUM_M1,4, COLOR_YELLOW,COLOR_RED);
								count2++;
								g_key_state = KEY_STATE_4;//按键没有释放，继续回到KEY_STATE_4
						}
						else
						{
								g_key_state = KEY_STATE_0;
								mode(g_key_num);
						}
				}break;
				default:break;
		}
		return g_key_num;
}

