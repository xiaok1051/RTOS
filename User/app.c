/**************************************************************
 *包含的头文件
**************************************************************/
#include "os.h"
#include "os_cpu.h"

/**************************************************************
 *宏定义
**************************************************************/


/**************************************************************
 * 全局变量
**************************************************************/
uint32_t flag1;
uint32_t flag2;


/**************************************************************
 * TCB & STARK & 任务声明
**************************************************************/
#define TASK1_STK_SIZE 		20
#define TASK2_STK_SIZE 		20

/*任务堆栈，其实就是数组*/
static CPU_STK Task1Stk[TASK1_STK_SIZE];
static CPU_STK Task2Stk[TASK2_STK_SIZE];

static OS_TCB Task1TCB;
static OS_TCB Task2TCB;

/*任务函数声明，函数原型放在后面*/
void Task1(void *p_arg);
void Task2(void *p_arg);

/**************************************************************
 * 函数声明
**************************************************************/
/*延时函数声明，函数原型在后边*/
void delay(uint32_t count);
void OSSched(void);
extern void OS_CPU_SysTickInit(CPU_INT32U ms);

/**************************************************************
 * main函数
**************************************************************/
int main(void)
{
	OS_ERR err;

	/*关闭中断，汇编实现，在os_cpu_a.s中*/
	CPU_IntDis();

	/*配置SYSTick 10ms 中断一次*/
	OS_CPU_SysTickInit (10);

	/*初始化相关全局变量*/
	OSInit(&err);

	/*创建任务*/
	OSTaskCreate( 	(OS_TCB *) 			&Task1TCB,
					(OS_TASK_PTR ) 	Task1,
					(void *) 			0,
					(CPU_STK *) 		&Task1Stk[0],
					(CPU_STK_SIZE) 		TASK1_STK_SIZE,	
					(OS_ERR *) 			&err);

	OSTaskCreate( 	(OS_TCB *) 			&Task2TCB,
					(OS_TASK_PTR ) 	Task2,
					(void *) 			0,
					(CPU_STK *) 		&Task2Stk[0],
					(CPU_STK_SIZE) 		TASK2_STK_SIZE,	
					(OS_ERR *) 			&err);

	/*将任务加入就绪列表*/
	OSRdyList[0].HeadPtr = &Task1TCB;
	OSRdyList[1].HeadPtr = &Task2TCB;

	/*启动OS，将不再返回*/
	OSStart(&err);
}

/**************************************************************
 * 函数实现
**************************************************************/
/*延时函数*/
void delay(uint32_t count)
{
	for( ; count!=0; count--);
}

/*任务1*/
void Task1(void *p_arg)
{
	for ( ; ; )
	{
		flag1 = 1;
		//delay(100);
		OSTimeDly(2);
		flag1 = 0;
		//delay(100);
		OSTimeDly(2);

		/*任务切换，这里是手动切换，加入SysTick后这里便可以不使用了*/
		//OSSched();
	}
}

/*任务2*/
void Task2(void *p_arg)
{
	for ( ; ; )
	{
		flag2 = 1;
		//delay(100);
		OSTimeDly(2);
		flag2 = 0;
		//delay(100);
		OSTimeDly(2);

		/*任务切换，这里是手动切换，加入SysTick后这里便可以不使用了*/
		//OSSched();
	}
}

/*任务切换，实际就是触发PendSV异常，然后在PendSV异常中进行上下文切换*/
void OSSched(void)
{
#if 0
	if(OSTCBCurPtr == OSRdyList[0].HeadPtr)
	{
		OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
	}
	else
	{
		OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
	}
#endif

	/*如果当前任务是空闲任务，那么就去尝试执行任务1或者任务2，
	 * 看看他们的延时时间是否结束，如果任务的延时时间均没有到期，
	 * 那就返回继续执行空闲任务*/
	if(OSTCBCurPtr == &OSIdleTaskTCB)
	{
		if(OSRdyList[0].HeadPtr->TaskDelayTicks == 0)
		{
			OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;	
		}
		else if(OSRdyList[1].HeadPtr->TaskDelayTicks == 0)
		{
			OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;	
		}
		else /*任务延时均没有到期则返回，继续执行空闲任务*/
		{
			return;
		}
	}
	/*如果是task1或者task2的话，检查下另外一个任务，
	 * 如果另外的任务不再延时中，就切换到该任务，
	 * 否则，判断下当前任务是否应该进入延时状态，
	 * 如果是的话，就切换到空闲任务。否则就不进行任何切换*/
	else
	{
		if(OSTCBCurPtr == OSRdyList[0].HeadPtr)
		{
			if(OSRdyList[1].HeadPtr->TaskDelayTicks == 0)
			{
				OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;	
			}
			else if(OSTCBCurPtr->TaskDelayTicks != 0)
			{
				OSTCBHighRdyPtr = &OSIdleTaskTCB;	
			}
			else /*返回，不进行任务切换，因为两个任务都处于延时中*/
			{
				return;	
			}
		}
		else if(OSTCBCurPtr == OSRdyList[1].HeadPtr)
		{
			if(OSRdyList[0].HeadPtr->TaskDelayTicks == 0)
			{
				OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;	
			}
			else if(OSTCBCurPtr->TaskDelayTicks != 0)
			{
				OSTCBHighRdyPtr = &OSIdleTaskTCB;	
			}
			else /*返回，不进行任务切换，因为两个任务都处于延时中*/
			{
				return;	
			}
		}
	}
	
	/*任务切换，触发PendSV中断*/
	OS_TASK_SW();
}





