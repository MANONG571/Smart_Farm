#ifndef __I2C_ANALOG_H
#define __I2C_ANALOG_H

#include "stm32f10x.h"
#include "./debug/debug.h"

//I2C SCL ʱ�Ӷ˿�
#define  I2C_SCL_GPIO_CLK   RCC_APB2Periph_GPIOB
#define  I2C_SCL_GPIO_PORT  GPIOB
#define  I2C_SCL_PIN        GPIO_Pin_6

//I2C SDA ���ݶ˿�
#define  I2C_SDA_GPIO_CLK   RCC_APB2Periph_GPIOB
#define  I2C_SDA_GPIO_PORT  GPIOB
#define  I2C_SDA_PIN        GPIO_Pin_7

//I2C SCL �˿ڿ���
#define  I2C_SCL_HIGH              GPIO_SetBits(I2C_SCL_GPIO_PORT,I2C_SCL_PIN)
#define  I2C_SCL_LOW               GPIO_ResetBits(I2C_SCL_GPIO_PORT,I2C_SCL_PIN)

//I2C SDA �˿ڿ���
#define  I2C_SDA_HIGH              GPIO_SetBits(I2C_SDA_GPIO_PORT,I2C_SDA_PIN)
#define  I2C_SDA_LOW               GPIO_ResetBits(I2C_SDA_GPIO_PORT,I2C_SDA_PIN)

//������������ߵ�ƽ
#define  I2C_SDA_IN                GPIO_ReadInputDataBit(I2C_SDA_GPIO_PORT,I2C_SDA_PIN)
#define  I2C_SCL_IN                GPIO_ReadInputDataBit(I2C_SCL_GPIO_PORT,I2C_SCL_PIN)
//��������Ƿ�ռ��
#define  I2C_BUSY_IN               (I2C_SCL_IN && I2C_SDA_IN)
 
typedef enum {NOACK = 0, ACK = !NOACK} AckStatus;


void I2C_OLED_Config(void);
static void I2C_Delay(void);
ErrorStatus I2C_Start(void);
ErrorStatus I2C_Start_Second(void);
void I2C_Stop(void);
AckStatus WaitAck(void);
void SendAck(void);
void ReadByte(uint8_t* Data);
void SendByte(uint8_t Data);


#endif /* __I2C_ANALOG_H */
