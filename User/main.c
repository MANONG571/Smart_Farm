#include "stm32f10x.h"
#include "./systick/systick.h"
#include "./debug/debug.h"
#include "./led/led.h"
#include "./fan/fan.h"
#include "./oled/i2c_oled_analog.h"
#include "./oled/i2c_oled.h"
#include "./light/light.h"
#include "./smog/smog.h"
#include "./temp/temp.h"
#include "./speaker/speaker.h"
#include "./bluetooth/bluetooth.h"

#define  Temp_limit    23
#define  Light_limit   200
#define  Smog_limit    1.8

uint8_t Receive[20];

__IO uint8_t Debug_Flag=0;
__IO uint8_t Bluetooth_Flag=0;
__IO uint8_t Speaker_Flag=0;


void HardwareInit(void);
void Bluetooth_Control(void);
void Bluetooth_Send(TempHumi_Inform TempHumi,float Somg,uint16_t Light);

/**
  * @brief  ������
  * @param  None
	* @retval None
	* @note   ����ģ���������ú�
  */
int main(void)
{
	float somg_value_pre,somg_value_now;
	TempHumi_Inform TempHumi_Pre,TempHumi_Now;
	uint16_t light_value_pre,light_value_now;		
	
	HardwareInit(); //Ӳ����ʼ��		
	
	printf("��ʼ���ɹ�\r\n");
	
	for(;;)  //��ѭ��
	{				
		
		//��������
		somg_value_pre=somg_value_now;
		somg_value_now=Smog_GetValue(); //�ɼ�����ֵ
		printf("����:%f\r\n",somg_value_now);		
		
		//���մ�����
		light_value_pre=light_value_now;
		light_value_now=Light_GetValue(); //�ɼ�����ֵ
		printf("����:%d\r\n",(int)light_value_now);
		printf("\n");
		
		//�¶ȴ�����
		TempHumi_Pre = TempHumi_Now;
		if(ERROR == Temp_GetValue(&TempHumi_Now))//�ɼ���ʪ��ֵ
		{
			Debug("�ɼ���ʪ��ʧ��");
		}
		printf("ʪ��:%d%%\r\n",TempHumi_Now.Humi_Integer);
		printf("�¶�:%d.%d\r\n",TempHumi_Now.Temp_Integer,TempHumi_Now.Temp_Decimal);		
		
		//��ʾOLED��Ϣ
		ShowTemp(TempHumi_Now.Temp_Integer,TempHumi_Now.Temp_Decimal); //��ʾ�¶�
		ShowHumi(TempHumi_Now.Humi_Integer);                       //��ʾʪ��
  	ShowLight(light_value_now);                                //��ʾ����
		ShowSmog(somg_value_now);                                  //��ʾ����
		
		/*         ���Ʒ���           */
		if( (TempHumi_Pre.Temp_Integer < Temp_limit)&&(TempHumi_Now.Temp_Integer >= Temp_limit) )  //����¶ȵ�һ�γ���,������
		{	
			Speaker_Play(6);  //���� �¶Ⱦ���
			FAN_ON;	
		}
		else if( (TempHumi_Pre.Temp_Integer >= Temp_limit)&&(TempHumi_Now.Temp_Integer < Temp_limit) )  //����¶ȵ�һ�β�����,�ط���
		{
			FAN_OFF;
		}
		
		/*           ���Ƶ�           */
		if( (light_value_pre>Light_limit)&&(light_value_now<=Light_limit) )       //�������ֵ��һ�ι���,����
		{	
			Speaker_Play(7);  //���� ���վ���
			LED_WHITE;
		}
		else if( (light_value_pre<=Light_limit)&&(light_value_now>Light_limit) )  //�������ֵ��һ�β�����,�ص�
		{
			LED_RGBOFF;
		}		
		
		/*         ��������           */
		if( (somg_value_pre<Smog_limit)&&(somg_value_now>=Smog_limit) )
		{
			Speaker_Play(8);  //���� ������
		}
		
		
		Bluetooth_Control();  //��������������������
		
		Bluetooth_Send(TempHumi_Now,somg_value_now,light_value_now); //�������ݸ�����
		  
		
		Delays_SysTick(3);
	}
}

/**
  * @brief  Ӳ����ʼ��
  * @param  None
	* @retval None
  */
void HardwareInit(void)
{
	SysTick_MyConfig();   //�δ�ʱ��
	LED_Config();         //LED
	Debug_USART_Config(); //���ڵ���
	
	I2C_OLED_Config();    //OLED GPIO
	I2C_OLED_CmdInit();   //OLED ģ��
	Temp_GPIO_Config();   //�¶ȴ�����	
	Smog_ADC_Config();    //��������
	Light_I2C_Config();   //���մ�����
	Fan_Config();         //����(���)
	Speaker_Config();      //��������
	BTH_Config();         //����ģ��˿�
}

/**
  * @brief  ���յ���������Ϣ���п���
  * @param  None
	* @retval None
  */
void Bluetooth_Control(void)
{
	/* ���ڿ������� */
		if(Debug_Flag == 1)     //���Debug���յ������ݷ��͸�Bluetooth
		{
			Bluetooth_SendString((char*)Receive);
	    while((USART_GetFlagStatus(BTH_USART,USART_FLAG_TC) == RESET));  //�ȴ����ݷ������
			Debug_Flag=0;
		}
			
		/* �����������ݴ�����  */ 
		if(Bluetooth_Flag == 1) //��Bluetooth�����������ݷ���Debug
		{
			Debug_SendString((char*)Receive);
		  while((USART_GetFlagStatus(DEBUG_USART,USART_FLAG_TC) == RESET));
			 
			if( NULL != strstr((char*)Receive,"led0") )  //������յ��ص�ָ��
			{
				LED_RGBOFF;
			}
			if( NULL != strstr((char*)Receive,"led1") )  //������յ�����ָ��
			{
				LED_WHITE;
			}
			if( NULL != strstr((char*)Receive,"fan0") )  //������յ�������ָ��
			{
				FAN_OFF;
			}
			if( NULL != strstr((char*)Receive,"fan1") )  //������յ��ط���ָ��
			{
				FAN_ON;
			}			
			
			Bluetooth_Flag=0;
		}
		
//		/* Speaker���᷵������ */
//		if(Speaker_Flag == 1) //��Speaker���յ����ݷ��͸�Debug
//		{
//			for(i=0;i<6;i++)
//			{
//				Debug_SendByte(Receive[i]);//Speaker_SendByte
//			} 
//			while((USART_GetFlagStatus(DEBUG_USART,USART_FLAG_TC) == RESET));
//			Speaker_Flag=0;
//		}
}

/**
	* @brief  ���ʹ��������ݸ�����
	* @param  TempHumi:��ʪ����Ϣ
	* @param  Somg:������Ϣ
	* @param  Light:������Ϣ
	* @retval None
  */
void Bluetooth_Send(TempHumi_Inform TempHumi,float Somg,uint16_t Light)
{
	char SendData[50];
	
	sprintf(SendData,"temp %2d.%2d humi %2d somg %2.2f light %d",TempHumi.Temp_Integer,TempHumi.Temp_Decimal,TempHumi.Humi_Integer,Somg,Light);
	Bluetooth_SendString(SendData);
}

