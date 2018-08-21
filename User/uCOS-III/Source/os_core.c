#include "os.h"

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


/*系统初始化函数，初始化OS中用到的全局变量*/
void OSInit(OS_ERR *p_err)
{
	/*指示系统的运行状态*/
	OSRunning = OS_STATE_OS_STOPPED;
	
	OSTCBCurPtr = (OS_TCB *)0;
	OSTCBHighRdyPtr = (OS_TCB *)0;
	
	OS_RdyListInit();
	
	*p_err = OS_ERR_NONE;
}