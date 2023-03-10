#include "config.h"

/*****************************************************************
* @sysWork：项目的主要功能初始化函数
* @arg:
*		无
* @return:
*		无
*****************************************************************/
void sysInit(void)
{
	
}

/*****************************************************************
* @sysWork：项目的主要功能逻辑函数
* @arg:
*		无
* @return:
*		无
*****************************************************************/
void sysWork(void)
{
	static char count = 0;
	segBuff[0] = scanBtn();
	// 需要间隔一段时间再刷新 否则数码管上会出现重影 但是实际开发中应该不需要 因为此处时间应该没有10us 当一个工程较大人任务较多 就完全不需要隔间刷新了
	if(count++ % 10 ==0)
		segDisplay();
}

