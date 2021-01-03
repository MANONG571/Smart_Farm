#include "./temp/temp.h"

/**
  * @brief  �¶ȴ�������ʼ��,����GPIO.
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
  * @brief  ��ȡ��ʪ�����ݶ˿�һ���ֽ�����
  * @param  None.
  * @retval ����һ���ֽ�����
  */
uint8_t Temp_ReadByte(void)
{
	uint8_t i,data=0;
	uint16_t TimeOut;
	for(i=0;i<8;i++)
	{		
//		while(Bit_SET != TEMP_IN);  //��DHT11����
		TimeOut=0xFFFF;
		while(TimeOut--)//��DHT11����
		{
			if(Bit_SET == TEMP_IN)
				break;
		}
		if(0 == TimeOut) return ERROR;
		
		Delayus_SysTick(40);
		data=data<<1;
		data|=TEMP_IN;	
		
//		while(Bit_RESET != TEMP_IN);//��DHT11����
		TimeOut=0xFFFF;
		while(TimeOut--)//��DHT11����
		{
			if(Bit_RESET == TEMP_IN)
				break;
		}
		if(0 == TimeOut) return ERROR;
	}
	
	
	return data;
}

/**
  * @brief  ��ȡ��ʪ�����ݶ˿ڵ���������
  * @param  TempHumi: ��ʪ�����ݽṹ���ַ,���ڴ������.
  * @retval None.
  */
ErrorStatus Temp_GetValue(TempHumi_Inform* TempHumi) 
{
	TEMP_OUT_LOW;
	Delayms_SysTick(20);
	TEMP_OUT_HIGH;
	
	uint16_t TimeOut=0xFFFF;
	while(TimeOut--)//��DHT11����		
	{
		if(Bit_RESET == TEMP_IN)
			break;
	}
	if(0 == TimeOut) return ERROR;
	
	TimeOut=0xFFFF;
	while(TimeOut--)  //��DHT11����
	{
		if(Bit_SET == TEMP_IN)
			break;
	}
	if(0 == TimeOut) return ERROR;
	
	TimeOut=0xFFFF;
	while(TimeOut--)  //��DHT11����
	{
		if(Bit_RESET == TEMP_IN)
			break;
	}
	if(0 == TimeOut) return ERROR;

//	while(Bit_RESET != TEMP_IN);//��DHT11����			
//	while(Bit_SET != TEMP_IN);  //��DHT11����
//	while(Bit_RESET != TEMP_IN);//��DHT11����
	
	//��ʼ��������
	TempHumi->Humi_Integer = Temp_ReadByte();//ʪ������
	TempHumi->Humi_Decimal = Temp_ReadByte();//ʪ��С��
	TempHumi->Temp_Integer = Temp_ReadByte();//�¶�����
	TempHumi->Temp_Decimal = Temp_ReadByte();//�¶�С��
	TempHumi->Verification = Temp_ReadByte(); //У��λ
	
	Delayus_SysTick(60);
	TEMP_OUT_HIGH;
	if(TempHumi->Verification != TempHumi->Humi_Integer+TempHumi->Humi_Decimal+TempHumi->Temp_Integer+TempHumi->Temp_Decimal)
	{
		//Debug("У��Ͳ���\r\n");	
		return ERROR;		
	}	
	
	return SUCCESS;
}


