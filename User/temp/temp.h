#ifndef __TEMP_H
#define __TEMP_H

#include "stm32f10x.h"
#include "./systick/systick.h"
#include "./debug/debug.h"

//��ʪ�����ݽṹ��
typedef struct
{
	uint8_t Humi_Integer;  //ʪ����������
	uint8_t Humi_Decimal;  //ʪ��С������  
	uint8_t Temp_Integer;  //�¶���������
	uint8_t Temp_Decimal;  //�¶�С������
	uint8_t Verification;  //У��λ
}TempHumi_Inform;


//GPIO������� ��©���ģʽ
#define TEMP_GPIO_FUN     RCC_APB2PeriphClockCmd
#define TEMP_GPIO_CLOCK   RCC_APB2Periph_GPIOE
#define TEMP_GPIO_PORT    GPIOE
#define TEMP_GPIO_PIN     GPIO_Pin_6
#define TEMP_GPIO_MODE    GPIO_Mode_Out_OD

//���ݶ˿�
#define TEMP_OUT_HIGH     GPIO_SetBits(TEMP_GPIO_PORT,TEMP_GPIO_PIN)
#define TEMP_OUT_LOW      GPIO_ResetBits(TEMP_GPIO_PORT,TEMP_GPIO_PIN)
#define TEMP_IN           GPIO_ReadInputDataBit(TEMP_GPIO_PORT,TEMP_GPIO_PIN)

void Temp_GPIO_Config(void);
void Temp_Start(void);
uint8_t Temp_ReadByte(void);
ErrorStatus Temp_GetValue(TempHumi_Inform* TempHumi);

#endif /* __TEMP_H */
