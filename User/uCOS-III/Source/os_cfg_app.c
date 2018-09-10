#include "cpu.h"
#include "os_cfg_app.h"


/*定义空闲任务堆栈*/

/*数据域*/
CPU_STK 	OSCfg_IdleTaskStk[OS_CFG_IDLE_TASK_STK_SIZE];

/*常量*/
CPU_STK 	*const OSCfg_IdleTaskStkBasePtr = (CPU_STK *)&OSCfg_IdleTaskStk[0];

/*空闲任务堆栈大小*/
CPU_STK 	const OSCfg_IdleTaskStkSize = (CPU_STK_SIZE)OS_CFG_IDLE_TASK_STK_SIZE;
