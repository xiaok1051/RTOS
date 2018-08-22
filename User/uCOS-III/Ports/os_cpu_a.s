;常量
NVIC_INT_CTRL		EQU		0xE000ED04
NVIC_SYSPRI14		EQU		0xE000ED22

NVIC_PENDSV_PRI		EQU		0xFF
NVIC_PENDSVSET		EQU		0x10000000

;******************************************************
;开始第一次上下文切换
;1.配置PendSV异常的优先级为最低
;2.在开始第一次上下文切换之前，设置psp=0
;3.触发PendSV异常，开始上下文切换
;******************************************************

OSStartHighRdy
	LDR		R0, = NVIC_SYSPRI14		;设置PendSV异常优先级为最低
	LDR		R1, = NVIC_PENDSV_PRI
	STRB	R1, [R0]
	
	MOVS	R0, #0					;设置psp的值为0，开始第一次上下文切换
	MSR		PSP, R0
	
	LDR		R0, = NVIC_INT_CTRL		;触发PendSV异常
	LDR		R1, = NVIC_PENDSVSET
	STR		R1, [R0]
	
	CPSIE	I						;使能总中断，NMI和HardFault除外，进入中断服务程序
	
OSStartHang
	B		OSStartHang				;程序应该永远不会运行到这里

EXPORT	OSStartHighRdy
