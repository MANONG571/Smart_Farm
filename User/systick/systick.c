#include "./systick/systick.h"

__IO uint32_t SysTick_Num;

void SysTick_MyConfig(void)
{
	//每1us发生一次滴答定时器中断
	if(SysTick_Config(72))
	{
		printf("滴答定时器设置错误\n");
	}
	//失能滴答定时器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Delayus_SysTick(uint32_t time)
{
	SysTick_Num=time;
	
	//使能滴答定时器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	while(SysTick_Num != 0);
	
	//失能滴答定时器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Delayms_SysTick(uint32_t time)
{
	uint32_t num;
	for(num=0;num<time;num++)
	{
		Delayus_SysTick(1000);
	}
}

void Delays_SysTick(uint32_t time)
{
	uint32_t num;
	for(num=0;num<time;num++)
	{
		Delayms_SysTick(1000);
	}
}
