#include "ebox.h"
#include "bsp_ebox.h"

#include "ws2812.h"
#include "ebox_virtual_lcd.h"
#include "gui.h"
#include "color_convert.h"

#include "arm_math.h"


/**
	*	1	VLcd（虚拟显示器），依赖ebox_virtual_lcd,font.h
    *       和GUI组件
	*	2	VLcd实现了基本gui的接口。可以使用gui来实现顶层效果
    *   3   VLcd还像底层设备提供了虚拟lcd显存入口，大小等信息，
    *       可以直接写到真实的显示器上。
    *   4   此例程使用了gui创建了几个元素，并通过串口打印出来
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Virtual LCD example"
#define EXAMPLE_DATE	"2018-08-14"




//#define TEST_LENGTH_SAMPLES 1024
//#define FFTH 40
//#define FFTWH 38
//#define FFTRH 39
//#define FFTRL 2

//u8 updata;


//////////////////////////////////////////////////////////////////
//float32_t datasin2[TEST_LENGTH_SAMPLES];
///* -------------------------------------------------------------------
//* External Input and Output buffer Declarations for FFT Bin Example
//* ------------------------------------------------------------------- */
//static float32_t testOutput[TEST_LENGTH_SAMPLES];
//
///* ------------------------------------------------------------------
//* Global variables for FFT Bin Example
//* ------------------------------------------------------------------- */
//uint32_t fftSize = TEST_LENGTH_SAMPLES;
//uint32_t ifftFlag = 0;
//uint32_t doBitReverse = 1;
///* Reference index at which max energy of bin ocuurs */
//uint32_t refIndex = 213, testIndex = 0;
//
//void fft()
//{
//	arm_status status;
//	arm_rfft_instance_f32 S;
//	float32_t maxValue;
//	int px=0,py=0,nx=0,ny=0;
//	float f,f1,fax=1;
//	int i,j,k;

//		status = ARM_MATH_SUCCESS;
//
//		/* Initialize the CFFT/CIFFT module */
//		status = arm_rfft_init_f32(&S, fftSize,ifftFlag, doBitReverse);
//		/* Process the data through the CFFT/CIFFT module */
//		arm_rfft_f32(&S, datasin2);
//		/* Process the data through the Complex Magnitude Module for
//		calculating the magnitude at each bin */
//		arm_cmplx_mag_f32(datasin2, testOutput,fftSize);
//		/* Calculates maxValue and returns corresponding BIN value */
//		arm_max_f32(testOutput, fftSize, &maxValue, &testIndex);
//
//
//		for(i=0;i<1024;i++)
//		{
//            uart1.printf("%d\t%0.3f\r\n",i,testOutput[i]);

//		}


//}

float32_t testInput_f32_10khz[2048];
#define TEST_LENGTH_SAMPLES 2048

/* -------------------------------------------------------------------
* External Input and Output buffer Declarations for FFT Bin Example
* ------------------------------------------------------------------- */
extern float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES];
static float32_t testOutput[TEST_LENGTH_SAMPLES / 2];

/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t fftSize = 1024;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;

/* Reference index at which max energy of bin ocuurs */
uint32_t refIndex = 213, testIndex = 0;


static void arm_cfft_f32_app(void)
{

    arm_status status;
    arm_cfft_radix4_instance_f32 S;
    float32_t maxValue;

    status = ARM_MATH_SUCCESS;

    /* Initialize the CFFT/CIFFT module */
    status = arm_cfft_radix4_init_f32(&S, fftSize,
                                      ifftFlag, doBitReverse);

    /* Process the data through the CFFT/CIFFT module */
    arm_cfft_radix4_f32(&S, testInput_f32_10khz);


    /* Process the data through the Complex Magnitude Module for
    calculating the magnitude at each bin */
    arm_cmplx_mag_f32(testInput_f32_10khz, testOutput,
                      fftSize);

    /* Calculates maxValue and returns corresponding BIN value */
    arm_max_f32(testOutput, fftSize, &maxValue, &testIndex);



    //将fft结果转换为电压值
    testOutput[0] = testOutput[0] / fftSize;
    for(int i = 1 ; i < TEST_LENGTH_SAMPLES / 2; i++)
    {
        testOutput[i] = testOutput[i] * (2.0 / fftSize);
    }
    //输出结果
    for(int i = 0 ; i < TEST_LENGTH_SAMPLES / 2; i++)
    {
        uart1.printf("%0.5f\r\n", testOutput[i]);

    }

}
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    for(int i = 0; i < 1024; i++)
    {
        testInput_f32_10khz[2 * i] = 2.2386 + 4.1257 * sin(2 * PI * 50 * (i / 1024.0));
        testInput_f32_10khz[2 * i + 1] = 0;
    }
    arm_cfft_f32_app();

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