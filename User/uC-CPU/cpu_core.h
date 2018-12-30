#ifndef CPU_CORE_H
#define CPU_CORE_H

#include "cpu.h"

/*extern*/
#ifdef CPU_CORE_MODULE
#define CPU_CORE_EXT
#else
#define CPU_CORE_EXT extern
#endif

/*时间戳数据类型定义*/
typedef CPU_INT32U CPU_TS32;
typedef CPU_INT32U CPU_TS_TMR_FREQ;
typedef CPU_TS32   CPU_TS;
typedef CPU_INT32U CPU_TS_TMR;

/*CPU_CFG_TS_EN	宏定义*/
#if ((CPU_CFG_TS_32_EN == DEF__ENABLED) || (CPU_CFG_TS_64_EN == DEF__ENABLED)) 
#define 	CPU_CFG_TS_EN 		DEF_ENABLED
#else
#define 	CPU_CFG_TS_EN 		DEF_DISABLED
#endif

/*CPU_CFG_TS_TMR_EN 宏定义*/
#if ((CPU_CFG_TS_EN == DEF__ENABLED) || (defined(CPU_CFG_INT_DIS_MEAS_EN))) 
#define 	CPU_CFG_TS_TMR_EN 	DEF_ENABLED
#else
#define 	CPU_CFG_TS_TMR_EN 	DEF_DISABLED	
#endif

/*全局变量*/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_CORE_EXT CPU_TS_TMR_FREQ CPU_TS_TmrFreq_Hz;
#endif

void CPU_TS_TmrFreqSet (CPU_TS_TMR_FREQ freq_hz);
void CPU_TS_TmrInit(void);
static void CPU_TS_Init();
CPU_INT32U BSP_CPU_ClkFreq (void);
void CPU_TS_TmrFreqSet (CPU_TS_TMR_FREQ freq_hz);

#endif
