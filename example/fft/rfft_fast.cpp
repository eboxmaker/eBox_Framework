#include "ebox.h"
#include "bsp_ebox.h"


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
#define EXAMPLE_NAME	"fast rfft  example"
#define EXAMPLE_DATE	"2018-08-28"



uint32_t time1, time2, time3;


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




    /* 初始化结构体S中的参数 */
    arm_rfft_fast_init_f32(&S, fftSize);

    /* 按照实部，虚部，实部，虚部..... 的顺序存储数据 */
    for(int i = 0; i < TEST_LENGTH_SAMPLES; i++)
    {
        testInput_f32_10khz[i] = 5.2345678f * cos(2 * PI * 50 * i / TEST_LENGTH_SAMPLES + PI / 3.0) + 5.2345678;
    }

    time1 = micros();

    /* 1024点实序列快速FFT */
    arm_rfft_fast_f32(&S, testInput_f32_10khz, testOutput_f32_10khz, ifftFlag);
    time2 = micros();

    /* 为了方便跟函数arm_cfft_f32计算的结果做对比，这里求解了1024组模值，实际函数arm_rfft_fast_f32
       只求解出了512组
    */
    arm_cmplx_mag_f32(testOutput_f32_10khz, testOutput, fftSize);
    time3 = micros();

    /* 串口打印复数向量 */
    //     for(int i=0; i<TEST_LENGTH_SAMPLES; i++)
    //     {
    //         uart1.printf("%0.5f\r\n", testOutput_f32_10khz[i]);
    //     }

    /* 串口打印求解后的模值 */
    //     for(int i=0; i<TEST_LENGTH_SAMPLES/2; i++)
    //     {
    //         uart1.printf("%0.5f\r\n", testOutput[i]);
    //     }

    /* 串口打印最终信号的电压值 */
    testOutput[0] = testOutput[0] / (float)TEST_LENGTH_SAMPLES;
    for(int i = 1; i < TEST_LENGTH_SAMPLES / 2; i++)
    {
        testOutput[i] = testOutput[i] / (TEST_LENGTH_SAMPLES / 2);
    }

    for(int i = 0; i < TEST_LENGTH_SAMPLES / 2; i++)
    {
        uart1.printf("%0.8f\r\n", testOutput[i]);
    }
    phase = atan2( testOutput_f32_10khz[101], testOutput_f32_10khz[100]);
    uart1.printf("相位：   %0.3f\t%0.3f\r\n", phase, phase * 180 / PI);
    uart1.printf("时间2-1：%0.3fms\r\n", (time2 - time1) / 1000.0);
    uart1.printf("时间3-2：%0.3fms\r\n", (time3 - time2) / 1000.0);
    uart1.printf("时间3-1：%0.3fms\r\n", (time3 - time1) / 1000.0);
}

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

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