#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"
#include "./debug/debug.h"

extern __IO uint32_t SysTick_Num;

void SysTick_MyConfig(void);
void Delayus_SysTick(uint32_t time);
void Delayms_SysTick(uint32_t time);
void Delays_SysTick(uint32_t time);

#endif /* __SYSTICK_H */
