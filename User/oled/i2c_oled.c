#include "./oled/i2c_oled.h"
#include "./oled/oled_font.h"

uint8_t white[128]={0};


/**
  * @brief  发送一个字节.
  * @param  Data: 发送的数据字节.
	* @retval SUCCESS: 写入成功.
	*         ERROR: 写入失败.
  */
ErrorStatus Send_SingleData(uint8_t Data)
{
	SendByte(Data);                           //发送个控制字节
	 
	if( NOACK == WaitAck() )                     //从设备不应答
	{
		I2C_OLED_USART1("从设备没有应答"); 
		I2C_Stop();
		return ERROR;
	}
	
	return SUCCESS;
}


/**
  * @brief  发送一个控制字节和数据字节.
  * @param  Type: 控制字节
  * @param  Data: 发送的数据字节.
	* @retval SUCCESS: 写入成功.
	*         ERROR: 写入失败.
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
  * @brief  向OLED写入一个字节数据.
  * @param  Type: 要向EEPROM传输的数据类型(COM/DAT)
  * @param  Data: 要写入的一个字节数据.
	* @retval SUCCESS: 写入成功.
	*         ERROR: 写入失败.
  */
ErrorStatus OLED_ByteWrite(SENDTYPE Type, uint8_t Data)
{	
	if( SUCCESS != I2C_Start() )                 //起始信号启动不成功
	{
		I2C_OLED_USART1("总线已被占用");
		return ERROR;		
	}
	
	if( ERROR == Send_SingleData(I2C_SLAVE_ADDRESS8))  //发送从设备地址
		goto ByteWrite_ERROR;
	
	if( ERROR == Send_TwoData(Type,Data))                //要发送(命令/数据)命令
		goto ByteWrite_ERROR;
			
	I2C_Stop();	
	return SUCCESS;	 
	
ByteWrite_ERROR:
	I2C_Stop();
	return ERROR;
}


/**
  * @brief  向OLED的整页连续写入数据.
  * @param  PageNum: 向OLED写入的页数(0~7)
  * @param  Data: 向OLED写入的数据地址.
	* @retval SUCCESS: 写入成功.
	*         ERROR: 写入失败.
  */
ErrorStatus OLED_PageWrite(uint8_t PageNum, uint8_t* Data )
{
	int16_t i;
	
	if( ERROR == I2C_Start() )                 //起始信号启动不成功
	{
		I2C_OLED_USART1("总线已被占用");
		goto PageWrite_ERROR;		
	}
	
	if( ERROR == Send_SingleData(I2C_SLAVE_ADDRESS8) )//发送从设备地址和写操作
		goto PageWrite_ERROR;  
	
	if( ERROR == Send_TwoData(ContCOMMAND, 0xb0+PageNum) ) //设置页数
		goto PageWrite_ERROR;  
		
	if( ERROR == Send_TwoData(ContCOMMAND, 0x00) )//设置起始列的低四位
		goto PageWrite_ERROR;    
	
	if( ERROR == Send_TwoData(ContCOMMAND, 0x10) )//设置起始列的高四位
		goto PageWrite_ERROR;
	
	for(i=0;i<128;i++) //发送数据
	{			
		if( ERROR == Send_TwoData(ContDATA, Data[i]) )
			goto PageWrite_ERROR;
	}

	I2C_Stop();	
	return SUCCESS;	 
	
PageWrite_ERROR:           //从设备未响应,返回失败
		return ERROR;
}


/**
  * @brief  OLED初始化.
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
  * @brief  OLED整屏清0.
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
  * @brief  页对齐显示单个中文.
  * @param  Page:起始页(0~6)
  * @param  Column:起始列(0~127)
  * @param  Chinese:中文地址  二维数组地址需强制转换(*uint8_t)
  * @param  Seq: 中文地址中的第几个.
	* @retval None.
  */
