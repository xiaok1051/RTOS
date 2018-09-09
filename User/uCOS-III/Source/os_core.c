#include "os.h"
#include "os_cpu.h"

/*空闲任务函数*/
void OS_IdleTask (void *p_arg)
{
	p_arg = p_arg;

	/*空闲任务什么都不做，只对全局变量OSIdleTaskCtr ++ 操作*/
	for ( ; ; )
	{
		OSIdleTaskCtr ++;
	}
}

/*初始化就绪列表*/
void OS_RdyListInit(void)
{
	OS_PRIO i;
	OS_RDY_LIST *p_rdy_list;
	
	for(i=0u; i<OS_CFG_PRIO_MAX; i++)
	{
		p_rdy_list = &OSRdyList[i];
		p_rdy_list->HeadPtr = (OS_TCB *)0;
		p_rdy_list->TailPtr = (OS_TCB *)0;
	}
}

/*空闲任务初始化*/
void OS_IdleTaskInit(OS_ERR *p_err)
{
	/*初始化空闲任务计数器*/
	OSIdleTaskCtr = (OS_Idle_CTR)0;

	/*创建空闲任务*/
	OSTaskCreate( 	(OS_TCB 		*)&OSIdleTaskTCB,
					(OS_TASK_PTR 	 )OS_IdleTask,
					(void 			*)0,
					(CPU_STK 		*)OSCfg_IdleTaskStkBasePtr,
					(CPU_STK_SIZE    )OSCfg_IdleTaskStkSize,
					(OS_ERR 		*)p_err );
}

/*系统初始化函数，初始化OS中用到的全局变量*/
void OSInit(OS_ERR *p_err)
{
	/*指示系统的运行状态*/
	OSRunning = OS_STATE_OS_STOPPED;
	
	/*初始化两个全局TCB，这两个TCB用于任务切换*/
	OSTCBCurPtr = (OS_TCB *)0;
	OSTCBHighRdyPtr = (OS_TCB *)0;
	
	/*初始化就绪列表*/
	OS_RdyListInit();

	/*初始化空闲任务*/
	OS_IdleTaskInit(p_err);

	if(*p_err = OS_ERR_NONE;)
	{
		return;	
	}
}

/*系统启动函数*/
void OSStart (OS_ERR *p_err)
{
	if(OSRunning == OS_STATE_OS_STOPPED)
	{
		/*手动配置任务1先运行*/
		OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
		
		/*启动任务切换，不会返回*/
		OSStartHighRdy();
		
		/*不会运行到这里，运行到这里表示发生了致命的错误*/
		*p_err = OS_ERR_FATAL_RETURN;
	}
	else
	{
		*p_err = OS_STATE_OS_RUNNING;
	}
}
