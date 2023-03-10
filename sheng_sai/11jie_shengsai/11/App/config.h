#ifndef _CONFIG_H
#define _CONFIG_H

#include "main.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "myadc.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

//����һ����ʾ���ݵĽṹ��
struct displayData{
	//������ʾ�Ľ���
	int page;	
	//��¼ģʽ
	int mode;
	//��¼��ѹֵ
	double vValue;
	//��¼PA6ռ�ձ�
	int frdP6;
	//��¼PA7ռ�ձ�
	int frdP7;
};


//�����ⲿ����ı���
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim17;
extern ADC_HandleTypeDef hadc2;;
/***********************************************
* �������ܣ��Զ����ϵͳ��ʼ������
* ������������
* ��������ֵ����
***********************************************/
void sysInit(void);

/***********************************************
* �������ܣ��Զ����ϵͳ��������
* ������������
* ��������ֵ����
***********************************************/
void sysWork(void);

/***********************************************
* �������ܣ���ʾ����
* ������������
* ��������ֵ����
***********************************************/
void display(void);

/***********************************************
* �������ܣ�����PWMռ�ձȺ���
* ������������
* ��������ֵ����
***********************************************/
void changePwmFrd(void);

/***********************************************
* �������ܣ�LED��������
* ������������
* ��������ֵ����
***********************************************/
void ledWork(void);


#endif