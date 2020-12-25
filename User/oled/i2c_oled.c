#include "./oled/i2c_oled.h"
#include "./oled/oled_font.h"

uint8_t white[128]={0};


/**
  * @brief  ����һ���ֽ�.
  * @param  Data: ���͵������ֽ�.
	* @retval SUCCESS: д��ɹ�.
	*         ERROR: д��ʧ��.
  */
ErrorStatus Send_SingleData(uint8_t Data)
{
	SendByte(Data);                           //���͸������ֽ�
	 
	if( NOACK == WaitAck() )                     //���豸��Ӧ��
	{
		I2C_OLED_USART1("���豸û��Ӧ��"); 
		I2C_Stop();
		return ERROR;
	}
	
	return SUCCESS;
}


/**
  * @brief  ����һ�������ֽں������ֽ�.
  * @param  Type: �����ֽ�
  * @param  Data: ���͵������ֽ�.
	* @retval SUCCESS: д��ɹ�.
	*         ERROR: д��ʧ��.
  */
ErrorStatus Send_TwoData(SENDTYPE Type, uint8_t Data)
{
	if( ERROR == Send_SingleData(Type) )
		return ERROR;
	if( ERROR == Send_SingleData(Data) )
		return ERROR;
	
	return SUCCESS;
}


/**
  * @brief  ��OLEDд��һ���ֽ�����.
  * @param  Type: Ҫ��EEPROM�������������(COM/DAT)
  * @param  Data: Ҫд���һ���ֽ�����.
	* @retval SUCCESS: д��ɹ�.
	*         ERROR: д��ʧ��.
  */
ErrorStatus OLED_ByteWrite(SENDTYPE Type, uint8_t Data)
{	
	if( SUCCESS != I2C_Start() )                 //��ʼ�ź��������ɹ�
	{
		I2C_OLED_USART1("�����ѱ�ռ��");
		return ERROR;		
	}
	
	if( ERROR == Send_SingleData(I2C_SLAVE_ADDRESS8))  //���ʹ��豸��ַ
		goto ByteWrite_ERROR;
	
	if( ERROR == Send_TwoData(Type,Data))                //Ҫ����(����/����)����
		goto ByteWrite_ERROR;
			
	I2C_Stop();	
	return SUCCESS;	 
	
ByteWrite_ERROR:
	I2C_Stop();
	return ERROR;
}


/**
  * @brief  ��OLED����ҳ����д������.
  * @param  PageNum: ��OLEDд���ҳ��(0~7)
  * @param  Data: ��OLEDд������ݵ�ַ.
	* @retval SUCCESS: д��ɹ�.
	*         ERROR: д��ʧ��.
  */
ErrorStatus OLED_PageWrite(uint8_t PageNum, uint8_t* Data )
{
	int16_t i;
	
	if( ERROR == I2C_Start() )                 //��ʼ�ź��������ɹ�
	{
		I2C_OLED_USART1("�����ѱ�ռ��");
		goto PageWrite_ERROR;		
	}
	
	if( ERROR == Send_SingleData(I2C_SLAVE_ADDRESS8) )//���ʹ��豸��ַ��д����
		goto PageWrite_ERROR;  
	
	if( ERROR == Send_TwoData(ContCOMMAND, 0xb0+PageNum) ) //����ҳ��
		goto PageWrite_ERROR;  
		
	if( ERROR == Send_TwoData(ContCOMMAND, 0x00) )//������ʼ�еĵ���λ
		goto PageWrite_ERROR;    
	
	if( ERROR == Send_TwoData(ContCOMMAND, 0x10) )//������ʼ�еĸ���λ
		goto PageWrite_ERROR;
	
	for(i=0;i<128;i++) //��������
	{			
		if( ERROR == Send_TwoData(ContDATA, Data[i]) )
			goto PageWrite_ERROR;
	}

	I2C_Stop();	
	return SUCCESS;	 
	
PageWrite_ERROR:           //���豸δ��Ӧ,����ʧ��
		return ERROR;
}


/**
  * @brief  OLED��ʼ��.
  * @param  None.
	* @retval None.
  */
