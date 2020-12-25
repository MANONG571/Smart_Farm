#include "./light/light.h"

// light means light sensor
/**
  * @brief  光照传感器的I2C GPIO端口配置
  * @param  None.
	* @retval None.
  */
void Light_I2C_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//打开I2C的SCL和SDA端口时钟
	RCC_APB2PeriphClockCmd(LIGHT_SCL_GPIO_CLK|LIGHT_SDA_GPIO_CLK, ENABLE);
		
	//SLK端口配置,开漏输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = LIGHT_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LIGHT_SCL_GPIO_PORT,&GPIO_InitStructure);
	
	//SDA端口配置,开漏输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = LIGHT_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LIGHT_SDA_GPIO_PORT,&GPIO_InitStructure);	
	
	Light_I2C_Stop();
	
//	Debug("SCL:%d, SDA:%d\n",LIGHT_SCL_IN,LIGHT_SDA_IN);
}

/**
  * @brief  I2C文件里的延时函数
  * @param  None.
	* @retval None.
  */
static void Light_I2C_Delay(void)
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
ErrorStatus Light_I2C_Start(void)
{
	//页写入时,SCL是1,SDA是0
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
  * @brief  发送停止信号
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
  * @brief  等待应答信号
  * @param  None.
	* @retval ERROR:没有应答
	*         SUCCESS:有应答
  */
ErrorStatus Light_WaitAck(void)
{
	LIGHT_SDA_HIGH;            //释放数据线
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
  * @brief  主设备发送一个应答信号
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
	LIGHT_SDA_HIGH;            //释放数据线
}

/**
  * @brief  读一个字节数据
  * @param  None.
	* @retval None.
  */
void Light_ReadByte(uint8_t* Data)
{
	int8_t bit;
	
	LIGHT_SDA_HIGH;            //释放数据线
	Light_I2C_Delay();
	
	for(bit=7;bit>=0;bit--)  //接收数据
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
  * @brief  发送一个字节数据
  * @param  None.
	* @retval None.
  */
void Light_SendByte(uint8_t Data)
{
	int8_t bit;
	
	for(bit=7;bit>=0;bit--)
	{
		if( Data&(1<<bit) )     //高电平
		{
			LIGHT_SDA_HIGH;
		}
		else                    //低电平
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
  * @brief  发送命令给光照传感器
	* @param  Cmd: 命令
	* @retval ErrorStatus: ERROR:错误
	*                      SUCCESS:成功
  */
ErrorStatus Light_SendCmd(uint8_t Cmd)
{
	if( ERROR == Light_I2C_Start() )  //若总线被占用,启动失败
	{
		Debug("总线已被占用");
		return ERROR;		
	}
	
	Light_SendByte(LIGHT_ADDR_WRITE);   //发送光照传感器从机地址
	
	if( ERROR == Light_WaitAck())  //如果从机没有响应
	{
		Debug("从机没有响应");
		Light_I2C_Stop();
		return ERROR;
	}
	
	Light_SendByte(Cmd);
	
	if( ERROR == Light_WaitAck())  //如果从机没有响应
	{
		Debug("从机没有响应");
		Light_I2C_Stop();
		return ERROR;
	}
	
	Light_I2C_Stop();	
//	Debug("SCL:%d, SDA:%d\n",LIGHT_SCL_IN,LIGHT_SDA_IN);
	return SUCCESS;
}

/**
  * @brief  向光照传感器读取数据
	* @param  Data: 命令
	* @retval ErrorStatus: ERROR:错误
	*                      SUCCESS:成功
  */
ErrorStatus Light_ReadData(uint16_t* Data)
{
	uint8_t DataHigh,DataLow;
	
	if( ERROR == Light_I2C_Start() )  //若总线被占用,启动失败
	{
		Debug("总线已被占用");
		return ERROR;		
	}
	
	Light_SendByte(LIGHT_ADDR_READ);   //发送光照传感器从机地址
	
	if( ERROR == Light_WaitAck())  //如果从机没有响应
	{
		Debug("从机没有响应");
		Light_I2C_Stop();
		return ERROR;
	}
	
	Light_ReadByte(&DataHigh);  //接收高8位数据
	Light_SendAck();            //发送响应信号
	Light_ReadByte(&DataLow);   //接收低8位数据
	Light_I2C_Stop();
	
	*Data = (DataHigh<<8)|DataLow;
	return SUCCESS;
}

/**
  * @brief  采集光照值(转化后)
	* @param  None
	* @retval uint16_t: 光照值(转化后)
  */
uint16_t Light_GetValue(void)
{
	uint16_t light_value;
  float light_compute;
	
	Light_SendCmd(LIGHT_POWER_ON);       //开机
	Light_SendCmd(LIGHT_ONE_H_RESO_MODE);//单次模式
	
	Light_ReadData(&light_value);
	
	light_compute=light_value/1.2;
	
	return (int)light_compute;
//	Debug("测得光照值为%d\n",(int)light_compute);
}


