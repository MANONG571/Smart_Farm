#include "./oled/i2c_oled_analog.h"

/**
  * @brief  I2C的GPIO端口配置
  * @param  None.
	* @retval None.
  */
void I2C_OLED_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//打开I2C的SCL和SDA端口时钟
	RCC_APB2PeriphClockCmd(I2C_SCL_GPIO_CLK|I2C_SDA_GPIO_CLK, ENABLE);
		
	//SLK端口配置,开漏输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_SCL_GPIO_PORT,&GPIO_InitStructure);
//	GPIO_SetBits(I2C_EEPROM_SCL_GPIO_PORT,I2C_EEPROM_SCL_PIN);
	
	//SDA端口配置,开漏输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_SDA_GPIO_PORT,&GPIO_InitStructure);	
//	GPIO_SetBits(I2C_EEPROM_SDA_GPIO_PORT,I2C_EEPROM_SDA_PIN);
	
	I2C_Stop();
	
}

/**
  * @brief  I2C文件里的延时函数
  * @param  None.
	* @retval None.
  */
static void I2C_Delay(void)
{
	uint8_t time;
	
	for(time=0;time<10;time--);
}


/**
  * @brief  发送起始信号
  * @param  None.
	* @retval ERROR:  总线被占用
	*         SUCCESS:总线没被占用
  */
ErrorStatus I2C_Start(void)
{
	//页写入时,SCL是1,SDA是0
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
  * @brief  读操作中发送第二次起始信号
  * @param  None.
	* @retval ERROR:  总线被占用
	*         SUCCESS:总线没被占用
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
  * @brief  发送停止信号
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
  * @brief  等待应答信号
  * @param  None.
	* @retval NOACK:没有应答
	*         ACK:  有应答
  */
AckStatus WaitAck(void)
{
	I2C_SDA_HIGH;            //释放数据线
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
  * @brief  主设备发送一个应答信号
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
	I2C_SDA_HIGH;            //释放数据线
}


/**
  * @brief  读一个字节数据
  * @param  None.
	* @retval None.
  */
void ReadByte(uint8_t* Data)
{
	int8_t bit;
	
	I2C_SDA_HIGH;            //释放数据线
	I2C_Delay();
	
	for(bit=7;bit>=0;bit--)  //接收数据
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
  * @brief  发送一个字节数据
  * @param  None.
	* @retval None.
  */
void SendByte(uint8_t Data)
{
	int8_t bit;
	
	for(bit=7;bit>=0;bit--)
	{
		if( Data&(1<<bit) )     //高电平
		{
			I2C_SDA_HIGH;
		}
		else                    //低电平
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

