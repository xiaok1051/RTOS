

/*暂时先将此函数从外部包含进来吧*/
extern void OSSched(void);
void OSTimeTick (void)
{
	/*任务切换函数*/
	OSSched();
}

/*阻塞延时*/
void OSTimeDly(OS_TICK dly)
{
	/*设置延时时间*/
	OSTCBCurPtr->TaskDelayTicks = dly;

	/*进行任务调度*/
	OSSched();
}
