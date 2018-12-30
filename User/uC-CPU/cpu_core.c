/*cup_core.c*/

/*包含头文件*/
#include "cpu_core.h" 

/*寄存器定义*/
#define BSP_REG_DEM_CR 					(*(CPU_REG32 *)0xE00EEDFC) 
#define BSP_REG_DWT_CR 					(*(CPU_REG32 *)0xE0001000)
#define BSP_REG_DWT_CYCCNT 				(*(CPU_REG32 *)0xE0001004)
#define BSP_REG_DBGMCU_CR 				(*(CPU_REG32 *)0xE0042004)

/*寄存器位定义*/
#define BSP_DBGMCU_CR_TRACE_IOEN_MASK 				0X10
#define BSP_DBGMCU_CR_TRACE_IOEN_ASYNC 				0X10
#define BSP_DBGMCU_CR_TRACE_IOEN_SYNC_01 			0X10
#define BSP_DBGMCU_CR_TRACE_IOEN_SYNC_02 			0X10
#define BSP_DBGMCU_CR_TRACE_IOEN_SYNC_04 			0X10
#define BSP_DBGMCU_CR_TRACE_IOEN_MASK 				0X10
#define BSP_BIT_DEM_CR_TRCENA 						(1<<24)
#define BSP_BIT_DWT_CR_CYCCNTENA 					(1<<0)

/*cpu初始化函数*/
void CPU_Init(void)
{
	/*CPU初始化函数中总共做3件事
	 * 1.初始化时间戳
	 * 2.初始化中断使能时间测量
	 * 3.初始化CPU名字
	 * 这里暂时只用到 初始化时间戳 功能*/
#if ((CPU_CFG_TS_EN == DEF_ENABLED) || (CPU_CFG_TS_TMR_EN == DEF_ENABLED))
	CPU_TS_Init();
#endif
}

/*CPU_TS_Init()函数，时间戳初始化函数*/
#if ((CPU_CFG_TS_EN == DEF_ENABLED) || (CPU_CFG_TS_TMR_EN == DEF_ENABLED))
static void CPU_TS_Init()
{
#if(CPU_CFG_TS_TMR_EN == DEF_ENABLED)
	CPU_TS_TmrFreq_Hz = 0u;	
	CPU_TS_TmrInit();
#endif
}

/*CPU_TS_TmrInit()函数，时间戳定时器初始化函数*/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void CPU_TS_TmrInit(void)
{
	CPU_INT32 fclk_freq;	
	fclk_freq = BSP_CPU_CLKFreq();

	/*使能DWT外设*/
	BSP_REG_DEM_CR |= (CPU_INT32U)BSP_BIT_DEM_CR_TRCENA;
	/*DWT CYCCNT寄存器计数清0*/
	BSP_REG_DWT_CYCCNT = (CPU_INT32U)0u;
	/*注意：当使用软件仿真全速运行的时候，会先停在这里，
	 * 当好像在这里设置了一个断点一样，需要手动运行才能跳过，
	 * 当使用硬件仿真的时候却不会*/
	/*使能Cortex-M3 DWT CYNNT寄存器*/
	BSP_REG_DWT_CR  |= (CPU_INT32U)BSP_BIT_DWT_CR_CYCCNTENA;

	CPU_TS_TmrFreqSet((CPU_TS_TMR_FREQ)fclk_freq);
}
#endif

/*获取CPU的HCLK时钟*/
/*这个是跟硬件相关的，目前我们是软件仿真，我们暂时把跟硬件相关的代码屏蔽掉
 * 直接手动设置CPU的HCLK时钟*/
CPU_INT32U BSP_CPU_ClkFreq (void)
{
#if 0
	RCC_ClocksTypeDef 	rcc_clocks;

	RCC_GetClocksFreq(&rcc_clocks);
	
	return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
#else
	CPU_INT32U CPU_HCLK;

	/*目前软件仿真我们使用25M的系统时钟*/
	CPU_HCLK = 25000000;

	return CPU_HCLK;

#endif
}

/*初始化CPU_TS_TmrFreq_Hz,这个就是系统的时钟，单位为Hz*/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void CPU_TS_TmrFreqSet (CPU_TS_TMR_FREQ freq_hz)
{
	CPU_TS_TmrFreq_Hz = freq_hz;
}
#endif

/*用于获取CYCNNT计数器的值*/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR CPU_TS_TmrRd (void)
{
	CPU_TS_TMR ts_tmr_cnts;
	ts_tmr_cnts = (CPU_TS_TMR)BSP_REG_DWT_CYCCNT;

	return (ts_tmr_cnts);
}
#endif

#endif

