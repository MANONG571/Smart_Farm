#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//LED IO初始化
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	//定义一个GPIO_InitTypeDef类型的结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	//开启LED的外设时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	//选择要控制的GPIO引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置引脚速率为50MHz
	GPIO_Init(GPIOF, &GPIO_InitStructure);	//调用库函数，初始化GPIO

	GPIO_SetBits(GPIOF, GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);	//关闭LED1、LED2、LED3、LED4灯
}
 
