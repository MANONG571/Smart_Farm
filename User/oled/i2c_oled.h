#ifndef __I2C_OLED_H
#define __I2C_OLED_H

#include "stm32f10x.h"
#include "./debug/debug.h"
#include "./oled/i2c_oled_analog.h"

//调试信息输出
#define DebugInform_I2C   
#ifdef  DebugInform_I2C
#define    I2C_OLED_USART1(fmt,args...)    printf("File:<%s> Fun:[%s} Line:%d\n"fmt"\n",__FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define    I2C_OLED_USART1(fmt,arg...)     /* do nothings */
#endif

//I2C从设备地址(8位)
#define  I2C_SLAVE_ADDRESS8        0x78

#define  PAGE_SIZE                 8

typedef enum {COMMAND=0x00,DATA=0x40,ContCOMMAND=0x80,ContDATA=0xC0} SENDTYPE;

ErrorStatus Send_SingleData(uint8_t Data);
ErrorStatus Send_TwoData(SENDTYPE Type, uint8_t Data);
ErrorStatus OLED_ByteWrite( SENDTYPE Type, uint8_t Data);
ErrorStatus OLED_PageWrite(uint8_t PageNum, uint8_t* Data );

void I2C_OLED_CmdInit(void);
void OLED_Clear(void);

void OLED_ShowSingleChinese(uint8_t Page, uint8_t Column, const uint8_t* Chinese, uint8_t Seq);
void OLED_ShowString_Chinese(uint8_t Page, uint8_t Column, uint8_t* Chinese, uint8_t ChineseNum);

void OLED_ShowSingleWord(uint8_t x, uint8_t y, uint8_t* Word, uint8_t Seq, uint8_t size);
void OLED_ShowSingleLine(uint8_t x, uint8_t y, uint8_t* Word, uint8_t WordNum, uint8_t size);

void OLED_Scroll_Enable(void);
void OLED_Scroll_Disable(void);

void ShowTemp(uint8_t temp_int, uint8_t temp_deci);
void ShowHumi(uint8_t humi);
void ShowLight(uint16_t light);
void ShowSmog(float smog);

#endif /* __I2C_OLED_H */
