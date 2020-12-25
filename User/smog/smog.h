#ifndef __SMOG_H
#define __SMOG_H

#include "stm32f10x.h"

//ADCµÄÍ¨µÀGPIO
#define  SMOG_GPIO_CLK_FUN     RCC_APB2PeriphClockCmd
#define  SMOG_GPIO_CLK         RCC_APB2Periph_GPIOC
#define  SMOG_GPIO_PORT        GPIOC
#define  SMOG_GPIO_PIN         GPIO_Pin_1

//ADC
#define  SMOG_ADC_CLK_FUN      RCC_APB2PeriphClockCmd
#define  SMOG_ADC_CLK          RCC_APB2Periph_ADC1
#define  SMOG_ADC              ADC1
#define  SMOG_ADC_CHANNEL      ADC_Channel_11
#define  SMOG_ADC_SAMPLETIME   ADC_SampleTime_55Cycles5

void Smog_ADC_Config(void);
float Smog_GetValue(void);
	
#endif /* __SMOG_H */