void OLED_ShowSingleChinese(uint8_t Page, uint8_t Column, const uint8_t* Chinese, uint8_t Seq)
{
	uint8_t i;
	
	I2C_Start();
	Send_SingleData(I2C_SLAVE_ADDRESS8);  	
	
	Send_TwoData(ContCOMMAND, 0xB0+Page);	//设置起始页
	  
	Send_TwoData(ContCOMMAND, 0x00+Column&0x0F);	//设置起始列 低4位
	Send_TwoData(ContCOMMAND, 0x10|(Column>>4));  //设置起始列 高4位
		
	for(i=0;i<16;i++)
	{
		Send_TwoData(ContDATA, *(Chinese+Seq*32+i));
	}
		
	Send_TwoData(ContCOMMAND, 0xB0+Page+1);	//设置起始页
	  
	Send_TwoData(ContCOMMAND, 0x00+Column&0x0F);	//设置起始列 低4位 
	Send_TwoData(ContCOMMAND, 0x10|(Column>>4));  //设置起始列 高4位
		
	for(i=0;i<16;i++)
	{
		Send_TwoData(ContDATA, *(Chinese+Seq*32+16+i));
	}
		
	I2C_Stop();	
}

/**
  * @brief  页对齐显示一串中文.
  * @param  Page:起始页(0~6)
  * @param  Column: 起始列(0~127)
  * @param  Chinese: 中文地址
  * @param  ChineseNum: 要写入的中文个数
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
  * @brief  在指定地址(x,y)显示一个字
  * @param  x:列起始地址(0~127)
  * @param  y:行起始地址(0~63)
  * @param  Word:字符地址
  * @param  Seq: 字符位置
  * @param  size:字符大小(8/16)(中文16*16,英文8*16)
	* @retval None.
  */
