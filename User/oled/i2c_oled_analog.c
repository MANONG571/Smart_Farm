#include "./oled/i2c_oled_analog.h"

/**
  * @brief  I2C��GPIO�˿�����
  * @param  None.
	* @retval None.
  */
void I2C_OLED_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//��I2C��SCL��SDA�˿�ʱ��
	RCC_APB2PeriphClockCmd(I2C_SCL_GPIO_CLK|I2C_SDA_GPIO_CLK, ENABLE);
		
	//SLK�˿�����,��©���ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_SCL_GPIO_PORT,&GPIO_InitStructure);
//	GPIO_SetBits(I2C_EEPROM_SCL_GPIO_PORT,I2C_EEPROM_SCL_PIN);
	
	//SDA�˿�����,��©���ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_SDA_GPIO_PORT,&GPIO_InitStructure);	
//	GPIO_SetBits(I2C_EEPROM_SDA_GPIO_PORT,I2C_EEPROM_SDA_PIN);
	
	I2C_Stop();
	
}

/**
  * @brief  I2C�ļ������ʱ����
  * @param  None.
	* @retval None.
  */
static void I2C_Delay(void)
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
ErrorStatus I2C_Start(void)
{
	//ҳд��ʱ,SCL��1,SDA��0
	if( 0 == I2C_BUSY_IN )
	{
		return ERROR;
	}
	I2C_SDA_HIGH;
	I2C_SCL_HIGH;
	I2C_Delay();
	I2C_SDA_LOW;
	I2C_Delay();
	I2C_SCL_LOW;
	
	return SUCCESS;
}


/**
  * @brief  �������з��͵ڶ�����ʼ�ź�
  * @param  None.
	* @retval ERROR:  ���߱�ռ��
	*         SUCCESS:����û��ռ��
  */
ErrorStatus I2C_Start_Second(void)
{
	I2C_SDA_HIGH;
	I2C_SCL_HIGH;
	I2C_Delay();
	I2C_SDA_LOW;
	I2C_Delay();
	I2C_SCL_LOW;
	
	return SUCCESS;
}


/**
  * @brief  ����ֹͣ�ź�
  * @param  None.
	* @retval None.
  */
void I2C_Stop(void)
{
	I2C_SDA_LOW;
	I2C_SCL_HIGH;
	I2C_Delay();
	I2C_SDA_HIGH;
}


/**
  * @brief  �ȴ�Ӧ���ź�
  * @param  None.
	* @retval NOACK:û��Ӧ��
	*         ACK:  ��Ӧ��
  */
AckStatus WaitAck(void)
{
	I2C_SDA_HIGH;            //�ͷ�������
	I2C_Delay();
	I2C_SCL_HIGH;
	I2C_Delay();
	if(I2C_SDA_IN)
	{
		I2C_SCL_LOW;
	  I2C_Delay();
		
		return NOACK;
	}
	else
	{
		I2C_SCL_LOW;
	  I2C_Delay();
		return ACK;
	}
}

/**
  * @brief  ���豸����һ��Ӧ���ź�
  * @param  None.
	* @retval None.
  */
void SendAck(void)
{
	I2C_SCL_LOW;
	I2C_SDA_HIGH;
	I2C_Delay();
	I2C_SDA_LOW;
	I2C_Delay();
	I2C_SCL_HIGH;
	I2C_Delay();
	I2C_SCL_LOW;
	I2C_Delay();
	I2C_SDA_HIGH;            //�ͷ�������
}


/**
  * @brief  ��һ���ֽ�����
  * @param  None.
	* @retval None.
  */
void ReadByte(uint8_t* Data)
{
	int8_t bit;
	
	I2C_SDA_HIGH;            //�ͷ�������
	I2C_Delay();
	
	for(bit=7;bit>=0;bit--)  //��������
	{
		I2C_SCL_HIGH;
		if(I2C_SDA_IN)
		{
			*Data |= (1<<bit);
		}
		else
		{
			*Data &= ~(1<<bit);
		}
				
		I2C_Delay();
		I2C_SCL_LOW;
		I2C_Delay();
	}
}

/**
  * @brief  ����һ���ֽ�����
  * @param  None.
	* @retval None.
  */
void SendByte(uint8_t Data)
{
	int8_t bit;
	
	for(bit=7;bit>=0;bit--)
	{
		if( Data&(1<<bit) )     //�ߵ�ƽ
		{
			I2C_SDA_HIGH;
		}
		else                    //�͵�ƽ
		{
			I2C_SDA_LOW;
		}
		
		I2C_Delay();
		I2C_SCL_HIGH;
		I2C_Delay();
		I2C_SCL_LOW;
		I2C_Delay();
	}
	
	I2C_SDA_HIGH;
	
}

