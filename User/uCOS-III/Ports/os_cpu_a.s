;常量
NVIC_INT_CTRL		EQU		0xE000ED04
NVIC_SYSPRI14		EQU		0xE000ED22

NVIC_PENDSV_PRI		EQU		0xFF
NVIC_PENDSVSET		EQU		0x10000000

;******************************************************
;全局变量&函数
;******************************************************
 IMPORT	OSTCBCurPtr
 IMPORT	OSTCBHighRdyPtr


;******************************************************
;开始第一次上下文切换
;1.配置PendSV异常的优先级为最低
;2.在开始第一次上下文切换之前，设置psp=0
;3.触发PendSV异常，开始上下文切换
;******************************************************
OSStartHighRdy
	LDR		R0, =NVIC_SYSPRI14		;设置PendSV异常优先级为最低
	LDR		R1, =NVIC_PENDSV_PRI
	STRB	R1, [R0]
	
	MOVS	R0, #0					;设置psp的值为0，开始第一次上下文切换
	MSR		PSP, R0
	
	LDR		R0, =NVIC_INT_CTRL		;触发PendSV异常
	LDR		R1, =NVIC_PENDSVSET
	STR		R1, [R0]
	
	CPSIE	I						;使能总中断，NMI和HardFault除外，进入中断服务程序
	
OSStartHang
	B		OSStartHang				;程序应该永远不会运行到这里



;******************************************************
;PendSV_Handler异常
;******************************************************
PendSV_Handler
	CPSID	I			;关中断：NMI和HardFault除外，防止上下文切换被中断
	
	MRS		R0, PSP		;将psp的值加载到R0
	
;判断R0，如果值为0则跳转到OS_CPU_PendSVHandler_nosave
;进行第一次任务切换的时候，R0肯定为0
	CBZ		R0, OS_CPU_PendSVHandler_nosave
	
;-------------------------保存上文-------------------------
;保存当前正在运行的任务的环境参数
;----------------------------------------------------------
;在进入PendSV异常的时候，当前CPU的xPSR,PC(任务入口地址)，
;R14，R12，R3，R2，R1，R0会自动存储到当前任务堆栈，
;同时递减PSP的值，通过代码将PSP的值传给R0

;手动存储CPU寄存器R4-R11的值到当前的任务堆栈中
	STMDB	R0!, {R4-R11}
	
;加载OSTCBCurPtr指针地址到R1，这里LDR属于伪指令
	LDR		R1, = OSTCBCurPtr
;加载OSTCBCurPtr指针到R1，这里LDR属于RAM指令
	LDR		R1， [R1]
;存储R0的值到OSTCBCurPtr->OSTCBStkPtr，这个时候R0存的是任务空闲栈的栈顶
	STR		R0， [R1]
	

;-------------------------切换上文-------------------------
;实现OSTCBCurPtr = OSTCBHighRdyPtr
;把下一个要运行的任务的堆栈内容加载到CPU寄存器中
;----------------------------------------------------------
OS_CPU_PendSVHandler_nosave

;加载OSTCBCurPtr指针的地址到R0，这里LDR属于伪指令
	LDR		R0, = OSTCBCurPtr
;加载OSTCBHighRdyPtr指针的地址到R1，这里LDR属于伪指令
	LDR		R1, = OSTCBHighRdyPtr
;加载OSTCBHighRdyPtr指针到R2，这里LDR属于ARM指令
	LDR		R2, [R1]
;存储OSTCBHighRdyPtr到OSTCBCurPtr
	STR		R2, [R0]
	
;加载OSTCBHighRdyPtr到R0
	LDR		R0, [R2]
;加载需要手动保存的信息到CPU寄存器R4-R11
	LDMIA	R0!, {R4-R11}

;更新psp的值，这个时候PSP指向下一个要执行的任务的堆栈的栈底
;(这个栈底已经加上刚刚手动加载到CPU寄存器R4-R11的偏移)
	MSR		PSP, R0
	
;确保异常返回使用的堆栈指针是PSP，即LR寄存器的位2要为1
	ORR		LR, LR, #0x04
	
;开中断
	CPSIE	I
	
;异常返回，这个时候任务堆栈中的剩下的内容将会自动加载到xPSR
;PC(任务入口函数)，R14,R12,R3,R2,R1,R0(任务的形参)
;同时PSP的值也将更新，即指向任务堆栈的栈顶
;在STM32中，堆栈是由高地址向低地址生长的
	BX		LR

 EXPORT	OSStartHighRdy
 EXPORT	PendSV_Handler