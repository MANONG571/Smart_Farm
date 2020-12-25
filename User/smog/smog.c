#include "./smog/smog.h"

/**
  * @brief  ����������ʼ������,����GPIO��ADC.
  * @param  None.
  * @retval None.
  */
void Smog_ADC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//����GPIO��ADC��ʱ��
	SMOG_GPIO_CLK_FUN(SMOG_GPIO_CLK, ENABLE);
	SMOG_ADC_CLK_FUN(SMOG_ADC_CLK, ENABLE);
	
	//ADC��GPIO�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;     //ģ������ģʽ
	GPIO_InitStructure.GPIO_Pin = SMOG_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SMOG_GPIO_PORT,&GPIO_InitStructure);
	
	//ADC����
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;                   //����ת��
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;               //�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;  //û���ⲿ������ʽ
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;                   //����ģʽ
	ADC_InitStructure.ADC_NbrOfChannel=1;                              //ת��ͨ����:1
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;                        //������ɨ��ģʽ
	ADC_Init(SMOG_ADC,&ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                                  //6��Ƶ,ADCCLK=12MHz
	ADC_RegularChannelConfig(SMOG_ADC,SMOG_ADC_CHANNEL,1,SMOG_ADC_SAMPLETIME);//����ADC��ͨ��1
				
	ADC_Cmd(SMOG_ADC,ENABLE);                              //ʹ��ADC
	
	ADC_ResetCalibration(SMOG_ADC);                           //��λУ׼
	while( SET == ADC_GetResetCalibrationStatus(SMOG_ADC) );  //��λУ׼����
	ADC_StartCalibration(SMOG_ADC);                           //У׼
	while( SET == ADC_GetCalibrationStatus(SMOG_ADC) );       //У׼����
}

/**
  * @brief  ��ȡ��������ת�����ֵ.ȡʮ�ε�ƽ��ֵ
  * @param  None.
  * @retval ��������ת�����ֵ.
  */
float Smog_GetValue(void)
{
	uint8_t i;
	uint32_t value=0;
	float ret_value;
	
	for(i=0;i<10;i++)                                          //ȡʮ�ε�ƽ��ֵ
	{
		ADC_SoftwareStartConvCmd(SMOG_ADC,ENABLE);//��ʼת��
		while( SET == ADC_GetSoftwareStartConvStatus(SMOG_ADC) ); //ת�������ɹ�			
		while( RESET == ADC_GetFlagStatus(SMOG_ADC,ADC_FLAG_EOC));//ת�����
		value += ADC_GetConversionValue(SMOG_ADC);
	}
	value=value/10;
	ret_value=(float)value/4096*3.3;
	return ret_value;	
}

