#ifndef __KEY_H
#define __KEY_H

#include "main.h"

//控制按键B4初始化 1-初始化 0-不初始化
#define KEYPA 1

//控制按键B1-B3初始化 1-初始化 0-不初始化
#define KEYPB 1
//设置需要初始化的GPIO的引脚
#define KEYGPIOB GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2

//systick毫秒级定时器触发的次数
extern uint16_t uTick_ms;

/*********************************
 * 函数功能：按键的初始化函数
 * 函数参数：无
 * 函数返回值：无
*********************************/
void keyInit(void);

/********************************************
 * 函数功能：按键扫描 含按键消抖 无长按短按设计
 * 函数参数：无
 * 函数返回值：按键的位置
 *            返回值说明：B1-1 B2-2 B3-3 B4-4
*/
unsigned char scanKey(void);

/******************************************************************************
 * 函数功能：判断按键是长按还是短按
 * 函数参数：无
 * 函数返回值：按键的值以及长按（长按在原本的数值上加10） 还是短按（不做任何处理）
******************************************************************************/
char getKeyNumberByTime(void);

#endif
