#include "./speaker/speaker.h"

/**
  * @brief  ���������˿�����.
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
	
	//�ж����ȼ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel=SPEAKER_ITQChannel;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	//���ն˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = SPEAKER_GPIO_RX_PIN;
	GPIO_Init(SPEAKER_GPIO_RX_RPORT,&GPIO_InitStructure);
	
	//���Ͷ˿�����
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
	//�����ж�ʹ��
	USART_ITConfig(SPEAKER_USART,USART_IT_RXNE,ENABLE);
	USART_ITConfig(SPEAKER_USART,USART_IT_IDLE,ENABLE);
			
	//ʹ��USART1
	USART_Cmd(SPEAKER_USART,ENABLE);
	Speaker_SetVol(15);
}


/**
  * @brief  ��������ģ����������.
	* @param  None.
	* @retval None.
  */
void Speaker_SetVol(uint16_t vol)
{
	//��������
	Speaker_SendByte(0xAA);
	Speaker_SendByte(0x13);
	Speaker_SendByte(0x01);
	Speaker_SendByte(vol);
	Speaker_SendByte(0xAA+0x13+0x01+vol);	
}

/**
	* @brief  ����һ���ֽ�.
	* @param  Data:���͵�����
	* @retval None.
  */
void Speaker_SendByte(uint16_t Data)
{
	USART_SendData(SPEAKER_USART,Data);
	while( (USART_GetFlagStatus(SPEAKER_USART,USART_FLAG_TXE) == RESET) );
}

/**
	* @brief  ָ����Ŀ����.
	* @param  Num:ָ������Ŀ(06~08)
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



