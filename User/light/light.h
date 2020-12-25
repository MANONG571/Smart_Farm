#ifndef __LIGHT_H
#define __LIGHT_H

#include "stm32f10x.h"
#include "./debug/debug.h"

//I2C SCL ʱ�Ӷ˿�
#define  LIGHT_SCL_GPIO_CLK   RCC_APB2Periph_GPIOC
#define  LIGHT_SCL_GPIO_PORT  GPIOC
#define  LIGHT_SCL_PIN        GPIO_Pin_8

//I2C SDA ���ݶ˿�
#define  LIGHT_SDA_GPIO_CLK   RCC_APB2Periph_GPIOC
#define  LIGHT_SDA_GPIO_PORT  GPIOC
#define  LIGHT_SDA_PIN        GPIO_Pin_9

//I2C SCL �˿ڿ���
#define  LIGHT_SCL_HIGH              GPIO_SetBits(LIGHT_SCL_GPIO_PORT,LIGHT_SCL_PIN)
#define  LIGHT_SCL_LOW               GPIO_ResetBits(LIGHT_SCL_GPIO_PORT,LIGHT_SCL_PIN)

//I2C SDA �˿ڿ���
#define  LIGHT_SDA_HIGH              GPIO_SetBits(LIGHT_SDA_GPIO_PORT,LIGHT_SDA_PIN)
#define  LIGHT_SDA_LOW               GPIO_ResetBits(LIGHT_SDA_GPIO_PORT,LIGHT_SDA_PIN)

//������������ߵ�ƽ
#define  LIGHT_SDA_IN                GPIO_ReadInputDataBit(LIGHT_SDA_GPIO_PORT,LIGHT_SDA_PIN)
#define  LIGHT_SCL_IN                GPIO_ReadInputDataBit(LIGHT_SCL_GPIO_PORT,LIGHT_SCL_PIN)
//��������Ƿ�ռ��
#define  LIGHT_BUSY_IN               (LIGHT_SCL_IN && LIGHT_SDA_IN)
 
#define  LIGHT_ADDR_READ             0x47//0b0100 0111
#define  LIGHT_ADDR_WRITE            0x46//0b0100 0110

#define  LIGHT_POWER_ON              0x01//0b0000 0001
#define  LIGHT_CON_H_RESO_MODE       0x10//0b0001 0000
#define  LIGHT_ONE_H_RESO_MODE       0x20//0b0010 0000

void Light_I2C_Config(void);
static void Light_I2C_Delay(void);
ErrorStatus Light_I2C_Start(void);
void Light_I2C_Stop(void);
ErrorStatus Light_WaitAck(void);
void Light_SendAck(void);
void Light_ReadByte(uint8_t* Data);
void Light_SendByte(uint8_t Data);

ErrorStatus Light_SendCmd(uint8_t Cmd);
ErrorStatus Light_ReadData(uint16_t* Data);
uint16_t Light_GetValue(void);
	
#endif /* __LIGHT_H */
