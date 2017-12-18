/*===========================================================================
 *  ��������:  key.c                             	   
 *  Դ�ļ���:  key.c                                                       	   
 *  ��������:  2016/3/20                                                                                                                      
 *  ��    ��:  ʩ�ɷ�                                                             
 *  ��	  վ�� http://www.cnblogs.com/xzwj/														         
 *  �汾˵��:  V4.0 														 
 *--------------------------------------------------------------------------- 
 *  Ӳ������:                                                                                 
 *     		          			
 *--------------------------------------------------------------------------- 
 *  �������:                                                                                                  
 *     		 					
 *---------------------------------------------------------------------------                                                    
 *  ��������:                                                                        
 *                                
 *                                                 							                                                                     
 *--------------------------------------------------------------------------- 
 * ����Ȩ��  Copyright(C)             All Rights Reserved  				          
 * ��������  �˳������ڲ��ԣ�������ע����Ȩ��������Ϣ��                    
 *===========================================================================*/

/***********************************************************
 *                  ͷ �� �� �� �� ��                     *
 ***********************************************************/
#include "ebox.h"
//#include "num.h"
#include "key_dm.h"
/*--------------------------------------------------------*
 *                  �� �� �� �� ��                        *
 *--------------------------------------------------------*/

/*--------------------------------------------------------*
 *                  ȫ�ֱ���������                         *
 *--------------------------------------------------------*/
//KEY_M KEY_STATUS[2];
unsigned char g_key_num;
unsigned char g_key_state;
unsigned char g_key_value;
//extern NUM_FONT num_font;

/*---------------------------------------------------------
-   �������ƣ� void key_init(void)                   
-   ����������������ʼ������                            
-   �����б�                              
-   ���ؽ����  
-   ��    ע��                                            
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
-   �������ƣ� void key_init(void)                   
-   ����������������ʼ������                            
-   �����б�                              
-   ���ؽ����  
-   ��    ע��                                            
----------------------------------------------------------*/
void BUTTON::initialize(void)
{

	g_key_num = 16;
	g_key_state = 0;
	g_key_value = 0;
}
/*---------------------------------------------------------
-   �������ƣ� void key_init(void)                   
-   ����������������ʼ������                            
-   �����б�                              
-   ���ؽ����  
-   ��    ע��                                            
----------------------------------------------------------*/
char BUTTON::mode(uint8_t key_num)
{
	if ((count1>0)&&(count1<600)&&(count2==0))
	{
		bmode = KEY_CLICK;    //����
	}
	else if ((count1 > 600)&&(count2==0))
	{
		bmode = KEY_LONPR;    //����
	}
	else if(count2)
	{
		bmode = KEY_DBLCK;     //˫��
	}
//		num_font.printf(0,0,NUM_M1, bmode, COLOR_YELLOW,COLOR_RED);
	count1 = 0;
	count2 = 0;
	return 1;
}
/*---------------------------------------------------------
-   �������ƣ� uchar key_con_mul(void)
-   ���������� ����״̬���������İ�����������ʵ�ֶ��ְ���״̬��ʶ��
-   �����б�
-   ���ؽ����
-   ��    ע�� Key_Control_multifunction(void)
----------------------------------------------------------*/
char BUTTON::control_mul(void)
{
//		if (data_pin->read())
//		{
//				g_key_value = 1;   //�а�������
//		}
//		else
//		{
//				g_key_value = 0;   //û�а������� �򰴼��Ѿ��ͷ�
//		}
//		num_font.printf(0,0,NUM_M1, 0, COLOR_YELLOW,COLOR_RED);
		switch(g_key_state)
		{
				case KEY_STATE_0:
				{
						if(!data_pin->read())//g_key_value
						{
								g_key_state = KEY_STATE_1;   //�а������£��л���״̬1
//								num_font.printf(32,0,NUM_M1, 1, COLOR_YELLOW,COLOR_RED);
						}
				}break;
				case KEY_STATE_1:
				{
						if(!data_pin->read())					//��������԰��£�ʶ������ĸ�����
						{							
								g_key_state = KEY_STATE_2;  //ȷ���ĸ��������º��л���״̬2

						}
						else
						{
								g_key_state = KEY_STATE_0;  // ���������̧����ʶ��Ϊ�������ת����������ʼ̬0
						}
				}break;
				case KEY_STATE_2:
				{
						if(!data_pin->read())                //����������������״̬���ð�����Ӧ�ļ�����1 ��1
						{
								count1++;
								g_key_state = KEY_STATE_2; //����δ�ͷţ�����ִ�� ״̬2 
//								num_font.printf(64,0,NUM_M1,2, COLOR_YELLOW,COLOR_RED);

						}
						else
						{
								g_key_state = KEY_STATE_3; //�������ͷţ��л���״̬3 
						}
				}break;
				case KEY_STATE_3:
				{
						if(data_pin->read())              //���������ͷ�״̬���ð�����Ӧ�ļ�����2 ��1
						{
								count2++;
								if (count2 < 100)//������Ӧ�ļ�����2 �ۼ�ֵû�г����ж�ֵ������ִ��״̬3
								{
										g_key_state = KEY_STATE_3; 
								} 
								else
								{
										g_key_state = KEY_STATE_0;  //������Ӧ�ļ�����2 �ۼ�ֵû�г����ж�ֵ �����жϰ������ͷţ�ת����������ʼ̬
										count2=0;
										mode(g_key_num);
								}		
						}
						else
						{
								g_key_state = KEY_STATE_4;  //���еڶ��ΰ�������ʱ��ת��������״̬4
								count2 = 0;
						}
				}break;
				case KEY_STATE_4:
				{
						if(!data_pin->read())
						{
//								num_font.printf(100,0,NUM_M1,4, COLOR_YELLOW,COLOR_RED);
								count2++;
								g_key_state = KEY_STATE_4;//����û���ͷţ������ص�KEY_STATE_4
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

