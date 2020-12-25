#include "./led/led.h"

/**
  * @brief  LED初始化
	* @param  None.
	* @retval None.
  */
void LED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED1_GPIO_CLOCK|LED1_GPIO_CLOCK|LED1_GPIO_CLOCK, ENABLE);
	
	//LED1初始化
	GPIO_InitStructure.GPIO_Mode = LED1_GPIO_MODE;
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = LED1_GPIO_SPEED;
	GPIO_Init(LED1_GPIO,&GPIO_InitStructure);
	
	//LED2初始化
	GPIO_InitStructure.GPIO_Mode = LED2_GPIO_MODE;
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = LED2_GPIO_SPEED;
	GPIO_Init(LED2_GPIO,&GPIO_InitStructure);
	
	//LED3初始化
	GPIO_InitStructure.GPIO_Mode = LED3_GPIO_MODE;
	GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = LED3_GPIO_SPEED;
	GPIO_Init(LED3_GPIO,&GPIO_InitStructure);
	
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
}
