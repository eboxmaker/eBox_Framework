#include "ebox.h"
#include "bsp_ebox.h"


#include "arm_math.h"


/**
	*	1	VLcd��������ʾ����������ebox_virtual_lcd,font.h
    *       ��GUI���
	*	2	VLcdʵ���˻���gui�Ľӿڡ�����ʹ��gui��ʵ�ֶ���Ч��
    *   3   VLcd����ײ��豸�ṩ������lcd�Դ���ڣ���С����Ϣ��
    *       ����ֱ��д����ʵ����ʾ���ϡ�
    *   4   ������ʹ����gui�����˼���Ԫ�أ���ͨ�����ڴ�ӡ����
	*/
	

/* ���������������̷������� */
#define EXAMPLE_NAME	"fast rfft  example"
#define EXAMPLE_DATE	"2018-08-28"

 

uint32_t time1,time2,time3;


#define TEST_LENGTH_SAMPLES 256 
/* ------------------------------------------------------------------- 
* External Input and Output buffer Declarations for FFT Bin Example 
* ------------------------------------------------------------------- */ 
float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES];
static float32_t testOutput[TEST_LENGTH_SAMPLES]; 
static float32_t testOutput_f32_10khz[TEST_LENGTH_SAMPLES]; 

 static void arm_rfft_fast_f32_app(void)
 {
    uint32_t testIndex = 0;  
    uint32_t fftSize = TEST_LENGTH_SAMPLES; 
    uint32_t ifftFlag = 0; 
    uint32_t doBitReverse = 1; 
	float32_t maxValue; 
    float32_t phase = 0;

     arm_rfft_fast_instance_f32 S;
    

    

     /* ��ʼ���ṹ��S�еĲ��� */
     arm_rfft_fast_init_f32(&S, fftSize);
    
     /* ����ʵ�����鲿��ʵ�����鲿..... ��˳��洢���� */
    for(int i = 0; i < TEST_LENGTH_SAMPLES; i++)
    {
        testInput_f32_10khz[i] = 5.2345678f *cos(2*PI*50*i/TEST_LENGTH_SAMPLES + PI/3.0) + 5.2345678;
    }

        time1 = micros();

     /* 1024��ʵ���п���FFT */
     arm_rfft_fast_f32(&S, testInput_f32_10khz, testOutput_f32_10khz, ifftFlag);
        time2 = micros();

     /* Ϊ�˷��������arm_cfft_f32����Ľ�����Աȣ����������1024��ģֵ��ʵ�ʺ���arm_rfft_fast_f32
        ֻ������512�� 
     */
     arm_cmplx_mag_f32(testOutput_f32_10khz, testOutput, fftSize);
        time3 = micros();

     /* ���ڴ�ӡ�������� */
//     for(int i=0; i<TEST_LENGTH_SAMPLES; i++)
//     {
//         uart1.printf("%0.5f\r\n", testOutput_f32_10khz[i]);
//     }
    
     /* ���ڴ�ӡ�����ģֵ */
//     for(int i=0; i<TEST_LENGTH_SAMPLES/2; i++)
//     {
//         uart1.printf("%0.5f\r\n", testOutput[i]);
//     }
     
     /* ���ڴ�ӡ�����źŵĵ�ѹֵ */
     testOutput[0] = testOutput[0] / (float)TEST_LENGTH_SAMPLES;
     for(int i = 1; i < TEST_LENGTH_SAMPLES/2; i++)
     {
         testOutput[i] = testOutput[i]/(TEST_LENGTH_SAMPLES/2);
     } 
     
     for(int i = 0; i < TEST_LENGTH_SAMPLES/2; i++)
     {
         uart1.printf("%0.8f\r\n", testOutput[i]);
     }    
     phase = atan2( testOutput_f32_10khz[101],testOutput_f32_10khz[100]);
     uart1.printf("��λ��   %0.3f\t%0.3f\r\n",phase,phase*180/PI);
     uart1.printf("ʱ��2-1��%0.3fms\r\n",(time2 - time1)/1000.0);
     uart1.printf("ʱ��3-2��%0.3fms\r\n",(time3 - time2)/1000.0);
     uart1.printf("ʱ��3-1��%0.3fms\r\n",(time3 - time1)/1000.0);
 }

void setup()
{
	ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    
    arm_rfft_fast_f32_app();


    PB8.mode(OUTPUT);;
}

int main(void)
{

    
	setup();
	while (1)
	{
        
       PB8.toggle();
        delay_ms(1000);        
	}
}