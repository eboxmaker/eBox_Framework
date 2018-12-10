
#include "ebox.h"
#include "../os/ebox_os/os.h"


#define TASK_1_STK_SIZE 128
#define TASK_2_STK_SIZE 128
#define TASK_3_STK_SIZE 128

static STACK_TypeDef TASK_1_STK[TASK_1_STK_SIZE];
static STACK_TypeDef TASK_2_STK[TASK_2_STK_SIZE];
static STACK_TypeDef TASK_3_STK[TASK_3_STK_SIZE];

#define TASK1_PRIO 0
#define TASK2_PRIO 1
#define TASK3_PRIO 2

void task_1();
void task_2();
void task_3();

uint32_t task1_counter = 0;
uint32_t task2_counter = 0;
uint32_t task3_counter;
void setup()
{
    ebox_init();
    os_init();

    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!");

    uart1.printf("\r\nos初始化!");
    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP);
    PB10.mode(OUTPUT_PP);

    os_task_create(task_1, &TASK_1_STK[TASK_1_STK_SIZE - 1], TASK1_PRIO);
    os_task_create(task_2, &TASK_2_STK[TASK_2_STK_SIZE - 1], TASK2_PRIO);
    os_task_create(task_3, &TASK_3_STK[TASK_3_STK_SIZE - 1], TASK3_PRIO);
    uart1.printf("\r\nos创建任务成功");

    os_start();
}
void task_1()
{
    while(1)
    {
        task1_counter++;
        uart1.printf("Task 1 Running!!! cpu usage:%0.2f[%05d]\r\n",os_get_cpu_usage(),task1_counter);
        PB8 = !PB8;
        os_time_delay(1000);
    }
}
void task_2()
{
    while(1)
    {
        task2_counter++;
        PB9 = !PB9;
        uart1.printf("Task 2 Running!!![%05d]\r\n",task2_counter);
        os_time_delay(1000);
    }
}
void task_3()
{
    while(1)
    {
        //PB10 = !PB10;
        //uart1.printf("Task 3 Running!!!\r\n");
        int len = uart1.available();
        for(int i = 0; i < len; i++ )
        {
            char c = uart1.read();

            uart1.printf("%c", c);

        }
        os_time_delay(1);
    }
}

int main(void)
{
    setup();
    while(1)
    {  
    }
}




