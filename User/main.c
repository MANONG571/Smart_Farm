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
  * @brief  主函数
  * @param  None
	* @retval None
	* @note   蓝牙模块事先配置好
  */
int main(void)
{
	float somg_value_pre,somg_value_now;
	TempHumi_Inform TempHumi_Pre,TempHumi_Now;
	uint16_t light_value_pre,light_value_now;		
	
	HardwareInit(); //硬件初始化		
	
	printf("初始化成功\r\n");
	
	for(;;)  //死循环
	{				
		
		//烟雾传感器
		somg_value_pre=somg_value_now;
		somg_value_now=Smog_GetValue(); //采集烟雾值
		printf("烟雾:%f\r\n",somg_value_now);		
		
		//光照传感器
		light_value_pre=light_value_now;
		light_value_now=Light_GetValue(); //采集光照值
		printf("光照:%d\r\n",(int)light_value_now);
		printf("\n");
		
		//温度传感器
		TempHumi_Pre = TempHumi_Now;
		if(ERROR == Temp_GetValue(&TempHumi_Now))//采集温湿度值
		{
			Debug("采集温湿度失败");
		}
		printf("湿度:%d%%\r\n",TempHumi_Now.Humi_Integer);
		printf("温度:%d.%d\r\n",TempHumi_Now.Temp_Integer,TempHumi_Now.Temp_Decimal);		
		
		//显示OLED信息
		ShowTemp(TempHumi_Now.Temp_Integer,TempHumi_Now.Temp_Decimal); //显示温度
		ShowHumi(TempHumi_Now.Humi_Integer);                       //显示湿度
  	ShowLight(light_value_now);                                //显示光照
		ShowSmog(somg_value_now);                                  //显示烟雾
		
		/*         控制风扇           */
		if( (TempHumi_Pre.Temp_Integer < Temp_limit)&&(TempHumi_Now.Temp_Integer >= Temp_limit) )  //如果温度第一次超标,开风扇
		{	
			Speaker_Play(6);  //语音 温度警报
			FAN_ON;	
		}
		else if( (TempHumi_Pre.Temp_Integer >= Temp_limit)&&(TempHumi_Now.Temp_Integer < Temp_limit) )  //如果温度第一次不超标,关风扇
		{
			FAN_OFF;
		}
		
		/*           控制灯           */
		if( (light_value_pre>Light_limit)&&(light_value_now<=Light_limit) )       //如果光照值第一次过低,开灯
		{	
			Speaker_Play(7);  //语音 光照警报
			LED_WHITE;
		}
		else if( (light_value_pre<=Light_limit)&&(light_value_now>Light_limit) )  //如果光照值第一次不过低,关灯
		{
			LED_RGBOFF;
		}		
		
		/*         烟雾提醒           */
		if( (somg_value_pre<Smog_limit)&&(somg_value_now>=Smog_limit) )
		{
			Speaker_Play(8);  //语音 烟雾警报
		}
		
		
		Bluetooth_Control();  //处理蓝牙发送来的数据
		
		Bluetooth_Send(TempHumi_Now,somg_value_now,light_value_now); //发送数据给蓝牙
		  
		
		Delays_SysTick(3);
	}
}

/**
  * @brief  硬件初始化
  * @param  None
	* @retval None
  */
void HardwareInit(void)
{
	SysTick_MyConfig();   //滴答定时器
	LED_Config();         //LED
	Debug_USART_Config(); //串口调试
	
	I2C_OLED_Config();    //OLED GPIO
	I2C_OLED_CmdInit();   //OLED 模块
	Temp_GPIO_Config();   //温度传感器	
	Smog_ADC_Config();    //烟雾传感器
	Light_I2C_Config();   //光照传感器
	Fan_Config();         //风扇(电机)
	Speaker_Config();      //语音播报
	BTH_Config();         //蓝牙模块端口
}

/**
  * @brief  接收到蓝牙的信息进行控制
  * @param  None
	* @retval None
  */
void Bluetooth_Control(void)
{
	/* 串口控制蓝牙 */
		if(Debug_Flag == 1)     //如果Debug接收到的数据发送给Bluetooth
		{
			Bluetooth_SendString((char*)Receive);
	    while((USART_GetFlagStatus(BTH_USART,USART_FLAG_TC) == RESET));  //等待数据发送完成
			Debug_Flag=0;
		}
			
		/* 接收蓝牙数据处理部分  */ 
		if(Bluetooth_Flag == 1) //将Bluetooth发送来的数据发给Debug
		{
			Debug_SendString((char*)Receive);
		  while((USART_GetFlagStatus(DEBUG_USART,USART_FLAG_TC) == RESET));
			 
			if( NULL != strstr((char*)Receive,"led0") )  //如果接收到关灯指令
			{
				LED_RGBOFF;
			}
			if( NULL != strstr((char*)Receive,"led1") )  //如果接收到开灯指令
			{
				LED_WHITE;
			}
			if( NULL != strstr((char*)Receive,"fan0") )  //如果接收到开风扇指令
			{
				FAN_OFF;
			}
			if( NULL != strstr((char*)Receive,"fan1") )  //如果接收到关风扇指令
			{
				FAN_ON;
			}			
			
			Bluetooth_Flag=0;
		}
		
//		/* Speaker不会返回数据 */
//		if(Speaker_Flag == 1) //从Speaker接收到数据发送给Debug
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
	* @brief  发送传感器数据给蓝牙
	* @param  TempHumi:温湿度信息
	* @param  Somg:烟雾信息
	* @param  Light:光照信息
	* @retval None
  */
void Bluetooth_Send(TempHumi_Inform TempHumi,float Somg,uint16_t Light)
{
	char SendData[50];
	
	sprintf(SendData,"temp %2d.%2d humi %2d somg %2.2f light %d",TempHumi.Temp_Integer,TempHumi.Temp_Decimal,TempHumi.Humi_Integer,Somg,Light);
	Bluetooth_SendString(SendData);
}

