#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"


 
int main()
{
	
	float temp;
	u16 adcx;
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);	 	//串口初始化为9600
  Adc_Init();		  		//ADC初始化	    
	while(1)	
  {
		adcx=Get_Adc_Average(ADC_Channel_11,10);
		//LCD_ShowxNum(156,130,adcx,4,16,0);//显示ADC的值
		temp=(float)adcx*(3.3/4096);
		printf("value:%f\n",temp);
	  delay_ms(1500);	
  }
}
