#include "./smog/smog.h"

/**
  * @brief  烟雾传感器初始化函数,配置GPIO和ADC.
  * @param  None.
  * @retval None.
  */
void Smog_ADC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//开启GPIO和ADC的时钟
	SMOG_GPIO_CLK_FUN(SMOG_GPIO_CLK, ENABLE);
	SMOG_ADC_CLK_FUN(SMOG_ADC_CLK, ENABLE);
	
	//ADC的GPIO端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;     //模拟输入模式
	GPIO_InitStructure.GPIO_Pin = SMOG_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SMOG_GPIO_PORT,&GPIO_InitStructure);
	
	//ADC配置
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;                   //连续转换
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;               //右对齐
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;  //没有外部触发方式
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;                   //独立模式
	ADC_InitStructure.ADC_NbrOfChannel=1;                              //转换通道数:1
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;                        //不启用扫描模式
	ADC_Init(SMOG_ADC,&ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                                  //6分频,ADCCLK=12MHz
	ADC_RegularChannelConfig(SMOG_ADC,SMOG_ADC_CHANNEL,1,SMOG_ADC_SAMPLETIME);//配置ADC的通道1
				
	ADC_Cmd(SMOG_ADC,ENABLE);                              //使能ADC
	
	ADC_ResetCalibration(SMOG_ADC);                           //复位校准
	while( SET == ADC_GetResetCalibrationStatus(SMOG_ADC) );  //复位校准结束
	ADC_StartCalibration(SMOG_ADC);                           //校准
	while( SET == ADC_GetCalibrationStatus(SMOG_ADC) );       //校准结束
}

/**
  * @brief  获取烟雾传感器转化后的值.取十次的平均值
  * @param  None.
  * @retval 烟雾传感器转化后的值.
  */
float Smog_GetValue(void)
{
	uint8_t i;
	uint32_t value=0;
	float ret_value;
	
	for(i=0;i<10;i++)                                          //取十次的平均值
	{
		ADC_SoftwareStartConvCmd(SMOG_ADC,ENABLE);//开始转换
		while( SET == ADC_GetSoftwareStartConvStatus(SMOG_ADC) ); //转换启动成功			
		while( RESET == ADC_GetFlagStatus(SMOG_ADC,ADC_FLAG_EOC));//转换完成
		value += ADC_GetConversionValue(SMOG_ADC);
	}
	value=value/10;
	ret_value=(float)value/4096*3.3;
	return ret_value;	
}

