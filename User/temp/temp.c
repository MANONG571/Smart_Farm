#include "./temp/temp.h"

/**
  * @brief  温度传感器初始化,配置GPIO.
	* @param  None.
	* @retval None.
  */
void Temp_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TEMP_GPIO_FUN(TEMP_GPIO_CLOCK, ENABLE);
		
	GPIO_InitStructure.GPIO_Mode = TEMP_GPIO_MODE;
	GPIO_InitStructure.GPIO_Pin = TEMP_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TEMP_GPIO_PORT,&GPIO_InitStructure);
}

/**
  * @brief  读取温湿度数据端口一个字节数据
  * @param  None.
  * @retval 返回一个字节数据
  */
uint8_t Temp_ReadByte(void)
{
	uint8_t i,data=0;
	uint16_t TimeOut;
	for(i=0;i<8;i++)
	{		
//		while(Bit_SET != TEMP_IN);  //等DHT11拉高
		TimeOut=0xFFFF;
		while(TimeOut--)//等DHT11拉高
		{
			if(Bit_SET == TEMP_IN)
				break;
		}
		if(0 == TimeOut) return ERROR;
		
		Delayus_SysTick(40);
		data=data<<1;
		data|=TEMP_IN;	
		
//		while(Bit_RESET != TEMP_IN);//等DHT11拉低
		TimeOut=0xFFFF;
		while(TimeOut--)//等DHT11拉低
		{
			if(Bit_RESET == TEMP_IN)
				break;
		}
		if(0 == TimeOut) return ERROR;
	}
	
	
	return data;
}

/**
  * @brief  读取温湿度数据端口的所有数据
  * @param  TempHumi: 温湿度数据结构体地址,用于存放数据.
  * @retval None.
  */
ErrorStatus Temp_GetValue(TempHumi_Inform* TempHumi) 
{
	TEMP_OUT_LOW;
	Delayms_SysTick(20);
	TEMP_OUT_HIGH;
	
	uint16_t TimeOut=0xFFFF;
	while(TimeOut--)//等DHT11拉低		
	{
		if(Bit_RESET == TEMP_IN)
			break;
	}
	if(0 == TimeOut) return ERROR;
	
	TimeOut=0xFFFF;
	while(TimeOut--)  //等DHT11拉高
	{
		if(Bit_SET == TEMP_IN)
			break;
	}
	if(0 == TimeOut) return ERROR;
	
	TimeOut=0xFFFF;
	while(TimeOut--)  //等DHT11拉低
	{
		if(Bit_RESET == TEMP_IN)
			break;
	}
	if(0 == TimeOut) return ERROR;

//	while(Bit_RESET != TEMP_IN);//等DHT11拉低			
//	while(Bit_SET != TEMP_IN);  //等DHT11拉高
//	while(Bit_RESET != TEMP_IN);//等DHT11拉低
	
	//开始传送数据
	TempHumi->Humi_Integer = Temp_ReadByte();//湿度整数
	TempHumi->Humi_Decimal = Temp_ReadByte();//湿度小数
	TempHumi->Temp_Integer = Temp_ReadByte();//温度整数
	TempHumi->Temp_Decimal = Temp_ReadByte();//温度小数
	TempHumi->Verification = Temp_ReadByte(); //校验位
	
	Delayus_SysTick(60);
	TEMP_OUT_HIGH;
	if(TempHumi->Verification != TempHumi->Humi_Integer+TempHumi->Humi_Decimal+TempHumi->Temp_Integer+TempHumi->Temp_Decimal)
	{
		//Debug("校验和不对\r\n");	
		return ERROR;		
	}	
	
	return SUCCESS;
}


