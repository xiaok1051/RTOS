/**************************************************************
 *包含的头文件
**************************************************************/
#include "os.h"

/**************************************************************
 *宏定义
**************************************************************/


/**************************************************************
 * 全局变量
**************************************************************/
uint32_t flag1;


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

/**************************************************************
 * main函数
**************************************************************/
int main(void)
{
	OS_ERR err;

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
		delay(100);
		flag1 = 0;
		delay(100);

		/*任务切换，这里是手动切换*/
		OSSched();
	}
}

/*任务2*/
void Task2(void *p_arg)
{
	for ( ; ; )
	{
		flag2 = 1;
		delay(100);
		flag2 = 0;
		delay(100);

		/*任务切换，这里是手动切换*/
		OSSched();
	}
}

/*任务切换，实际就是触发PendSV异常，然后在PendSV异常中进行上下文切换*/
void OSSched(void)
{
	if(OSTCBCurPtr == OSRdyList[0].HeadPtr)
	{
		OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
	}
	else
	{
		OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
	}
	
	OS_TASK_SW();
}





