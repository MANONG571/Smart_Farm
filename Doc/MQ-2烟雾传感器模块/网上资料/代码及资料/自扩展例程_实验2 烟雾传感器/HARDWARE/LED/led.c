#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//LED IO��ʼ��
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	//����һ��GPIO_InitTypeDef���͵Ľṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	//����LED������ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	//ѡ��Ҫ���Ƶ�GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//��������ģʽΪͨ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //������������Ϊ50MHz
	GPIO_Init(GPIOF, &GPIO_InitStructure);	//���ÿ⺯������ʼ��GPIO

	GPIO_SetBits(GPIOF, GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);	//�ر�LED1��LED2��LED3��LED4��
}
 
