#ifndef OS_TYPE_H
#define OS_TYPE_H

#include "cpu.h"

typedef   CPU_INT08U      OS_STATE;                    /* State variable                                    <8>/16/32 */
typedef   CPU_INT08U      OS_PRIO;                     /* Priority of a task,                               <8>/16/32 */

/*空闲任务计数变量定义*/
typedef CPU_INT32U OD_IDLE_CTR;

typedef CPU_INT32U OS_TICK;

#endif
