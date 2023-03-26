#include "config.h"
#include "main.h"

// LCD显示数据
struct data displayData[2] = {{1,1000,10},{2,1000,10}},oldData[2] = {{1,1000,10},{2,1000,10}};
// 显示LCD的顺序
char displayCount = 0;
// 模式控制切换
char contrlMod = 0,flag = 0;

/***************************************
* 函数功能：按键逻辑函数
* 函数参数：无
* 函数返回值：无
***************************************/
static void lcdDisplay(void)
{
	char temp[20];
	
	// 显示名字
	sprintf(temp,"      PA%d         ",displayData[displayCount].name);
	LCD_DisplayStringLine(Line2,(u8*)temp);
	// 显示频率
	sprintf(temp,"    F:%dHz        ",displayData[displayCount].f);
	LCD_DisplayStringLine(Line3,(u8*)temp);
	// 显示占空比
	sprintf(temp,"    D:%.1f%%      ",displayData[displayCount].d);
	LCD_DisplayStringLine(Line4,(u8*)temp);
	
	// 显示测试数据
	sprintf(temp,"%s  %d  %d  %d  ",_ucRxbuff,contrlMod,flag,tim7Count);
	LCD_DisplayStringLine(Line6,(u8*)temp);
}

/***************************************
* 函数功能：按键逻辑函数
* 函数参数：无
* 函数返回值：无
***************************************/
static void changePWM(void)
{
	//设置重装载值
	if(oldData[0].f != displayData[0].f)
	{
		__HAL_TIM_SetAutoreload(&htim2,displayData[0].f-1);
		oldData[0].f = displayData[0].f;
	}
	if(oldData[1].f != displayData[1].f)
	{
		__HAL_TIM_SetAutoreload(&htim3,displayData[1].f-1);
		oldData[1].f = displayData[1].f;
	}
	
	//设置比较值 用于得到占空比
	if(oldData[0].d != displayData[0].d)
	{
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,displayData[0].f*displayData[0].d/100);
		oldData[0].d = displayData[0].d;
	}
	if(oldData[1].d != displayData[1].d)
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,displayData[1].f*displayData[1].d/100);
		oldData[1].d = displayData[1].d;
	}
}

/***************************************
* 函数功能：按键逻辑函数
* 函数参数：无
* 函数返回值：无
***************************************/
static void keyPro(void)
{
	// 按键扫描
	unsigned char keyValue = scanKey();
	switch(keyValue)
	{
		// B1 频率增加
		case 1:
			if(contrlMod) return ;
			displayData[displayCount].f += 1000;
			if( displayData[displayCount].f == 11000 )
					displayData[displayCount].f = 1000;
			break;
		// B2 占空比增加
		case 2:
			if(contrlMod) return ;
			displayData[displayCount].d += 10;
			if( displayData[displayCount].d == 100 )
				displayData[displayCount].d = 10;
			break;
		// B3 切换PA1 PA7界面
		case 3:
			if(contrlMod) return ;
			displayCount ^= 1;
			break;
		// B4 切换模式
		case 4:
			contrlMod ^= 1;
			break;
		default :break;
	}
}

/***************************************
* 函数功能：串口逻辑函数
* 函数参数：无
* 函数返回值：无
***************************************/
static void usartPro(void)
{
	// 判断是否接收到串口数据
	if(strlen((char*)_ucRxbuff) == 0)
		return ;
	
	// 按键控制模式下接收到串口数据
	if(contrlMod == 0) 
		HAL_UART_Transmit(&huart1,(uint8_t*)"KEY CONTROL\r\n",sizeof("KEY CONTROL\r\n"),20); //  HAL_UART_Transmit_DMA
	
	// 串口模式下接收到数据
	if(contrlMod == 1)
	{
		if(strcmp((char*)_ucRxbuff,"@") == 0)
			displayCount = 0;
		else if(strcmp((char*)_ucRxbuff,"#") == 0)
			displayCount = 1;
		else 
			HAL_UART_Transmit(&huart1,(uint8_t*)"ERROR\r\n",sizeof("ERROR\r\n"),20); //  HAL_UART_Transmit
	}
	memset(_ucRxbuff,0,sizeof((char*)_ucRxbuff));
}

/***************************************
* 函数功能：LED显示逻辑函数
* 函数参数：无
* 函数返回值：无
***************************************/
static void ledPro(void)
{
//	static char flag = 0;
	// PA1数据界面  LED1点亮
	if(displayCount == 0 && flag != 1)
		changeLedStateByLocation(LED1,1);
	else if(displayCount == 1 && flag != 1)
		changeLedStateByLocation(LED1,0);
	// PA7数据界面  LED2点亮
	if(displayCount == 1 && flag != 2)
		changeLedStateByLocation(LED2,1);
	else if(displayCount == 0 && flag != 2)
		changeLedStateByLocation(LED2,0);
	// 按键控制模式下
	if(contrlMod == 0)
		changeLedStateByLocation(LED3,1);
	else
		changeLedStateByLocation(LED3,0);
	// PA1频率大于PA7  LED1以0.1秒闪烁
	if(displayData[0].f > displayData[1].f)
		flag = 1;
	// PA1频率小于于PA7  LED2以0.1秒闪烁
	else if(displayData[0].f < displayData[1].f)
		flag = 2;
	else
		flag = 0;
	// 对LED1 LED2进行闪烁
	if(flag == 1 && tim7Count%100)
		rollbackLedByLocation(LED1);
	if(flag == 2 && tim7Count%100)
		rollbackLedByLocation(LED2);
}

/***************************************
* 函数功能：系统自定义初始化函数
* 函数参数：无
* 函数返回值：无
***************************************/
void sysInit(void)
{
	// LCD初始化函数
	lcdInit();
	// 关闭所有LED
	changeAllLedByStateNumber(0);
	//打开定时器7中断
	HAL_TIM_Base_Start_IT(&htim7);
	// 打开定时器的PWM
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	// 开启串口中断接收
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
}

/***************************************
* 函数功能：系统自定义初始化函数
* 函数参数：无
* 函数返回值：无
***************************************/
void sysWork(void)
{
	// 按键逻辑
	keyPro();
	// 改变PWM的状态
	changePWM();
	// LCD显示逻辑
	lcdDisplay();
	// 串口逻辑
	usartPro();
	// LED逻辑
	ledPro();
}


