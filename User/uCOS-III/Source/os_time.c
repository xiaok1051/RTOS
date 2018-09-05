

/*暂时先将此函数从外部包含进来吧*/
extern void OSSched(void);
void OSTimeTick (void)
{
	/*任务切换函数*/
	OSSched();
}
