#include "./light/light.h"

// light means light sensor
/**
  * @brief  ���մ�������I2C GPIO�˿�����
  * @param  None.
	* @retval None.
  */
void Light_I2C_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//��I2C��SCL��SDA�˿�ʱ��
	RCC_APB2PeriphClockCmd(LIGHT_SCL_GPIO_CLK|LIGHT_SDA_GPIO_CLK, ENABLE);
		
	//SLK�˿�����,��©���ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = LIGHT_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LIGHT_SCL_GPIO_PORT,&GPIO_InitStructure);
	
	//SDA�˿�����,��©���ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = LIGHT_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LIGHT_SDA_GPIO_PORT,&GPIO_InitStructure);	
	
	Light_I2C_Stop();
	
//	Debug("SCL:%d, SDA:%d\n",LIGHT_SCL_IN,LIGHT_SDA_IN);
}

/**
  * @brief  I2C�ļ������ʱ����
  * @param  None.
	* @retval None.
  */
static void Light_I2C_Delay(void)
{
	uint8_t time;
	
	for(time=0;time<10;time--);
}

/**
  * @brief  ������ʼ�ź�
  * @param  None.
	* @retval ERROR:  ���߱�ռ��
	*         SUCCESS:����û��ռ��
  */
ErrorStatus Light_I2C_Start(void)
{
	//ҳд��ʱ,SCL��1,SDA��0
	if( 0 == LIGHT_BUSY_IN )
	{
		return ERROR;
	}
	LIGHT_SDA_HIGH;
	LIGHT_SCL_HIGH;
	Light_I2C_Delay();
	LIGHT_SDA_LOW;
	Light_I2C_Delay();
	LIGHT_SCL_LOW;
	
	return SUCCESS;
}

/**
  * @brief  ����ֹͣ�ź�
  * @param  None.
	* @retval None.
  */
void Light_I2C_Stop(void)
{
	LIGHT_SDA_LOW;
	LIGHT_SCL_HIGH;
	Light_I2C_Delay();
	LIGHT_SDA_HIGH;
}

/**
  * @brief  �ȴ�Ӧ���ź�
  * @param  None.
	* @retval ERROR:û��Ӧ��
	*         SUCCESS:��Ӧ��
  */
ErrorStatus Light_WaitAck(void)
{
	LIGHT_SDA_HIGH;            //�ͷ�������
	Light_I2C_Delay();
	LIGHT_SCL_HIGH;
	Light_I2C_Delay();
	if(LIGHT_SDA_IN)
	{
		LIGHT_SCL_LOW;
	  Light_I2C_Delay();
		
		return ERROR;
	}
	else
	{
		LIGHT_SCL_LOW;
	  Light_I2C_Delay();
		return SUCCESS;
	}
}

/**
  * @brief  ���豸����һ��Ӧ���ź�
  * @param  None.
	* @retval None.
  */
void Light_SendAck(void)
{
	LIGHT_SCL_LOW;
	LIGHT_SDA_HIGH;
	Light_I2C_Delay();
	LIGHT_SDA_LOW;
	Light_I2C_Delay();
	LIGHT_SCL_HIGH;
	Light_I2C_Delay();
	LIGHT_SCL_LOW;
	Light_I2C_Delay();
	LIGHT_SDA_HIGH;            //�ͷ�������
}

/**
  * @brief  ��һ���ֽ�����
  * @param  None.
	* @retval None.
  */
void Light_ReadByte(uint8_t* Data)
{
	int8_t bit;
	
	LIGHT_SDA_HIGH;            //�ͷ�������
	Light_I2C_Delay();
	
	for(bit=7;bit>=0;bit--)  //��������
	{
		LIGHT_SCL_HIGH;
		if(LIGHT_SDA_IN)
		{
			*Data |= (1<<bit);
		}
		else
		{
			*Data &= ~(1<<bit);
		}
				
		Light_I2C_Delay();
		LIGHT_SCL_LOW;
		Light_I2C_Delay();
	}
}

/**
  * @brief  ����һ���ֽ�����
  * @param  None.
	* @retval None.
  */
void Light_SendByte(uint8_t Data)
{
	int8_t bit;
	
	for(bit=7;bit>=0;bit--)
	{
		if( Data&(1<<bit) )     //�ߵ�ƽ
		{
			LIGHT_SDA_HIGH;
		}
		else                    //�͵�ƽ
		{
			LIGHT_SDA_LOW;
		}
		
		Light_I2C_Delay();
		LIGHT_SCL_HIGH;
		Light_I2C_Delay();
		LIGHT_SCL_LOW;
		Light_I2C_Delay();
	}
	
	LIGHT_SDA_HIGH;
	
}

/**
  * @brief  ������������մ�����
	* @param  Cmd: ����
	* @retval ErrorStatus: ERROR:����
	*                      SUCCESS:�ɹ�
  */
ErrorStatus Light_SendCmd(uint8_t Cmd)
{
	if( ERROR == Light_I2C_Start() )  //�����߱�ռ��,����ʧ��
	{
		Debug("�����ѱ�ռ��");
		return ERROR;		
	}
	
	Light_SendByte(LIGHT_ADDR_WRITE);   //���͹��մ������ӻ���ַ
	
	if( ERROR == Light_WaitAck())  //����ӻ�û����Ӧ
	{
		Debug("�ӻ�û����Ӧ");
		Light_I2C_Stop();
		return ERROR;
	}
	
	Light_SendByte(Cmd);
	
	if( ERROR == Light_WaitAck())  //����ӻ�û����Ӧ
	{
		Debug("�ӻ�û����Ӧ");
		Light_I2C_Stop();
		return ERROR;
	}
	
	Light_I2C_Stop();	
//	Debug("SCL:%d, SDA:%d\n",LIGHT_SCL_IN,LIGHT_SDA_IN);
	return SUCCESS;
}

/**
  * @brief  ����մ�������ȡ����
	* @param  Data: ����
	* @retval ErrorStatus: ERROR:����
	*                      SUCCESS:�ɹ�
  */
ErrorStatus Light_ReadData(uint16_t* Data)
{
	uint8_t DataHigh,DataLow;
	
	if( ERROR == Light_I2C_Start() )  //�����߱�ռ��,����ʧ��
	{
		Debug("�����ѱ�ռ��");
		return ERROR;		
	}
	
	Light_SendByte(LIGHT_ADDR_READ);   //���͹��մ������ӻ���ַ
	
	if( ERROR == Light_WaitAck())  //����ӻ�û����Ӧ
	{
		Debug("�ӻ�û����Ӧ");
		Light_I2C_Stop();
		return ERROR;
	}
	
	Light_ReadByte(&DataHigh);  //���ո�8λ����
	Light_SendAck();            //������Ӧ�ź�
	Light_ReadByte(&DataLow);   //���յ�8λ����
	Light_I2C_Stop();
	
	*Data = (DataHigh<<8)|DataLow;
	return SUCCESS;
}

/**
  * @brief  �ɼ�����ֵ(ת����)
	* @param  None
	* @retval uint16_t: ����ֵ(ת����)
  */
uint16_t Light_GetValue(void)
{
	uint16_t light_value;
  float light_compute;
	
	Light_SendCmd(LIGHT_POWER_ON);       //����
	Light_SendCmd(LIGHT_ONE_H_RESO_MODE);//����ģʽ
	
	Light_ReadData(&light_value);
	
	light_compute=light_value/1.2;
	
	return (int)light_compute;
//	Debug("��ù���ֵΪ%d\n",(int)light_compute);
}


