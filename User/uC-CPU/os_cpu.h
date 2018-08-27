#ifndef OS_CPU_H
#define OS_CPU_H

/**************************************************************
 *宏定义
**************************************************************/
#ifndef NVIC_INT_CTRL
/*中断控制及状态寄存器*/
#define NVIC_INT_CTRL 		*((CPU_REG32 *)0xE000ED04)
#endif

#ifndef NVIC_PENDSVSET
/*触发PendSV异常的值Bit28，PENDSVSET*/
#define NVIC_PENDSVSET 		0x10000000
#endif

/*触发PendSV异常，设置中断控制寄存器的bit28为1*/
#define  OS_TASK_SW()               NVIC_INT_CTRL = NVIC_PENDSVSET
/*触发PendSV异常，设置中断控制寄存器的bit28为1*/
#define  OSIntCtxSw()               NVIC_INT_CTRL = NVIC_PENDSVSET

/**************************************************************
 *函数声明
**************************************************************/
/*在os_cpu_a.s中实现*/
void  OSStartHighRdy       (void);
void  OS_CPU_PendSVHandler (void);

#endif