void I2C_OLED_CmdInit(void)
{		
	I2C_Start();
	Send_SingleData(I2C_SLAVE_ADDRESS8);
		
	Send_TwoData(ContCOMMAND,0xAE);// turn off oled panel
	
	Send_TwoData(ContCOMMAND,0x00);// set low column address
	
	Send_TwoData(ContCOMMAND,0x10);// set high column address
	
	Send_TwoData(ContCOMMAND,0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	
	Send_TwoData(ContCOMMAND,0x81);//--set contrast control register
	
	Send_TwoData(ContCOMMAND,0xCF);// Set SEG Output Current Brightness
	
	Send_TwoData(ContCOMMAND,0xA1);// Set SEG/Column Mapping  
		
	Send_TwoData(ContCOMMAND,0xC8);// Set COM/Row Scan Direction 
		
	Send_TwoData(ContCOMMAND,0xA6);// set normal display
		
	Send_TwoData(ContCOMMAND,0xA8);// set multiplex ratio(1 to 64)
		
	Send_TwoData(ContCOMMAND,0x3f);// 1/64 duty
		
	Send_TwoData(ContCOMMAND,0xD3);// set display offset	Shift Mapping RAM Counter (0x00~0x3F)
		
	Send_TwoData(ContCOMMAND,0x00);// not offset
		
	Send_TwoData(ContCOMMAND,0xd5);// set display clock divide ratio/oscillator frequency
		
	Send_TwoData(ContCOMMAND,0x80);// set divide ratio, Set Clock as 100 Frames/Sec
		
	Send_TwoData(ContCOMMAND,0xD9);// set pre-charge period
		
	Send_TwoData(ContCOMMAND,0xF1);// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
		
	Send_TwoData(ContCOMMAND,0xDA);// set com pins hardware configuration
		
	Send_TwoData(ContCOMMAND,0x12);
		
	Send_TwoData(ContCOMMAND,0xDB);// set vcomh
		
	Send_TwoData(ContCOMMAND,0x40);// Set VCOM Deselect Level
		
	Send_TwoData(ContCOMMAND,0x20);// Set Page Addressing Mode (0x00/0x01/0x02)
		
	Send_TwoData(ContCOMMAND,0x02);
		
	Send_TwoData(ContCOMMAND,0x8D);// set Charge Pump enable/disable
		
	Send_TwoData(ContCOMMAND,0x14);// set(0x10) disable
	
	Send_TwoData(ContCOMMAND,0xA4);// Disable Entire Display On (0xa4/0xa5)
		
	Send_TwoData(ContCOMMAND,0xA6);// Disable Inverse Display On (0xa6/a7)
		
	Send_TwoData(ContCOMMAND,0xAF);
	
	I2C_Stop();
	
	OLED_Clear();
}

/**
  * @brief  OLED������0.
  * @param  None.
	* @retval None.
  */
void OLED_Clear(void)
{
	uint8_t i;
	
	for(i=0;i<8;i++)	
		OLED_PageWrite(i,white); 
}

/**
  * @brief  ҳ������ʾ��������.
  * @param  Page:��ʼҳ(0~6)
  * @param  Column:��ʼ��(0~127)
  * @param  Chinese:���ĵ�ַ  ��ά�����ַ��ǿ��ת��(*uint8_t)
  * @param  Seq: ���ĵ�ַ�еĵڼ���.
	* @retval None.
  */
void OLED_ShowSingleChinese(uint8_t Page, uint8_t Column, const uint8_t* Chinese, uint8_t Seq)
{
	uint8_t i;
	
	I2C_Start();
	Send_SingleData(I2C_SLAVE_ADDRESS8);  	
	
	Send_TwoData(ContCOMMAND, 0xB0+Page);	//������ʼҳ
	  
	Send_TwoData(ContCOMMAND, 0x00+Column&0x0F);	//������ʼ�� ��4λ
	Send_TwoData(ContCOMMAND, 0x10|(Column>>4));  //������ʼ�� ��4λ
		
	for(i=0;i<16;i++)
	{
		Send_TwoData(ContDATA, *(Chinese+Seq*32+i));
	}
		
	Send_TwoData(ContCOMMAND, 0xB0+Page+1);	//������ʼҳ
	  
	Send_TwoData(ContCOMMAND, 0x00+Column&0x0F);	//������ʼ�� ��4λ 
	Send_TwoData(ContCOMMAND, 0x10|(Column>>4));  //������ʼ�� ��4λ
		
	for(i=0;i<16;i++)
	{
		Send_TwoData(ContDATA, *(Chinese+Seq*32+16+i));
	}
		
	I2C_Stop();	
}

/**
  * @brief  ҳ������ʾһ������.
  * @param  Page:��ʼҳ(0~6)
  * @param  Column: ��ʼ��(0~127)
  * @param  Chinese: ���ĵ�ַ
  * @param  ChineseNum: Ҫд������ĸ���
	* @retval None.
  */
void OLED_ShowString_Chinese(uint8_t Page, uint8_t Column, uint8_t* Chinese, uint8_t ChineseNum)
{	
	uint8_t i;
	for(i=0;i<ChineseNum;i++)
	{
		OLED_ShowSingleChinese(Page,Column+16*i,Chinese,i);
		
	}
}

/**
  * @brief  ��ָ����ַ(x,y)��ʾһ����
  * @param  x:����ʼ��ַ(0~127)
  * @param  y:����ʼ��ַ(0~63)
  * @param  Word:�ַ���ַ
  * @param  Seq: �ַ�λ��
  * @param  size:�ַ���С(8/16)(����16*16,Ӣ��8*16)
	* @retval None.
  */
void OLED_ShowSingleWord(uint8_t x, uint8_t y, uint8_t* Word, uint8_t Seq, uint8_t size)
{
	uint8_t i,j,page=0xB0+y/PAGE_SIZE;
	uint8_t* addr=Word+Seq*size*2; //�ַ���ʼ��ַ
	
	I2C_Start();
	Send_SingleData(I2C_SLAVE_ADDRESS8);
	
	for(i=0;i<3;i++)
	{
		if((i==2)&&(y%PAGE_SIZE==0))  ///����ж���ҳ, ��������3ҳ��д
			break;
				
		Send_TwoData(ContCOMMAND, page++);	//������ʼҳ
	  
		if(page-1==0xB7)
			page=0xB0;
		
		Send_TwoData(ContCOMMAND, 0x00|(x&0x0F));	//������ʼ�� ��4λ
		Send_TwoData(ContCOMMAND, 0x10|(x>>4));   //������ʼ�� ��4λ
		
		for(j=0;j<size;j++)
		{
			switch (i)
			{
				case 0:
					Send_TwoData(ContDATA, (*addr)<<(y%PAGE_SIZE) );
					break;
				
				case 1:
					Send_TwoData(ContDATA,(*(addr-size)>>(PAGE_SIZE-y%PAGE_SIZE)) | ((*addr)<<(y%PAGE_SIZE)) );
					break;
				
				case 2:
					Send_TwoData(ContDATA, *(addr-size)>>(PAGE_SIZE-y%PAGE_SIZE));
					break;				
			}
			addr++;
		}
	}
		
	I2C_Stop();
	
}


/**
  * @brief  ��ָ����ַ(x,y)��ʾһ����
  * @param  x:����ʼ��ַ(0~127)
  * @param  y:����ʼ��ַ(0~63)
  * @param  Word:�ַ���ַ
  * @param  WordNum: Ҫд����ַ�����
  * @param  size:�ַ���С(8/16)(����16*16,Ӣ��8*16)
	* @retval None.
  */
void OLED_ShowSingleLine(uint8_t x, uint8_t y, uint8_t* Word, uint8_t WordNum, uint8_t size)
{
	uint8_t i;
	
	for(i=0;i<WordNum;i++)
	{
		OLED_ShowSingleWord(x+size*i,y,Word,i,size);
	} 
}

/**
  * @brief  ʹ�ܹ���
  * @param  None.
	* @retval None.
  */
void OLED_Scroll_Enable(void)
{
	I2C_Start();
	Send_SingleData(I2C_SLAVE_ADDRESS8);  	
	
	Send_TwoData(ContCOMMAND,0x29);//��ֱ������ˮƽ����
	Send_TwoData(ContCOMMAND,0x00);//
	Send_TwoData(ContCOMMAND,0x00);//������ʼҳ
	Send_TwoData(ContCOMMAND,0x00);//����ÿһ�ι�����ʱ����
	Send_TwoData(ContCOMMAND,0x07);//������ֹҳ
	Send_TwoData(ContCOMMAND,0x00);//���ô�ֱ����ƫ����
	Send_TwoData(ContCOMMAND,0x2F);//ʹ�ܹ���
	
	I2C_Stop();
}

/**
  * @brief  ʧ�ܹ���
  * @param  None.
	* @retval None.
  */
void OLED_Scroll_Disable(void)
{
	OLED_ByteWrite(COMMAND,0x2E); 	
}


/**
  * @brief  ��ʾ�¶���Ϣ
	* @param  temp_int:�¶�����
	* @param  temp_deci:�¶�С��
	* @retval None.
  */
void ShowTemp(uint8_t temp_int, uint8_t temp_deci)
{
	OLED_ShowSingleWord(0,0,(uint8_t*)Chinese_Noun,0,16);//��ʾ"��"
	OLED_ShowSingleWord(16,0,(uint8_t*)Chinese_Noun,2,16);//��ʾ"��"
	OLED_ShowSingleWord(32,0,(uint8_t*)ASCII_8X16,26,8);//��ʾ":"
	
	OLED_ShowSingleWord(40,0,(uint8_t*)ASCII_8X16,temp_int/10+16,8);//��ʾʮλ��
	OLED_ShowSingleWord(48,0,(uint8_t*)ASCII_8X16,temp_int%10+16,8);//��ʾ��λ��
	OLED_ShowSingleWord(56,0,(uint8_t*)ASCII_8X16,14,8);            //��ʾ"."
	OLED_ShowSingleWord(64,0,(uint8_t*)ASCII_8X16,temp_deci%10+16,8);//��ʾС��
	
}

/**
  * @brief  ��ʾʪ����Ϣ
* @param  humi:ʪ��ֵ
	* @retval None.
  */
void ShowHumi(uint8_t humi)
{
	OLED_ShowSingleWord(0,16,(uint8_t*)Chinese_Noun,1,16);//��ʾ"ʪ"
	OLED_ShowSingleWord(16,16,(uint8_t*)Chinese_Noun,2,16);//��ʾ"��"
	OLED_ShowSingleWord(32,16,(uint8_t*)ASCII_8X16,26,8);//��ʾ":"
	
	OLED_ShowSingleWord(48,16,(uint8_t*)ASCII_8X16,humi/10+16,8); //��ʾʮλ��
	OLED_ShowSingleWord(56,16,(uint8_t*)ASCII_8X16,humi%10+16,8); //��ʾ��λ��
	OLED_ShowSingleWord(64,16,(uint8_t*)ASCII_8X16,5,8);//��ʾ"%"
}

/**
  * @brief  ��ʾ������Ϣ
	* @param  light:����ֵ
	* @retval None.
  */
void ShowLight(uint16_t light)
{
	uint8_t i;
	uint16_t light_c=light;  //i:lightֵ��λ��    
	
	OLED_ShowSingleWord(0,32,(uint8_t*)Chinese_Noun,3,16);//��ʾ"��"
	OLED_ShowSingleWord(16,32,(uint8_t*)Chinese_Noun,4,16);//��ʾ"��"
	OLED_ShowSingleWord(32,32,(uint8_t*)ASCII_8X16,26,8);//��ʾ":"
	
	for(i=0;i<4;i++)  //������λ����
	{
		OLED_ShowSingleWord(40+8*i,32,(uint8_t*)ASCII_8X16,0,8);
	}
	
	
	OLED_ShowSingleWord(64,32,(uint8_t*)ASCII_8X16,light_c%10+16,8);  ///��ʾ��λ��
	
	for(i=1;light_c/10!=0;i++)  //��ʾ������
	{
		light_c /= 10;
		OLED_ShowSingleWord(64-8*i,32,(uint8_t*)ASCII_8X16,light_c%10+16,8);		
	}
	
}

/**
  * @brief  ��ʾ������Ϣ
	* @param  smog:����ֵ
	* @retval None.
  */
void ShowSmog(float smog)
{
	float smog_c=smog;
	
	OLED_ShowSingleWord(0,48,(uint8_t*)Chinese_Noun,5,16);//��ʾ"��"
	OLED_ShowSingleWord(16,48,(uint8_t*)Chinese_Noun,6,16);//��ʾ"��"
	OLED_ShowSingleWord(32,48,(uint8_t*)ASCII_8X16,26,8);//��ʾ":"
	
	OLED_ShowSingleWord(40,48,(uint8_t*)ASCII_8X16,(int)smog_c%10+16,8); //��ʾ��λ��
	OLED_ShowSingleWord(48,48,(uint8_t*)ASCII_8X16,14,8);//��ʾ"."
	smog_c *= 10;
	OLED_ShowSingleWord(56,48,(uint8_t*)ASCII_8X16,(int)smog_c%10+16,8); //��ʾ��һλС��
	smog_c *= 10;
	OLED_ShowSingleWord(64,48,(uint8_t*)ASCII_8X16,(int)smog_c%10+16,8); //��ʾ�ڶ�λС��
	
}

