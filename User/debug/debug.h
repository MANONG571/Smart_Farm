#ifndef __DEBUG_H
#define __DEBUG_H

#include "stm32f10x.h"
#include <stdio.h>

//GPIO端口
#define  USART1_GPIO_CLK       RCC_APB2Periph_GPIOA

#define  USART1_GPIO_TX_PORT   GPIOA
#define  USART1_GPIO_TX_PIN    GPIO_Pin_9

#define  USART1_GPIO_RX_RPORT  GPIOA
#define  USART1_GPIO_RX_PIN    GPIO_Pin_10

//USART1
#define  DEBUG_USART_CLK_FUN   RCC_APB2PeriphClockCmd
#define  DEBUG_USART_CLK       RCC_APB2Periph_USART1
#define  DEBUG_BaudRate        115200
#define  DEBUG_USART           USART1
#define  DEBUG_Handler         USART1_IRQHandler
#define  DEBUG_IRQChannel      USART1_IRQn

//调试信息输出
#define DebugInform   
#ifdef  DebugInform
#define    Debug(fmt,args...)    printf("File:<%s> Fun:[%s} Line:%d\n"fmt"\n",__FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define    Debug(fmt,arg...)     /* do nothings */
#endif


void Debug_USART_Config(void);
void USART_GPIO_Config(void);
void USART_USART_Config(void);
static void NVIC_Config(void);
void Debug_SendByte(uint16_t Data);
void Debug_SendString(char* Data);

#endif /* __DEBUG_H */
