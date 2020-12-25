#include "./speaker/speaker.h"

/**
  * @brief  语音播报端口配置.
	* @param  None.
	* @retval None.
  */
void Speaker_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	SPEAKER_GPIO_CLK_FUN(SPEAKER_GPIO_CLK, ENABLE);
	SPEAKER_USART_CLK_FUN(SPEAKER_USART_CLK, ENABLE);
	
	//中断优先级配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel=SPEAKER_ITQChannel;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	//接收端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = SPEAKER_GPIO_RX_PIN;
	GPIO_Init(SPEAKER_GPIO_RX_RPORT,&GPIO_InitStructure);
	
	//发送端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = SPEAKER_GPIO_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPEAKER_GPIO_TX_PORT,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=SPEAKER_BaudRate;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(SPEAKER_USART,&USART_InitStructure);
	//开启中断使能
	USART_ITConfig(SPEAKER_USART,USART_IT_RXNE,ENABLE);
	USART_ITConfig(SPEAKER_USART,USART_IT_IDLE,ENABLE);
			
	//使能USART1
	USART_Cmd(SPEAKER_USART,ENABLE);
	Speaker_SetVol(15);
}


/**
  * @brief  语音播报模块设置音量.
	* @param  None.
	* @retval None.
  */
void Speaker_SetVol(uint16_t vol)
{
	//音量设置
	Speaker_SendByte(0xAA);
	Speaker_SendByte(0x13);
	Speaker_SendByte(0x01);
	Speaker_SendByte(vol);
	Speaker_SendByte(0xAA+0x13+0x01+vol);	
}

/**
	* @brief  发送一个字节.
	* @param  Data:发送的数据
	* @retval None.
  */
void Speaker_SendByte(uint16_t Data)
{
	USART_SendData(SPEAKER_USART,Data);
	while( (USART_GetFlagStatus(SPEAKER_USART,USART_FLAG_TXE) == RESET) );
}

/**
	* @brief  指定曲目播报.
	* @param  Num:指定的曲目(06~08)
	* @retval None.
  */
void Speaker_Play(uint8_t Num)
{
	uint16_t SM = 0xAA+0x07+0x02+Num;
	Speaker_SendByte(0xAA);
	
	Speaker_SendByte(0x07);
	Speaker_SendByte(0x02);
	
	Speaker_SendByte(0x00);
	Speaker_SendByte(Num);
	Speaker_SendByte(SM);
}



