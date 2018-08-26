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
	OSTaskCreate( 	(OS_TCB *) 			&Tadk1TCB,
					(OS_TASK_PTR *) 	Task1,
					(void *) 			o,
					(CPU_STK *) 		&Task1Stk(0),
					(CPU_STK_SIZE) 		TASK1_STK_SIZE,	
					(OS_ERR *) 			&err);

	OSTaskCreate( 	(OS_TCB *) 			&Tadk2TCB,
					(OS_TASK_PTR *) 	Task2,
					(void *) 			o,
					(CPU_STK *) 		&Task2Stk(0),
					(CPU_STK_SIZE) 		TASK2_STK_SIZE,	
					(OS_ERR *) 			&err);

	/*将任务加入就绪列表*/

	/*启动OS，将不再返回*/


	return 0;
}

/**************************************************************
 * 函数实现
**************************************************************/

