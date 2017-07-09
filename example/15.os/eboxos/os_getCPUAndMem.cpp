
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

float cpu_usage;
INT16U mem;
u8 task2count = 0;

void setup()
{
    ebox_init();

    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!");
    uart1.printf("\r\ncpu:%d", get_cpu_calculate_per_sec());
    PB8.mode(OUTPUT_PP);
    os_init();
    os_task_create(task_1, &TASK_1_STK[TASK_1_STK_SIZE - 1], TASK1_PRIO);
    os_task_create(task_2, &TASK_2_STK[TASK_2_STK_SIZE - 1], TASK2_PRIO);
    os_task_create(task_3, &TASK_3_STK[TASK_3_STK_SIZE - 1], TASK3_PRIO);

    os_start();

}
void task_1()
{
    while(1)
    {
        uart1.printf("Task 1 Running!!!\r\n");
        PB8 = !PB8;
        os_time_delay(500);
    }
}
void task_2()
{
    while(1)
    {
        uart1.printf("Task 2 Running!!!\r\n");
        task2count++;
        os_time_delay(1000);
    }
}
void task_3()
{
    while(1)
    {
        uart1.printf("Task 3 Running!!!\r\n");
        cpu_usage = os_get_cpu_usage();
        mem = os_get_stack_max_usage(TASK_1_STK, TASK_1_STK_SIZE);
        uart1.printf("cpu = %0.2f%%\r\n", cpu_usage);
        uart1.printf("mem = %02d%%\r\n", mem);
        os_time_delay(1000);
    }
}

int main(void)
{
    setup();
    while(1)
    {    }
}




