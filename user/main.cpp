
#include "ebox.h"
#include "os.h"


#define TASK_1_STK_SIZE 128
#define TASK_2_STK_SIZE 128
#define TASK_3_STK_SIZE 128
#define TASK_4_STK_SIZE 128

static STACK_TypeDef TASK_1_STK[TASK_1_STK_SIZE];
static STACK_TypeDef TASK_2_STK[TASK_2_STK_SIZE];
static STACK_TypeDef TASK_3_STK[TASK_3_STK_SIZE];
static STACK_TypeDef TASK_4_STK[TASK_4_STK_SIZE];

#define TASK1_PRIO 0
#define TASK2_PRIO 1
#define TASK3_PRIO 2
#define TASK4_PRIO 3
void task_1();
void task_2();
void task_3();


float cpu;
INT16U mem;
u8 task2count = 0;

void setup()
{

	ebox_init();
	
	uart1.begin(9600);
	uart1.printf("\r\nuart1 9600 ok!");
	
	OS_Init();
	OS_TaskCreate(task_1,&TASK_1_STK[TASK_1_STK_SIZE-1],TASK1_PRIO);
	OS_TaskCreate(task_2,&TASK_2_STK[TASK_2_STK_SIZE-1],TASK2_PRIO);
	OS_TaskCreate(task_3,&TASK_3_STK[TASK_3_STK_SIZE-1],TASK3_PRIO);
	
	OS_Start();

}
void task_1()
{
	while(1)
	{
		uart1.printf("Task 1 Running!!!\r\n");
		OS_DelayTimes(1000);
	}
}
void task_2()
{
  while(1)
	{
		task2count++;
		uart1.printf("Task 2 Running!!!,runtimes = %d\r\n",task2count);
		OS_DelayTimes(1000);
	}

}
void task_3()
{
  while(1)
	{
		uart1.printf("Task 3 Running!!!\r\n");
		cpu = OS_GetCPU();
		mem = OS_GetStackMaxUsage(TASK_1_STK,TASK_1_STK_SIZE);
		uart1.printf("cpu = %0.1f%%\r\n",cpu);
		uart1.printf("mem = %02d%%\r\n",mem);
		OS_DelayTimes(1000);
	}

}

int main(void)
{
	setup();

	while(1)
	{

	}


}




