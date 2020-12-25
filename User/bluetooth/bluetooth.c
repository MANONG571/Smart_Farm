#include "./bluetooth/bluetooth.h"

/**
  * @brief  蓝牙端口配置.
	* @param  None.
	* @retval None.
  */
void BTH_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//开启时钟
	BTH_GPIO_CLK_FUN(BTH_GPIO_CLK, ENABLE);
	BTH_USART_CLK_FUN(BTH_USART_CLK, ENABLE);
	
	//中断优先级配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel=BTH_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	//接收端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = BTH_GPIO_RX_PIN;
	GPIO_Init(BTH_GPIO_RX_RPORT,&GPIO_InitStructure);
	
	//发送端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = BTH_GPIO_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BTH_GPIO_TX_PORT,&GPIO_InitStructure);
	
	//KEY模式端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BTH_GPIO_KEY_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BTH_GPIO_KEY_RPORT,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=BTH_BaudRate;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(BTH_USART,&USART_InitStructure);
	
	//开启中断使能
	USART_ITConfig(BTH_USART,USART_IT_RXNE,ENABLE);
	USART_ITConfig(BTH_USART,USART_IT_IDLE,ENABLE);
	
	//使能USART3
	USART_Cmd(BTH_USART,ENABLE);
}

/**
	* @brief  向蓝牙模块发送一个字节数据.
	* @param  Data:字节数据
	* @retval None.
  */
void Bluetooth_SendByte(uint8_t Data)
{
	USART_SendData(BTH_USART,Data);
	while( (USART_GetFlagStatus(BTH_USART,USART_FLAG_TXE) == RESET) );
}

/**
	* @brief  向蓝牙模块发送一连串数据.
	* @param  Data:数据地址
	* @retval None.
  */
void Bluetooth_SendString(char* Data)
{
	uint8_t i;
	
	for(i=0;Data[i]!='\0';i++)
	{
		Bluetooth_SendByte(Data[i]);
	}
	while((USART_GetFlagStatus(BTH_USART,USART_FLAG_TC) == RESET));
}

