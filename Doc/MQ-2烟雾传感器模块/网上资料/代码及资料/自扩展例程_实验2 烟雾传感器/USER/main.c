#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "smog.h"
 
/**
  ******************************************************************************
  * @file    main.c
  * @author  沅茝醴兰
  * @date    2015-07-22
  * @brief   烟雾传感器实验
  ******************************************************************************
  * @attention
  *
  * 实验平台:神舟III号 开发板 模仿 正点原子 战舰 STM32 开发板
  *
  ******************************************************************************
  */

int main(void)
{
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	Smog_Init();
	while(1)
	{
		Smog_Trans_Concentration();
        delay_ms(300);
	}
}

