#include "./fan/fan.h"

/**
  * @brief  风扇(电机)初始化
	* @param  None.
	* @retval None.
  */
void Fan_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	FAN_GPIO_CLK_FUN(FAN_GPIO_CLK, ENABLE);
	
	//LED1初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = FAN_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FAN_GPIO_PORT,&GPIO_InitStructure);
	FAN_OFF;
}
