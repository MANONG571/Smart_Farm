#include "./systick/systick.h"

__IO uint32_t SysTick_Num;

void SysTick_MyConfig(void)
{
	//ÿ1us����һ�εδ�ʱ���ж�
	if(SysTick_Config(72))
	{
		printf("�δ�ʱ�����ô���\n");
	}
	//ʧ�ܵδ�ʱ��
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Delayus_SysTick(uint32_t time)
{
	SysTick_Num=time;
	
	//ʹ�ܵδ�ʱ��
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	while(SysTick_Num != 0);
	
	//ʧ�ܵδ�ʱ��
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
