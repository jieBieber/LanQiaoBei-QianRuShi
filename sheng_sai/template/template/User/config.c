#include "config.h"
#include "main.h"

/************************该.c文件用于写系统初始化以及系统初始化后的主逻辑***********************************/


/**********************************************
* 函数功能：系统初始化函数
* 函数参数：无
* 函数返回值：无
**********************************************/
void sysInit(void)
{
	//LED的初始化
	ledInit();
}

/**********************************************
* 函数功能：系统主逻辑函数(由于主函数中含while(1) 此处不在含有while(1))
* 函数参数：无
* 函数返回值：无
***********************************************/
void sysWork(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8,GPIO_PIN_RESET);
}





