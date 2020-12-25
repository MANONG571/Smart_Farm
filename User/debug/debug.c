#include "./debug/debug.h"

/**
  * @brief  stm32与电脑串口调试助手配置.
	* @param  None.
	* @retval None.
  */
void Debug_USART_Config(void)
{
	USART_GPIO_Config();
	NVIC_Config();
	USART_USART_Config();
}

/**
  * @brief  串口调试中的GPIO配置.
	* @param  None.
	* @retval None.
  */
static void USART_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//USART1初始化
	RCC_APB2PeriphClockCmd(USART1_GPIO_CLK, ENABLE);
	
	//接收端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = USART1_GPIO_RX_PIN;
	GPIO_Init(USART1_GPIO_RX_RPORT,&GPIO_InitStructure);
	
	//发送端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = USART1_GPIO_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_GPIO_TX_PORT,&GPIO_InitStructure);
		
}

/**
  * @brief  串口调试中的USART配置.
	* @param  None.
	* @retval None.
  */
static void USART_USART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	DEBUG_USART_CLK_FUN(DEBUG_USART_CLK, ENABLE);
	
	USART_InitStructure.USART_BaudRate=DEBUG_BaudRate;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	//开启中断使能
	USART_ITConfig(DEBUG_USART,USART_IT_RXNE,ENABLE);
	USART_ITConfig(DEBUG_USART,USART_IT_IDLE,ENABLE);
	
	//使能USART1
	USART_Cmd(DEBUG_USART,ENABLE);
}

/**
  * @brief  串口调试中的NVIC配置.
	* @param  None.
	* @retval None.
  */
static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel=DEBUG_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  向指定串口发送一个字节数据.
  * @param  USARTx: 指定的USART串口.
  * @param  Data: 要传送的数据.
	* @retval None.
  */
void Debug_SendByte(uint16_t Data)
{
	USART_SendData(DEBUG_USART,Data);
	while( (USART_GetFlagStatus(DEBUG_USART,USART_FLAG_TXE) == RESET) );
}

/**
  * @brief  向指定串口发送一个字符串.
  * @param  USARTx: 指定的USART串口.
  * @param  Data: 要传送的数据地址.
	* @retval None.
  */
void Debug_SendString(char* Data)
{
	uint8_t i;
	
	for(i=0;Data[i]!='\0';i++)
	{
		Debug_SendByte(Data[i]);
	}
	while((USART_GetFlagStatus(DEBUG_USART,USART_FLAG_TC) == RESET));
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

