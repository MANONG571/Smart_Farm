#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f10x.h"
#include "./systick/systick.h"
#include <string.h>

//GPIO¶Ë¿Ú
#define  BTH_GPIO_CLK_FUN        RCC_APB2PeriphClockCmd
#define  BTH_GPIO_CLK            RCC_APB2Periph_GPIOB

#define  BTH_GPIO_TX_PORT        GPIOB
#define  BTH_GPIO_TX_PIN         GPIO_Pin_10

#define  BTH_GPIO_RX_RPORT       GPIOB
#define  BTH_GPIO_RX_PIN         GPIO_Pin_11

#define  BTH_GPIO_KEY_RPORT      GPIOB
#define  BTH_GPIO_KEY_PIN        GPIO_Pin_8

//USART3
#define  BTH_USART_CLK_FUN       RCC_APB1PeriphClockCmd
#define  BTH_USART_CLK           RCC_APB1Periph_USART3
#define  BTH_BaudRate            115200//38400  9600
#define  BTH_USART               USART3
#define  BTH_Handler             USART3_IRQHandler
#define  BTH_IRQChannel          USART3_IRQn

#define  KEY_HIGH                GPIO_SetBits(BTH_GPIO_KEY_RPORT,BTH_GPIO_KEY_PIN)
#define  KEY_LOW                 GPIO_ResetBits(BTH_GPIO_KEY_RPORT,BTH_GPIO_KEY_PIN)

extern uint8_t Receive[20];
extern __IO uint8_t Bluetooth_Flag;

void BTH_Config(void);
void Bluetooth_SendByte(uint8_t Data);
void Bluetooth_SendString(char* Data);

#endif /* __BLUETOOTH_H */
