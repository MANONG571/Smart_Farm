#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "smog.h"
 
/**
  ******************************************************************************
  * @file    main.c
  * @author  ��Ə����
  * @date    2015-07-22
  * @brief   ��������ʵ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����III�� ������ ģ�� ����ԭ�� ս�� STM32 ������
  *
  ******************************************************************************
  */

int main(void)
{
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	Smog_Init();
	while(1)
	{
		Smog_Trans_Concentration();
        delay_ms(300);
	}
}

