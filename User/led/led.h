#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

#define LED1_GPIO         GPIOB
#define LED1_GPIO_PIN     GPIO_Pin_5
#define LED1_GPIO_MODE    GPIO_Mode_Out_PP
#define LED1_GPIO_SPEED   GPIO_Speed_50MHz
#define LED1_GPIO_CLOCK   RCC_APB2Periph_GPIOB

#define LED2_GPIO         GPIOB
#define LED2_GPIO_PIN     GPIO_Pin_0
#define LED2_GPIO_MODE    GPIO_Mode_Out_PP
#define LED2_GPIO_SPEED   GPIO_Speed_50MHz
#define LED2_GPIO_CLOCK   RCC_APB2Periph_GPIOB

#define LED3_GPIO         GPIOB
#define LED3_GPIO_PIN     GPIO_Pin_1
#define LED3_GPIO_MODE    GPIO_Mode_Out_PP
#define LED3_GPIO_SPEED   GPIO_Speed_50MHz
#define LED3_GPIO_CLOCK   RCC_APB2Periph_GPIOB


#define LED1_OFF          GPIO_SetBits(LED1_GPIO,LED1_GPIO_PIN)
#define LED1_ON           GPIO_ResetBits(LED1_GPIO,LED1_GPIO_PIN)

#define LED2_OFF          GPIO_SetBits(LED2_GPIO,LED2_GPIO_PIN)
#define LED2_ON           GPIO_ResetBits(LED2_GPIO,LED2_GPIO_PIN)

#define LED3_OFF          GPIO_SetBits(LED3_GPIO,LED3_GPIO_PIN)
#define LED3_ON           GPIO_ResetBits(LED3_GPIO,LED3_GPIO_PIN)

//°×(ºì+ÂÌ+À¶)
#define LED_WHITE	\
					LED1_ON;\
					LED2_ON;\
					LED3_ON
					
//ºÚ(È«²¿¹Ø±Õ)
#define LED_RGBOFF	\
					LED1_OFF;\
					LED2_OFF;\
					LED3_OFF
					
void LED_Config(void);
	
#endif /* __LED_H */
