#ifndef __SPEAKER_H
#define __SPEAKER_H

#include "stm32f10x.h"

//GPIO¶Ë¿Ú
#define  SPEAKER_GPIO_CLK_FUN   RCC_APB2PeriphClockCmd
#define  SPEAKER_GPIO_CLK       RCC_APB2Periph_GPIOA

#define  SPEAKER_GPIO_TX_PORT   GPIOA
#define  SPEAKER_GPIO_TX_PIN    GPIO_Pin_2

#define  SPEAKER_GPIO_RX_RPORT  GPIOA
#define  SPEAKER_GPIO_RX_PIN    GPIO_Pin_3

//USART2
#define  SPEAKER_USART_CLK_FUN  RCC_APB1PeriphClockCmd
#define  SPEAKER_USART_CLK      RCC_APB1Periph_USART2
#define  SPEAKER_BaudRate       9600
#define  SPEAKER_USART          USART2
#define  SPEAKER_Handler        USART2_IRQHandler
#define  SPEAKER_ITQChannel     USART2_IRQn

void Speaker_Config(void);
void Speaker_SetVol(uint16_t vol);
void Speaker_SendByte(uint16_t Data);
void Speaker_Play(uint8_t Num);

#endif /* __SPEAKER_H */
