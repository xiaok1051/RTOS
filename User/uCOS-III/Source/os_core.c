#include "os.h"
#include "os_cpu.h"

/*����������*/
void OS_IdleTask (void *p_arg)
{
	p_arg = p_arg;

	/*��������ʲô��������ֻ��ȫ�ֱ���OSIdleTaskCtr ++ ����*/
	for ( ; ; )
	{
		OSIdleTaskCtr ++;
	}
}

/*��ʼ�������б�*/
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

/*���������ʼ��*/
void OS_IdleTaskInit(OS_ERR *p_err)
{
	/*��ʼ���������������*/
	OSIdleTaskCtr = (OS_Idle_CTR)0;

	/*������������*/
	OSTaskCreate( 	(OS_TCB 		*)&OSIdleTaskTCB,
					(OS_TASK_PTR 	 )OS_IdleTask,
					(void 			*)0,
					(CPU_STK 		*)OSCfg_IdleTaskStkBasePtr,
					(CPU_STK_SIZE    )OSCfg_IdleTaskStkSize,
					(OS_ERR 		*)p_err );
}

/*ϵͳ��ʼ����������ʼ��OS���õ���ȫ�ֱ���*/
void OSInit(OS_ERR *p_err)
{
	/*ָʾϵͳ������״̬*/
	OSRunning = OS_STATE_OS_STOPPED;
	
	/*��ʼ������ȫ��TCB��������TCB���������л�*/
	OSTCBCurPtr = (OS_TCB *)0;
	OSTCBHighRdyPtr = (OS_TCB *)0;
	
	/*��ʼ�������б�*/
	OS_RdyListInit();

	/*��ʼ����������*/
	OS_IdleTaskInit(p_err);

	if(*p_err = OS_ERR_NONE;)
	{
		return;	
	}
}

/*ϵͳ��������*/
void OSStart (OS_ERR *p_err)
{
	if(OSRunning == OS_STATE_OS_STOPPED)
	{
		/*�ֶ���������1������*/
		OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
		
		/*���������л������᷵��*/
		OSStartHighRdy();
		
		/*�������е�������е������ʾ�����������Ĵ���*/
		*p_err = OS_ERR_FATAL_RETURN;
	}
	else
	{
		*p_err = OS_STATE_OS_RUNNING;
	}
}
