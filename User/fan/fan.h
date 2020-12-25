#ifndef __FAN_H
#define __FAN_H

#include "stm32f10x.h"

#define  FAN_GPIO_CLK_FUN      RCC_APB2PeriphClockCmd
#define  FAN_GPIO_CLK          RCC_APB2Periph_GPIOC
#define  FAN_GPIO_PORT         GPIOC
#define  FAN_GPIO_PIN          GPIO_Pin_12

#define  FAN_ON                GPIO_SetBits(FAN_GPIO_PORT,FAN_GPIO_PIN)
#define  FAN_OFF               GPIO_ResetBits(FAN_GPIO_PORT,FAN_GPIO_PIN)

void Fan_Config(void);

#endif /* __FAN_H */