void OLED_ShowSingleWord(uint8_t x, uint8_t y, uint8_t* Word, uint8_t Seq, uint8_t size)
{
	uint8_t i,j,page=0xB0+y/PAGE_SIZE;
	uint8_t* addr=Word+Seq*size*2; //字符起始地址
	
	I2C_Start();
	Send_SingleData(I2C_SLAVE_ADDRESS8);
	
	for(i=0;i<3;i++)
	{
		if((i==2)&&(y%PAGE_SIZE==0))  ///如果行对齐页, 则跳过第3页的写
			break;
				
		Send_TwoData(ContCOMMAND, page++);	//设置起始页
	  
		if(page-1==0xB7)
			page=0xB0;
		
		Send_TwoData(ContCOMMAND, 0x00|(x&0x0F));	//设置起始列 低4位
		Send_TwoData(ContCOMMAND, 0x10|(x>>4));   //设置起始列 高4位
		
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
  * @brief  在指定地址(x,y)显示一串字
  * @param  x:列起始地址(0~127)
  * @param  y:行起始地址(0~63)
  * @param  Word:字符地址
  * @param  WordNum: 要写入的字符个数
  * @param  size:字符大小(8/16)(中文16*16,英文8*16)
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
  * @brief  使能滚动
  * @param  None.
	* @retval None.
  */
void OLED_Scroll_Enable(void)
{
	I2C_Start();
	Send_SingleData(I2C_SLAVE_ADDRESS8);  	
	
	Send_TwoData(ContCOMMAND,0x29);//垂直和向右水平滚动
	Send_TwoData(ContCOMMAND,0x00);//
	Send_TwoData(ContCOMMAND,0x00);//设置起始页
	Send_TwoData(ContCOMMAND,0x00);//设置每一次滚动的时间间隔
	Send_TwoData(ContCOMMAND,0x07);//设置终止页
	Send_TwoData(ContCOMMAND,0x00);//设置垂直滚动偏移量
	Send_TwoData(ContCOMMAND,0x2F);//使能滚动
	
	I2C_Stop();
}

/**
  * @brief  失能滚动
  * @param  None.
	* @retval None.
  */
void OLED_Scroll_Disable(void)
{
	OLED_ByteWrite(COMMAND,0x2E); 	
}


/**
  * @brief  显示温度信息
	* @param  temp_int:温度整数
	* @param  temp_deci:温度小数
	* @retval None.
  */
void ShowTemp(uint8_t temp_int, uint8_t temp_deci)
{
	OLED_ShowSingleWord(0,0,(uint8_t*)Chinese_Noun,0,16);//显示"温"
	OLED_ShowSingleWord(16,0,(uint8_t*)Chinese_Noun,2,16);//显示"度"
	OLED_ShowSingleWord(32,0,(uint8_t*)ASCII_8X16,26,8);//显示":"
	
	OLED_ShowSingleWord(40,0,(uint8_t*)ASCII_8X16,temp_int/10+16,8);//显示十位数
	OLED_ShowSingleWord(48,0,(uint8_t*)ASCII_8X16,temp_int%10+16,8);//显示个位数
	OLED_ShowSingleWord(56,0,(uint8_t*)ASCII_8X16,14,8);            //显示"."
	OLED_ShowSingleWord(64,0,(uint8_t*)ASCII_8X16,temp_deci%10+16,8);//显示小数
	
}

/**
  * @brief  显示湿度信息
* @param  humi:湿度值
	* @retval None.
  */
void ShowHumi(uint8_t humi)
{
	OLED_ShowSingleWord(0,16,(uint8_t*)Chinese_Noun,1,16);//显示"湿"
	OLED_ShowSingleWord(16,16,(uint8_t*)Chinese_Noun,2,16);//显示"度"
	OLED_ShowSingleWord(32,16,(uint8_t*)ASCII_8X16,26,8);//显示":"
	
	OLED_ShowSingleWord(48,16,(uint8_t*)ASCII_8X16,humi/10+16,8); //显示十位数
	OLED_ShowSingleWord(56,16,(uint8_t*)ASCII_8X16,humi%10+16,8); //显示个位数
	OLED_ShowSingleWord(64,16,(uint8_t*)ASCII_8X16,5,8);//显示"%"
}

/**
  * @brief  显示光照信息
	* @param  light:光照值
	* @retval None.
  */
void ShowLight(uint16_t light)
{
	uint8_t i;
	uint16_t light_c=light;  //i:light值的位数    
	
	OLED_ShowSingleWord(0,32,(uint8_t*)Chinese_Noun,3,16);//显示"光"
	OLED_ShowSingleWord(16,32,(uint8_t*)Chinese_Noun,4,16);//显示"照"
	OLED_ShowSingleWord(32,32,(uint8_t*)ASCII_8X16,26,8);//显示":"
	
	for(i=0;i<4;i++)  //擦除四位数据
	{
		OLED_ShowSingleWord(40+8*i,32,(uint8_t*)ASCII_8X16,0,8);
	}
	
	
	OLED_ShowSingleWord(64,32,(uint8_t*)ASCII_8X16,light_c%10+16,8);  ///显示个位数
	
	for(i=1;light_c/10!=0;i++)  //显示其他数
	{
		light_c /= 10;
		OLED_ShowSingleWord(64-8*i,32,(uint8_t*)ASCII_8X16,light_c%10+16,8);		
	}
	
}

/**
  * @brief  显示烟雾信息
	* @param  smog:烟雾值
	* @retval None.
  */
void ShowSmog(float smog)
{
	float smog_c=smog;
	
	OLED_ShowSingleWord(0,48,(uint8_t*)Chinese_Noun,5,16);//显示"烟"
	OLED_ShowSingleWord(16,48,(uint8_t*)Chinese_Noun,6,16);//显示"雾"
	OLED_ShowSingleWord(32,48,(uint8_t*)ASCII_8X16,26,8);//显示":"
	
	OLED_ShowSingleWord(40,48,(uint8_t*)ASCII_8X16,(int)smog_c%10+16,8); //显示个位数
	OLED_ShowSingleWord(48,48,(uint8_t*)ASCII_8X16,14,8);//显示"."
	smog_c *= 10;
	OLED_ShowSingleWord(56,48,(uint8_t*)ASCII_8X16,(int)smog_c%10+16,8); //显示第一位小数
	smog_c *= 10;
	OLED_ShowSingleWord(64,48,(uint8_t*)ASCII_8X16,(int)smog_c%10+16,8); //显示第二位小数
	
}

