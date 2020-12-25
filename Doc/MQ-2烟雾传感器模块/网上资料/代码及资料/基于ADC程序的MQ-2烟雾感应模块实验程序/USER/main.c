#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"

/**
  ******************************************************************************
  * @file    main.c
  * @author  ��Ə����
  * @date    2015-07-20
  * @brief   ADC ʵ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����III�� ������ ģ�� ����ԭ�� ս�� STM32 ������
  *
  ******************************************************************************
  */


 
 int main(void)
 {	 
    u16 adcx1,adcx2;
	float temp;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();			 	
 	Adc_Init();		  		//ADC��ʼ��

	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(60,70,200,16,16,"ADC TEST");	
	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,200,16,16,"2015/1/14");	
	//��ʾ��ʾ��Ϣ
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	LCD_ShowString(60,130,200,16,16,"ADC_CH0_VAL:");	      
	LCD_ShowString(60,150,200,16,16,"ADC_CH0_VOL:0.000V");
    LCD_ShowString(60,190,200,16,16,"ADC_CH0_VAL:");	      
	LCD_ShowString(60,210,200,16,16,"ADC_CH0_VOL:0.000V");
	while(1)
	{
		adcx1=Get_Adc_Average(ADC_Channel_10,10);
		LCD_ShowxNum(156,130,adcx1,4,16,0);//��ʾADC��ֵ
		temp=(float)adcx1*(3.3/4096);
		adcx1=temp;
		LCD_ShowxNum(156,150,adcx1,1,16,0);//��ʾ��ѹֵ
		temp-=adcx1;
		temp*=1000;
		LCD_ShowxNum(172,150,temp,3,16,0X80);
        
        adcx2=Get_Adc_Average(ADC_Channel_11,10);
		LCD_ShowxNum(156,190,adcx2,4,16,0);//��ʾADC��ֵ
		temp=(float)adcx2*(3.3/4096);
		adcx2=temp;
		LCD_ShowxNum(156,210,adcx2,1,16,0);//��ʾ��ѹֵ
		temp-=adcx2;
		temp*=1000;
		LCD_ShowxNum(172,210,temp,3,16,0X80);
        
		LED1=!LED1;
		delay_ms(250);	
	}
 }

