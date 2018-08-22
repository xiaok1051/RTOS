;����
NVIC_INT_CTRL		EQU		0xE000ED04
NVIC_SYSPRI14		EQU		0xE000ED22

NVIC_PENDSV_PRI		EQU		0xFF
NVIC_PENDSVSET		EQU		0x10000000

;******************************************************
;��ʼ��һ���������л�
;1.����PendSV�쳣�����ȼ�Ϊ���
;2.�ڿ�ʼ��һ���������л�֮ǰ������psp=0
;3.����PendSV�쳣����ʼ�������л�
;******************************************************

OSStartHighRdy
	LDR		R0, = NVIC_SYSPRI14		;����PendSV�쳣���ȼ�Ϊ���
	LDR		R1, = NVIC_PENDSV_PRI
	STRB	R1, [R0]
	
	MOVS	R0, #0					;����psp��ֵΪ0����ʼ��һ���������л�
	MSR		PSP, R0
	
	LDR		R0, = NVIC_INT_CTRL		;����PendSV�쳣
	LDR		R1, = NVIC_PENDSVSET
	STR		R1, [R0]
	
	CPSIE	I						;ʹ�����жϣ�NMI��HardFault���⣬�����жϷ������
	
OSStartHang
	B		OSStartHang				;����Ӧ����Զ�������е�����

EXPORT	OSStartHighRdy
