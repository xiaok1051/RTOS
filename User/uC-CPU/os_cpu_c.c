#include "cpu.h"
#include "os.h"
#include "os_type.h"
#include "ARMCM3.h"

/*任务堆栈初始化*/
CPU_STK *OSTaskStkInit ( 				OS_TASK_PTR		p_task,
										void 			*p_arg,
										CPU_STK 		*p_stk_base,
										CPU_STK_SIZE	stk_size)
{
	CPU_STK *p_stk;
	p_stk = &p_stk_base[stk_size];
	
	/*异常发生时自动保存的寄存器*/
	*--p_stk = (CPU_STK)0x01000000u;	/*xPSR的bit24必须置1									  */
	*--p_stk = (CPU_STK)p_task;			/*R15(PC)任务的入口地址									  */
	*--p_stk = (CPU_STK)0x14141414u;    /* R14 (LR)                                               */
	*--p_stk = (CPU_STK)0x12121212u;    /* R12                                                    */
	*--p_stk = (CPU_STK)0x03030303u;    /* R3                                                     */
 	*--p_stk = (CPU_STK)0x02020202u;    /* R2                                                     */
	*--p_stk = (CPU_STK)0x01010101u;    /* R1                                                     */
	*--p_stk = (CPU_STK)p_arg;          /* R0 : argument                                          */
  
	/* Remaining registers saved on process stack*/
	*--p_stk = (CPU_STK)0x11111111u;    /* R11                                                    */
 	*--p_stk = (CPU_STK)0x10101010u;    /* R10                                                    */
	*--p_stk = (CPU_STK)0x09090909u;    /* R9                                                     */
	*--p_stk = (CPU_STK)0x08080808u;    /* R8                                                     */
	*--p_stk = (CPU_STK)0x07070707u;    /* R7                                                     */
	*--p_stk = (CPU_STK)0x06060606u;    /* R6                                                     */
	*--p_stk = (CPU_STK)0x05050505u;    /* R5                                                     */
	*--p_stk = (CPU_STK)0x04040404u;    /* R4                                                     */
	
	return p_stk;
}

/*SysTick初始化函数*/
#if 0 	/*此函数不使用ucos-III官方的*/
void OS_CPU_SysTickInit(CPU_INT32U cnts)
{

}
#else  /*使用头文件ARMCM3.h里面现有的寄存器定义和函数来实现*/
void OS_CPU_SysTickInit(CPU_INT32U ms)
{
	/*设置重装载寄存器的值*/	
	SysTick->LOAD = ms * SystemCoreClock / 1000 -1;
	
	/*配置中断优先级为最低*/
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);

	/*复位当前计数器的值*/
	SysTick->VAL = 0;

	/*选择时钟源、使能中断、使能计数器*/
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
					SysTick_CTRL_TICKINT_Msk   |
					SysTick_CTRL_ENABLE_Msk    ;
}
#endif

extern void OSTimeTick(void);
/*SysTick定时器的中断服务函数，定时器的数值递减到0时触发
 * 在此完成任务的切换,SysTick来提供系统的心跳*/
void SysTick_Handler(void)
{
	OSRdyList[0].HeadPtr->TaskDelayTicks--;
	OSRdyList[1].HeadPtr->TaskDelayTicks--;
	/*此函数中仅仅完成了任务的切换，函数原型在os_time.c中*/
	OSTimeTick();
}


