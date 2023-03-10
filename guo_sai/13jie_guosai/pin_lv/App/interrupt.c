#include "config.h"

/***ʹ��HAL_UART_Receive_IT�жϽ������� ÿ�ν���������ݺ�ͻ�ִ�иú���***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
	if(huart->Instance == USART1){
		// ����ʹ���ж�
		if(ucRxbuff[0]=='P' && _ucRxbuff[0]=='A') 
			ucRxbuff[1] = _ucRxbuff[0];
		else if((ucRxbuff[0]=='P' && ucRxbuff[1]=='A') && (_ucRxbuff[0]=='1' || _ucRxbuff[0]=='4' || _ucRxbuff[0]=='5'))
			ucRxbuff[2] = _ucRxbuff[0];
		else
			ucRxbuff[0] = _ucRxbuff[0];
		HAL_UART_Receive_IT(huart,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff));
	}
}

//���벶��ص�����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint32_t temp = 0;
	//�����жϵĶ�ʱ��Ϊ��ʱ��2ͨ��2
	if(htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		//��ȡ��ʱ���ļ���ֵ
		temp = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
		//����ʱ���ļ���ֵ���ó�0
		__HAL_TIM_SetCounter(htim,0);
		//����Ƶ��
		P1 = (80000000/80)/temp;
		//Ƶ��<100ʱ ǯ����100
		if(1000000/P1>10000)
			P1=100;
		//Ƶ��>10kʱ ǯ����10k
		else if(1000000/P1<100)
			P1=10000;
		//���¿�����ʱ��
		HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_2);
	}
}

extern unsigned int uiTime7Count ;

/***������ģʽ�¶�ʱ���жϻص�����***/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//��ʱ��7���жϻص�����
	if(htim->Instance == TIM7)
	{
		//ͨ�ü�ʱ
		if(++uiTime7Count%100 == 0)
			LED3TimeFlag = 1;
	}
}