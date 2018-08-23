;����
NVIC_INT_CTRL		EQU		0xE000ED04
NVIC_SYSPRI14		EQU		0xE000ED22

NVIC_PENDSV_PRI		EQU		0xFF
NVIC_PENDSVSET		EQU		0x10000000

;******************************************************
;ȫ�ֱ���&����
;******************************************************
 IMPORT	OSTCBCurPtr
 IMPORT	OSTCBHighRdyPtr


;******************************************************
;��ʼ��һ���������л�
;1.����PendSV�쳣�����ȼ�Ϊ���
;2.�ڿ�ʼ��һ���������л�֮ǰ������psp=0
;3.����PendSV�쳣����ʼ�������л�
;******************************************************
OSStartHighRdy
	LDR		R0, =NVIC_SYSPRI14		;����PendSV�쳣���ȼ�Ϊ���
	LDR		R1, =NVIC_PENDSV_PRI
	STRB	R1, [R0]
	
	MOVS	R0, #0					;����psp��ֵΪ0����ʼ��һ���������л�
	MSR		PSP, R0
	
	LDR		R0, =NVIC_INT_CTRL		;����PendSV�쳣
	LDR		R1, =NVIC_PENDSVSET
	STR		R1, [R0]
	
	CPSIE	I						;ʹ�����жϣ�NMI��HardFault���⣬�����жϷ������
	
OSStartHang
	B		OSStartHang				;����Ӧ����Զ�������е�����



;******************************************************
;PendSV_Handler�쳣
;******************************************************
PendSV_Handler
	CPSID	I			;���жϣ�NMI��HardFault���⣬��ֹ�������л����ж�
	
	MRS		R0, PSP		;��psp��ֵ���ص�R0
	
;�ж�R0�����ֵΪ0����ת��OS_CPU_PendSVHandler_nosave
;���е�һ�������л���ʱ��R0�϶�Ϊ0
	CBZ		R0, OS_CPU_PendSVHandler_nosave
	
;-------------------------��������-------------------------
;���浱ǰ�������е�����Ļ�������
;----------------------------------------------------------
;�ڽ���PendSV�쳣��ʱ�򣬵�ǰCPU��xPSR,PC(������ڵ�ַ)��
;R14��R12��R3��R2��R1��R0���Զ��洢����ǰ�����ջ��
;ͬʱ�ݼ�PSP��ֵ��ͨ�����뽫PSP��ֵ����R0

;�ֶ��洢CPU�Ĵ���R4-R11��ֵ����ǰ�������ջ��
	STMDB	R0!, {R4-R11}
	
;����OSTCBCurPtrָ���ַ��R1������LDR����αָ��
	LDR		R1, = OSTCBCurPtr
;����OSTCBCurPtrָ�뵽R1������LDR����RAMָ��
	LDR		R1�� [R1]
;�洢R0��ֵ��OSTCBCurPtr->OSTCBStkPtr�����ʱ��R0������������ջ��ջ��
	STR		R0�� [R1]
	

;-------------------------�л�����-------------------------
;ʵ��OSTCBCurPtr = OSTCBHighRdyPtr
;����һ��Ҫ���е�����Ķ�ջ���ݼ��ص�CPU�Ĵ�����
;----------------------------------------------------------
OS_CPU_PendSVHandler_nosave

;����OSTCBCurPtrָ��ĵ�ַ��R0������LDR����αָ��
	LDR		R0, = OSTCBCurPtr
;����OSTCBHighRdyPtrָ��ĵ�ַ��R1������LDR����αָ��
	LDR		R1, = OSTCBHighRdyPtr
;����OSTCBHighRdyPtrָ�뵽R2������LDR����ARMָ��
	LDR		R2, [R1]
;�洢OSTCBHighRdyPtr��OSTCBCurPtr
	STR		R2, [R0]
	
;����OSTCBHighRdyPtr��R0
	LDR		R0, [R2]
;������Ҫ�ֶ��������Ϣ��CPU�Ĵ���R4-R11
	LDMIA	R0!, {R4-R11}

;����psp��ֵ�����ʱ��PSPָ����һ��Ҫִ�е�����Ķ�ջ��ջ��
;(���ջ���Ѿ����ϸո��ֶ����ص�CPU�Ĵ���R4-R11��ƫ��)
	MSR		PSP, R0
	
;ȷ���쳣����ʹ�õĶ�ջָ����PSP����LR�Ĵ�����λ2ҪΪ1
	ORR		LR, LR, #0x04
	
;���ж�
	CPSIE	I
	
;�쳣���أ����ʱ�������ջ�е�ʣ�µ����ݽ����Զ����ص�xPSR
;PC(������ں���)��R14,R12,R3,R2,R1,R0(������β�)
;ͬʱPSP��ֵҲ�����£���ָ�������ջ��ջ��
;��STM32�У���ջ���ɸߵ�ַ��͵�ַ������
	BX		LR

 EXPORT	OSStartHighRdy
 EXPORT	PendSV_Handler