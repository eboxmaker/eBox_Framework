
#include "ebox.h"
#include "os.h"


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


float cpu;
INT16U mem;
u8 task2count = 0;

void setup()
{
    ebox_init();
    os_init();

    uart1.begin(9600);
    uart1.printf("\r\nuart1 9600 ok!");

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
        uart1.printf("Task 1 Running!!!\r\n");
        PB8 = !PB8;
        os_time_delay(500);
    }
}
void task_2()
{
    while(1)
    {
        PB9 = !PB9;
        uart1.printf("Task 2 Running!!!\r\n", task2count);
        os_time_delay(250);
    }

}
void task_3()
{
    while(1)
    {
        PB10 = !PB10;
        uart1.printf("Task 3 Running!!!\r\n");
        os_time_delay(100);
    }

}

int main(void)
{

    setup();

    while(1)
    {

    }


}




